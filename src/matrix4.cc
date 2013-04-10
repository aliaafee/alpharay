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

#include "matrix4.h"

Matrix4::Matrix4() {
	int i,j;
	for(i = 0; i < 4; i++)
		for(j = 0; j < 4; j++)
    		value[i][j] = 0;
}

Matrix4::Matrix4(float a00,float a01,float a02,float a03,
				float a10, float a11,float a12,float a13,
				float a20, float a21,float a22,float a23,
				float a30, float a31,float a32,float a33) 
{
	value[0][0] = a00;
	value[0][1] = a01;
	value[0][2] = a02;
	value[0][3] = a03;

	value[1][0] = a10;
	value[1][1] = a11;
	value[1][2] = a12;
	value[1][3] = a13;

	value[2][0] = a20;
	value[2][1] = a21;
	value[2][2] = a22;
	value[2][3] = a23;

	value[3][0] = a30;
	value[3][1] = a31;
	value[3][2] = a32;
	value[3][3] = a33;
}

Matrix4 Matrix4::operator*(Matrix4 B) {
	int i,j,k;
	Matrix4 result;
	
	for(i = 0; i < 4; i++)
		for(j = 0; j < 4; j++)
			for(k = 0, result.value[i][j] = 0; k < 4; k++)
    			result.value[i][j] += value[i][k] * B.value[k][j];
	return result;
}

Matrix4 Matrix4::operator*(float factor) {
	int i,j;
	Matrix4 result;
	for(i = 0; i < 4; i++)
		for(j = 0; j < 4; j++)
			result.value[i][j] = value[i][j] * factor;
	return result;
}

