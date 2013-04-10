/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */

#include "float-image.h"

FloatImage::FloatImage(int width, int height) : Image("") {
    width_ = width;
    height_ = height;
    image_ = new float[width_ * height_ * 3];
}

FloatImage::~FloatImage() {
    delete[] image_;
}

int FloatImage::height() {
    if (image_ != NULL) {
        return height_;
    }
    return 0;
}


int FloatImage::width() {
    if (image_ != NULL) {
        return width_;
    }
    return 0;
}

Vector FloatImage::getColor(float u, float v) {
    Vector result;

    int ui = int(u) - 1;
    int vi = int(v) - 1;
    int i = vi*width_*3 + ui*3;
    
    result.x = image_[i];
	result.y = image_[i+1];
	result.z = image_[i+2];
    
    return result;
}


bool FloatImage::setColor(float u, float v, Vector color) {
    int ui = int(u) - 1;
    int vi = int(v) - 1;
    int i = vi*width_*3 + ui*3;

    image_[i]   = color.x;
    image_[i+1] = color.y;
    image_[i+2] = color.z;
    
    return true;
}


Vector FloatImage::getColor(float u, float v, Vector* result) {
    int ui = int(u) - 1;
    int vi = int(v) - 1;
    int i = vi*width_*3 + ui*3;

    result->x = image_[i];
	result->y = image_[i+1];
	result->z = image_[i+2];
}


void FloatImage::glow(int radius) {
    float *result = new float[width_ * height_ * 3];
    
    int i;
    float exposure_ = -1;
    Vector color;
    for (int u = 0; u < width_; u ++) {
        for (int v = 0; v < height_; v ++) {
            i = v*width_*3 + u*3;

            Vector total;
            float count = 0;
            for (int su = u - radius; su < u + radius; su ++) {
                if (su > 0 && su <= width_) {
                    for (int sv = v - radius; sv < v + radius; sv ++) {
                        if (sv > 0 && sv <= height_) {
                            getColor(su, sv, &color);
                            total += color;
                            count += 1;
                        }
                    }
                }
            }
            /*
            for (int sv = v - radius; sv < v + radius; sv ++) {
                if (sv > 0 && sv <= height_) {
                    getColor(u, sv, &color);
                    total += color;
                    count += 1;
                }
            }*/

            total.x /= count;
            total.y /= count;
            total.z /= count;

            float average = (total.x + total.y + total.z) / 3.0f;

            /*result[i+0] = expfunc(total.x, -8);// + expfunc(image_[i+0], -1);
            result[i+1] = expfunc(total.y, -8);// + expfunc(image_[i+1], -1);
            result[i+2] = expfunc(total.z, -8);// + expfunc(image_[i+2], -1);*/

            result[i+0] = total.x + expfunc(image_[i+0], -1);
            result[i+1] = total.y + expfunc(image_[i+1], -1);
            result[i+2] = total.z + expfunc(image_[i+2], -1);

        }
    }


    delete[] image_;
    image_ = result;
}
