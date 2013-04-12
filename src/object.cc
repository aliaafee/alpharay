/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */

#include "object.h"

void Object::GenerateTransformationMatrix() {
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
	transMatrixND = xrot * yrot * zrot * scale;
	transMatrixNormal = xrot * yrot * zrot * iscale;
	transMatrixNDInv = iscale * izrot * iyrot * ixrot;
}

Vector Object::transformPoint(Vector vector) {
	if (parent_ != NULL) {
		vector = parent_->transformPoint(vector);
	}
	//return vector * transMatrix;
    Vector result;
    V_MUL_MATRIX(result, vector, transMatrix);
    return result;
}

Vector Object::transformPointInv(Vector vector) {
	if (parent_ != NULL) {
		vector = parent_->transformPointInv(vector);
	}
	//return vector * transMatrixInv;
    Vector result;
    V_MUL_MATRIX(result, vector, transMatrixInv);
    return result;
}

Vector Object::transformDisplacement(Vector vector) {
	if (parent_ != NULL) {
		vector = parent_->transformDisplacement(vector);
	}
	//return vector * transMatrixND;
    Vector result;
    V_MUL_MATRIX(result, vector, transMatrixND);
    return result;
}

Vector Object::transformDisplacementInv(Vector vector) {
	if (parent_ != NULL) {
		vector = parent_->transformDisplacementInv(vector);
	}
	//return vector * transMatrixNDInv;
    Vector result;
    V_MUL_MATRIX(result, vector, transMatrixNDInv);
    return result;
}

Vector Object::transformNormal(Vector normal) {
    if (parent_ != NULL) {
		normal = parent_->transformNormal(normal);
	}
	//return normal * transMatrixNormal;
    Vector result;
    V_MUL_MATRIX(result, normal, transMatrixNormal);
    return result;
}

bool Object::loadXml(TiXmlElement* pElem, LinkList <Material> *linkMaterials) {
    name_ = "object"; 

    pElem->QueryStringAttribute ("name", &name_);
    pElem->QueryValueAttribute <Vector> ("position", &position_);
    pElem->QueryValueAttribute <Vector> ("rotation", &rotation_);
    pElem->QueryValueAttribute <Vector> ("scale", &scale_);

    std::string matname = "";
    pElem->QueryStringAttribute ("material", &matname);
    if (matname != "") {
        linkMaterials->add(matname, &material_);
    } else {
        material_ = NULL;
    }

    return true;
}

TiXmlElement* Object::getXml() {
    TiXmlElement* root = new TiXmlElement(xmlName.c_str());

    root->SetAttribute("name", name_);
    root->SetAttribute("position", position_.str());
    root->SetAttribute("rotation", rotation_.str());
    root->SetAttribute("scale", scale_.str());

    if (material_ == NULL) {
        root->SetAttribute("material", "");
    } else {
        root->SetAttribute("material", material_->name_);
    }


    return root;
}
