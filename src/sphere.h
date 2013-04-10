/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */

#ifndef _SPHERE_H_
#define _SPHERE_H_

#include <string>
#include <tinyxml.h>

#include "object.h"

class Sphere : public Object
{
public:
    virtual void init() { xmlName = "sphere"; }

    Sphere () {init();}
	Sphere (std::string name, Vector position, Material *material);	

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

#endif // _SPHERE_H_
