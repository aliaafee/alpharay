/* vim: set ts=4 ss=4 sw=4 noet ai cindent : */

#include "sphere.h"

Bounds Sphere::bounds()
{
    return Bounds( Vector(-1, -1, -1), Vector(1, 1, 1) );
}

        
Vector Sphere::normal(Vector point)
{
    return transformNormal(point);
}


BaseObject* Sphere::intersection(Ray &ray, float *distance, float limit)
{
    /*
	 Ray Sphere Intersection
	 ----------------------
	 Equation of a sphere with radius r is:
		P.P = r^2
	 defining a ray at position Ro and direction Rd:
		R(t) = Ro + t * Rd
	 so substituting P for R(t) and solving for t gives:
		(Ro + t * Rd) . (Ro + t * Rd) = r^2
		t^2(Rd.Rd) + 2t(Ro * Rd) + (Ro.Ro) -r^2 = 0
	 solve using the general formula for a quadratic equation
	*/

	//transform the ray to fit into object space
    Vector Ro = transformPointInv(ray.position_);
    Vector Rd = transformDisplacementInv(ray.direction_);

		
	float a = V_DOT(Rd , Rd);
	float b = 2 * V_DOT(Ro , Rd);
	//float c = (Ro * Ro) - (radius_*radius_);
    float c = V_DOT(Ro , Ro) - 1;

	float determinant = b*b - (4*a*c);

	if (determinant < 0) {
		return NULL;
	}

	float t = 0;

	if (determinant == 0) {
		t = (determinant * -1)/(2 * a);
	} else {
		determinant = sqrt(determinant);
		float t1 = ((b*-1)+determinant)/(2*a);
		float t2 = ((b*-1)-determinant)/(2*a);
		if (t1 < t2) {
			t = t1;
            if (t < 0.0001) { //if too close pick the far one
                t = t2;
            }
        } else {
			t = t2;
            if (t < 0.0001) { //if too close pick the far one
                t = t1;
            }
        }
	}

	if (t < 0) { // intersection point behind the ray
        
		return NULL;
	}

    *distance = t;

	return this;
}
