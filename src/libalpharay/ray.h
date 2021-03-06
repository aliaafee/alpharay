/* vim: set ts=4 ss=4 sw=4 noet ai cindent : */

#ifndef _RAY_H_
#define _RAY_H_

#include "vector.h"

class Ray
{
	friend std::ostream& operator<<(std::ostream& os, Ray& ray);
public:
	Ray() { position_ = Vector(0,0,0); direction_ = Vector(0,0,1); shadowRay_ = false; log_ = false; opticDensity_ = 1.0f;}
	Ray(Vector origin, Vector target, bool shadowRay);
	Ray(Vector origin, Vector target);
	Ray getReflectedRay(Vector origin, Vector normal);
	Ray getRefractedRay(Vector origin, Vector normal, float index);
    Ray getRamdomRayInHemisphere(Vector origin, Vector normal, float scatter);

    void calculateInverse();

    Vector inv_direction;
    int sign[3];
	
	Vector position_;
	Vector direction_;
	bool shadowRay_;

	Vector2 position2_;

    float opticDensity_;
	bool log_;
};

inline std::ostream& operator<<(std::ostream& os, Ray& ray){
	os << "ray["<< ray.position_ << "," << ray.direction_ <<"]";
	return os;
}

#endif // _RAY_H_
