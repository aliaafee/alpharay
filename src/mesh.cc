/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */

#include <string>
#include <tinyxml.h>

#include "mesh.h"

Mesh::Mesh (std::string name, Vector position, Material *material) 
    : Object(name, position, material) 
{	
	xmlName = "mesh";
}

Vertex* Mesh::getVertex(int index)
{
    for (int i=0; i < vertexs.size(); i++) {
        if (vertexs[i]->i == index) {
            return vertexs[i];
        }
    }
}

Vector2* Mesh::getUVPoint(int index)
{
    for (int i=0; i < uvpoints.size(); i++) {
        if (uvpoints[i]->i == index) {
            return uvpoints[i];
        }
    }
}


void Mesh::transform(){
    Object::transform();
    for (int i=0; i < triangles.size(); i++) {
        triangles[i]->transform();
    }
}

void Mesh::setBounds() {
    Vector max(-BIG_NUM, -BIG_NUM, -BIG_NUM);
    Vector min( BIG_NUM,  BIG_NUM,  BIG_NUM);

    for (int i=0; i < vertexs.size(); i++) {
        if (vertexs[i]->p.x < min.x) min.x = vertexs[i]->p.x;
        if (vertexs[i]->p.x > max.x) max.x = vertexs[i]->p.x;

        if (vertexs[i]->p.y < min.y) min.y = vertexs[i]->p.y;
        if (vertexs[i]->p.y > max.y) max.y = vertexs[i]->p.y;

        if (vertexs[i]->p.z < min.z) min.z = vertexs[i]->p.z;
        if (vertexs[i]->p.z > max.z) max.z = vertexs[i]->p.z;
    }

    //This fixes a precision error
    min -= Vector(0.01,0.01,0.01);
    max += Vector(0.01,0.01,0.01);

    cout << "   gen octree " << min << max << endl;

    int leafCount = 0;

    octree_ = Octree(min, max, 10, 25);

    octree_.add(&triangles, 0, &leafCount);

    cout << "     leaves " << leafCount << endl;
}

Vector Mesh::normal(Vector localPoint, UVTriangle *uvtriangle, Material *material) {
    if (material->normalMap_ == NULL) {
        return transformNormal(
                uvtriangle->getNormal(localPoint, !(material->flatShading_))
            );
    } else {
        //Get from normal map
        Vector imageNormal = material->getNormal(localPoint, uvtriangle);
        return transformNormal(
                uvtriangle->getNormal(localPoint, imageNormal, !(material->flatShading_))
            );
    }
}
	
Object* Mesh::intersection(
            Ray &ray,
            Vector *intersectionPoint=NULL,
            Vector *intersectionPointLocal=NULL,
            UVTriangle **intersectionUVTriangle=NULL,
            float *distance=NULL) 
{
	//transform the ray to fit into object space
    /*
	Vector Ro = transformPointInv(ray.position_);
	Vector Rd = transformDisplacementInv(ray.direction_);
    */

    Ray rayt = transformRay(ray);

    /*
    //Check for bounding box intersection
    if ( !(bbox_.intersection(Ro, Rd)) )
        return NULL;

    bool result;
    Triangle* curI=NULL;
    float curt;

    Triangle* cloI=NULL;
    float clot;

    int closestTrig = -1;

    clot = BIG_NUM;

    Vector pvec, tvec, qvec;
    float det, inv_det, u, v;

    for (int i=0; i < triangles.size(); i++) {
        //This is real ugly, but kinda fast. is it worth it?
        RAY_TRIG(result, (triangles[i]), Ro, Rd, curt, pvec, tvec, qvec, det, inv_det, u, v);

        //Or use the function alternative. Much nicer looking. But a teeny bit slower
        //result = triangles[i]->intersection(Ro, Rd, &curt);

        if (result) {
            if (curt > 0.0001 && curt < clot) {
                closestTrig = i;
                clot = curt;
            }
        }
    }

    if (closestTrig == -1) {
        return NULL;
    }

    cloI = triangles[closestTrig];
    
    */
    Triangle* cloI=NULL;
    float clot;

    cloI = octree_.intersection(rayt, &clot);

    if (cloI == NULL) {
        return NULL;
    }

    if (intersectionPointLocal != NULL)
        *intersectionPointLocal = rayt.position_ + (rayt.direction_ * clot);

    if (intersectionPoint != NULL)
	    *intersectionPoint  = ray.position_ + (ray.direction_ * clot);

    if (intersectionUVTriangle != NULL)
        *intersectionUVTriangle = cloI->uvtriangle; 

    if (distance != NULL)
	    *distance = clot;

	return this;
}


