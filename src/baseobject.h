/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */

#ifndef _BASE_OBJECT_H_
#define _BASE_OBJECT_H_

#include "vector.h"
#include "ray.h"
#include "material.h"


class BaseObject
{
    public:
        virtual void init();
        BaseObject() { init(); }
        ~BaseObject() { } ;

        //Basic setup operations for the object
        virtual void transform() {}; 

        virtual Bounds bounds() { return Bounds(Vector(0,0,0), Vector(0,0,0)); }
        virtual bool inBounds(Bounds targetBounds);

        virtual Vector point(Ray &ray, float t);
        virtual Vector pointLocal(Ray &ray, float t);

        virtual Vector normal(Vector point)  { return Vector(0,0,0); } //Return normal in world space at point (local space)
        virtual Color color(Vector point, Material* material) { return Color(0,0,0); }  //Return color at point  (local space)
        virtual Material material() { return Material(*material_); } //Return a copy of the material

        virtual BaseObject* intersection(Ray &ray, float *t, float limit) {return false; };

        virtual Vector transformPoint(Vector &point) {}; //Convert a point in object space to world space
        virtual Vector transformPointInv(Vector &point) {}; //Convert a point in world space to object space
        virtual Vector transformNormal(Vector &point) {}; //Convert a normal in object space to world space
        virtual Vector transformNormalInv(Vector &point) {}; //Convert a normal in world space to object space
        virtual Vector transformDisplacement(Vector &point) {};
    	virtual Vector transformDisplacementInv(Vector &point) {};


    protected:
        Material *material_;

        BaseObject *parent_;
};


inline void BaseObject::init()
{
    //initialisation code (called by constructor)
    material_ = NULL;
    parent_ = NULL;
}


inline Vector BaseObject::point(Ray &ray, float t) 
{
    //Given a ray and t returns intersection point (world space)
    Vector result;
    
    V_INT_POINT(result, ray.position_, ray.direction_, t);

    return result;
}


inline Vector BaseObject::pointLocal(Ray &ray, float t)
{
    //Given a ray and t returns intersection point (object space)
    Vector worldpoint = point(ray, t);
    return transformPointInv(worldpoint);
}


inline bool BaseObject::inBounds(Bounds targetBounds)
{
    //Check if the triangle is in the given bounds
    //A bit too many false positives when taking trigs bounding box only

    Bounds selfBounds = bounds();

    bool a = inRange(targetBounds.min.x, selfBounds.min.x, selfBounds.max.x)   || 
             inRange(targetBounds.max.x, selfBounds.min.x, selfBounds.max.x)   || 
             inRange(selfBounds.min.x, targetBounds.min.x, targetBounds.max.x) || 
             inRange(selfBounds.max.x, targetBounds.min.x, targetBounds.max.x) ;

    bool b = inRange(targetBounds.min.y, selfBounds.min.y, selfBounds.max.y)   ||
             inRange(targetBounds.max.y, selfBounds.min.y, selfBounds.max.y)   || 
             inRange(selfBounds.min.y, targetBounds.min.y, targetBounds.max.y) ||
             inRange(selfBounds.max.y, targetBounds.min.y, targetBounds.max.y) ;

    bool c = inRange(targetBounds.min.z, selfBounds.min.z, selfBounds.max.z)   ||
             inRange(targetBounds.max.z, selfBounds.min.z, selfBounds.max.z)   || 
             inRange(selfBounds.min.z, targetBounds.min.z, targetBounds.max.z) ||
             inRange(selfBounds.max.z, targetBounds.min.z, targetBounds.max.z) ;

    if ( a && b && c) {
        return true;
    }    

    return false;
}



#endif // _BASE_OBJECT_H_
