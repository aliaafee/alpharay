/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */

#include "gl-image.h"
#include <iostream>

GLImage::GLImage(int width, int height) : Image("") {
    width_ = width;
    height_ = height;
    image_ = new GLubyte[width_ * height_ * 3];
}

GLImage::~GLImage() {
    delete[] image_;
}

int GLImage::height() {
    if (image_ != NULL) {
        return height_;
    }
    return 0;
}


int GLImage::width() {
    if (image_ != NULL) {
        return width_;
    }
    return 0;
}


Vector GLImage::getColor(float u, float v) {
    Vector result;

    int ui = int(u)-1;
    int vi = height_ - int(v);
    int i = vi*width_*3 + ui*3;
    
    result.x = float(image_[i])/255.0f;
	result.y = float(image_[i+1])/255.0f;
	result.z = float(image_[i+2])/255.0f;
    
    return result;
}


bool GLImage::setColor(float u, float v, Vector color) {
    int ui = int(u)-1;
    int vi = height_ - int(v);
    int i = vi*width_*3 + ui*3;

    color.capColor();

    image_[i] = GLubyte(color.x*255.0f);
    image_[i+1] = GLubyte(color.y*255.0f);
    image_[i+2] = GLubyte(color.z*255.0f);
    
    return true;
}


void GLImage::display() {
    glDrawPixels(width_, height_, GL_RGB, GL_UNSIGNED_BYTE, image_ );
}
