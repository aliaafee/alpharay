/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */

#include "map.h"


TiXmlElement* Map::getXml()
{
    TiXmlElement* root = XmlObjectNamed::getXml();

    return root;
}


bool Map::loadXml(TiXmlElement* pElem, std::string path, LinkList *linkList) 
{
    XmlObjectNamed::loadXml(pElem, path);
}

void Map2d::init()
{
    Map::init();

    image_ = NULL;
    imageScale_ = Vector2(1, 1);
}

void Map2dPlane::init()
{
    Map2d::init();

    position_ = Vector(0, 0, 0);
    rotation_ = Vector(0, 0, 0);
    scale_ = Vector(1, 1, 1);

    image_ = NULL;
    imageScale_ = Vector2(1, 1);
}


void Map2dPlane::transform() 
{
	Matrix4 scale(
		scale_.x , 0, 0, 0,
	    0, scale_.y, 0, 0,
	    0, 0, scale_.z, 0, 
	    0, 0, 0, 1
	);
	Matrix4 iscale = scale.getInverse();
	
	Matrix4 xrot(
	    1,  0,  0,  0,
		0,  cos(rotation_.x),  -1*sin(rotation_.x),  0,
		0,  sin(rotation_.x),  cos(rotation_.x),  0,
		0,  0,  0,  1
	);
	Matrix4 ixrot = xrot.getInverse();
	
	Matrix4 yrot(
	    cos(rotation_.y),  0,  sin(rotation_.y),  0,
		0,  1,  0,  0,
		-1*sin(rotation_.y),  0,  cos(rotation_.y),  0,
		0,  0,  0,  1
	);
	Matrix4 iyrot = yrot.getInverse();
	
	Matrix4 zrot(
	    cos(rotation_.z),  -1*sin(rotation_.z),  0,  0,
		sin(rotation_.z),  cos(rotation_.z),  0,  0,
		0,  0,  1,  0,
		0,  0,  0,  1
	);
	Matrix4 izrot = zrot.getInverse();
	
	Matrix4 translation(
	    1,  0,  0,  position_.x,
		0,  1,  0,  position_.y,
		0,  0,  1,  position_.z,
		0,  0,  0,  1
	);
	Matrix4 itranslation = translation.getInverse();

	transMatrix = translation * xrot * yrot * zrot * scale;
	transMatrixInv = iscale * izrot * iyrot * ixrot * itranslation;
}


Vector Map2dPlane::transformPoint(Vector vector) 
{
    Vector result;
    V_MUL_MATRIX(result, vector, transMatrix);
    return result;
}


Vector Map2dPlane::transformPointInv(Vector vector) 
{
    Vector result;
    V_MUL_MATRIX(result, vector, transMatrixInv);
    return result;
}


Color Map2d::color(Vector2 point2)
{
    if (image_ == NULL)
        return Color(0,0,0);

    float u = point2.x / imageScale_.x;
    float v = point2.y / imageScale_.y;

    u = u - float(int(u));
    v = v - float(int(v));

    if (u < 0) { u = 1.0f + u; }
    if (v < 0) { v = 1.0f + v; }
            
    u = u * float(image_->width());
    v = v * float(image_->height());

    point2.x = u;
    point2.y = v;
            
    return Color(image_->getColor(point2));
}


Color Map2d::color(Vector  point, Vector2 point2 = Vector2(0,0))
{
    return color(point2);
}


Color Map2dPlane::color(Vector  point, Vector2 point2 = Vector2(0,0))
{
    point = transformPointInv(point);

    point2.x = -0.5f - (point.x/2.0f);
    point2.y = -0.5f - (point.y/2.0f);

    return Map2d::color(point2);
}


Color Map2dCylindrical::color(Vector  point, Vector2 point2 = Vector2(0,0))
{
    point = transformPointInv(point);

    point2.x = atan2(point.y, point.x) / (M_PI*2) * -1;
    point2.y = -0.5 - (point.z/2);

    return Map2d::color(point2);
}


Color Map2dSpherical::color(Vector  point, Vector2 point2 = Vector2(0,0))
{
    point = transformPointInv(point);

    point.toSpherical();
    point2.x = point.z / (M_PI*2) * -1;
    point2.y = point.y / (M_PI);

    return Map2d::color(point2);
}


TiXmlElement* Map2d::getXml()
{
    TiXmlElement* root = Map::getXml();

    root->SetAttribute("imagescale", imageScale_.str());

    if (image_ == NULL) {
        root->SetAttribute("image", "");
    } else {
        root->SetAttribute("image", image_->name());
    }

    return root;
}


bool Map2d::loadXml(TiXmlElement* pElem, std::string path, LinkList *linkList) 
{
    init();

    Map::loadXml(pElem, path, linkList);

    pElem->QueryValueAttribute <Vector2> ("imagescale", &imageScale_);

    std::string imagename = "";
    pElem->QueryStringAttribute ("image", &imagename);
    linkList->add(imagename, &image_);
}


TiXmlElement* Map2dPlane::getXml()
{
    TiXmlElement* root = Map2d::getXml();

    root->SetAttribute("name", name_);
    root->SetAttribute("position", position_.str());
    root->SetAttribute("rotation", rotation_.str());
    root->SetAttribute("scale", scale_.str());

    return root;
}


bool Map2dPlane::loadXml(TiXmlElement* pElem, std::string path, LinkList *linkList) 
{
    init();

    Map2d::loadXml(pElem, path, linkList);

    pElem->QueryStringAttribute ("name", &name_);
    pElem->QueryValueAttribute <Vector> ("position", &position_);
    pElem->QueryValueAttribute <Vector> ("rotation", &rotation_);
    pElem->QueryValueAttribute <Vector> ("scale", &scale_);
}


