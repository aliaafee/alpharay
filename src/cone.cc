/* vim: set ts=4 ss=4 sw=4 noet ai cindent : */

#include "cone.h"

Bounds Cone::bounds()
{
    return Bounds( Vector(-1, -1, -1), Vector(1, 1, 1) );
}

        
Vector Cone::normal(Vector point, Material* material)
{
	point.normalize();
    Vector normal(point.x, point.y, 0.5);

    return transformNormal(normal);
}


BaseObject* Cone::intersection(Ray &ray, float *distance, float limit)
{
	/*
	 Ray Cone Intersection
	 ----------------------
	 Equation of a sphere with radius r is:
		x^2 + y^2 = z^2
	 defining a ray at position Ro and direction Rd:
		R(t) = Ro + t * Rd
	 so substituting and solving for t gives:
		t^2(Rd.x^2 + Rd.y^2 - Rd.z^2) + t(2*Ro.x*Rd.x + 2*Ro.y*Rd.y - 2*Ro.z*Rd.z) 
						+ (Ro.x^2 + Ro.y^2 - Ro.z^2) = 0
	 solve using the general formula for a quadratic equation
	 
	*/

	//transform the ray to fit into object space
	Vector Ro = transformPointInv(ray.position_);
	Vector Rd = transformDisplacementInv(ray.direction_);
	Ro.z -= 1;
		
	float a = Rd.x*Rd.x + Rd.y*Rd.y - Rd.z*Rd.z;
	float b = 2*Ro.x*Rd.x + 2*Ro.y*Rd.y - 2*Ro.z*Rd.z;
	float c = Ro.x*Ro.x + Ro.y*Ro.y - Ro.z*Ro.z;

	float determinant = b*b - (4*a*c);

	if (determinant < 0) {
		return NULL;
	}

	float t;

	determinant = sqrt(determinant);
	float t1 = ((b*-1)+determinant)/(2*a);
	float t2 = ((b*-1)-determinant)/(2*a);
	
	if (t1 < t2) {
		t = t1;
	} else {
		t = t2;
	}

	if (t < 0) { // intersection point behind the ray
		return NULL;
	}

	Vector i;
	V_INT_POINT(i, Ro, Rd, t);

	if (i.z > 0 || i.z < -1)
		return NULL;

	*distance = t;

	return this;
}
