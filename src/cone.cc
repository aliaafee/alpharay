/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */

#include "cone.h"


Cone::Cone (Vector position, Material *material) {
	material_ = material;

	position_ = position;
	rotation_ = Vector(0,0,0);
	scale_ = Vector(1,1,2);
	parent_ = NULL;
	normalDirection_ = 1;
	
	e_ = Vector(0,1,0);
	n_ = Vector(0,0,1);
	w_ = e_ % n_;
}

void Cone::transform() {
	GenerateTransformationMatrix();
}

Vector Cone::getColor(Vector &intersectionPoint, Vector &reflection) {
	if (material_->diffuseTexture_ == NULL) {
		return material_->getColor(reflection);
	}
	
	Vector i = intersectionPoint;
	i = transformPointInv(i);

	float u = (atan2(i.x, i.y)+(M_PI))/(M_PI*2);
	float v = i.z * -1;
	
	return material_->getColor(u,v,reflection);
}

bool Cone::isInside(Vector point) {
	return false;
}
	
Object* Cone::intersection(Ray &ray, 
                             Vector *intersectionPoint=NULL,
                             Vector *intersectionNormal=NULL,
                             float *distance=NULL,
                             float *u=NULL,
                             float *v=NULL) 
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
	if (t1 < t2)
		t = t1;
	else 
		t = t2;

	if (t < 0) { // intersection point behind the ray
		return NULL;
	}

	Vector i = Ro + Rd*t;
	if (i.z > 0)
		return NULL;
	if (i.z < -1)
		return NULL;
	
	i.z = (tan(M_PI/4) * sqrt(i.x*i.x + i.y*i.y)) ;//* -1;
	
	
	
	*distance = t;

    if (u != NULL)
        *u = (atan2(i.x, i.y)+(M_PI))/(M_PI*2);

    if (v != NULL)
        *v = i.z;

    if (intersectionPoint != NULL)
	    *intersectionPoint  = ray.position_ + (ray.direction_ * t);
    
    if (intersectionNormal != NULL) {
        Vector V = i;
        V.normalize();
        Vector N(V.x*1, V.y*1, 0.5);
        if (u != NULL && v != NULL) {
            N.normalize();
            Vector U = N % Vector(0,0,1);
            Vector V = N % U;
            *intersectionNormal = transformNormal(N, U, V, *u, *v) * normalDirection_;
            //*intersectionNormal = transformNormal(i) * normalDirection_;
        } else {
	        *intersectionNormal = transformNormal(N) * normalDirection_;
        }
    }

    if (distance != NULL)
	    *distance = t;

	
	return this;
}
