/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */

#include "sky-light.h"


void SkyLight::init() { 
    Light::init(); 

    samples_ = 10;
}


void SkyLight::set(std::vector<Object*>* objects, Material &material, Vector &point, Vector &pointNormal, Vector &viewDirection)
{
    ;
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
