/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */

#include "light.h"


void Light::init() { 
    XmlObjectNamed::init(); 

    position_ = Vector(0, 0, 0); 
    intensity_ = Color(100, 100, 100);
    shadowsOn_ = true;
}


Color Light::getIntensity(std::vector<Object*>* objects, Vector &point)
{
    Ray lightRay(point, position_, true);
    float distance = point.distanceTo(position_);

    Vector intensity = getIntensityByDistance(intensity_ , distance) ;

    if (intensity.x < 0.1 && intensity.y < 0.1 && intensity.z < 0.1) {
        return intensity;
    }

    BaseObject* intObject = getFirstIntersection(objects, lightRay, distance);

    if (intObject != NULL) {
        return Color(0, 0, 0);
    }

    return intensity;
}


Color Light::getIntensityByDistance(Color intensity, float &distance) {
    return (intensity / (distance * distance)) * 5.0f;
}


BaseObject* Light::getFirstIntersection(std::vector<Object*>* objects, Ray &ray, float rayLimit) 
{
    if (!shadowsOn_) {
        return NULL;
    }

    BaseObject *currentObject = NULL;
	float distance;

    for (int i=0; i < (*objects).size(); i++) {
            currentObject = ((*objects)[i])->intersection(ray, &distance, rayLimit);
            if (currentObject != NULL) {
                if (distance < rayLimit) {
                    if (distance > 0.0001) {
				        return currentObject;
                    }
                }
            }
    }
    return NULL;
}


TiXmlElement* Light::getXml() {
    TiXmlElement* root = XmlObjectNamed::getXml();

    root->SetAttribute("position", position_.str());
    root->SetAttribute("intensity", intensity_.str());
    root->SetAttribute("shadow", shadowsOn_);

    return root;
}


bool Light::loadXml(TiXmlElement* pElem, std::string path) {
    init();

    XmlObjectNamed::loadXml(pElem, path);

    pElem->QueryValueAttribute <Vector> ("position", &position_);
    pElem->QueryValueAttribute <Vector> ("intensity", &intensity_);
    pElem->QueryBoolAttribute ("shadow", &shadowsOn_);

    return true;
}


