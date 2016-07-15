/* vim: set ts=4 ss=4 sw=4 noet ai cindent : */

#include "plane.h"

Bounds Plane::bounds()
{
    return Bounds( Vector(-1, -1, -1), Vector(1, 1, 1) );
}

        
Vector Plane::normal(Vector point, Material* material)
{
	Vector os_normal = normal_;

	if (material->normalMap_ != NULL) {
		Vector os_bitangent;// = material->normalMap_->os_bitangent(point);
		Vector os_tangent;// = material->normalMap_->os_tangent(point);

		material->normalMap_->getTangents(point, &os_tangent, &os_bitangent);
		
		os_normal = material->normalObjectSpace(os_normal, os_tangent, os_bitangent);
	}

    return transformNormal(os_normal);
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
    
    /*
    Vector intersectionPoint;
    V_INT_POINT(intersectionPoint, Ro, Rd, t)

    float u = intersectionPoint * Vector(0,1,0);
    if (u < (-0.5)) {
        return NULL;
    }
    if (u > 0.5) {
        return NULL;
    }

    float v = intersectionPoint * Vector(1,0,0);

    if (v < (-0.5)) {
        return NULL;
    }
    if (v > 0.5) {
        return NULL;
    }*/
    
    *distance = t;

    return this;
}
