/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */

#ifndef _MESH_H_
#define _MESH_H_

#include <vector>
#include <string>
#include <tinyxml.h>
#include <fstream>
#include <sstream>

#include "object.h"
#include "map.h"
#include "vertex.h"
#include "triangle.h"

#include "octree.h"


class Mesh : public Object
{
public:
    std::vector<Vertex*> vertexs;
    std::vector<Vector*> normals;
    std::vector<Vector2*> uvpoints;
    std::vector<Triangle*> triangles;
    Octree octree_;

    virtual void init() { xmlName = "mesh"; }

    Mesh() {};
	Mesh(std::string name, Vector position, Material *material);

    void add(Vertex *vertex) 
		{vertexs.push_back(vertex); }
    Vertex* getVertex(int index);

    void add(Vector2 *uvp)
        {uvpoints.push_back(uvp);}
    Vector2* getUVPoint(int index);

    void add(Triangle *triangle) 
		{triangles.push_back(triangle); }

    Vector normal(Vector localPoint, UVTriangle *uvtriangle, Material *material);

    Object* intersection(
            Ray &ray,
            Vector *intersectionPoint,
            Vector *intersectionPointLocal,
            UVTriangle **intersectionUVTriangle,
            float *distance);

    void transform();

    void setBounds();

    bool loadXml(TiXmlElement* pElem, LinkList <Material> *linkMaterials);
    TiXmlElement* getXml();

    bool loadWavefrontObj(std::string filename, float scale, Vector position);
};

#endif // _MESH_H_
