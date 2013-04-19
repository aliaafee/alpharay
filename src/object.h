/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */

#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "baseobject.h"

class Object : public BaseObject, virtual public XmlObjectNamed
{
    public:
        void init();

        Object() 
            : XmlObjectNamed ("object", "unnamed") 
            { init(); parent_ = NULL; }

        Object(std::string name, BaseObject* parent=NULL) 
            : XmlObjectNamed ("object", name) 
            { init(); parent_ = parent; }

        ~Object() {};

        virtual void transform();

        virtual Bounds bounds()  { return Bounds(Vector(0,0,0), Vector(0,0,0)); }
        
        virtual Vector normal(Vector point) { return Vector(0, 0, 1); }
        virtual Color color(Vector point, Material* material) 
            { Vector2 point2; return material->color(point, point2); }

        virtual BaseObject* intersection(Ray &ray, float *t, float limit) { return NULL; }

        virtual Vector transformPoint(Vector &point);
        virtual Vector transformPointInv(Vector &point);
        virtual Vector transformNormal(Vector &point);
        virtual Vector transformNormalInv(Vector &point);
        virtual Vector transformDisplacement(Vector &point);
    	virtual Vector transformDisplacementInv(Vector &point);
        virtual Ray transformRay(Ray ray);
        
        virtual TiXmlElement* getXml();
        virtual bool loadXml(TiXmlElement* pElem, std::string path, LinkList <Material> *linkMaterials);

    //protected:
        Vector position_;
    	Vector rotation_;
    	Vector scale_;

    private:
        Matrix4 transMatrix; // TRS
        Matrix4 transMatrixInv; // S^R^T^
        Matrix4 transMatrixND; //RS
        Matrix4 transMatrixNDInv; // S^R^
        Matrix4 transMatrixNormal; // RS^
        Matrix4 transMatrixNormalInv; // SR^
};




/*
#include <string>
#include <tinyxml.h>

#include "vector.h"
#include "ray.h"
#include "matrix4.h"
#include "linklist.h"
#include "map.h"
#include "material.h"

#include "bbox.h"

class Object
{
public:
    std::string xmlName;
    std::string name_;

	Material *material_;

	Vector position_;
	Vector rotation_;
	Vector scale_;

    BBox bbox_;

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

    virtual Vector normal(Vector localPoint, UVTriangle *uvtriangle, Material *material) {
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
        if (material_ != NULL) {
            material_->transform();
        }
    }

    virtual void setBounds() {
        ;
    }

    virtual bool loadXml(TiXmlElement* pElem, LinkList <Material> *linkMaterials);
    virtual TiXmlElement* getXml();

    Vector transformNormalInv(Vector);
protected:
	void GenerateTransformationMatrix();
	Vector transformPoint(Vector);
	Vector transformPointInv(Vector);
	Vector transformDisplacement(Vector);
	Vector transformDisplacementInv(Vector);
    Vector transformNormal(Vector);
    Ray transformRay(Ray);
    
private:
	Matrix4 transMatrix; // TRS
	Matrix4 transMatrixInv; // S^R^T^
	Matrix4 transMatrixND; //RS
	Matrix4 transMatrixNormal; // RS^
    Matrix4 transMatrixNormalInv; // SR^
	Matrix4 transMatrixNDInv; // S^R^
};
*/

#endif // _OBJECT_H_
