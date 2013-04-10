/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */

#include "bitmap.h"

Bitmap::Bitmap(Image *image) {
	image_ = image;
}

Vector Bitmap::getColor2D(float u, float v) {
	u = u - float(int(u));
	v = v - float(int(v));

	if (u < 0) { u = 1 + u; }
	if (v < 0) { v = 1 + v; }
	
	u = u * image_->width();
	v = v * image_->height();
	
	return image_->getColor(u, v);
}

Vector Bitmap::getColor3D(Vector point) {
	return getColor2D(point.x,point.y);
}

void Bitmap::getNormalDeviation(float u, float v, float *du, float *dv) {
    /*
    for u, pick image points(where u1 = u):
                *    *    *
                u0 - u1 - u2
                *    *    *

                du = (u2-u1) - ( u1-u0) = u2-2*u1+u0

    for v, pick image points(where v1 = v):
                *     v0    *
                *     v1    *
                *     v2    *

                and deviation as above
    */

    float q;
        
    q = 1.0f / float(image_->width());
    float u0 = (getColor2D(u-q, v)).colorIntensity();
    float u1 = (getColor2D(u, v)).colorIntensity();
    float u2 = (getColor2D(u+q, v)).colorIntensity();

    *du = u2-(2*u1)+u0;

    q = 1.0f / float(image_->height());
    float v0 = (getColor2D(u, v-q)).colorIntensity();
    float v1 = u1;
    float v2 = (getColor2D(u, v+q)).colorIntensity();

    *dv = v2-(2*v1)+v0;

}


