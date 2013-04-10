/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */

#ifndef _CIMG_BITMAP_H_
#define _CIMG_BITMAP_H_

#include "texture.h"
#include "image.h"

class Bitmap : public Texture
{
public:
	Bitmap(Image *image);

	Vector getColor2D(float x, float y);
	Vector getColor3D(Vector point);
    void getNormalDeviation(float u, float v, float *du, float *dv);

	Image* image_;
};

#endif // _CIMG_BITMAP_H_
