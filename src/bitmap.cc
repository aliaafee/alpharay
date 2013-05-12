/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */

#include "bitmap.h"


void Bitmap::init() 
{ 
    Image::init(); 
    
    image_ = NULL;

#ifdef OPENGL
    preview_ = NULL;
#endif
}


Bitmap::~Bitmap()
{
    if (image_ != NULL) {
        delete image_;
    }

#ifdef OPENGL
    if (preview_ != NULL) {
        delete preview_;
    }
#endif
}


bool Bitmap::create(int width, int height)
{
    if (image_ != NULL) {
        delete image_;
    }

#ifdef OPENGL
    if (preview_ != NULL) {
        delete preview_;
    }
#endif

    init();

    image_ = new cimg_library::CImg<float>(width, height, 1, 3, 0);

    return true;
}


bool Bitmap::load(std::string filename)
{
    if (image_ != NULL) {
        delete image_;
    }

#ifdef OPENGL
    if (preview_ != NULL) {
        delete preview_;
    }
#endif

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


void Bitmap::enableDisplay()
{
#ifdef OPENGL
    if (preview_ == NULL) {
        preview_ = new GLImage(width(), height());
    }
#endif
}


void Bitmap::display()
{
#ifdef OPENGL
    if (image_ != NULL && preview_ != NULL) {
        preview_->display();
    }
#else
    std::cout << "Compile with OpenGL to display preview"
#endif
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
    if (!image_)
        return false;

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

#ifdef OPENGL
    if (!preview_)
        return true;

    preview_->setColor(point, color);
#endif

    return true;
}


void Bitmap::bloom(float size, float highpass) {
    if (!image_)
        return;

    CImg<float> blured = *image_;

    //High pass filter
    cimg_forXYZ(blured,x,y,z) {
        if ( ( blured(x,y,z,0) + blured(x,y,z,1) + blured(x,y,z,2) ) > highpass) {
            blured(x,y,z,0) = blured(x,y,z,0);
            blured(x,y,z,1) = blured(x,y,z,1);
            blured(x,y,z,2) = blured(x,y,z,2);
        } else {
            blured(x,y,z,0) = 0.0;
            blured(x,y,z,1) = 0.0;
            blured(x,y,z,2) = 0.0;
        }
    }

    //Blur
    blured.blur(size,size,1);

    //Combine
    cimg_forXYZC((*image_),x,y,z,k) {
        (*image_)(x,y,z,k) += blured(x,y,z,k);
    }
}


void Bitmap::correctExposure(float exposure) {
    if (!image_)
        return;

    bloom(40, 60000);

    //Correct Exposure
    cimg_forXYZC((*image_),x,y,z,k) {
        (*image_)(x,y,z,k) = (1.0f - expf(((*image_)(x,y,z,k)/255.0f) * exposure)) * 255.0f;
    }

#ifdef OPENGL
    if (!preview_)
        return;

    copyTo(preview_);
#endif
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
