/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */

#include "spot-light.h"


void SpotLight::init() { 
    Light::init(); 

    target_ = Vector(0,0,1);
    angle_ = M_PI / 5;
    angleFalloff_ = M_PI / 5;
}


Color SpotLight::getIntensityByAngle(Color intensity, Vector PO, Vector TO)
{
    PO.normalize();
    TO.normalize();
    
    float angle = acos(V_DOT(PO, TO));

    if (angle > angle_) {
        return Vector(0, 0, 0);
    }

    if (angle > (angle_ - angleFalloff_)) {
        intensity = intensity * ((angle_-angle)/angleFalloff_);
    }

    return intensity;
}

Color SpotLight::getIntensity(std::vector<Object*>* objects, Vector &point)
{
    Ray lightRay(point, position_, true);

    Color intensity = intensity_;
    
    intensity = getIntensityByAngle(intensity, lightRay.direction_*-1, target_ - position_);

    if (intensity.isNull()) {
        return intensity;
    }

    float distance = point.distanceTo(position_);

    intensity = getIntensityByDistance(intensity , distance) ;

    if (intensity.x < 0.1 && intensity.y < 0.1 && intensity.z < 0.1) {
        return intensity;
    }

    BaseObject* intObject = getFirstIntersection(objects, lightRay, distance);

    if (intObject != NULL) {
        return Vector(0, 0, 0);
    }

    return intensity;    
}


TiXmlElement* SpotLight::getXml() {
    TiXmlElement* root = Light::getXml();

    root->SetAttribute("target", target_.str());
    root->SetAttribute("angle", ftos(angle_));
    root->SetAttribute("anglefalloff", ftos(angleFalloff_));

    return root;
}


bool SpotLight::loadXml(TiXmlElement* pElem, std::string path) {
    init();

    Light::loadXml(pElem, path);

    pElem->QueryValueAttribute <Vector> ("target", &target_);
    pElem->QueryFloatAttribute("angle", &angle_);
    pElem->QueryFloatAttribute("anglefalloff", &angleFalloff_);

    return true;
}
