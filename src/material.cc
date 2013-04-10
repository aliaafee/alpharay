/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */

#include "material.h"

/*Phong reflection model
 *
 *src: http://en.wikipedia.org/wiki/Phong_reflection_model
 */

Material::Material() {
    init();
    name_ = "material";

	diffuseColor_ = Vector(255,0,0);
	highlightColor_ = Vector(255,255,255);
	reflectivity_ = 0;
    opticDensity_ = 1.66f;
    dielectric_ = false;
    scatterFactor_ = 0;
    scatterSamples_ = 1;
	ka_ = 0.1;
	kd_ = 0.8;
	ks_ = 0.8; 
	alpha_ = 15; 

	diffuseMap_ = NULL;
    normalMap_ = NULL;
}

Material::Material(std::string name, Vector diffuseColor, Vector highlightColor,float reflectivity, float ka, float kd, float ks, float alpha, Map *diffuseMap=NULL, Map *normalMap=NULL) {
    init();
    name_ = name;

	diffuseColor_ = diffuseColor;
	highlightColor_ = highlightColor;
	reflectivity_ = reflectivity;
    opticDensity_ = 1.66f;
    dielectric_ = false;
    scatterFactor_ = 0;
    scatterSamples_ = 1;
	ka_ = ka;
	kd_ = kd;
	ks_ = ks; 
	alpha_ = alpha; 

	diffuseMap_ = diffuseMap;
    normalMap_ = normalMap;
}

Vector Material::getReflection() {
    Vector color;

    color.x = (highlightColor_.x * highlightIntensity_.x);
    
    color.y = (highlightColor_.y * highlightIntensity_.y);

    color.z = (highlightColor_.z * highlightIntensity_.z);

    return color;
}

Vector Material::getColor(Vector &reflection) {
    Vector color;

    color = diffuseColor_;

    
    color.x = (color.x * ka_) + (color.x * diffuseIntensity_.x) ;
    
    color.y = (color.y * ka_) + (color.y * diffuseIntensity_.y) ;

    color.z = (color.z * ka_) + (color.z * diffuseIntensity_.z) ;

    if (reflectivity_ > 0) {
        color = color * ( 1.0f - reflectivity_ ) + reflection * reflectivity_;
    }

    if (!dielectric_) {
        color += getReflection();
    }
    
    return color;
}

Vector Material::getColor(Vector &point, UVTriangle *uvtriangle, Vector &reflection) {
	if (diffuseMap_ != NULL) {
        float u, v;

		diffuseColor_   = diffuseMap_->getColor(point, uvtriangle);
	}

    return getColor(reflection);
    //return diffuseColor_;
}

void Material::resetLights() {
    diffuseIntensity_.setNull();
    highlightIntensity_.setNull();
}

void Material::addLight(Vector &lightIntensity, 
                        Vector &lightPosition, 
                        Vector &viewerDirection, 
                        Vector &point, 
                        Vector &N) 
{
	Vector L = (lightPosition - point).getUnitVector();
    N.normalize();
	Vector R = L.getReflection(N);
	Vector V = viewerDirection.getUnitVector();

    double LdN = L*N;
    double RdV = R*V;

	if (LdN > 0.0) {
        diffuseIntensity_ += lightIntensity * kd_ * LdN ;
	    if (RdV > 0.0) {
            highlightIntensity_ += lightIntensity * ks_ * pow(RdV,alpha_);
        }
    }

}

bool Material::loadXml(TiXmlElement* pElem, LinkList <Map> *linkMaps, LinkList <Material> *linkMaterials) {
    name_ = "material";

    diffuseColor_ = Vector(255,0,0);
	highlightColor_ = Vector(255,255,255);
	reflectivity_ = 0;
    opticDensity_ = 1.66f;
    dielectric_ = false;
    scatterFactor_ = 0;
    scatterSamples_ = 1;
	ka_ = 0.1;
	kd_ = 0.8;
	ks_ = 0.8; 
	alpha_ = 15; 

	diffuseMap_ = NULL;
    normalMap_ = NULL;

    pElem->QueryStringAttribute ("name", &name_);
    pElem->QueryValueAttribute <Vector> ("diffusecolor", &diffuseColor_);
    pElem->QueryValueAttribute <Vector> ("highlightcolor", &highlightColor_);
    pElem->QueryFloatAttribute ("reflectivity", &reflectivity_);
    pElem->QueryBoolAttribute ("dielectric", &dielectric_);
    pElem->QueryFloatAttribute("opticdensity", &opticDensity_);
    pElem->QueryFloatAttribute ("scatterfactor", &scatterFactor_);
    pElem->QueryIntAttribute ("scattersamples", &scatterSamples_);
    pElem->QueryFloatAttribute ("ka", &ka_);
    pElem->QueryFloatAttribute ("kd", &kd_);
    pElem->QueryFloatAttribute ("ks", &ks_);
    pElem->QueryFloatAttribute ("alpha", &alpha_);

    std::string mapname; 

    mapname = "";
    pElem->QueryStringAttribute ("diffusemap", &mapname);
    linkMaps->add(mapname, &diffuseMap_);

    mapname = "";
    pElem->QueryStringAttribute ("normalmap", &mapname);
    linkMaps->add(mapname, &normalMap_);

    return true;
}


TiXmlElement* Material::getXml() {
    TiXmlElement* root = new TiXmlElement(xmlName.c_str());

    root->SetAttribute("name", name_);
    root->SetAttribute("diffusecolor", diffuseColor_.str());
    root->SetAttribute("highlightcolor", highlightColor_.str());
    root->SetAttribute("reflectivity", ftos(reflectivity_));
    root->SetAttribute("dielectric", dielectric_);
    root->SetAttribute("opticdensity", ftos(opticDensity_));
    root->SetAttribute("scatterfactor", ftos(scatterFactor_));
    root->SetAttribute("scattersamples", scatterSamples_);
    root->SetAttribute("ka", ftos(ka_));
    root->SetAttribute("kd", ftos(kd_));
    root->SetAttribute("ks", ftos(ks_));
    root->SetAttribute("alpha", ftos(alpha_));
    
    if (diffuseMap_ == NULL) {
        root->SetAttribute("diffusemap", "");
    } else {
        root->SetAttribute("diffusemap", diffuseMap_->name_);
    }

    if (normalMap_ == NULL) {
        root->SetAttribute("normalmap", "");
    } else {
        root->SetAttribute("normalmap", normalMap_->name_);
    }

    return root;
}
