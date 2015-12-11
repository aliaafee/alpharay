/* vim: set ts=4 ss=4 sw=4 noet ai cindent : */

#include "canvas.h"


void Canvas::init() 
{
	Image::init();
	
	width_ = 1;
	height_ = 1;

	backgroudColor_ = Color(1, 1, 1);
}


Color Canvas::getColor(Vector2 point)
{
	for (unsigned long i = 0; i < objects.size(); i++) {
		if (objects[i]->isInside(point)) {
			return objects[i]->getColor(point);
		}
	}

	return backgroudColor_;

	/*
	Vector2 center(0.5,0.5);
	Vector2 dir = point - center;
	float m = dir.magnitude();
	if (m <= 0.5) {
		return Color (1,0,0);
	} else {
		return Color(1,1,1);
	}
	*/
	
	/*
	Vector2 p1(0.5,0.5);
	Vector2 p2(0.75,0.75);
	if (point.x > p1.x && point.x < p2.x) {
		if (point.y > p1.y && point.y < p2.y) {
			return Color(1,0,0);
		}
	}
	return Color(1,1,1);
	*/
	/*
	if (point.x > 0.5 && point.y > 0.5) {
        return Color(1,1,1);
    }

    if (point.x < 0.5 && point.y < 0.5) {
        return Color(1,1,1);
    }

    if (point.x > 0.5 && point.y < 0.5) {
        return Color(1,0,0);
    }

	return Color(1,0,0);
	*/
}


void Canvas::transform()
{
	for (unsigned long i = 0; i < objects.size(); i++) {
		objects[i]->transform();
	}
}


TiXmlElement* Canvas::getXml()
{
	TiXmlElement* root = Image::getXml();

    root->SetAttribute("width", width_);
	root->SetAttribute("height", height_);
	root->SetAttribute("backgroundcolor", backgroudColor_.str());

	for (unsigned long i = 0; i < objects.size(); i++) {
        root->LinkEndChild( objects[i]->getXml() );
    }

    return root;
}


bool Canvas::loadXml(TiXmlElement* pElem, std::string path, LinkList *linkList)
{
	init();

    Image::loadXml(pElem, path, linkList);

	pElem->QueryValueAttribute <int> ("width", &width_);
	pElem->QueryValueAttribute <int> ("height", &height_);
	pElem->QueryValueAttribute <Vector> ("backgroundcolor", &backgroudColor_);

	pElem = pElem->FirstChildElement();

	for (pElem; pElem; pElem = pElem->NextSiblingElement()) {
		std::string name(pElem->Value());

		Object2d* object = NULL;

		if (name == "circle") {
			object = new Circle("");
		} else if (name == "square") {
			object = new Square("");
		} else if (name == "polygon") {
			object = new Polygon("");
		}

		if (object) {
			object->loadXml(pElem, path, linkList);
			objects.push_back(object);
		}
    }

    return true;	
}
