/* vim: set ts=4 ss=4 sw=4 noet ai cindent : */

#include "image.h"

void Image::copyTo(Image* target)
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


bool Image::loadXml(TiXmlElement* pElem, std::string path, LinkList *linkList)
{
	XmlObjectNamed::loadXml <LinkList> (pElem, path, linkList);

    return true;
}


void Checker2d::init()
{
	width_ = 1; 
	height_ = 1; 
	
	addEditable(new Editable<Color>("color1", &color1_, Color(0, 0, 0)));
	addEditable(new Editable<Color>("color2", &color2_, Color(0, 0, 0)));
}


Color Checker2d::getColor(Vector2 point)
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

