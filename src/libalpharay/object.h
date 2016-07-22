/* vim: set ts=4 ss=4 sw=4 noet ai cindent : */

#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "baseobject.h"

#include <typeinfo>


class Object : public BaseObject, virtual public XmlObjectNamed
{
		friend class Group;
    public:
        void init();

        Object() 
            : XmlObjectNamed ("object", "unnamed") 
            { init(); parent_ = NULL; }

        Object(std::string name, BaseObject* parent=NULL) 
            : XmlObjectNamed ("object", name) 
            { init(); parent_ = parent; }

        virtual ~Object() {};

        virtual void transform();

        virtual Bounds bounds()  { return Bounds(Vector(0,0,0), Vector(0,0,0)); }
        
        virtual Vector normal(Vector point, Material* material) { return Vector(0, 0, 1); }
        virtual void setPoint(Vector point, Material* material) 
            { Vector2 point2; return material->setPoint(point, point2); }

        virtual BaseObject* intersection(Ray &ray, float *t, float limit) { return NULL; }

        virtual Vector transformPoint(Vector &point);
        virtual Vector transformPointInv(Vector &point);
        virtual Vector transformNormal(Vector &point);
        virtual Vector transformNormalInv(Vector &point);
        virtual Vector transformDisplacement(Vector &point);
    	virtual Vector transformDisplacementInv(Vector &point);
        virtual Ray transformRay(Ray ray);

		virtual std::string str() { return name(); }

    //protected:
        Vector position_;
    	Vector rotation_;
    	Vector scale_;


    protected:
        Matrix4 transMatrix; // TRS
        Matrix4 transMatrixInv; // S^R^T^
        Matrix4 transMatrixND; //RS
        Matrix4 transMatrixNDInv; // S^R^
        Matrix4 transMatrixNormal; // RS^
        Matrix4 transMatrixNormalInv; // SR^
};


#endif // _OBJECT_H_
