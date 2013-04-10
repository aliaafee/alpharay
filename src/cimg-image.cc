/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */

#include "cimg-image.h"

CimgImage::CimgImage(int width, int height) : Image("") {
    init();
    image_ = new cimg_library::CImg<unsigned char>(width, height, 1, 3, 0);
}


CimgImage::CimgImage(std::string name, std::string filename) : Image(name) {
    init();
    image_ = new cimg_library::CImg<unsigned char>();
    image_->load(filename.c_str());
    filename_ = filename;
}

CimgImage::~CimgImage() {
    delete image_;
}
    
bool CimgImage::save(std::string filename) {
    if (image_ != NULL) {
        image_->save(filename.c_str());
        return true;
    }
    return false;
}


int CimgImage::height() {
    if (image_ != NULL) {
        return image_->height();
    }
    return 0;
}


int CimgImage::width() {
    if (image_ != NULL) {
        return image_->width();
    }
    return 0;
}


Vector CimgImage::getColor(float u, float v) {
    if (!image_) {
        return Vector(0,0,0);
    }
    unsigned char col[3];
    
    col[0] = image_->linear_atXY (u, v, 0);
	col[1] = image_->linear_atXY (u, v, 1);
	col[2] = image_->linear_atXY (u, v, 2);

    return Vector(float(col[0])/255.0f ,float(col[1])/255.0f ,float(col[2])/255.0f);
}


bool CimgImage::setColor(float u, float v, Vector color) {
    if (!image_) {
        return false;
    }
    unsigned char col[3];

    color.capColor();

    col[0] = unsigned(char(color.x * 255.0f));
    col[1] = unsigned(char(color.y * 255.0f));
    col[2] = unsigned(char(color.z * 255.0f));

    image_->draw_point(int(u) ,int(v) ,col);

    return true;
}

