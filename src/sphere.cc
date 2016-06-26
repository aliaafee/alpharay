/* vim: set ts=4 ss=4 sw=4 noet ai cindent : */

#include "sphere.h"

Bounds Sphere::bounds()
{
    return Bounds( Vector(-1, -1, -1), Vector(1, 1, 1) );
}

        
Vector Sphere::normal(Vector point, Material* material)
{
	point *= normal_;
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

	if (ray.log_) {
		std::cout << "  " << name() << ": ";
	}

	//transform the ray to fit into object space
    Vector Ro = transformPointInv(ray.position_);
    Vector Rd = transformDisplacementInv(ray.direction_);

		
	float a = V_DOT(Rd , Rd);
	float b = 2 * V_DOT(Ro , Rd);
	//float c = (Ro * Ro) - (radius_*radius_);
    float c = V_DOT(Ro , Ro) - 1;

	float determinant = b*b - (4*a*c);

	if (determinant < 0) {
		if (ray.log_) { std::cout << std::endl;}
		return NULL;
	}

	float t = 0;

	if (determinant == 0) {
		t = (determinant * -1)/(2 * a);
	} else {
		determinant = sqrt(determinant);
		float t1 = ((b*-1)+determinant)/(2*a);
		float t2 = ((b*-1)-determinant)/(2*a);

		
		if (normal_ > 0) {
			if (t1 < 0 && t2 > 0) {
				if (ray.log_) { std::cout << std::endl; }
				return NULL;
			}

			if (t2 < 0 && t1 > 0) {
				if (ray.log_) { std::cout << std::endl; }
				return NULL;
			}
		}
		
		
		
		
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

		if (ray.log_) { std::cout << "t1=" << t1 << ",t2=" << t2 << ",t=" << t; }
	}

	if (t < 0) { // intersection point behind the ray
        if (ray.log_) { std::cout << std::endl; }
		return NULL;
	}

    *distance = t;

	if (ray.log_) { std::cout << std::endl;}


	return this;
}


bool Sphere::loadXml(TiXmlElement* pElem, std::string path, LinkList *linkList)
{
    init();

    Object::loadXml(pElem, path, linkList);

	pElem->QueryValueAttribute <int> ("normal", &normal_);

	if (normal_ < 0) { normal_ = -1; } else { normal_ = 1; }

	return true;
}


TiXmlElement* Sphere::getXml()
{
    TiXmlElement* root = Object::getXml();

	root->SetAttribute("normal", normal_);

    return root;
}

