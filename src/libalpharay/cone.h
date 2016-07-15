/* vim: set ts=4 ss=4 sw=4 noet ai cindent : */

#ifndef _CONE_H_
#define _CONE_H_

#include "object.h"

class Cone : virtual public Object, virtual public XmlObjectNamed
{
    public:
        void init() { Object::init(); }

        Cone(std::string name, BaseObject* parent=NULL) 
            : XmlObjectNamed ("cone", name) 
            { init(); parent_ = parent; }

        ~Cone() {};

        virtual Bounds bounds();
        
        virtual Vector normal(Vector point, Material* material);

        virtual BaseObject* intersection(Ray &ray, float *t, float limit);
};


#endif // _CONE_H_
