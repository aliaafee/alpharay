/* vim: set ts=4 ss=4 sw=4 noet ai cindent : */

#include "vector.h"

Vector2::Vector2 (std::string val) {
    x = 0;
    y = 0;
}


float Vector2::magnitude() {
	float m = pow(x,2) + pow(y,2);
	if (m == 1 || m == 0) {
		return m;
	} else {
		return sqrt(m);
	}
}


float Vector2::magnitude2() {
	return pow(x,2) + pow(y,2);
}


Vector2 Vector2::operator+(Vector2 param) {
	Vector2 t;
	t.x = x + param.x;
	t.y = y + param.y;
	return t;
}

Vector2 Vector2::operator-(Vector2 param) {
	Vector2 t;
	t.x = x - param.x;
	t.y = y - param.y;
	return t;
}

Vector2 Vector2::operator*(float factor) {//Multiply with a value
	return Vector2(
		x*factor,
		y*factor
	);
}

Vector2 Vector2::operator/(float factor) {//Divide by a value
	return Vector2(
		x/factor,
		y/factor
	);
}

std::string Vector2::str() {
    std::stringstream ss (std::stringstream::in | std::stringstream::out);
    ss << x << "," << y;
    return ss.str();
}



Vector::Vector (float nx, float ny, float nz) {
	x = nx;
	y = ny;
	z = nz;
}

Vector::Vector (std::string val) {
    x = 0;
    y = 0;
    z = 0;
}

bool Vector::isNull () {
	if (x == 0 && y == 0 && z == 0) {
		return true;
	}
	return false;
}

void Vector::setNull () {
    x = 0;
    y = 0;
    z = 0;
}

void Vector::normalize() {
	float mag = magnitude();
	if (mag != 1) {
		x = x/mag;
		y = y/mag;
		z = z/mag;
	}
}

Vector Vector::getUnitVector() {
	if (!isNull()) {
		float mag = magnitude();
		Vector t;
		if (mag != 1) {
			t.x = x/mag;
			t.y = y/mag;
			t.z = z/mag;
		} else {
			t.x = x;
			t.y = y;
			t.z = z;
		}
		return t;
	}
	return *this;
}

Vector Vector::getReflection(Vector a) {
	return *this - (a*(2*(((*this)*a)/(a*a))));
}

float Vector::magnitude () {
	float m = pow(x,2) + pow(y,2) + pow(z,2);
	if (m == 1 || m == 0) {
		return m;
	} else {
		return sqrt(m);
	}
}

float Vector::magnitude2 () {
	return pow(x,2) + pow(y,2) + pow(z,2);
}

float Vector::distanceTo (Vector v) {
	return sqrt(pow(x-v.x,2) + pow(y-v.y,2) + pow(z-v.z,2));
}

float Vector::distanceTo2 (Vector v) {
	return pow(x-v.x,2) + pow(y-v.y,2) + pow(z-v.z,2);
}


Vector& Vector::operator+=(const Vector& rhs) {
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    return *this;
}


Vector& Vector::operator-=(const Vector& rhs) {
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;
    return *this;
}

Vector& Vector::operator/=(const float f) {
    x /= f;
    y /= f;
    z /= f;
    return *this;
}

Vector& Vector::operator*=(const float f) {
    x *= f;
    y *= f;
    z *= f;
    return *this;
}

bool Vector::operator<(Vector& rhs) {
    if (magnitude2() < rhs.magnitude2()) {
        return true;
    }
    return false;
}

Vector Vector::operator+(Vector param) {
	Vector t;
	t.x = x + param.x;
	t.y = y + param.y;
	t.z = z + param.z;
	return t;
}

Vector Vector::operator-(Vector param) {
	Vector t;
	t.x = x - param.x;
	t.y = y - param.y;
	t.z = z - param.z;
	return t;
}

Vector Vector::operator*(float factor) {//Multiply with a value
	return Vector(
		x*factor,
		y*factor,
		z*factor
	);
}

