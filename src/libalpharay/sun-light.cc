/* vim: set ts=4 ss=4 sw=4 noet ai cindent : */

#include "sun-light.h"


void SunLight::init() { 
	addEditable(new Editable<int>("samples", &samples_, 10));
	addEditable(new Editable<float>("size", &size_, 0.6));

	addEditableLink(new EditableLink<Map>("lightmap", &lightMap_));
}


void SunLight::set(std::vector<Object*>* objects, Material &material, Vector &point, Vector &pointNormal, Vector &viewDirection)
{
    //lights rays from sun are all parallel to each other
    Ray lightRay(point, point+position_, true);

    Vector quantaOrigin, lightDirection;
    Color quantaIntensity = intensity_ / (float(samples_));

    lightDirection = lightRay.direction_;

    for (int i = 0; i < samples_; i++) {
        lightRay = lightRay.getRamdomRayInHemisphere(point, lightDirection, size_/M_PI);

        BaseObject* intObject = getFirstIntersection(objects, lightRay, BIG_NUM);

        if (intObject == NULL) {
            if (lightMap_ != NULL) {
                quantaIntensity = lightMap_->color(lightRay.direction_,Vector2(0,0)) / (float(samples_));
            }
            quantaOrigin = lightRay.position_ + lightRay.direction_;
            material.addLight(quantaIntensity,
                        quantaOrigin,
                        viewDirection,
                        point,
                        pointNormal );
        }

    }
}
