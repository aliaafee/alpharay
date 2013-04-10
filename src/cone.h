/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */

#ifndef _CONE_H_
#define _CONE_H_

#include "object.h"

class Cone : public Object
{
public:
	Cone (Vector position, Material *material);

	void transform();
	Object* intersection(Ray &ray,Vector *intersectionPoint ,Vector *intersectionNormal,float *distance,float *u, float *v);
	Vector getColor(Vector &intersectionPoint, Vector &reflection);
	bool isInside(Vector point);
private:
	Vector e_;
	Vector n_;
	Vector w_;
};

#endif // _Cone_H_
