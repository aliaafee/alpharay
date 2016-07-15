/* vim: set ts=4 ss=4 sw=4 noet ai cindent : */

#include "spot-light.h"


void SpotLight::init() { 
	addEditable(new Editable<Vector>("target", &target_, Vector(0, 0, 1)));
	addEditable(new Editable<float>("angle", &angle_, M_PI / 5));
	addEditable(new Editable<float>("anglefalloff", &angleFalloff_, M_PI / 5));
}


void SpotLight::set(std::vector<Object*>* objects, Material &material, Vector &point, Vector &pointNormal, Vector &viewDirection)
{
    Ray lightRay(point, position_, true);

    Color intensity = intensity_;
    
    intensity = getIntensityByAngle(intensity, lightRay.direction_*-1, target_ - position_);

    if (intensity.x < 0.1 && intensity.y < 0.1 && intensity.z < 0.1) {
        return;
    }

    float distance = point.distanceTo(position_);

    BaseObject* intObject = getFirstIntersection(objects, lightRay, distance);

    if (intObject != NULL) {
        return;
    }

    intensity = getIntensityByDistance(intensity , distance) ;

    material.addLight(intensity,
                        position_,
                        viewDirection,
                        point,
                        pointNormal );
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
