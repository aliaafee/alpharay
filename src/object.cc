/* vim: set ts=4 ss=4 sw=4 noet ai cindent : */

#include "object.h"


void Object::init()
{
    BaseObject::init(); 
    XmlObjectNamed::init();

    position_ = Vector(0, 0, 0);
    rotation_ = Vector(0, 0, 0);
    scale_ = Vector(1, 1, 1);
}

void Object::transform() {
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
    transMatrixND = zrot * yrot * xrot * scale;
    transMatrixNDInv = iscale * ixrot * iyrot * izrot;
	transMatrixNormal = zrot * yrot * xrot * iscale;
    transMatrixNormalInv = scale * ixrot * iyrot * izrot;
}


Vector Object::transformPoint(Vector &vector) {
	if (parent_ != NULL) {
		vector = parent_->transformPoint(vector);
	}
    Vector result;
    V_MUL_MATRIX(result, vector, transMatrix);
    return result;
}


Vector Object::transformPointInv(Vector &vector) {
	if (parent_ != NULL) {
		vector = parent_->transformPointInv(vector);
	}
    Vector result;
    V_MUL_MATRIX(result, vector, transMatrixInv);
    return result;
}


Vector Object::transformDisplacement(Vector &vector) {
	if (parent_ != NULL) {
		vector = parent_->transformDisplacement(vector);
	}
    Vector result;
    V_MUL_MATRIX(result, vector, transMatrixND);
    return result;
}


Vector Object::transformDisplacementInv(Vector &vector) {
	if (parent_ != NULL) {
		vector = parent_->transformDisplacementInv(vector);
	}
    Vector result;
    V_MUL_MATRIX(result, vector, transMatrixNDInv);
    return result;
}


Vector Object::transformNormal(Vector &normal) {
    if (parent_ != NULL) {
		normal = parent_->transformNormal(normal);
	}
    Vector result;
    V_MUL_MATRIX(result, normal, transMatrixNormal);
    return result;
}


Vector Object::transformNormalInv(Vector &normal) {
    return normal * transMatrixNormalInv;
}


Ray Object::transformRay(Ray ray) {
    Ray result;
    result.position_ = transformPointInv(ray.position_);
	result.direction_ = transformDisplacementInv(ray.direction_);
    result.shadowRay_ = ray.shadowRay_;

    return result;
}


bool Object::loadXml(TiXmlElement* pElem, std::string path, LinkList *linkList) {
    init();

    XmlObjectNamed::loadXml(pElem, path);

    pElem->QueryValueAttribute <Vector> ("position", &position_);
    pElem->QueryValueAttribute <Vector> ("rotation", &rotation_);
    pElem->QueryValueAttribute <Vector> ("scale", &scale_);

    std::string matname = "";
    pElem->QueryStringAttribute ("material", &matname);
    linkList->add(matname, &material_);

    return true;
}

TiXmlElement* Object::getXml() {
    TiXmlElement* root = XmlObjectNamed::getXml();

    root->SetAttribute("position", position_.str());
    root->SetAttribute("rotation", rotation_.str());
    root->SetAttribute("scale", scale_.str());

    if (material_ == NULL) {
        root->SetAttribute("material", "");
    } else {
        root->SetAttribute("material", material_->name());
    }


    return root;
}
