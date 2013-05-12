/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */

#include "sky-light.h"


void SkyLight::init() { 
    Light::init(); 

    samples_ = 10;
    lightMap_ = NULL;
}


void SkyLight::set(std::vector<Object*>* objects, Material &material, Vector &point, Vector &pointNormal, Vector &viewDirection)
{
    Ray lightRay;

    Vector quantaOrigin;
    Color quantaIntensity = intensity_ / (float(samples_));

    for (int i = 0; i < samples_; i++) {
        lightRay = lightRay.getRamdomRayInHemisphere(point, pointNormal, 1.0);

        BaseObject* intObject = getFirstIntersection(objects, lightRay, BIG_NUM);

        if (lightMap_ != NULL) {
            quantaIntensity = lightMap_->color(lightRay.direction_,Vector2(0,0)) / (float(samples_));
        }

        if (intObject == NULL) {
            quantaOrigin = lightRay.position_ + lightRay.direction_;
            material.addLight(quantaIntensity,
                        quantaOrigin,
                        viewDirection,
                        point,
                        pointNormal );
        }

    }
}


TiXmlElement* SkyLight::getXml() {
    TiXmlElement* root = Light::getXml();

    root->SetAttribute("samples", samples_);

    if (lightMap_ == NULL) {
        root->SetAttribute("lightmap", "");
    } else {
        root->SetAttribute("lightmap", lightMap_->name());
    }

    return root;
}


bool SkyLight::loadXml(TiXmlElement* pElem, std::string path, LinkList *linkList) {
    init();

    Light::loadXml(pElem, path, linkList);

    pElem->QueryIntAttribute("samples", &samples_);

    std::string mapname; 

    mapname = "";
    pElem->QueryStringAttribute ("lightmap", &mapname);
    linkList->add(mapname, &lightMap_);

    return true;
}
