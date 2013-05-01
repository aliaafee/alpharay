/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */

#include "pathtracer.h"


void Pathtracer::init()
{
    Renderer::init();

    samples_ = 10;
}


Color Pathtracer::pathTrace(Scene &scene ,Ray &ray, int depth)
{
    float t;
    raysCast_ += 1;

	BaseObject *closestObject = closestIntersection(scene, ray, &t);

    if (closestObject == NULL)
        return Color(0, 0, 0);

    Material material = closestObject->material();

    Color color = material.diffuseColor_;
    Color emission = material.emission_;
    Color reflectance = material.reflectance_;

    if (!emission.isNull())
        return emission;

    if (depth > traceDepth_)
        return color;

    Vector intPoint = closestObject->point(ray, t);
    Vector intPointLocal = closestObject->transformPointInv(intPoint);
    Vector intNormal = closestObject->normal(intPointLocal);

    Ray newRay;
        
    Color reflection;

    Vector Vrefl, Vtran;
    switch (material.reflectionType())
    {
        case SPEC:
            newRay = ray.getReflectedRay(intPoint, intNormal);
            reflection = pathTrace(scene, newRay, depth + 1);

            emission.x += reflection.x;
            emission.y += reflection.y;
            emission.z += reflection.z;

            break;
        case FRES:
            intNormal.normalize();
            float cosT1, n1, n2, Crefl, Ctran;
            
            cosT1 = intNormal * (ray.direction_*-1);
            if (cosT1 > 0) {
                n1 = 1.0f; n2 = material.opticDensity_;
            } else {
                n1 = material.opticDensity_; n2 = 1.0f;
                intNormal *= -1.0f;
                cosT1 *= -1.0f;
            }

            getFresnelValues(ray.direction_, intNormal,
                                n1, n2,
                                cosT1,
                                &Vrefl, &Vtran,
                                &Crefl, &Ctran);

            if ( ((float)rand()/RAND_MAX) <= Ctran) {
                newRay = Ray(intPoint, intPoint+Vtran);
                reflection = pathTrace(scene, newRay, depth + 1);
            } else {
                newRay = Ray(intPoint, intPoint+Vrefl);
                reflection = pathTrace(scene, newRay, depth + 1);
            }

            emission.x += reflection.x;
            emission.y += reflection.y;
            emission.z += reflection.z;

            break;
        default:
            newRay = ray.getRamdomRayInHemisphere(intPoint, intNormal, 1.0);
            reflection = pathTrace(scene, newRay, depth + 1);

            emission.x += color.x *  reflection.x;
            emission.y += color.y *  reflection.y;
            emission.z += color.z *  reflection.z;
    }

    return emission;
}


Color Pathtracer::trace(Scene &scene ,Ray ray, int depth)
{
    Color color;
    
    for (int i = 0 ; i < samples_; i++) {
        color += pathTrace(scene, ray, 1);
    }

    color /= float(samples_);

    return color;
}


bool Pathtracer::loadXml(TiXmlElement* pElem, std::string path) 
{
    init();

    Renderer::loadXml(pElem, path);

    pElem->QueryIntAttribute("samples", &samples_);

    return true;
}


TiXmlElement* Pathtracer::getXml() 
{
    TiXmlElement* root = Renderer::getXml();

    root->SetAttribute("type", "pathtracer");
    root->SetAttribute("samples", samples_);

    return root;
}
