/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * raytracer-a
 * Copyright (C) Ali Aafee 2011 <ali.aafee@gmail.com>
 * 
 * raytracer-a is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * raytracer is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "spot-light.h"

Vector SpotLight::getIntensityByAngle(Vector intensity, Vector PO, Vector TO) {
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

Vector SpotLight::getIntensity(std::vector<Object*>* objects, Vector &point, Object* ignore=NULL) {
    Ray lightRay(point, position_, true);

    Vector intensity = intensity_;
    
    intensity = getIntensityByAngle(intensity, lightRay.direction_*-1, target_ - position_);

    if (intensity.isNull()) {
        return intensity;
    }

    float distance = point.distanceTo(position_);

    intensity = getIntensityByDistance(intensity , distance) ;

    if (intensity.x < 0.1 && intensity.y < 0.1 && intensity.z < 0.1) {
        return intensity;
    }

    Object* intObject = getFirstIntersection(objects, lightRay, distance, ignore);

    if (intObject != NULL) {
        return Vector(0, 0, 0);
    }

    return intensity;
    
}

bool SpotLight::loadXml(TiXmlElement* pElem) {
    Light::loadXml(pElem);

    pElem->QueryValueAttribute <Vector> ("target", &target_);
    pElem->QueryFloatAttribute("angle", &angle_);
    pElem->QueryFloatAttribute("anglefalloff", &angleFalloff_);

    return true;
}

TiXmlElement* SpotLight::getXml() {
    TiXmlElement* root = Light::getXml();

    root->SetAttribute("target", target_.str());
    root->SetAttribute("angle", ftos(angle_));
    root->SetAttribute("anglefalloff", ftos(angleFalloff_));

    return root;
}

