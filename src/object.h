/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */

#ifndef _OBJECT_H_
#define _OBJECT_H_

#include <string>
#include <tinyxml.h>

#include "vector.h"
#include "ray.h"
#include "matrix4.h"
#include "linklist.h"
#include "map.h"
#include "material.h"

class Object
{
public:
    std::string xmlName;
    std::string name_;

	Material *material_;

	Vector position_;
	Vector rotation_;
	Vector scale_;

	float normalDirection_;
	Object* parent_;

    virtual void init() { xmlName = "object"; }

    Object() {
        init();
        name_ = "";

        position_ = Vector(0,0,0);
        rotation_ = Vector(0,0,0);
        scale_ = Vector(1,1,1);

        material_ = NULL; 

        normalDirection_ = 1;
        parent_ = NULL;
    }
	Object(std::string name, Vector position, Material *material) {
        init();
        name_ = name;

        position_ = position;
        rotation_ = Vector(0,0,0);
        scale_ = Vector(1,1,1);

        material_ = material; 

        normalDirection_ = 1;
        parent_ = NULL;
    };
	
	virtual ~Object() {};

    virtual Vector normal(Vector localPoint, UVTriangle *uvtriangle) {
        return Vector(0, 0, 0);
    };

	virtual Object* intersection(
            Ray &ray,
            Vector *intersectionPoint=NULL,
            Vector *intersectionPointLocal=NULL,
            UVTriangle **intersectionUVTriangle=NULL,
            float *distance=NULL) {
        return NULL;
    };

    virtual void transform() { 
        GenerateTransformationMatrix(); 
        material_->transform();
    }

    virtual bool loadXml(TiXmlElement* pElem, LinkList <Material> *linkMaterials);
    virtual TiXmlElement* getXml();

protected:
	void GenerateTransformationMatrix();
	Vector transformPoint(Vector);
	Vector transformPointInv(Vector);
	Vector transformDisplacement(Vector);
	Vector transformDisplacementInv(Vector);
    Vector transformNormal(Vector);
    
private:
	Matrix4 transMatrix; // TRS
	Matrix4 transMatrixInv; // S^R^T^
	Matrix4 transMatrixND; //RS
	Matrix4 transMatrixNormal; // RS^
	Matrix4 transMatrixNDInv; // S^R^
};


#endif // _OBJECT_H_