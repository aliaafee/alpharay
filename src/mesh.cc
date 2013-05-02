/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */

#include "mesh.h"


void Mesh::transform()
{
    Object::transform();

    for (unsigned long i=0; i < triangles.size(); i++) {
        triangles[i]->transform();
    }

}


void Mesh::genBounds()
{
    Bounds b = bounds();

    std::cout << "   gen octree " << b.min << b.max << std::endl;

    unsigned long leafCount = 0;

    octree_ = Octree(b.min, b.max, 10, 25);

    octree_.add(&triangles, 0, &leafCount);

    std::cout << "     leaves " << leafCount << std::endl;
}


Bounds Mesh::bounds()
{
    Vector max(-BIG_NUM, -BIG_NUM, -BIG_NUM);
    Vector min( BIG_NUM,  BIG_NUM,  BIG_NUM);

    for (unsigned long i=0; i < vertexs.size(); i++) {
        if (vertexs[i]->x < min.x) min.x = vertexs[i]->x;
        if (vertexs[i]->x > max.x) max.x = vertexs[i]->x;

        if (vertexs[i]->y < min.y) min.y = vertexs[i]->y;
        if (vertexs[i]->y > max.y) max.y = vertexs[i]->y;

        if (vertexs[i]->z < min.z) min.z = vertexs[i]->z;
        if (vertexs[i]->z > max.z) max.z = vertexs[i]->z;
    }

    //This fixes a precision error
    min -= Vector(0.01,0.01,0.01);
    max += Vector(0.01,0.01,0.01);

    return Bounds(min, max);
}


BaseObject* Mesh::intersection(Ray &ray, float *distance, float limit)
{
    Ray rayt = transformRay(ray);

    rayt.calculateInverse();

    BaseObject* closest=NULL;

    closest = octree_.intersection(rayt, distance, limit);

    if (closest == NULL)
        return NULL;

	return closest;
}


TiXmlElement* Mesh::getXml()
{
    TiXmlElement* root = Object::getXml();

    return root;
}


bool Mesh::loadXml(TiXmlElement* pElem, std::string path, LinkList *linkList)
{
    init();

    Object::loadXml(pElem, path, linkList);

    TiXmlHandle hRoot = TiXmlHandle(pElem);

    //if file is specified load mesh from file
    TiXmlDocument doc;
    pElem = hRoot.FirstChild( "file" ).Element();
    if (pElem) {
        std::string filename("");
        pElem->QueryStringAttribute ("filename", &filename);
        filename = pathJoin(path, filename);
        std::string type("");
        pElem->QueryStringAttribute ("type", &type);
        if (type == "" || type == "xml") {
            //Load an xml object file
            /*
            if (filename != "") {
                TiXmlHandle hDoc(&doc);
                if (doc.LoadFile(filename)) {
                    pElem = hDoc.FirstChildElement().Element();
                    if (pElem) {
                        hRoot = TiXmlHandle(pElem);
                    }
                }
            }
            */
        } else if (type == "obj") {
            //Load wavefront obj file
            Vector position(0,0,0);
            float scale=1;
            pElem->QueryValueAttribute <Vector> ("position", &position);
            pElem->QueryFloatAttribute("scale", &scale);
            return loadWavefrontObj(filename, scale, position);
        }
    }

    //TODO: XML loading


    return true;
}


bool Mesh::loadWavefrontObj(std::string filename, float scale, Vector position) {
    /*
       Wavefront Obj loader
       --------------------
       Needs a bit of work (bit slow)
    */
    std::cout << "  Loading obj file " << std::flush;

    std::string line;
    std::ifstream objfile (filename.c_str());

    uvpoints.push_back( new MapPoint2(0, 0, 0) );

    if (objfile.is_open()) {
        unsigned long v = 0;
        unsigned long m = 0;
        unsigned long n = 0;
        unsigned long t = 0;
        unsigned long pcount = 0;
		while (objfile.good()) {
			getline(objfile,line);
            
            std::stringstream ss(line);
            std::string token;

            token = "";
            getline(ss, token, ' ');
            if (token == "v") {
                //Vertex
                Vector p;
                token = "0"; getline(ss, token, ' ');
                p.x = stof(token) * scale;
                token = "0"; getline(ss, token, ' ');
                p.z = stof(token) * scale;
                token = "0"; getline(ss, token, ' ');
                p.y = stof(token) * scale;

                p += position;

                v += 1;

                vertexs.push_back( new Vertex(v, p.x, p.y, p.z) );
                
                
            } else if (token == "vn") {
                //Normal
                Vector p;
                token = "0"; getline(ss, token, ' ');
                p.x = stof(token);
                token = "0"; getline(ss, token, ' ');
                p.z = stof(token);
                token = "0"; getline(ss, token, ' ');
                p.y = stof(token);

                n += 1;
                
                normals.push_back( new Normal(n, p.x, p.y, p.z) );

            } else if (token == "vt") {
                //UVMapping
                Vector2 p;
                token = "0"; getline(ss, token, ' ');
                p.x = 1.0f - stof(token);
                token = "0"; getline(ss, token, ' ');
                p.y = 1.0f - stof(token);

                m += 1;

                uvpoints.push_back( new MapPoint2(v, p.x, p.y) );

            } else if (token == "f") {
                //Triangles
                std::string vert,map,norm;
                unsigned long v0, v1, v2;
                unsigned long m0, m1, m2;
                unsigned long n0, n1, n2;

                token = ""; getline(ss, token, ' '); 
                std::stringstream s0(token);
                vert = "0"; getline(s0, vert, '/'); 
                v0 = (stol(vert)) -1;
                map = "0"; getline(s0, map, '/'); 
                m0 = (stol(map));
                norm = "0"; getline(s0, norm, '/');
                n0 = (stol(norm)) -1;

                token = ""; getline(ss, token, ' '); 
                std::stringstream s1(token);
                vert = "0"; getline(s1, vert, '/'); 
                v1 = (stol(vert)) -1;
                map = "0"; getline(s1, map, '/'); 
                m1 = (stol(map));
                norm = "0"; getline(s1, norm, '/');
                n1 = (stol(norm)) -1;

                token = ""; getline(ss, token, ' '); 
                std::stringstream s2(token);
                vert = "0"; getline(s2, vert, '/'); 
                v2 = (stol(vert)) -1;
                map = "0"; getline(s2, map, '/'); 
                m2 = (stol(map));
                norm = "0"; getline(s2, norm, '/');
                n2 = (stol(norm)) -1;

                Vector n = ((*normals[n0]) + (*normals[n1]) + (*normals[n2])) / 3.0;

                t += 1;

                Triangle* trig = new Triangle(t, this);

                trig->set( vertexs[v0],  vertexs[v1],  vertexs[v2],
                          normals[n0],  normals[n1],  normals[n2],
                          uvpoints[m0], uvpoints[m1], uvpoints[m2],
                          Vector(n.x, n.y, n.z) );

                triangles.push_back(trig);
                          
            }

            //status
            if ( v+n+m+t > pcount + 5000) {
                pcount = v+n+m+t;
                std::cout << "." << std::flush;
            }

		}
		objfile.close();

        std::cout << std::endl;

        std::cout << "  mesh " << vertexs.size() << " verts, " << uvpoints.size() 
            << " uvps, " << normals.size() << " norms, " << triangles.size() << " trigs" << std::endl;
        
        genBounds();
        
        return true;
	}
	
    std::cout << "... failed (" << filename << ")" << std::endl;
    return false;
}


/*
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
    Ray rayt = transformRay(ray);

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
*/




