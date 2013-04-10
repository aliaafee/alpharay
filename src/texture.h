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
 * raytracer-a is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include "vector.h"

class Texture
{
public:
	virtual ~Texture() {};
	
	virtual Vector getColor2D(float x, float y) { return Vector(0,0,0); }
	virtual Vector getColor3D(Vector point) { return Vector(0,0,0); }
    virtual void getNormalDeviation(float u, float v, float *du, float *dv) { *du = 0; *dv = 0; };
};

#endif // _TEXTURE_H_
