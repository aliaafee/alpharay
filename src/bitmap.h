/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */

#ifndef _BITMAP_H_
#define _BITMAP_H_

#include "image.h"

#define cimg_display 0
#define cimg_OS 0
#include <CImg.h>

using namespace cimg_library;

#ifdef OPENGL
#include "gl-image.h"
#endif


class Bitmap : public Image, virtual public XmlObjectNamed
{
    public:
        virtual void init();

        Bitmap( int width, int height ) 
            : Image("unnamed"), XmlObjectNamed("bitmap", "unnamed")
            { init(); create(width, height); }
        
        Bitmap( std::string name ) 
            : Image(name), XmlObjectNamed("bitmap", name)
            { init(); }

        ~Bitmap();

        virtual bool create(int width, int height);
        virtual bool load(std::string filename);
        virtual bool save(std::string filename);

        virtual void enableDisplay();
        virtual void display();

        virtual Color getColor(Vector2 point);
        virtual bool setColor(Vector2 point, Color color);

        void correctExposure(float exposure);

        void bloom(float size, float highpass);

        virtual int width();
        virtual int height();

        virtual TiXmlElement* getXml();
        virtual bool loadXml(TiXmlElement* pElem, std::string path, LinkList *linkList);

    protected:
        std::string filename_;

        CImg<float>* image_;

#ifdef OPENGL
        GLImage* preview_;
#endif
};

#endif // _BITMAP_H_

