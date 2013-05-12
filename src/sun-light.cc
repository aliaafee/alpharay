/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */

#include "sun-light.h"


void SunLight::init() { 
    Light::init(); 

    samples_ = 1;
    size_ = 0;
}


void SunLight::set(std::vector<Object*>* objects, Material &material, Vector &point, Vector &pointNormal, Vector &viewDirection)
{
    //lights rays from sun are all parallel to each other
    Ray lightRay(point, point+position_, true);

    Vector quantaOrigin, lightDirection;
    Color quantaIntensity = intensity_ / (float(samples_) * kIntensity_);

    lightDirection = lightRay.direction_;

    for (int i = 0; i < samples_; i++) {
        lightRay = lightRay.getRamdomRayInHemisphere(point, lightDirection, size_/M_PI);

        BaseObject* intObject = getFirstIntersection(objects, lightRay, BIG_NUM);

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


TiXmlElement* SunLight::getXml() {
    TiXmlElement* root = Light::getXml();

    root->SetAttribute("samples", samples_);
    root->SetAttribute("size", ftos(size_));

    return root;
}


bool SunLight::loadXml(TiXmlElement* pElem, std::string path, LinkList *linkList) {
    init();

    Light::loadXml(pElem, path, linkList);

    pElem->QueryIntAttribute("samples", &samples_);
    pElem->QueryFloatAttribute("size", &size_);

    return true;
}
