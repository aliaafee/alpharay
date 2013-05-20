/* vim: set ts=4 ss=4 sw=4 noet ai cindent : */

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
