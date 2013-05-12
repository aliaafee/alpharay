/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */

#include "bitmap.h"


bool Bitmap::create(int width, int height)
{
    if (image_ != NULL) {
        delete image_;
    }

    init();

    image_ = new cimg_library::CImg<float>(width, height, 1, 3, 0);

    return true;
}


bool Bitmap::load(std::string filename)
{
    if (image_ != NULL) {
        delete image_;
    }

    init();

    image_ = new cimg_library::CImg<float>();
    image_->load(filename.c_str());

    return true;
}


bool Bitmap::save(std::string filename)
{
    if (image_ != NULL) {
        image_->save(filename.c_str());
        return true;
    }
    return false;
}


Color Bitmap::getColor(Vector2 point)
{
    if (!image_) {
        return Color(0,0,0);
    }

    float col[3];
    
    col[0] = image_->linear_atXY (point.x-1, point.y-1, 0);
	col[1] = image_->linear_atXY (point.x-1, point.y-1, 1);
	col[2] = image_->linear_atXY (point.x-1, point.y-1, 2);

    return Color(float(col[0])/255.0f ,float(col[1])/255.0f ,float(col[2])/255.0f);
}


bool Bitmap::setColor(Vector2 point, Color color)
{
    if (!image_) {
        return false;
    }

    //color.capColor();

    float col[3];

    /*
    col[0] = unsigned(char(color.x * 255.0f));
    col[1] = unsigned(char(color.y * 255.0f));
    col[2] = unsigned(char(color.z * 255.0f));
    */
    col[0] = color.x * 255.0f;
    col[1] = color.y * 255.0f;
    col[2] = color.z * 255.0f;

    image_->draw_point(int(point.x)-1 ,int(point.y)-1 ,col);

    return true;
}


int Bitmap::width()
{
    if (image_ != NULL)
        return image_->width();
    return 0;
}


int Bitmap::height()
{
    if (image_ != NULL)
        return image_->height();
    return 0;
}


inline TiXmlElement* Bitmap::getXml()
{
    TiXmlElement* root = Image::getXml();

    root->SetAttribute("filename", filename_);

    return root;
}


bool Bitmap::loadXml(TiXmlElement* pElem, std::string path, LinkList *linkList)
{
    init();

    Image::loadXml(pElem, path, linkList);

    filename_ = "";
    pElem->QueryStringAttribute("filename", &filename_);

    if (filename_ != "") {
        std::string absfilename = pathJoin(path, filename_);
        return load(absfilename);
    }

    return false;
}
