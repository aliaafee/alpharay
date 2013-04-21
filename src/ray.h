/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */

#ifndef _RAY_H_
#define _RAY_H_

#include "vector.h"

class Ray
{
	friend std::ostream& operator<<(std::ostream& os, Ray& ray);
public:
	Ray() { position_ = Vector(0,0,0); direction_ = Vector(0,0,1); shadowRay_ = false; opticDensity_ = 1.0f;}
	Ray(Vector origin, Vector target, bool shadowRay);
	Ray(Vector origin, Vector target);
	Ray getReflectedRay(Vector origin, Vector normal);
	Ray getRefractedRay(Vector origin, Vector normal, float index);

    void calculateInverse();

    Vector inv_direction;
    int sign[3];
	
	Vector position_;
	Vector direction_;
	bool shadowRay_;

    float opticDensity_;
};

inline std::ostream& operator<<(std::ostream& os, Ray& ray){
	os << "ray["<< ray.position_ << "," << ray.direction_ <<"]";
	return os;
}

#endif // _RAY_H_
