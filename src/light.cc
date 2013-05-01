/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */

#include "light.h"


void Light::init() { 
    XmlObjectNamed::init(); 

    kIntensity_ = 5.0f;

    position_ = Vector(0, 0, 0); 
    intensity_ = Color(100, 100, 100);
    shadowsOn_ = true;
}


void Light::set(std::vector<Object*>* objects, Material &material, Vector &point, Vector &pointNormal, Vector &viewDirection)
{
    Ray lightRay(point, position_, true);
    float distance = point.distanceTo(position_);

    BaseObject* intObject = getFirstIntersection(objects, lightRay, distance);

    if (intObject != NULL) {
        return;
    }

    Color intensity = getIntensityByDistance(intensity_ , distance) ;

    material.addLight(intensity,
                        position_,
                        viewDirection,
                        point,
                        pointNormal );
}


Color Light::getIntensityByDistance(Color intensity, float &distance) {
    return (intensity / (distance * distance)) * kIntensity_;
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


