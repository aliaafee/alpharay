/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */

#include "area-light.h"


void AreaLight::init() { 
    SpotLight::init(); 

    lightRadius_ = 0.6;
    samples_ = 5;
}


void AreaLight::set(std::vector<Object*>* objects, Material &material, Vector &point, Vector &pointNormal, Vector &viewDirection)
{
    Ray lightRay(point, position_, true);

    Color intensity = intensity_;
    
    intensity = getIntensityByAngle(intensity, lightRay.direction_*-1, target_ - position_);

    /*
    if ((intensity.x < 0.1 && intensity.y < 0.1 && intensity.z < 0.1)) {
        return;
    }
    */

    float distance = point.distanceTo(position_);
    intensity = getIntensityByDistance(intensity , distance) ;

    Vector ldir = lightRay.direction_ * -1;
    Vector dist;
    V_SUB(dist, target_, position_);

    Vector u;
    V_CROSS(u, ldir, dist);

    Vector v;
    V_CROSS(v, u, dist);

    u.normalize();
    v.normalize();

    Vector quantaOrigin;
    Vector quantaIntensity = intensity / float(samples_);
    intensity.setNull();

    for (int i=0; i < samples_; i++) {
        quantaOrigin = position_ 
                            + u * randf(lightRadius_*-1, lightRadius_)
                            + v * randf(lightRadius_*-1, lightRadius_);

        lightRay.direction_ = quantaOrigin - lightRay.position_;
        lightRay.direction_.normalize();

        BaseObject* intObject = getFirstIntersection(objects, lightRay, distance);

        if (intObject == NULL) {
            material.addLight(quantaIntensity,
                        quantaOrigin,
                        viewDirection,
                        point,
                        pointNormal );
        }
    }

    
}


TiXmlElement* AreaLight::getXml() {
    TiXmlElement* root = SpotLight::getXml();

    root->SetAttribute("lightradius", ftos(lightRadius_));
    root->SetAttribute("samples", samples_);

    return root;
}


bool AreaLight::loadXml(TiXmlElement* pElem, std::string path) {
    init();

    SpotLight::loadXml(pElem, path);

    pElem->QueryFloatAttribute("lightradius", &lightRadius_);
    pElem->QueryIntAttribute("samples", &samples_);

    return true;
}
