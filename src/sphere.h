/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */

#ifndef _SPHERE_H_
#define _SPHERE_H_

#include "object.h"

class Sphere : virtual public Object, virtual public XmlObjectNamed
{
    public:
        void init() { Object::init(); }

        Sphere(std::string name, BaseObject* parent=NULL) 
            : XmlObjectNamed ("sphere", name) 
            { init(); parent_ = parent; }

        ~Sphere() {};

        virtual Bounds bounds();
        
        virtual Vector normal(Vector point);

        virtual BaseObject* intersection(Ray &ray, float *t, float limit);
};


#endif // _SPHERE_H_
