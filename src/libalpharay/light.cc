/* vim: set ts=4 ss=4 sw=4 noet ai cindent : */

#include "light.h"


void Light::init() {
	type_ = "light";
	
    kIntensity_ = 5.0f;

	//addEditable(new Editable<Color>("intensity", &intensity_, Color(100, 100, 100)));
	addEditable(new Editable<Color>("color", &color_, Color(1,1,1)));
	addEditable(new Editable<float>("power", &power_, 100));
	
	addEditable(new Editable<bool>("shadow", &shadowsOn_, true));
}

void Light::transform()
{
    Object::transform();

	Vector zero(0,0,0);
	
	tposition_ = transformPoint(zero);

	intensity_ = color_ * power_;
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

	for (auto obj = objects->begin(); obj != objects->end(); ++obj) {
			currentObject = (*obj)->intersection(ray, &distance, rayLimit);
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
