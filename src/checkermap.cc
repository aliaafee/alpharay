/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */

#include "checkermap.h"

Checkermap::Checkermap(Vector color1, Vector color2) {
	color1_ = color1;
    color2_ = color2;
}

Vector Checkermap::getColor2D(float u, float v) {
	u = u - float(int(u));
	v = v - float(int(v));

	if (u < 0) { u = 1 + u; }
	if (v < 0) { v = 1 + v; }

    if (u > 0.5 && v > 0.5) {
        return color1_;
    }

    if (u < 0.5 && v < 0.5) {
        return color1_;
    }

    if (u > 0.5 && v < 0.5) {
        return color2_;
    }

    return color2_;
}

Vector Checkermap::getColor3D(Vector point) {
	return getColor2D(point.x,point.y);
}
