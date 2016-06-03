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
	Color color = backgroudColor_;
	Color objectcolor;
	float alpha;

	for (unsigned long i = 0; i < objects.size(); i++) {
		if (objects[i]->isInside(point)) {
			objectcolor = objects[i]->getColor(point);
			alpha = objectcolor.a;
			color = color * ( 1-alpha) + objectcolor * alpha;
		}
	}

	return color;
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

	for (; pElem; pElem = pElem->NextSiblingElement()) {
		std::string name(pElem->Value());

		Object2d* object = NULL;

		if (name == "circle") {
			object = new Circle("");
		} else if (name == "square") {
			object = new Square("");
		} else if (name == "polygon") {
			object = new Polygon("");
		} else if (name == "picture") {
			object = new Picture("");
		}

		if (object) {
			object->loadXml(pElem, path, linkList);
			objects.push_back(object);
		}
    }

    return true;	
}
