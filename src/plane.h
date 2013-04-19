/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */

#ifndef _PLANE_H_
#define _PLANE_H_

#include "object.h"


class Plane : virtual public Object, virtual public XmlObjectNamed
{
    public:
        void init() { Object::init(); normal_ = Vector(0, 0, 1);}

        Plane(std::string name, BaseObject* parent=NULL) 
            : XmlObjectNamed ("plane", name) 
            { init(); parent_ = parent; }

        ~Plane() {};

        virtual Bounds bounds();
        
        virtual Vector normal(Vector point);

        virtual BaseObject* intersection(Ray &ray, float *t, float limit);
    
    private:
        Vector normal_;
};

#endif // _PLANE_H_
