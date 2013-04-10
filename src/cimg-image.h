/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */

#ifndef _CIMG_IMAGE_H_
#define _CIMG_IMAGE_H_

#include "vector.h"
#include "image.h"

#define cimg_display 0
#define cimg_OS 0
#include "CImg.h"

class CimgImage: public Image
{
public:
    std::string filename_;
    cimg_library::CImg<unsigned char>* image_;

    virtual void init() { xmlName = "cimgimage";}

    CimgImage() {init(); }
    CimgImage(int width, int height);
    CimgImage(std::string name, std::string filename);
	~CimgImage();

    bool save(std::string filename);

    int height();
    int width();
	
	Vector getColor(float u, float v);
    bool setColor(float u, float v, Vector color);

    virtual bool loadXml(TiXmlElement* pElem) {
        Image::loadXml(pElem);
        
        filename_ = "";
        pElem->QueryStringAttribute("filename", &filename_);

        if (filename_ != "") {
            image_ = new cimg_library::CImg<unsigned char>();
            image_->load(filename_.c_str());
            return true;
        }
        return false;
    }

    virtual TiXmlElement* getXml() {
        TiXmlElement* root = Image::getXml();

        root->SetAttribute("filename", filename_);

        return root;
    }
};

#endif // _CIMG_IMAGE_H_

