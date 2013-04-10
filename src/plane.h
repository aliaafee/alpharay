/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */

#ifndef _PLANE_H_
#define _PLANE_H_

#include <string>
#include <tinyxml.h>

#include "object.h"

class Plane: public Object 
{
public:
    Vector normal_;

    virtual void init() { xmlName = "plane"; normal_ = Vector(0, 0, 1);}

    Plane () {init();}
    Plane (std::string name, Vector position, Material *material);	

    Object* intersection(
            Ray &ray,
            Vector *intersectionPoint,
            Vector *intersectionPointLocal,
            Vector *intersectionNormal,
            UVTriangle **intersectionUVTriangle,
            float *distance);

    bool loadXml(TiXmlElement* pElem, LinkList <Material> *linkMaterials) {
        return Object::loadXml(pElem, linkMaterials);
    }

    TiXmlElement* getXml() {
        TiXmlElement* root = Object::getXml();
        return root;
    }
};

#endif // _PLANE_H_
