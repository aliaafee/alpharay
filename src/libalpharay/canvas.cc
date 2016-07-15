/* vim: set ts=4 ss=4 sw=4 noet ai cindent : */

#include "canvas.h"


void Canvas::init() 
{
	//width_ = 1;
	//height_ = 1;

	//backgroundColor_ = Color(1, 1, 1);

	addEditable(new Editable<Color>("backgroundcolor", &backgroundColor_, Color(1, 1, 1)));

	addEditable(new Editable<int>("width", &width_, 1));
	addEditable(new Editable<int>("height", & height_, 1));
}


Color Canvas::getColor(Vector2 point)
{
	Color color = backgroundColor_;
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

	for (unsigned long i = 0; i < objects.size(); i++) {
        root->LinkEndChild( objects[i]->getXml() );
    }

    return root;
}


bool Canvas::loadXml(TiXmlElement* pElem, std::string path, LinkList *linkList)
{
	init();

    Image::loadXml(pElem, path, linkList);
	
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