Matrix4 Matrix4::getInverse() {
	Matrix4 result;
	
	float det = 
		  value[0][0] * value[1][1] * value[2][2] * value[3][3]
		+ value[0][0] * value[1][2] * value[2][3] * value[3][1]
		+ value[0][0] * value[1][3] * value[2][1] * value[3][2]

		+ value[0][1] * value[1][0] * value[2][3] * value[3][2]
		+ value[0][1] * value[1][2] * value[2][0] * value[3][3]
		+ value[0][1] * value[1][3] * value[2][2] * value[3][0]

		+ value[0][2] * value[1][0] * value[2][1] * value[3][3]
		+ value[0][2] * value[1][1] * value[2][3] * value[3][0]
		+ value[0][2] * value[1][3] * value[2][0] * value[3][1]

		+ value[0][3] * value[1][0] * value[2][2] * value[3][1]
		+ value[0][3] * value[1][1] * value[2][0] * value[3][2]
		+ value[0][3] * value[1][2] * value[2][1] * value[3][0]

		- value[0][0] * value[1][1] * value[2][3] * value[3][2]
		- value[0][0] * value[1][2] * value[2][1] * value[3][3]
		- value[0][0] * value[1][3] * value[2][2] * value[3][1]

		- value[0][1] * value[1][0] * value[2][2] * value[3][3]
		- value[0][1] * value[1][2] * value[2][3] * value[3][0]
		- value[0][1] * value[1][3] * value[2][0] * value[3][2]

		- value[0][2] * value[1][0] * value[2][3] * value[3][1]
		- value[0][2] * value[1][1] * value[2][0] * value[3][3]
		- value[0][2] * value[1][3] * value[2][1] * value[3][0]

		- value[0][3] * value[1][0] * value[2][1] * value[3][2]
		- value[0][3] * value[1][1] * value[2][2] * value[3][0]
		- value[0][3] * value[1][2] * value[2][0] * value[3][1] ;

	if (det == 0)
		return *this;

	float idet = 1/det;

	result.value[0][0] = idet * 
		  value[1][1] * value[2][2] * value[3][3]
		+ value[1][2] * value[2][3] * value[3][1]
		+ value[1][3] * value[2][1] * value[3][2]
		- value[1][1] * value[2][3] * value[3][2]
		- value[1][2] * value[2][1] * value[3][3]
		- value[1][3] * value[2][2] * value[3][1] ;

	result.value[0][1] = idet * 
		  value[0][1] * value[2][3] * value[3][2]
		+ value[0][2] * value[2][1] * value[3][3]
		+ value[0][3] * value[2][2] * value[3][1]
		- value[0][1] * value[2][2] * value[3][3]
		- value[0][2] * value[2][3] * value[3][1]
		- value[0][3] * value[2][1] * value[3][2] ;

	result.value[0][2] = idet * 
		  value[0][1] * value[1][2] * value[3][3]
		+ value[0][2] * value[1][3] * value[3][1]
		+ value[0][3] * value[1][1] * value[3][2]
		- value[0][1] * value[1][3] * value[3][2]
		- value[0][2] * value[1][1] * value[3][3]
		- value[0][3] * value[1][2] * value[3][1] ;

	result.value[0][3] = idet * 
		  value[0][1] * value[1][3] * value[2][2]
		+ value[0][2] * value[1][1] * value[2][3]
		+ value[0][3] * value[1][2] * value[2][1]
		- value[0][1] * value[1][2] * value[2][3]
		- value[0][2] * value[1][3] * value[2][1]
		- value[0][3] * value[1][1] * value[2][2] ;

	result.value[1][0] = idet * 
		  value[1][0] * value[2][3] * value[3][2]
		+ value[1][2] * value[2][0] * value[3][3]
		+ value[1][3] * value[2][2] * value[3][0]
		- value[1][0] * value[2][2] * value[3][3]
		- value[1][2] * value[2][3] * value[3][0]
		- value[1][3] * value[2][0] * value[3][2] ;

	result.value[1][1] = idet * 
		  value[0][0] * value[2][2] * value[3][3]
		+ value[0][2] * value[2][3] * value[3][0]
		+ value[0][3] * value[2][0] * value[3][2]
		- value[0][0] * value[2][3] * value[3][2]
		- value[0][2] * value[2][0] * value[3][3]
		- value[0][3] * value[2][2] * value[3][0] ;

	result.value[1][2] = idet * 
		  value[0][0] * value[1][3] * value[3][2]
		+ value[0][2] * value[1][0] * value[3][3]
		+ value[0][3] * value[1][2] * value[3][0]
		- value[0][0] * value[1][2] * value[3][3]
		- value[0][2] * value[1][3] * value[3][0]
		- value[0][3] * value[1][0] * value[3][2] ;

	result.value[1][3] = idet * 
		  value[0][0] * value[1][2] * value[2][3]
		+ value[0][2] * value[1][3] * value[2][0]
		+ value[0][3] * value[1][0] * value[2][2]
		- value[0][0] * value[1][3] * value[2][2]
		- value[0][2] * value[1][0] * value[2][3]
		- value[0][3] * value[1][2] * value[2][0] ;

	result.value[2][0] = idet * 
		  value[1][0] * value[2][1] * value[3][3]
		+ value[1][1] * value[2][3] * value[3][0]
		+ value[1][3] * value[2][0] * value[3][1]
		- value[1][0] * value[2][3] * value[3][1]
		- value[1][1] * value[2][0] * value[3][3]
		- value[1][3] * value[2][1] * value[3][0] ;

	result.value[2][1] = idet * 
		  value[0][0] * value[2][3] * value[3][1]
		+ value[0][1] * value[2][0] * value[3][3]
		+ value[0][3] * value[2][1] * value[3][0]
		- value[0][0] * value[2][1] * value[3][3]
		- value[0][1] * value[2][3] * value[3][0]
		- value[0][3] * value[2][0] * value[3][1] ;

	result.value[2][2] = idet * 
		  value[0][0] * value[1][1] * value[3][3]
		+ value[0][1] * value[1][3] * value[3][0]
		+ value[0][3] * value[1][0] * value[3][1]
		- value[0][0] * value[1][3] * value[3][1]
		- value[0][1] * value[1][0] * value[3][3]
		- value[0][3] * value[1][1] * value[3][0] ;

	result.value[2][3] = idet * 
		  value[0][0] * value[1][3] * value[2][1]
		+ value[0][1] * value[1][0] * value[2][3]
		+ value[0][3] * value[1][1] * value[2][0]
		- value[0][0] * value[1][1] * value[2][3]
		- value[0][1] * value[1][3] * value[2][0]
		- value[0][3] * value[1][0] * value[2][1] ;

	result.value[3][0] = idet * 
		  value[1][0] * value[2][2] * value[3][1]
		+ value[1][1] * value[2][0] * value[3][2]
		+ value[1][2] * value[2][1] * value[3][0]
		- value[1][0] * value[2][1] * value[3][2]
		- value[1][1] * value[2][2] * value[3][0]
		- value[1][2] * value[2][0] * value[3][1] ;

	result.value[3][1] = idet * 
		  value[0][0] * value[2][1] * value[3][2]
		+ value[0][1] * value[2][2] * value[3][0]
		+ value[0][2] * value[2][0] * value[3][1]
		- value[0][0] * value[2][2] * value[3][1]
		- value[0][1] * value[2][0] * value[3][2]
		- value[0][2] * value[2][1] * value[3][0] ;

	result.value[3][2] = idet * 
		  value[0][0] * value[1][2] * value[3][1]
		+ value[0][1] * value[1][0] * value[3][2]
		+ value[0][2] * value[1][1] * value[3][0]
		- value[0][0] * value[1][1] * value[3][2]
		- value[0][1] * value[1][2] * value[3][0]
		- value[0][2] * value[1][0] * value[3][1] ;

	result.value[3][3] = idet * 
		  value[0][0] * value[1][1] * value[2][2]
		+ value[0][1] * value[1][2] * value[2][0]
		+ value[0][2] * value[1][0] * value[2][1]
		- value[0][0] * value[1][2] * value[2][1]
		- value[0][1] * value[1][0] * value[2][2]
		- value[0][2] * value[1][1] * value[2][0] ;

	return result;
}