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

#ifndef _VECTOR_H_
#define _VECTOR_H_

#define     BIG_NUM 1000000000
#define   SMALL_NUM 0.0000001

#define V_CROSS(dest, v1, v2) \
    dest.x = v1.y * v2.z - v1.z * v2.y; \
    dest.y = v1.z * v2.x - v1.x * v2.z; \
    dest.z = v1.x * v2.y - v1.y * v2.x;

#define V_DOT(v1, v2) (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z)

#define V_SUB(dest, v1, v2) \
    dest.x = v1.x - v2.x; \
    dest.y = v1.y - v2.y; \
    dest.z = v1.z - v2.z; \

#define V_MUL(r, v, f) \
    r.x = v.x*f; \
    r.y = v.y*f; \
    r.z = v.z*f;

#define V_MUL_MATRIX(ret, v, matrix) \
    ret.x = v.x * matrix.value[0][0] + \
			v.y * matrix.value[0][1] + \
			v.z * matrix.value[0][2] + \
			1 * matrix.value[0][3];  \
	ret.y = v.x * matrix.value[1][0] + \
			v.y * matrix.value[1][1] + \
			v.z * matrix.value[1][2] + \
			1 * matrix.value[1][3];  \
	ret.z = v.x * matrix.value[2][0] + \
			v.y * matrix.value[2][1] + \
			v.z * matrix.value[2][2] + \
			1 * matrix.value[2][3];


#define V_INT_POINT(r, o, d, t) \
    r.x = o.x + d.x * t; \
    r.y = o.y + d.y * t; \
    r.z = o.z + d.z * t;

#define V_EQUAL(v1, v2) ( (v1.x == v2.x) && (v1.y == v2.y) && (v1.z == v2.z) )

#include <iostream>
#include <string>
#include <sstream>
#include <math.h>
#include "matrix4.h"

inline std::string ftos(float value) {
    std::stringstream ss (std::stringstream::in | std::stringstream::out);
    ss << value;
    return ss.str();
}

inline float stof(std::string value) {
    std::stringstream ss (std::stringstream::in | std::stringstream::out);
    float result;
    ss << value;
    ss >> result;
    return result;
}

inline float randf(float a, float b) {
    return ((b-a)*((float)rand()/RAND_MAX))+a;
}


class Vector2 {
public:
    int i;
    float x,y;

    friend std::ostream& operator<<(std::ostream& os, Vector2& vector);
    friend std::stringstream& operator>>(std::stringstream& ss, Vector2& vector);

    Vector2 () {x=0; y=0;};
    
    Vector2 (float nx, float ny) {x = nx; y = ny;}
    Vector2 (int ni, float nx, float ny) {i = ni; x = nx; y = ny;}
    Vector2 (std::string val);

    Vector2 operator+(Vector2); //Add
	Vector2 operator-(Vector2); //Subtract
	Vector2 operator*(float); //multiply by factor
	Vector2 operator/(float); //Div by factor

    std::string str();

};

class Vector {
public:
	float x,y,z;

    friend std::ostream& operator<<(std::ostream& os, Vector& vector);
    friend std::stringstream& operator>>(std::stringstream& ss, Vector& vector);

	Vector () { x=0; y=0; z=0; };
	Vector (float x, float y, float z);
    Vector (std::string val);
	
	bool isNull();
    void setNull();
    
	void normalize();
	Vector getUnitVector();
	
	Vector getReflection(Vector a);
	
	float magnitude();
	float magnitude2();
	float distanceTo(Vector);
	float distanceTo2(Vector);

    Vector& operator+=(const Vector& rhs);
    Vector& operator-=(const Vector& rhs);
    Vector& operator/=(const float f);
	
	Vector operator+(Vector); //Add
	Vector operator-(Vector); //Subtract
	Vector operator*(float); //multiply by factor
	Vector operator/(float); //Div by factor
	
	Vector operator%(Vector); //Cross Product
	float operator*(Vector); //Dot Product

	Vector operator*(Matrix4); //Multiply with 4x4 matrix
	
	Vector rotateX(float angle);
	Vector rotateY(float angle);
	Vector rotateZ(float angle);
	Vector rotate(Vector anglesXYZ);//porblem here
	Vector rotateAboutAxis(Vector axis, float angle);

    std::string str();

    void toSpherical();
    void toCylindrical();
	
    void capColor();
    float colorIntensity();
	void toColor(unsigned char *col);

    float randf(float a, float b) {
        return ((b-a)*((float)rand()/RAND_MAX))+a;
    }
};

inline std::ostream& operator<<(std::ostream& os, Vector& vector) {
	os << "(" << vector.x << "," << vector.y << "," << vector.z << ")";
	return os;
}

inline std::stringstream& operator>>(std::stringstream& ss, Vector& vector) {
    std::string token;

    token = "";
    std::getline(ss, token, ',');
    vector.x = stof(token);

    token = "";
    std::getline(ss, token, ',');
    vector.y = stof(token);

    token = "";
    std::getline(ss, token, ',');
    vector.z = stof(token);

    return ss;
}

inline std::ostream& operator<<(std::ostream& os, Vector2& vector) {
	os << "(" << vector.x << "," << vector.y << ")";
	return os;
}

inline std::stringstream& operator>>(std::stringstream& ss, Vector2& vector) {
    std::string token;

    token = "";
    std::getline(ss, token, ',');
    vector.x = stof(token);

    token = "";
    std::getline(ss, token, ',');
    vector.y = stof(token);

    return ss;
}



#endif // _VECTOR_H_
