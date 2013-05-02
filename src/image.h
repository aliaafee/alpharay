/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */

#ifndef _IMAGE_H_
#define _IMAGE_H_

#include "vector.h"
#include "linklist.h"
#include "xmlobject.h"


class Image : virtual public XmlObjectNamed
{
    public:
        virtual void init() { XmlObjectNamed::init(); width_ = 0; height_ = 0; }
        
        Image( std::string name ) 
            : XmlObjectNamed("image", name)
            { init(); }

        ~Image() { } ;

        virtual bool create(int width, int height) { return true; }
        virtual bool load(std::string filename) { return true; }
        virtual bool save(std::string filename) { return true; }

        virtual void copyTo(Image* target);

        virtual Color getColor(Vector2 point) { return Color(0, 0, 0); }
        virtual bool setColor(Vector2 point, Color color) { return true; }

        virtual int width()  { return width_; }
        virtual int height() { return height_; }

        virtual TiXmlElement* getXml();
        virtual bool loadXml(TiXmlElement* pElem, std::string path, LinkList *linkList);

    protected:
        int width_;
        int height_;
};


inline void Image::copyTo(Image* target)
{
    if (target->width() == width() && target->height() == height()) {
        int w = width();
        int h = height();

        Vector2 point;
        for (int u = 1; u <= w; u++) 
        for (int v = 1; v <= h; v++)
        {
            point.x = u; point.y = v;
            target->setColor(point, getColor(point));
        }
    }
}


inline TiXmlElement* Image::getXml()
{
    TiXmlElement* root = XmlObjectNamed::getXml();

    return root;
}


inline bool Image::loadXml(TiXmlElement* pElem, std::string path, LinkList *linkList)
{
    init();

    XmlObjectNamed::loadXml(pElem, path);

    return true;
}



class Checker2d : public Image, virtual public XmlObjectNamed
{
    public:
        virtual void init() 
            { Image::init(); width_ = 1; height_ = 1; color1_ = Color(0,0,0); color2_ = Color(1,1,1); }
        
        Checker2d( std::string name ) 
            : Image(name), XmlObjectNamed("checker2d", name)
            { init(); }

        ~Checker2d() { } ;

        virtual Color getColor(Vector2 point);

        virtual TiXmlElement* getXml();
        virtual bool loadXml(TiXmlElement* pElem, std::string path, LinkList *linkList);

    protected:
        Color color1_;
        Color color2_;
};


inline Color Checker2d::getColor(Vector2 point)
{
    if (point.x > 0.5 && point.y > 0.5) {
        return color1_;
    }

    if (point.x < 0.5 && point.y < 0.5) {
        return color1_;
    }

    if (point.x > 0.5 && point.y < 0.5) {
        return color2_;
    }

    return color2_;
}


inline TiXmlElement* Checker2d::getXml()
{
    TiXmlElement* root = Image::getXml();

    root->SetAttribute("color1", color1_.str());
    root->SetAttribute("color2", color2_.str());

    return root;
}


inline bool Checker2d::loadXml(TiXmlElement* pElem, std::string path, LinkList *linkList)
{
    init();

    Image::loadXml(pElem, path, linkList);

    pElem->QueryValueAttribute <Vector> ("color1", &color1_);
    pElem->QueryValueAttribute <Vector> ("color2", &color2_);

    return true;
}

#endif // _IMAGE_H_

