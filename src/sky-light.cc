/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */

#include "sky-light.h"


void SkyLight::init() { 
    Light::init(); 

    samples_ = 10;
}


void SkyLight::set(std::vector<Object*>* objects, Material &material, Vector &point, Vector &pointNormal, Vector &viewDirection)
{
    Ray lightRay;

    Vector quantaOrigin;
    Vector quantaIntensity = intensity_ / (float(samples_) * kIntensity_);

    for (int i = 0; i < samples_; i++) {
        lightRay = lightRay.getRamdomRayInHemisphere(point, pointNormal, 1.0);

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


TiXmlElement* SkyLight::getXml() {
    TiXmlElement* root = Light::getXml();

    root->SetAttribute("samples", samples_);

    return root;
}


bool SkyLight::loadXml(TiXmlElement* pElem, std::string path) {
    init();

    Light::loadXml(pElem, path);

    pElem->QueryIntAttribute("samples", &samples_);

    return true;
}
