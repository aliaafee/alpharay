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

#ifndef _MATRIX4_H_
#define _MATRIX4_H_

#include <iostream>

class Matrix4
{
	friend std::ostream& operator<<(std::ostream& os, Matrix4& matrix);
public:
	Matrix4();
	Matrix4(float a00,float a01,float a02,float a03,
			float a04,float a05,float a06,float a07,
			float a08,float a09,float a10, float a11,
			float a12,float a13,float a14,float a15);
	float value[4][4];
	Matrix4 operator*(Matrix4 B);
	Matrix4 operator*(float factor);
	Matrix4 getInverse();
protected:

private:

};

inline std::ostream& operator<<(std::ostream& os, Matrix4& matrix) {
	os << std::endl;
	int i,j;
	for(i = 0; i < 4; i++) {
		os << "|";
		for(j = 0; j < 3; j++) {
			os << matrix.value[i][j] << ",\t";
		}
		os << matrix.value[i][3] << "\t|" << std::endl;
	}
	os << std::endl;
	return os;
}

#endif // _MATRIX4_H_
