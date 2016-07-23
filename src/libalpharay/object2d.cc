/* vim: set ts=4 ss=4 sw=4 noet ai cindent : */

#include "object2d.h"


void Object2d::init()
{ 
	addEditable(new Editable<Vector2>("position", &position_, Vector2(0, 0)));
	addEditable(new Editable<float>("rotation", &rotation_, 0));
	addEditable(new Editable<Vector2>("scale", &scale_, Vector2(1, 1)));

	addEditable(new Editable<Color>("fillcolor", &fillColor_, Color(0,0,0)));
	addEditable(new Editable<Color>("strokecolor", &strokeColor_, Color(0,0,0)));
	addEditable(new Editable<float>("opacity", &opacity_, 1));
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

	if (newPoint.x >= -0.5 && newPoint.x <= 0.5) {
		if (newPoint.y >= -0.5 && newPoint.y <= 0.5) {
			return true;
		}
	}

	return false;
}


void Polygon::init()
{
	addEditable(new BaseEditable("points", &strPoints_, ""));
}


bool Polygon::isInside(Vector2 &point) 
{
	/* from http://alienryderflex.com/polygon/
	 */
	
	Vector2 o = transformPointInv(point);

	bool oddNodes = false;
	int i, j = points_.size() - 1;
	for (i = 0; i < points_.size(); i++) {
		if ((points_[i].y < o.y && points_[j].y  >= o.y) || (points_[j].y < o.y && points_[i].y >= o.y)) {
			if (points_[i].x + (o.y-points_[i].y)/(points_[j].y-points_[i].y)*(points_[j].x-points_[i].x)<o.x) {
				oddNodes = !oddNodes;
			}
		}
		j = i;
	}

	return oddNodes;

}


bool Polygon::loadXml(TiXmlElement* pElem, std::string path) 
{
	Object2d::loadXml(pElem, path);

	std::stringstream ss;
	ss << strPoints_;
    std::string token;	
	while(std::getline(ss, token, ' ')) {
		std::stringstream ss2;
		ss2 << token;
		Vector2 point;
		ss2 >> point;
		points_.push_back(point);
	}
	
    return true;
	
}


TiXmlElement* Polygon::getXml() 
{
	strPoints_ = "";
	for (unsigned long i = 0; i < points_.size(); i++) {
		strPoints_ += points_[i].str() + " ";
	}

    TiXmlElement* root = Object2d::getXml();
	
    return root;
}


bool Picture::isInside(Vector2 &point) 
{ 
	Vector2 newPoint = transformPointInv(point);

	if (newPoint.x >= -0.5 && newPoint.x <= 0.5) {
		if (newPoint.y >= -0.5 && newPoint.y <= 0.5) {
			return true;
		}
	}

	return false;
}


void Picture::init()
{
	addEditable(new Editable<Vector2>("tile", &tile_, Vector2(1, 1)));
	addEditableLink(new EditableLink<Image>("image", &image_));
	addEditableLink(new EditableLink<Image>("mask", &mask_));
}


Color Picture::getColor(Vector2 &point)
{
	Vector2 newPoint = transformPointInv(point);

	newPoint.x += 0.5;
	newPoint.y += 0.5;

	newPoint.x = newPoint.x * tile_.x;
	newPoint.y = newPoint.y * tile_.y;

	if (image_ == NULL)
        return Color(0,0,0);
	
	float u = newPoint.x;
    float v = newPoint.y;

    u = u - float(int(u));
    v = v - float(int(v));

    if (u < 0) { u = 1.0f + u; }
    if (v < 0) { v = 1.0f + v; }

	float ut,vt;
	
    ut = u * float(image_->width());
    vt = v * float(image_->height());

    newPoint.x = ut+1;
    newPoint.y = vt+1;
	
	Color color = image_->getColor(newPoint);

	if (mask_ != NULL) {
		ut = u * float(mask_->width());
		vt = v * float(mask_->height());

		newPoint.x = ut+1;
		newPoint.y = vt+1;

		color.a = mask_->getColor(newPoint).intensity();
	} else {
		color.a = 1;
	}
            
    return color;
}
