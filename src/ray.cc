/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * raytracer-a
 * Copyright (C) Ali Aafee 2011 <ali.aafee@gmail.com>
 * 
 * raytracer-a is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * raytracer is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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