bool Mesh::loadXml(TiXmlElement* pElem, LinkList <Material> *linkMaterials) {
    Object::loadXml(pElem, linkMaterials);

    TiXmlHandle hRoot = TiXmlHandle(pElem);

    //if file is specified load mesh from file
    TiXmlDocument doc;
    pElem = hRoot.FirstChild( "file" ).Element();
    if (pElem) {
        std::string filename("");
        pElem->QueryStringAttribute ("filename", &filename);
        std::string type("");
        pElem->QueryStringAttribute ("type", &type);
        if (type == "" || type == "xml") {
            //Load an xml object file
            if (filename != "") {
                TiXmlHandle hDoc(&doc);
                if (doc.LoadFile(filename)) {
                    pElem = hDoc.FirstChildElement().Element();
                    if (pElem) {
                        hRoot = TiXmlHandle(pElem);
                    }
                }
            }
        } else if (type == "obj") {
            //Load wavefront obj file
            Vector position(0,0,0);
            float scale=1;
            pElem->QueryValueAttribute <Vector> ("position", &position);
            pElem->QueryFloatAttribute("scale", &scale);
            return loadWavefrontObj(filename, scale, position);
        }
    }

    //vertices
    Vertex* vert;
    pElem = hRoot.FirstChild("verts").FirstChild().Element();
    if (pElem) {
        for (pElem; pElem; pElem = pElem->NextSiblingElement()) {
            vert = new Vertex();
            vert->loadXml(pElem);
            add(vert);
        }
    }

    //uvpoints
    Vector2* uvp;
    pElem = hRoot.FirstChild( "uvps" ).FirstChild().Element();
    if (pElem) {
        for (pElem; pElem; pElem = pElem->NextSiblingElement()) {
            uvp = new Vector2();
            pElem->QueryIntAttribute ("i", &(uvp->i));
            pElem->QueryValueAttribute <Vector2> ("p", uvp);
            add(uvp);
        }
    }

    //triangles
    Triangle* trig;
    pElem = hRoot.FirstChild( "trigs" ).FirstChild().Element();
    if (pElem) {
        for (pElem; pElem; pElem = pElem->NextSiblingElement()) {
            int i, v0, v1, v2, m0, m1, m2;
            Vector n;

            pElem->QueryIntAttribute ("i", &i);
            pElem->QueryValueAttribute <Vector> ("n", &n);
            pElem->QueryIntAttribute ("v0", &v0);
            pElem->QueryIntAttribute ("v1", &v1);
            pElem->QueryIntAttribute ("v2", &v2);
            pElem->QueryIntAttribute ("m0", &m0);
            pElem->QueryIntAttribute ("m1", &m1);
            pElem->QueryIntAttribute ("m2", &m2);

            trig = new Triangle( i,
                    getVertex(v0),
                    getVertex(v1),
                    getVertex(v2),
                    getUVPoint(m0),
                    getUVPoint(m1),
                    getUVPoint(m2),
                    1,
                    n);
            add(trig);
        }
    }

    std::cout << "  mesh " << vertexs.size() << " verts, " << triangles.size() << " trigs" << endl;
    
    setBounds();
    
    //std::cout << "    bbox[" << bbox_.bounds[0] << "," <<bbox_.bounds[1] << "]" << std::endl;
}


TiXmlElement* Mesh::getXml() {
    TiXmlElement* root = Object::getXml();

    //vertices
    TiXmlElement* vertexs_e = new TiXmlElement("verts");
    root->LinkEndChild(vertexs_e);

    for (int i=0; i < vertexs.size(); i++) {
        vertexs_e->LinkEndChild(vertexs[i]->getXml());
    }

    //uvpoints
    TiXmlElement* uvpoints_e = new TiXmlElement("uvps");
    root->LinkEndChild(uvpoints_e);

    for (int i=0; i < uvpoints.size(); i++) {
        TiXmlElement* uvp = new TiXmlElement("uvp");
        uvp->SetAttribute("i", uvpoints[i]->i);
        uvp->SetAttribute("p", uvpoints[i]->str());
        uvpoints_e->LinkEndChild(uvp);
    }

    //triangles
    TiXmlElement* triangles_e = new TiXmlElement("trigs");
    root->LinkEndChild(triangles_e );

    for (int i=0; i < triangles.size(); i++) {
        triangles_e->LinkEndChild(triangles[i]->getXml());
    }

    return root;
}



