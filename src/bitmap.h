/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */

#ifndef _BITMAP_H_
#define _BITMAP_H_

#include "image.h"

#define cimg_display 0
#define cimg_OS 0
#include <CImg.h>


class Bitmap : public Image, virtual public XmlObjectNamed
{
    public:
        virtual void init() { Image::init(); image_ = NULL; }

        Bitmap( int width, int height ) 
            : Image("unnamed"), XmlObjectNamed("bitmap", "unnamed")
            { init(); create(width, height); }
        
        Bitmap( std::string name ) 
            : Image(name), XmlObjectNamed("bitmap", name)
            { init(); }

        ~Bitmap() { delete image_; }

        virtual bool create(int width, int height);
        virtual bool load(std::string filename);
        virtual bool save(std::string filename);

        virtual Color getColor(Vector2 point);
        virtual bool setColor(Vector2 point, Color color);

        virtual int width();
        virtual int height();

        virtual TiXmlElement* getXml();
        virtual bool loadXml(TiXmlElement* pElem, std::string path, LinkList *linkList);

    protected:
        std::string filename_;

        cimg_library::CImg<float>* image_;
};

#endif // _BITMAP_H_

