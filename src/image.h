/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */

#ifndef _IMAGE_H_
#define _IMAGE_H_

#include <string>
#include <tinyxml.h>

#include "vector.h"

class Image
{
public:
    std::string xmlName;
    std::string name_;

    virtual void init() { xmlName = "image";}

    std::string name() { return name_;}

    Image() {init();}
    Image(std::string name) {
        init();
        name_ = name;
    };
	~Image() {};

    virtual bool save(std::string filename) {
        return true;
    }

    virtual int height() {
        return height_;
    }

    virtual int width() {
        return width_;
    }
	
	virtual Vector getColor(float u, float v) {
        return Vector(0, 0, 0);
    }

    virtual bool setColor(float u, float v, Vector color) {
        return true;
    }

    virtual void copyTo(Image* target) {
        if (target->width() == width() && target->height() == height()) {
            int w = width();
            int h = height();
            for (int u = 1; u <= w; u++) {
                for (int v = 1; v <= h; v++) {
                    target->setColor(u, v, getColor(u, v));
                }
            }
        }
    }

    virtual bool loadXml(TiXmlElement* pElem) {
        name_ = "image"; 

        pElem->QueryStringAttribute ("name", &name_);

        return true;
    }

    virtual TiXmlElement* getXml() {
        TiXmlElement* root = new TiXmlElement(xmlName.c_str());

        root->SetAttribute("name", name_);

        return root;
    }
protected:
    int width_;
    int height_;
};

class Checker2d : public Image
{
    public:
        Vector color1_;
        Vector color2_;

        virtual void init() { xmlName = "checker2d"; height_=1; width_=1;}

        Checker2d() {init();}
        Checker2d(std::string name, Vector c1, Vector c2)
            : Image(name)
        { init(); color1_ = c1; color2_ = c2;}

        Vector getColor(float u, float v) {
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

        };

        virtual bool loadXml(TiXmlElement* pElem) {
            Image::loadXml(pElem);

            pElem->QueryValueAttribute <Vector> ("color1", &color1_);
            pElem->QueryValueAttribute <Vector> ("color2", &color2_);

            return true;
        }

        virtual TiXmlElement* getXml() {
            TiXmlElement* root = Image::getXml();

            root->SetAttribute("color1", color1_.str());
            root->SetAttribute("color2", color2_.str());

            return root;
        }
};

#endif // _IMAGE_H_