Vector Vector::operator/(float factor) {//Divide by a value
	return Vector(
		x/factor,
		y/factor,
		z/factor
	);
}

Vector Vector::operator%(Vector param) {//Cross Product
	return Vector(
		(y*param.z)-(z*param.y),
		(z*param.x)-(x*param.z),
		(x*param.y)-(y*param.x)
	);
}

float Vector::operator*(Vector param) {//Dot Product
	return (x*param.x) + (y*param.y) + (z*param.z);
}

Vector Vector::operator*(Matrix4 matrix) { //multiply by a 4x4 matrix
	Vector ret;
	ret.x = x * matrix.value[0][0] +
			y * matrix.value[0][1] +
			z * matrix.value[0][2] +
			1 * matrix.value[0][3];
	
	ret.y = x * matrix.value[1][0] +
			y * matrix.value[1][1] +
			z * matrix.value[1][2] +
			1 * matrix.value[1][3];

	ret.z = x * matrix.value[2][0] +
			y * matrix.value[2][1] +
			z * matrix.value[2][2] +
			1 * matrix.value[2][3];
	return ret;
}

Vector Vector::rotateX(float angle) {
	Vector t;
	t.x = x;
	t.y = y*cos(angle) - z*sin(angle);
	t.z = y*sin(angle) + z*cos(angle);
	return t;
}

Vector Vector::rotateY(float angle) {
	Vector t;
	t.x = x*cos(angle) + z*sin(angle);
	t.y = y;
	t.z = -x*sin(angle) + z*cos(angle);
	return t;
}

Vector Vector::rotateZ(float angle) {
	Vector t;
	t.x = x*cos(angle) - y*sin(angle);
	t.y = x*sin(angle) + y*cos(angle);
	t.z = z;
	return t;
}

Vector Vector::rotate(Vector angle) {
	//It works, needs optimization
	Vector t(x,y,z);
	Vector b(x,y,z);

	b.x = x*cos(angle.y) + z*sin(angle.y);
	b.y = y;
	b.z = -x*sin(angle.y) + z*cos(angle.y);
	
	t.x = b.x;
	t.y = b.y*cos(angle.x) - b.z*sin(angle.x);
	t.z = b.y*sin(angle.x) + b.z*cos(angle.x);

	b.x = t.x*cos(angle.z) - t.y*sin(angle.z);
	b.y = t.x*sin(angle.z) + t.y*cos(angle.z);
	b.z = t.z;

	return b;
}

Vector Vector::rotateAboutAxis(Vector axis, float angle) {
	//TO DO
	Vector t;
	t.x = x;
	t.y = y;
	t.z = z;
	return t;
}

std::string Vector::str() {
    std::stringstream ss (std::stringstream::in | std::stringstream::out);
    ss << x << "," << y << "," << z;
    return ss.str();
}

void Vector::toSpherical() {
    float r = magnitude();
    float t = acos(z/r);
    float p = atan2(y, x);
    
    x = r;
    y = t;
    z = p;
}

void Vector::toCylindrical() {
    float r = sqrt(x*x + y*y);
    float t = atan2(y, x);
    
    x = r;
    y = t;
    z = z;
}


void Vector::capColor() {
    if (x > 1.0f) {x = 1.0f;}
    if (y > 1.0f) {y = 1.0f;}
    if (z > 1.0f) {z = 1.0f;}
}

float Vector::colorIntensity() {
    return (x+y+z)/3.0f;
}

void Vector::toColor(unsigned char *col) {
	Vector t;
	if (x > 254) {x = 255;}
	if (y > 254) {y = 255;}
	if (z > 254) {z = 255;}
	if (x < 1) {x = 0;}
	if (y < 1) {y = 0;}
	if (z < 1) {z = 0;}
	col[0] = unsigned(char(int(x)));
	col[1] = unsigned(char(int(y)));
	col[2] = unsigned(char(int(z)));
}