bool Mesh::loadWavefrontObj(std::string filename, float scale, Vector position) {
    /*
       Wavefront Obj loader
       --------------------
       Needs a bit of work
       TODO: fix the mesh to store normals separately from vertices

    */
    cout << "  Loading obj file..." << endl;

    std::string line;
    std::ifstream objfile (filename.c_str());

    //default mapping
    add(new Vector2(0,0));

    if (objfile.is_open()) {
        int i = 0;
        int pi = 0;
        int t = 0;
        int pt = 0;
        int m = 0;
		while (objfile.good()) {
			getline(objfile,line);
            
            std::stringstream ss(line);
            std::stringstream s2;
            std::string token;

            token = "";
            getline(ss, token, ' ');
            if (token == "v") {
                Vector p;
                token = ""; getline(ss, token, ' ');
                p.x = stof(token) * scale;
                token = ""; getline(ss, token, ' ');
                p.z = stof(token) * scale;
                token = ""; getline(ss, token, ' ');
                p.y = stof(token) * scale;
                
                vertexs[i-1]->p = p + position;

                //cout << vertexs[i-1]->p << vertexs[i-1]->n << endl;
            } else if (token == "vn") {
                Vector p;
                token = ""; getline(ss, token, ' ');
                p.x = stof(token);
                token = ""; getline(ss, token, ' ');
                p.z = stof(token);
                token = ""; getline(ss, token, ' ');
                p.y = stof(token);

                i += 1;
                vertexs.push_back(new Vertex( i, Vector(0,0,0), p ));
                if ( i > (pi + 10000)) {
                    std::cout << "." << std::flush;
                    pi = i;
                }

            } else if (token == "vt") {
                Vector2* p = new Vector2();

                token = ""; getline(ss, token, ' ');
                p->x = 1.0f - stof(token);
                token = ""; getline(ss, token, ' ');
                p->y = 1.0f - stof(token);

                m += 1;
                p->i = m;

                //cout << *p << endl;

                uvpoints.push_back(p);

            } else if (token == "f") {
                std::string vert,map;
                int v0, v1, v2;
                int m0, m1, m2;

                token = ""; getline(ss, token, ' '); s2.str(token);
                vert = ""; getline(s2, vert, '/');
                v0 = int(stof(vert) - 1);
                map = "0"; getline(s2, map, '/');
                m0 = int(stof(map));

                token = ""; getline(ss, token, ' '); s2.str(token);
                vert = ""; getline(s2, vert, '/');
                v1 = int(stof(vert) - 1);
                map = "0"; getline(s2, map, '/');
                m1 = int(stof(map));

                token = ""; getline(ss, token, ' '); s2.str(token);
                vert = ""; getline(s2, vert, '/');
                v2 = int(stof(vert) - 1);
                map = "0"; getline(s2, map, '/');
                m2 = int(stof(map));

                Vector n = (vertexs[v0]->n + vertexs[v1]->n + vertexs[v2]->n) / 3.0;

                //cout << v0 << "," << v1 << "," << v2 << endl;

                add( new Triangle( i,
                    vertexs[v0],
                    vertexs[v1],
                    vertexs[v2],
                    uvpoints[m0],
                    uvpoints[m1],
                    uvpoints[m2],
                    1,
                    n) );

                t += 1;
                if ( t > (pt + 10000)) {
                    std::cout << "," << std::flush;
                    pt = t;
                }

            }

		}
		objfile.close();

        std::cout << std::endl;

        std::cout << "  mesh " << vertexs.size() << " verts, " << triangles.size() << " trigs" << endl;
        setBounds();
        return true;
	}
	
    std::cout << "Unable to open file " << filename << std::endl;
    return false;
}

