/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */

#ifndef _MESH_H_
#define _MESH_H_

#include <vector>
#include <string>
#include <tinyxml.h>

#include "object.h"
#include "map.h"
#include "vertex.h"
#include "triangle.h"

class Mesh : public Object
{
public:
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

    std::vector<Vertex*> vertexs;
    std::vector<Vector2*> uvpoints;
    std::vector<Triangle*> triangles;

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
};

#endif // _MESH_H_
