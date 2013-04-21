/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */

#ifndef _GL_IMAGE_H_
#define _GL_IMAGE_H_

#include "image.h"

#include <GL/freeglut.h>


class GLImage : public Image, virtual public XmlObjectNamed
{
    public:
        virtual void init() { Image::init(); image_ = NULL; }

        GLImage( int width, int height ) 
            : Image("unnamed"), XmlObjectNamed("image", "unnamed")
            { init(); create(width, height); }
        
        GLImage( std::string name ) 
            : Image(name), XmlObjectNamed("image", name)
            { init(); }

        ~GLImage() { if (image_ != NULL) delete[] image_; }

        virtual bool create(int width, int height);

        virtual void display();

        virtual Color getColor(Vector2 point);
        virtual bool setColor(Vector2 point, Color color);

        virtual int width();
        virtual int height();

    protected:
        GLubyte *image_;

};

#endif // _GL_IMAGE_H_
