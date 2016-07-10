/* vim: set ts=4 ss=4 sw=4 noet ai cindent : */

#include "map.h"


bool Map::loadXml(TiXmlElement* pElem, std::string path, LinkList *linkList) 
{
	XmlObjectNamed::loadXml <LinkList> (pElem, path, linkList);

	return true;
}

void Map2d::init()
{
	addEditable(new Editable<Vector2>("imagescale", &imageScale_, Vector2(1, 1)));

	addEditableLink(new EditableLink<Image>("image", &image_));
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


void Map2dPlane::init()
{
	addEditable(new Editable<Vector>("position", &position_, Vector(0, 0, 0)));
	addEditable(new Editable<Vector>("rotation", &rotation_, Vector(0, 0, 0)));
	addEditable(new Editable<Vector>("scale", &scale_, Vector(1, 1, 1)));
}


void Map2dPlane::transform() 
{
	Map2d::transform();
	
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

	transMatrix = translation * zrot * yrot * xrot * scale;
	transMatrixInv = iscale * ixrot * iyrot * izrot * itranslation;
	transMatrixNormal = zrot * yrot * xrot * iscale;
    transMatrixNormalInv = scale * ixrot * iyrot * izrot;

	Vector ts_tangent = Vector(1, 0, 0);
	os_tangent_ = transformNormal(ts_tangent);
	os_tangent_.normalize();

	Vector ts_bitangent = Vector(0, 1, 0);
	os_bitangent_ = transformNormal(ts_bitangent);
	os_bitangent_.normalize();
}


Vector Map2dPlane::transformPoint(Vector &vector) 
{
    Vector result;
    V_MUL_MATRIX(result, vector, transMatrix);
    return result;
}


Vector Map2dPlane::transformPointInv(Vector &vector) 
{
    Vector result;
    V_MUL_MATRIX(result, vector, transMatrixInv);
    return result;
}


Vector Map2dPlane::transformNormal(Vector &normal) {
    Vector result;
    V_MUL_MATRIX(result, normal, transMatrixNormal);
    return result;
}


Vector Map2dPlane::transformNormalInv(Vector &normal) {
	Vector result;
	V_MUL_MATRIX(result, normal, transMatrixNormalInv);
    return result;
}


Color Map2dPlane::color(Vector  point, Vector2 point2 = Vector2(0,0))
{
    point = transformPointInv(point);

    point2.x = -0.5f - (point.x/2.0f);
    point2.y = -0.5f - (point.y/2.0f);

    return Map2d::color(point2);
}

void Map2dPlane::getTangents(Vector& point, Vector* os_tangent, Vector* os_bitangent) {
	*os_tangent = os_bitangent_;
	*os_bitangent = os_bitangent_;
}


void Map2dCylindrical::transform()
{
	Map2dPlane::transform();

	Vector ts_tangent = Vector(0, 0, 1);
	os_tangent_ = transformNormal(ts_tangent);
	os_tangent_.normalize();
}
	

Color Map2dCylindrical::color(Vector  point, Vector2 point2 = Vector2(0,0))
{
    point = transformPointInv(point);

    point2.x = atan2(point.y, point.x) / (M_PI*2);
    point2.y = -0.5 - (point.z/2);

    return Map2d::color(point2);
}


void Map2dCylindrical::getTangents(Vector& point, Vector* os_tangent, Vector* os_bitangent) {
	*os_tangent = os_bitangent_;
	point = transformPointInv(point);
	point.normalize();

	*os_bitangent = point % os_tangent_;
}


Color Map2dSpherical::color(Vector  point, Vector2 point2 = Vector2(0,0))
{
    point = transformPointInv(point);

    point.toSpherical();
    point2.x = point.z / (M_PI*2);
    point2.y = point.y / (M_PI);

    return Map2d::color(point2);
}


void Map2dSpherical::getTangents(Vector& point, Vector* os_tangent, Vector* os_bitangent) {
	point = transformPointInv(point);
	point.normalize();

	*os_bitangent = point % pole_;
	*os_tangent = point % *os_bitangent;
}
