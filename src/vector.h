/* vim: set ts=4 ss=4 sw=4 noet ai cindent : */

#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <limits>

#define     BIG_NUM 1000000000.0f
#define   SMALL_NUM 0.0001f
#define EPSILON 0.000001f

#include <cstdlib>
#include <cfloat>
#include <iostream>
#include <string>
#include <sstream>

#define _USE_MATH_DEFINES
#include <cmath>

#include <chrono>
inline std::string durationDisplay(std::chrono::nanoseconds ns)
{
	using namespace std;
    using namespace std::chrono;

	std::chrono::hours h = duration_cast<hours>(ns);
    ns -= h;
    std::chrono::minutes m = duration_cast<minutes>(ns);
    ns -= m;
	std::chrono::milliseconds ms = duration_cast<milliseconds>(ns);

	std::stringstream ss;
    ss << h.count() << "h:"
       << m.count() << "m:"
	   << ms.count() / 1000.0 << "s" ;

    return ss.str();
};
#define TIMER_START std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now()
#define TIMER_RESET t1 = std::chrono::high_resolution_clock::now()
#define TIMER_ELAPSED durationDisplay(std::chrono::duration_cast<std::chrono::nanoseconds>( std::chrono::high_resolution_clock::now() - t1 ))

#include "matrix4.h"

#define  ARRAY(x, y, z, w, h, d) (x*w*d + y*d + z)

#define V_CROSS(dest, v1, v2) \
    dest.x = v1.y * v2.z - v1.z * v2.y; \
    dest.y = v1.z * v2.x - v1.x * v2.z; \
    dest.z = v1.x * v2.y - v1.y * v2.x;

#define V_DOT(v1, v2) (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z)

#define V_ADD(dest, v1, v2) \
    dest.x = v1.x + v2.x; \
    dest.y = v1.y + v2.y; \
    dest.z = v1.z + v2.z; \

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

#define V_MIN_MAX(p, min, max) \
    if (p.x < min.x) min.x = p.x; \
    if (p.x > max.x) max.x = p.x; \
    if (p.y < min.y) min.y = p.y; \
    if (p.y > max.y) max.y = p.y; \
    if (p.z < min.z) min.z = p.z; \
    if (p.z > max.z) max.z = p.z; \

#define inRange(v, rmin, rmax) ( (rmin < v) && (v < rmax) )



inline float divide(float a, float b) {
    if (b > -FLT_EPSILON && b < FLT_EPSILON) {
        if (a == 0) {
            return 0;
        } else if (a > 0) {
            return FLT_MAX;
        } else {
            return -FLT_MAX;
        }
    } else {
        return a/b;
    }
}


inline std::string ftos(float value) {
    std::stringstream ss (std::stringstream::in | std::stringstream::out);
    ss << value;
    return ss.str();
}

inline float stringtofloat(std::string value) {
    std::stringstream ss (std::stringstream::in | std::stringstream::out);
    float result;
    ss << value;
    ss >> result;
    return result;
}

inline unsigned long stringtolong(std::string value) {
    std::stringstream ss (std::stringstream::in | std::stringstream::out);
    if (value == "" ) { value = "0"; }
    unsigned long result;
    ss << value;
    ss >> result;
    return result;
}

inline float randf(float a, float b) {
    return ((b-a)*((float)rand()/RAND_MAX))+a;
}


class Vector2 {
public:
    float x,y;

    friend std::ostream& operator<<(std::ostream& os, Vector2& vector);
    friend std::stringstream& operator>>(std::stringstream& ss, Vector2& vector);

    Vector2 () {x=0; y=0;};
    
    Vector2 (float nx, float ny) {x = nx; y = ny;}
    Vector2 (std::string val);

	float magnitude();
	float magnitude2();

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
    Vector& operator*=(const float f);


    bool operator<(Vector& rhs);
	
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
    vector.x = stringtofloat(token);

    token = "";
    std::getline(ss, token, ',');
    vector.y = stringtofloat(token);

    token = "";
    std::getline(ss, token, ',');
    vector.z = stringtofloat(token);

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
    vector.x = stringtofloat(token);

    token = "";
    std::getline(ss, token, ',');
    vector.y = stringtofloat(token);

    return ss;
}

class Bounds
{
    public:
        Bounds() {};
        Bounds(Vector mi, Vector ma) { min = mi; max = ma; }

        Vector min;
        Vector max;
};


class Color : public Vector
{
    public:
        Color()
			{ x = 0; y = 0; z = 0; a = 1; }
        Color(float r, float g, float b)
            { x = r; y = g; z = b; a = 1; }
		Color(float r, float g, float b, float a_)
			{ x = r; y = g; z = b; a = a_; }
        Color(Vector v) { x = v.x; y = v.y; z = v.z; }

        float r() { return x; }
        float g() { return y; }
        float b() { return z; }
		
        float intensity() { return (x + y + z)/3.0f; }
		
		float a;
};

inline Vector randomPointInHemisphere(Vector normal) {
    short unsigned int* Xi;
    double r1=2*M_PI*float((float)rand()/RAND_MAX);
    double r2=float((float)rand()/RAND_MAX);
    double r2s=sqrt(r2);

    Vector w= normal;
    Vector u= ((fabs(w.x)>.1?Vector(0,1,0):Vector(1,0,0))%w);
    u.normalize();
    Vector v= w % u;

    Vector d = (u*cos(r1)*r2s + v*sin(r1)*r2s + w*sqrt(1-r2));

    d.normalize();

    return d;
    /*
    float theta = acos(normal.z);
    float phi = atan2(normal.y, normal.x);

    float t = theta + randf(-1.0*M_PI/2.0, M_PI/2.0);
    float p = phi + randf(-1.0*M_PI, M_PI);

    Vector result;

    result.x = sin(t) * cos(p);
    result.y = sin(t) * sin(p);
    result.z = cos(t);

    return result;
    */
}

#endif // _VECTOR_H_
