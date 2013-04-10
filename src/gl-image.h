/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */

#ifndef _GL_IMAGE_H_
#define _GL_IMAGE_H_

#include "vector.h"
#include "image.h"
#include <GL/freeglut.h>


class GLImage : public Image
{
public:
    GLImage(int width, int height);
	~GLImage();
    
    bool create(int width, int height);
    
    int height();
    int width();
	
	Vector getColor(float u, float v);
    bool setColor(float u, float v, Vector color);
    
    void display();
    
private:
    GLubyte *image_;
};


#endif // _GL_IMAGE_H_
