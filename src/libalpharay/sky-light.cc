/* vim: set ts=4 ss=4 sw=4 noet ai cindent : */

#include "sky-light.h"


void SkyLight::init() { 
	addEditable(new Editable<int>("samples", &samples_, 10));

	addEditableLink(new EditableLink<Map>("lightmap", &lightMap_));
}


void SkyLight::set(std::vector<Object*>* objects, Material &material, Vector &point, Vector &pointNormal, Vector &viewDirection)
{
    Ray lightRay;

    Vector quantaOrigin;
	Color quantaIntensity = color_;
	quantaIntensity *= energy_ / (float(samples_));

    for (int i = 0; i < samples_; i++) {
        lightRay = lightRay.getRamdomRayInHemisphere(point, pointNormal, 1.0);

        BaseObject* intObject = getFirstIntersection(objects, lightRay, BIG_NUM);

        if (intObject == NULL) {
            if (lightMap_ != NULL) {
                quantaIntensity = lightMap_->color(lightRay.direction_,Vector2(0,0));
				quantaIntensity /= (float(samples_));
            }

			V_ADD(quantaOrigin, lightRay.position_, lightRay.direction_);

            material.addLight(quantaIntensity,
                        quantaOrigin,
                        viewDirection,
                        point,
                        pointNormal );
        }

    }
}
