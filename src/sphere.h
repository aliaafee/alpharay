/* vim: set ts=4 ss=4 sw=4 noet ai cindent : */

#ifndef _SPHERE_H_
#define _SPHERE_H_

#include "object.h"

class Sphere : virtual public Object, virtual public XmlObjectNamed
{
    public:
        void init() { Object::init(); normal_ = 1; }

        Sphere(std::string name, BaseObject* parent=NULL) 
            : XmlObjectNamed ("sphere", name) 
            { init(); parent_ = parent; }

        ~Sphere() {};

        virtual Bounds bounds();
        
        virtual Vector normal(Vector point, Material* material);

        virtual BaseObject* intersection(Ray &ray, float *t, float limit);

		virtual TiXmlElement* getXml();
        virtual bool loadXml(TiXmlElement* pElem, std::string path, LinkList *linkList);

	private:
		int normal_;
};


#endif // _SPHERE_H_
