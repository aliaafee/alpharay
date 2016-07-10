/* vim: set ts=4 ss=4 sw=4 noet ai cindent : */

#include "light.h"


void Light::init() { 
    kIntensity_ = 5.0f;

	addEditable(new Editable<Color>("intensity", &intensity_, Color(100, 100, 100)));
	addEditable(new Editable<bool>("shadow", &shadowsOn_, true));
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
		if (material.log()) {
			std::cout << " lightint -> " << intObject->str() << " , dist " << distance << std::endl;
		}

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
                    if (distance > SMALL_NUM) {
				        return currentObject;
                    }
                }
            }
    }
    return NULL;
}
