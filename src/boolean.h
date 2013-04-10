/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */

#ifndef _BOOLEAN_H_
#define _BOOLEAN_H_

#include "object.h"

#define NOT 0

class Boolean : public Object
{
public:
	Boolean (Object *A, char booleanOperator, Object *B);
	float normalDirection_;
	Object* A_;
	Object* B_;
	unsigned int booleanOperator_;

	void transform();
	Object* intersection(Ray &ray,Vector *intersectionPoint ,Vector *intersectionNormal,float *distance);
	Vector getColor(Vector &intersectionPoint);
	bool isInside(Vector point);
	void flipNormal();
};

#endif // _BOOLEAN_H_
