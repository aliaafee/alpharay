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
