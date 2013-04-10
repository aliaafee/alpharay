/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */

#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include <tinyxml.h>
#include <string>

#include "vector.h"
#include "texture.h"
#include "map.h"

#include "linklist.h"

class Material
{
public:
    std::string xmlName;

    std::string name_;

    Vector diffuseColor_;
	Vector highlightColor_;
	float reflectivity_;
    float opticDensity_;
    bool dielectric_;
    float scatterFactor_;
    int scatterSamples_;
	float ka_; //ambient reflection coefficient
	float kd_; //diffuse reflection coefficient
	float ks_; //specular reflection coefficient
	float alpha_; //specular highlight amount

	Map *diffuseMap_;
    Map *normalMap_;

    virtual void init() { xmlName = "material"; }

	Material();
	Material(
        std::string name, 
		Vector diffuseColor,
		Vector highlightColor,
	    float reflectivity,
		float ka,
		float kd,
		float ks,
		float alpha,
	    Map *diffuseMap,
        Map *normalMap
	);
	virtual ~Material() {};

    virtual void transform() {
        if (diffuseMap_ != NULL)
            diffuseMap_->transform();
        if (normalMap_ != NULL)
            normalMap_->transform();
    };

	virtual Vector getColor(Vector &reflection);
	virtual Vector getColor(Vector &point, UVTriangle *uvtriangle, Vector &reflection);
    virtual Vector getReflection();

	virtual void resetLights();
    virtual void addLight(Vector &lightIntensity, Vector &lightPosition, Vector &rayDirection, Vector &point, Vector &normal);

    virtual bool loadXml(TiXmlElement* pElem, LinkList <Map> *linkMaps, LinkList <Material> *linkMaterials);
    TiXmlElement* getXml();
private:
    Vector diffuseIntensity_;
    Vector highlightIntensity_;
};

#endif // _MATERIAL_H_
