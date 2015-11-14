/* vim: set ts=4 ss=4 sw=4 noet ai cindent : */

#include "light.h"


void Light::init() { 
    Object::init(); 

    kIntensity_ = 5.0f;

    intensity_ = Color(100, 100, 100);
    shadowsOn_ = true;
}

void Light::transform()
{
    Object::transform();

	Vector zero(0,0,0);
	
	tposition_ = transformPoint(zero);
}

void Light::set(std::vector<Object*>* objects, Material &material, Vector &point, Vector &pointNormal, Vector &viewDirection)
{
    Ray lightRay(point, tposition_, true);
    float distance = point.distanceTo(tposition_);

    BaseObject* intObject = getFirstIntersection(objects, lightRay, distance);

    if (intObject != NULL) {
        return;
    }

    Color intensity = getIntensityByDistance(intensity_ , distance) ;

    material.addLight(intensity,
                        tposition_,
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

    ray.shadowRay_ = true;

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
	TiXmlElement* root = Object::getXml();

	root->SetAttribute("intensity", intensity_.str());
    root->SetAttribute("shadow", shadowsOn_);

    return root;
}


bool Light::loadXml(TiXmlElement* pElem, std::string path, LinkList *linkList) {
	init();

    Object::loadXml(pElem, path, linkList);

    TiXmlHandle hRoot = TiXmlHandle(pElem);

	pElem->QueryValueAttribute <Vector> ("intensity", &intensity_);
    pElem->QueryBoolAttribute ("shadow", &shadowsOn_);

    return true;
}


