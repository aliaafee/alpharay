/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */

#include "ray.h"


Ray::Ray(Vector origin, Vector target, bool shadowRay) {
	position_ = origin;
	direction_ = target - origin;
	direction_.normalize();
	shadowRay_ = shadowRay;
    opticDensity_ = 1.0f;
}

Ray::Ray(Vector origin, Vector target) {
	position_ = origin;
	direction_ = target - origin;
	direction_.normalize();
	shadowRay_ = false;
    opticDensity_ = 1.0f;

}

Ray Ray::getReflectedRay(Vector origin,Vector normal) {
	Ray reflectedRay;
	reflectedRay.position_ = origin;
	reflectedRay.direction_ = direction_.getReflection(normal);
	return reflectedRay;
}

Ray Ray::getRefractedRay(Vector origin, Vector normal, float newOpticDensity_) {
	Ray refractedRay;

    float n1 = opticDensity_;
    float n2 = newOpticDensity_;

    //std::cout << " " << opticDensity_ << "->" << newOpticDensity_;
    
    float cosT1 = normal * (direction_*-1);

    float T1 = acos(cosT1);
    float sinT2 = (sin(T1) * n2)/n1;
    float T2 = asin(sinT2);
    float cosT2 = cos(T2);

    if (cosT1 > 0) {
	    refractedRay.direction_ = (direction_ * (n2/n1)) + (normal * ((n2/n1)*cosT1 - cosT2));
    } else {
        refractedRay.direction_ = (direction_ * (n2/n1)) + (normal * ((n2/n1)*cosT1 + cosT2));
    }

    refractedRay.direction_.normalize();
    refractedRay.position_ = origin;
    refractedRay.opticDensity_ = newOpticDensity_;

	return refractedRay;
}


Ray Ray::getRamdomRayInHemisphere(Vector origin, Vector normal, float scatter) {
    Ray newRay;

    double r1=2*M_PI*float((float)rand()/RAND_MAX);
    double r2=float((float)rand()/RAND_MAX) * scatter;
    double r2s=sqrt(r2);

    Vector w= normal;
    Vector u= ((fabs(w.x)>.1?Vector(0,1,0):Vector(1,0,0))%w);
    u.normalize();
    Vector v= w % u;

    newRay.direction_ = (u*cos(r1)*r2s + v*sin(r1)*r2s + w*sqrt(1-r2));
    newRay.direction_.normalize();
    newRay.position_ = origin;

    return newRay;
}


void Ray::calculateInverse()
{
    inv_direction = Vector(1/direction_.x, 1/direction_.y, 1/direction_.z);

    sign[0] = inv_direction.x < 0;
    sign[1] = inv_direction.y < 0;
    sign[2] = inv_direction.z < 0;
}
