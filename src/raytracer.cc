/* vim: set ts=4 ss=4 sw=4 noet ai cindent : */

#include "raytracer.h"


void Raytracer::init()
{
    Renderer::init();
}


void Raytracer::traceFresnel(Scene &scene, Ray ray, 
                        Vector intersectionPoint, Vector intersectionNormal,
                        Material &material, int depth)
{
    if (material.reflectivity() == 0.0) return;

    Vector color;
    if (depth < traceDepth_) {
        Vector Vrefl;
        Vector Vtran;

        float Crefl;
        float Ctran;

        float cosT1 = intersectionNormal * (ray.direction_*-1);
        if (cosT1 > -EPSILON && cosT1 < EPSILON) {
            Crefl = 1;
            Ctran = 0;
            Ray reflectedRay = ray.getReflectedRay(intersectionPoint, intersectionNormal);

            Color reflection = trace(scene, reflectedRay, depth );

            material.setFresnelCoeff(Crefl, Ctran);
            material.addReflection(reflection);
        } else if (cosT1 >= EPSILON) {
            //Entering object
            getFresnelValues(ray.direction_, intersectionNormal,
                                1.0f, material.opticDensity_,
                                cosT1,
                                &Vrefl, &Vtran,
                                &Crefl, &Ctran);

            //std::cout << " 1 to 2 ";
            Vector fRefl, fTran;
            Ray rRefl(intersectionPoint, intersectionPoint+Vrefl);
            Ray rTran(intersectionPoint, intersectionPoint+Vtran);
            if (material.scatterFactor_ == 0) {
                fRefl = trace(scene, rRefl, depth);
                fTran = trace(scene, rTran, depth);
            }

            material.setFresnelCoeff(Crefl, Ctran);
            material.addReflection(fRefl);
            material.addTransmission(fTran);
        } else {
            //Exiting object
            getFresnelValues(ray.direction_, intersectionNormal*-1,
                                material.opticDensity_, 1.0f,
                                cosT1 * -1,
                                &Vrefl, &Vtran,
                                &Crefl, &Ctran);
            //std::cout << " 2 to 1 ";
            Vector fRefl, fTran;
            Ray rRefl(intersectionPoint, intersectionPoint+Vrefl);
            Ray rTran(intersectionPoint, intersectionPoint+Vtran);
            if (material.scatterFactor_ == 0) {
                fRefl = trace(scene, rRefl, depth);
                fTran = trace(scene, rTran, depth);
            }

            if (Crefl < 0 || Ctran < 0) {
                std::cout << Crefl << "," << Ctran << std::endl;
            }
            
            material.setFresnelCoeff(0, 1);
            //material.addReflection(fRefl);
            material.addTransmission(fTran);
        }
    }
}


void Raytracer::traceReflection(Scene &scene, Ray ray, 
                        Vector intersectionPoint, Vector intersectionNormal,
                        Material &material, int depth)
{
    if (material.reflectivity() == 0.0) return;

    Vector reflection;
    
    Ray reflectedRay = ray.getReflectedRay(intersectionPoint, intersectionNormal);

    if (material.scatterFactor_ == 0) {
        //Plane Reflections
        reflection = trace(scene, 
                                reflectedRay, 
                                depth );
    } else {
        //Scattered Reflections
        reflection = raytraceDistributed(scene, 
                                reflectedRay, 
                                depth, 
                                material.scatterFactor_,
                                material.scatterSamples_);
    }
    material.addReflection(reflection);
}

Vector Raytracer::raytraceDistributed(Scene &scene, Ray ray, int depth, 
                                        float scatterFactor, int scatterSamples) {
    Vector color;
    Ray newray;
    for (int s = 0; s < scatterSamples; s++) {
        newray = ray.getRamdomRayInHemisphere(ray.position_, ray.direction_, scatterFactor);

        color += trace(scene, newray, depth);
    }
    color /= float(scatterSamples);
    return color;
}


void Raytracer::setLighting
            (Scene &scene, Material &material, Vector &point, Vector &pointNormal, Vector &viewDirection) 
{
    for (int l=0; l < scene.lights.size(); l++) {
        scene.lights[l]->set(&(scene.objects), material, point, pointNormal, viewDirection);
    }
}


Color Raytracer::trace(Scene &scene ,Ray ray, int depth)
{
    depth += 1;

    if (depth > traceDepth_)
        return scene.envColor(ray);

    float t;
    raysCast_ += 1;

	if (scene.rayLog()) {
		ray.log_ = true;
		std::cout << "d" << depth << ": int test [" << std::endl;
	}

	BaseObject *closestObject = closestIntersection(scene, ray, &t);
	
	if (scene.rayLog()) {
		std::cout << "]" << std::endl;
	}

    if (closestObject == NULL)
        return scene.envColor(ray);	

    //TODO: Optimize
    Material material = closestObject->material();

	if (scene.rayLog()) {
		material.log_ = true;
	}

    Vector intPoint = closestObject->point(ray, t);
    Vector intPointLocal = closestObject->transformPointInv(intPoint);
    Vector intNormal = closestObject->normal(intPointLocal);
    intNormal.normalize();

    closestObject->setPoint(intPointLocal, &material);

	if (scene.rayLog()) {
		std::cout << "d" << depth << ": int -> " << closestObject->str() << " @ " << intPoint << " n:" << intNormal << std::endl;
	}
    
    setLighting(scene, material, intPoint, intNormal, ray.direction_);

    Color reflection;

    switch (material.reflectionType())
    {
        case SPEC:
            traceReflection(scene, ray, intPoint, intNormal, material, depth);
            break;
        case FRES:
            traceFresnel(scene, ray, intPoint, intNormal, material, depth);
            break;
		case DIFF:
			break;
    }

    return material.color();
	
}


bool Raytracer::loadXml(TiXmlElement* pElem, std::string path) 
{
    init();

    Renderer::loadXml(pElem, path);

    return true;
}


TiXmlElement* Raytracer::getXml() 
{
    TiXmlElement* root = Renderer::getXml();

    root->SetAttribute("type", "raytracer");

    return root;
}

