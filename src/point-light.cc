/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */

#include "point-light.h"

Vector PointLight::getIntensity(std::vector<Object*>* objects, Vector &point, Object* ignore=NULL) {
    Ray lightRay(point, position_, true);
    float distance = point.distanceTo(position_);

    Vector intensity = getIntensityByDistance(intensity_ , distance) ;

    if (intensity.x < 0.1 && intensity.y < 0.1 && intensity.z < 0.1) {
        return intensity;
    }

    Object* intObject = getFirstIntersection(objects, lightRay, distance, ignore);

    if (intObject != NULL) {
        return Vector(0, 0, 0);
    }

    return intensity;
    
}

bool PointLight::loadXml(TiXmlElement* pElem) {
    return Light::loadXml(pElem);
}

TiXmlElement* PointLight::getXml() {
    return Light::getXml();
}
