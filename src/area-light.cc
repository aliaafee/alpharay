/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */

#include "area-light.h"


void AreaLight::init() { 
    SpotLight::init(); 

    lightRadius_ = 0.6;
    samples_ = 5;
}


Color AreaLight::getIntensity(std::vector<Object*>* objects, Vector &point)
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

    Color intensityTotal;

    Vector ldir = lightRay.direction_ * -1;
    Vector dist;// = target_ - position;
    V_SUB(dist, target_, position_);

    Vector u;// = (lightRay.direction_*-1) % (target_ - position_);
    V_CROSS(u, ldir, dist);

    Vector v;// = u % (target_ - position_);
    V_CROSS(v, u, dist);

    u.normalize();
    v.normalize();

    for (int i=0; i < samples_; i++) {
        
        Ray lightRay(point, 
                position_ 
                    + u * randf(lightRadius_*-1, lightRadius_)
                    + v * randf(lightRadius_*-1, lightRadius_)
                        , 
                        true);
        /*
        Ray lightRay(point, 
                position_ + 
                Vector( intensity.randf(lightRadius_*-1, lightRadius_),
                        intensity.randf(lightRadius_*-1, lightRadius_),
                        intensity.randf(lightRadius_*-1, lightRadius_)
                        ), 
                        true);*/

        BaseObject* intObject = getFirstIntersection(objects, lightRay, distance);

        if (intObject == NULL) {
            intensityTotal += intensity;
        }
    }
    
    intensity = intensityTotal / samples_;

    return intensity;    
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
