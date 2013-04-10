/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */

#include "plane.h"

Plane::Plane (std::string name, Vector position, Material *material) 
    : Object(name, position, material) 
{	
    init();
}


Object* Plane::intersection(
            Ray &ray,
            Vector *intersectionPoint=NULL,
            Vector *intersectionPointLocal=NULL,
            Vector *intersectionNormal=NULL,
            UVTriangle **intersectionUVTriangle=NULL,
            float *distance=NULL)  
{
    /*
	 Ray Plane Intersection
	 ----------------------
	 equation of a plane with normal Pn = (A,B,C) at distance D
	 from the origin (0,0,0) is:
		Ax + By + Cz + D = 0
	 defining a ray at position Ro and direction Rd:
		R(t) = Ro + t * Rd
	 substituting R(t) for (x,y,z) and solving for t gives:
		t = -(Pn.Ro+D)/(Pn.Rd)
	*/

    Vector Ro = transformPointInv(ray.position_);
    Vector Rd = transformDisplacementInv(ray.direction_);

    //calculating the denominator Vd
	float Vd = V_DOT(normal_ , Rd);

	if (Vd == 0) {
		//ray is parallel to the plane
		return NULL;
	}

	if (Vd > 0) {
		//plane pointing away from the ray
		return NULL;
	}

	float t = (-1 * V_DOT(normal_ , Ro)) / Vd;

	if (t < 0) {
		//ray is going away from the plane 
		return NULL;
	}
    
    if (intersectionPointLocal != NULL) {
        //*intersectionPointLocal = Ro + (Rd * t);
        V_INT_POINT((*intersectionPointLocal), Ro, Rd, t);
    }

    if (intersectionPoint != NULL) {
	    //*intersectionPoint = ray.position_ + (ray.direction_ * t);
        V_INT_POINT((*intersectionPoint), ray.position_, ray.direction_, t);
    }

    if (intersectionNormal != NULL)
        *intersectionNormal = transformNormal(normal_) * normalDirection_;

    if (intersectionUVTriangle != NULL)
        *intersectionUVTriangle = NULL;

    if (distance != NULL)
	    *distance = t;

    return this;
}
