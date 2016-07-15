/* vim: set ts=4 ss=4 sw=4 noet ai cindent : */

#include "area-light.h"


void AreaLight::init() { 
	addEditable(new Editable<float>("lightradius", &lightRadius_, 0.6));

	addEditable(new Editable<int>("samples", &samples_, 5));
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