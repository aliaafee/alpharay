/* vim: set ts=4 ss=4 sw=4 noet ai cindent : */

#include "light.h"


void Light::init() {
	type_ = "light";
	
    kIntensity_ = 5.0f;

	//addEditable(new Editable<Color>("intensity", &intensity_, Color(100, 100, 100)));
	addEditable(new Editable<Color>("color", &color_, Color(1,1,1)));
	addEditable(new Editable<float>("energy", &energy_, 100));
	
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

	Color intensity = color_;
	intensity *= getEnergyAtDistance(energy_, distance);

    material.addLight(intensity,
                        tposition_,
                        viewDirection,
                        point,
                        pointNormal );
}


float Light::getEnergyAtDistance(const float &totalEnergy, const float &distance) {
	if (distance == 0) {
		return totalEnergy;
	}
	return totalEnergy/ (4 * M_PI * distance * distance);
}


Color Light::getIntensityByDistance(Color intensity, float &distance) {
	float d2 = distance * distance * 10 * 10;
	if (d2 < 1.0f) {
		return intensity;
	}
    return (intensity / (distance * distance));
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
