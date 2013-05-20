/* vim: set ts=4 ss=4 sw=4 noet ai cindent : */

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
