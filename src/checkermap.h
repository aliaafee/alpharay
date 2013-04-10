/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */

#ifndef _CHECKERMAP_H_
#define _CHECKERMAP_H_

#include "texture.h"

class Checkermap : public Texture
{
public:
	Checkermap(Vector color1, Vector color2);

	Vector getColor2D(float x, float y);
	Vector getColor3D(Vector point);

    Vector color1_;
    Vector color2_;
};

#endif // _CHECKERMAP_H_
