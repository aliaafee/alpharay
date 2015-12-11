/* vim: set ts=4 ss=4 sw=4 noet ai cindent : */

#include "object2d.h"


void Object2d::init()
{ 
	XmlObjectNamed::init(); 
	
	position_ = Vector2(0,0); 
	rotation_ = 0; 
	scale_ = Vector2(1,1); 
}


void Object2d::transform()
{
	Matrix4 scale(
		scale_.x , 0, 0, 0,
	    0, scale_.y, 0, 0,
	    0, 0, 1, 0, 
	    0, 0, 0, 1
	);
	Matrix4 iscale = scale.getInverse();
		
	Matrix4 zrot(
	    cos(rotation_),  -1*sin(rotation_),  0,  0,
		sin(rotation_),  cos(rotation_),  0,  0,
		0,  0,  1,  0,
		0,  0,  0,  1
	);
	Matrix4 izrot = zrot.getInverse();
	
	Matrix4 translation(
	    1,  0,  0,  position_.x,
		0,  1,  0,  position_.y,
		0,  0,  1,  0,
		0,  0,  0,  1
	);
	Matrix4 itranslation = translation.getInverse();

	transMatrix = translation * zrot * scale;
	transMatrixInv = iscale * izrot * itranslation;
}


Vector2 Object2d::transformPoint(Vector2 &point)
{
	Vector point3d(point.x, point.y, 0);
	Vector result;
    V_MUL_MATRIX(result, point3d, transMatrix);
    return Vector2(result.x, result.y);
}


Vector2 Object2d::transformPointInv(Vector2 &point)
{
	Vector point3d(point.x, point.y, 0);
	Vector result;
    V_MUL_MATRIX(result, point3d, transMatrixInv);
    return Vector2(result.x, result.y);
}


bool Object2d::loadXml(TiXmlElement* pElem, std::string path, LinkList *linkList) 
{
    init();

    XmlObjectNamed::loadXml(pElem, path);

    pElem->QueryValueAttribute <Vector2> ("position", &position_);
    pElem->QueryFloatAttribute("rotation", &rotation_);
    pElem->QueryValueAttribute <Vector2> ("scale", &scale_);

	pElem->QueryValueAttribute <Vector> ("fillcolor", &fillColor_);
	pElem->QueryValueAttribute <Vector> ("strokecolor", &strokeColor_);

    return true;
}


TiXmlElement* Object2d::getXml() 
{
    TiXmlElement* root = XmlObjectNamed::getXml();

    root->SetAttribute("position", position_.str());
    root->SetAttribute("rotation", ftos(rotation_));
    root->SetAttribute("scale", scale_.str());

	root->SetAttribute("fillcolor", fillColor_.str());
	root->SetAttribute("strokecolor", strokeColor_.str());

    return root;
}


bool Circle::isInside(Vector2 &point) 
{ 
	Vector2 newPoint = transformPointInv(point);
	
	Vector2 center(0,0);
	Vector2 dir = newPoint - center;
	float m = dir.magnitude();
	if (m <= 0.5) {
		return true;
	} else {
		return false;
	}
}


bool Square::isInside(Vector2 &point) 
{ 
	Vector2 newPoint = transformPointInv(point);

	if (newPoint.x >= -1 && newPoint.x <= 1) {
		if (newPoint.y >= -1 && newPoint.y <= 1) {
			return true;
		}
	}

	return false;
}


bool Polygon::isInside(Vector2 &point) 
{
	/* from http://alienryderflex.com/polygon/
	 */
	
	Vector2 o = transformPointInv(point);

	bool oddNodes = false;
	int i, j = points_.size() - 1;
	for (i = 0; i < points_.size(); i++) {
		if (points_[i].y < o.y && points_[j].y  >= o.y || points_[j].y < o.y && points_[i].y >= o.y ) {
			if (points_[i].x + (o.y-points_[i].y)/(points_[j].y-points_[i].y)*(points_[j].x-points_[i].x)<o.x) {
				oddNodes = !oddNodes;
			}
		}
		j = i;
	}

	return oddNodes;

}


bool Polygon::loadXml(TiXmlElement* pElem, std::string path, LinkList *linkList) 
{
	init();

    Object2d::loadXml(pElem, path, linkList);

	std::string points;
	pElem->QueryStringAttribute ("points", &points);

	std::stringstream ss(points);
    std::string token, p1, p2;	
	while(std::getline(ss, token, ' ')) {
		std::stringstream ss2(token);
		Vector2 point;
		ss2 >> point;
		points_.push_back(point);
	}
	
    return true;
	
}


TiXmlElement* Polygon::getXml() 
{
    TiXmlElement* root = Object2d::getXml();

	std::string points;
	for (unsigned long i = 0; i < points_.size(); i++) {
		points += points_[i].str() + " ";
	}

	root->SetAttribute("points", points);
	
    return root;
}

