/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */

#include "plane.h"

Bounds Plane::bounds()
{
    return Bounds( Vector(-1, -1, -1), Vector(1, 1, 1) );
}

        
Vector Plane::normal(Vector point)
{
    return transformNormal(normal_);
}


BaseObject* Plane::intersection(Ray &ray, float *distance, float limit)  
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
    
    *distance = t;

    return this;
}
