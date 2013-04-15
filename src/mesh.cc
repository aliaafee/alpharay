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
    return transformNormal(
                uvtriangle->getNormal(localPoint, !(material->flatShading_))
            );
}
	
Object* Mesh::intersection(
            Ray &ray,
            Vector *intersectionPoint=NULL,
            Vector *intersectionPointLocal=NULL,
            UVTriangle **intersectionUVTriangle=NULL,
            float *distance=NULL) 
{
	//transform the ray to fit into object space
	Vector Ro = transformPointInv(ray.position_);
	Vector Rd = transformDisplacementInv(ray.direction_);

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

    cloI = octree_.intersection(Ro, Rd, &clot);

    if (cloI == NULL) {
        return NULL;
    }

    if (intersectionPointLocal != NULL)
        *intersectionPointLocal = Ro + (Rd * clot);

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
        if (filename != "") {
            TiXmlHandle hDoc(&doc);
            if (doc.LoadFile(filename)) {
                pElem = hDoc.FirstChildElement().Element();
                if (pElem) {
                    hRoot = TiXmlHandle(pElem);
                }
            }
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
    } else {
        std::cout << "noverts" << endl;
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
