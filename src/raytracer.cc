/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */

#include "raytracer.h"


void Raytracer::init()
{
    Renderer::init();
}


void Raytracer::traceFresnel(Scene &scene, Ray ray, 
                        Vector intersectionPoint, Vector intersectionNormal,
                        Material &material, int depth)
{
    Vector color;
    if (depth < traceDepth_) {
        Vector Vrefl;
        Vector Vtran;

        float Crefl;
        float Ctran;

        float cosT1 = intersectionNormal * (ray.direction_*-1);
        if (cosT1 > 0) {
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
                fRefl = trace(scene, rRefl, depth + 1);
                fTran = trace(scene, rTran, depth + 1);
            } else {
                fRefl = raytraceDistributed(scene, rRefl, depth + 1,material.scatterFactor_,material.scatterSamples_);
                fTran = raytraceDistributed(scene, rTran, depth + 1,material.scatterFactor_,material.scatterSamples_);
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
            //Ray rRefl(intersectionPoint, intersectionPoint+Vrefl);
            Ray rTran(intersectionPoint, intersectionPoint+Vtran);
            if (material.scatterFactor_ == 0) {
                //fRefl = trace(scene, rRefl, depth + 1);
                fTran = trace(scene, rTran, depth + 1);
            } else {
                fTran = raytraceDistributed(scene, rTran, depth + 1,material.scatterFactor_,material.scatterSamples_);
            }
            
            material.setFresnelCoeff(Crefl, Ctran);
            //material.addReflection(fRefl);
            material.addTransmission(fTran);
        }
    }
}


void Raytracer::traceReflection(Scene &scene, Ray ray, 
                        Vector intersectionPoint, Vector intersectionNormal,
                        Material &material, int depth)
{
    Vector reflection;
    
    Ray reflectedRay = ray.getReflectedRay(intersectionPoint, intersectionNormal);

    if (depth < traceDepth_) {
        if (material.scatterFactor_ == 0) {
            //Plane Reflections
            reflection = trace(scene, 
                                    reflectedRay, 
                                    depth + 1);
        } else {
            //Scattered Reflections
            reflection = raytraceDistributed(scene, 
                                    reflectedRay, 
                                    depth + 1, 
                                    material.scatterFactor_,
                                    material.scatterSamples_);
        }
    }
    material.addReflection(reflection);
}

Vector Raytracer::raytraceDistributed(Scene &scene, Ray ray, int depth, 
                                        float scatterFactor, int scatterSamples) {
    Vector color;
    float sf = scatterFactor * M_PI/2;
    float theta = acos(ray.direction_.z);
    float phi = atan2(ray.direction_.y, ray.direction_.x);
    float t,p;
    for (int s = 0; s < scatterSamples; s ++) {
        t = theta + randf(sf*-1,sf);
        p = phi + randf(sf*-1,sf);
        ray.direction_.x = sin(t) * cos(p);
        ray.direction_.y = sin(t) * sin(p);
        ray.direction_.z = cos(t);
        color += trace(scene, ray, depth + 1);
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
    float t;
    raysCast_ += 1;

	BaseObject *closestObject = closestIntersection(scene, ray, &t);
	
    if (closestObject != NULL) {
        //TODO: Optimize
        Material material = closestObject->material();

        Vector intPoint = closestObject->point(ray, t);
        Vector intPointLocal = closestObject->transformPointInv(intPoint);
        Vector intNormal = closestObject->normal(intPointLocal);
        intNormal.normalize();
        
        setLighting(scene, material, intPoint, intNormal, ray.direction_);

        Color reflection;

        switch (material.reflectionType())
        {
            case SPEC:
                traceReflection(scene, ray, intPoint, intNormal, material, depth + 1);
                break;
            case FRES:
                traceFresnel(scene, ray, intPoint, intNormal, material, depth + 1);
                break;
        }

        return closestObject->color(intPointLocal, &material);
	}
	
	return Color(0, 0, 0);
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

