#include "light.h"


Vector Light::getIntensityByDistance(Vector &intensity, float &distance) {
    return intensity / (distance * distance);
}

Object* Light::getFirstIntersection(std::vector<Object*>* objects, 
                                            Ray &ray,
                                            float rayLimit,
                                            Object *ignore=NULL) 
{
    if (!shadowsOn_) {
        return NULL;
    }
    Object *currentObject = NULL;
    Vector intersectionPoint,intersectionNormal;
	float distance;

    for (int i=0; i < (*objects).size(); i++) {
        if ((*objects)[i] != ignore) {
            currentObject = ((*objects)[i])->intersection(ray, NULL, NULL, NULL, NULL, &distance);
            //currentObject = ((*objects)[i])->intersection(ray);
            if (currentObject != NULL) {
                if (distance < rayLimit) {
                    if (distance > 0.0001) {
                        //*firstIntersectionPoint = intersectionPoint;
				        //*firstIntersectionNormal = intersectionNormal;
				        return currentObject;
                    }
                }
            }
        }
    }
    return NULL;
}

bool Light::loadXml(TiXmlElement* pElem) {
    name_ = "light"; 
    position_ = Vector(0, 0, 0); 
    intensity_ = Vector(1, 1, 1);
    shadowsOn_ = true;

    pElem->QueryStringAttribute ("name", &name_);
    pElem->QueryValueAttribute <Vector> ("position", &position_);
    pElem->QueryValueAttribute <Vector> ("intensity", &intensity_);
    pElem->QueryBoolAttribute ("shadow", &shadowsOn_);

    return true;
}

TiXmlElement* Light::getXml() {
    TiXmlElement* root = new TiXmlElement(xmlName.c_str());

    root->SetAttribute("name", name_);
    root->SetAttribute("position", position_.str());
    root->SetAttribute("intensity", intensity_.str());
    root->SetAttribute("shadow", shadowsOn_);

    return root;
}
