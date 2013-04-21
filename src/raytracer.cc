/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */

#include "raytracer.h"

BaseObject* Raytracer::getClosestIntersection(Scene &scene, Ray &ray, float *closest) 
{
	float distance;
	*closest = BIG_NUM;
	
	BaseObject *currentObject = NULL;
	BaseObject *closestObject = NULL;
	
	for (int i=0; i < scene.objects.size(); i++) {
			currentObject = scene.objects[i]->intersection(ray, &distance, BIG_NUM);   

			if (currentObject != NULL) {
                if (distance > 0.0001) {
				    if (distance < *closest) {
                        closestObject = currentObject;
                        *closest = distance;
				    }
                }
			}
	}
	return closestObject;
}


void Raytracer::setLighting(Scene &scene, Material &material, Vector &point, Vector &pointNormal, Vector &viewDirection) 
{
    //material.resetLights();

    for (int l=0; l < scene.lights.size(); l++) {
        scene.lights[l]->set(&(scene.objects), material, point, pointNormal, viewDirection);

        /*
        Vector lightIntensity = scene.lights[l]->getIntensity(&(scene.objects), point);

        if (lightIntensity.x > 0.01 && lightIntensity.y > 0.01 && lightIntensity.z > 0.01) {
            material.addLight(lightIntensity,
                                        scene.lights[l]->position_,
                                        viewDirection,
                                        point,
                                        pointNormal );
        }
        */
    }
}


void Raytracer::getFresnelValues(Vector Vincident, Vector normal,
                        float n1, float n2,
                        float cosT1, 
                        Vector *Vreflect, Vector *Vrefract,
                        float *reflectionCoeff, float *transmissionCoeff )
{
    //cosT1 = normal * (Vincident*-1);

    float cosT2 = sqrt(1 - ( pow(n1/n2, 2) * (1 - pow(cosT1,2)) ) );

    *Vreflect = Vincident + (normal * (2 * cosT1));
    
    if (cosT1 > 0) {
        *Vrefract = ( Vincident * (n1/n2) ) + ( normal * ( (n1/n2 * cosT1) - cosT2) );
    } else {
        *Vrefract = ( Vincident * (n1/n2) ) + ( normal * ( (n1/n2 * cosT1) + cosT2) );
    }

    float T1 = acos(cosT1);
    float T2 = acos(cosT2);

    //float rI = tan(T1 - T2) / tan(T1 + T2);
    //float rT = sin(T1 - T2) / sin(T1 + T2);

    //float sinT2 = sin(T2);
    //float sinT1pT2 = sin(T1 + T2);

    //float tI = (2 * sinT2 * cosT1) / ( sinT1pT2 * cos(T1 - T2) );
    //float tT = (2 * sinT2 * cosT1) / ( sinT1pT2 );

    //Parallel case
    /*float rI = tan(T1 - T2) / tan(T1 + T2);
    *reflectionCoeff   = rI * rI;
    *transmissionCoeff = 1 - *reflectionCoeff;
    */
    
    float rI = tan(T1 - T2) / tan(T1 + T2);
    float rT = sin(T1 - T2) / sin(T1 + T2);
    float r = (rI + rT)/2;
    *reflectionCoeff   = r * r;
    *transmissionCoeff = 1 - *reflectionCoeff;

}


Vector Raytracer::traceFresnel(Scene &scene, Ray ray, 
                        Vector intersectionPoint, Vector intersectionNormal,
                        Material &material,
                        int *reflectionDepth)
{
    Vector color;
    *reflectionDepth += 1;
    if (*reflectionDepth < maxReflectionDepth_) {
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
                fRefl = raytrace(scene, rRefl, reflectionDepth);
                fTran = raytrace(scene, rTran, reflectionDepth);
            } else {
                fRefl = raytraceDistributed(scene, rRefl, reflectionDepth,material.scatterFactor_,material.scatterSamples_);
                fTran = raytraceDistributed(scene, rTran, reflectionDepth,material.scatterFactor_,material.scatterSamples_);
            }
            //color = (fRefl + material.getReflection()) * Crefl + fTran * Ctran;
            color = fRefl * Crefl + fTran * Ctran;
        } else {
            //Exiting object
            getFresnelValues(ray.direction_, intersectionNormal*-1,
                                material.opticDensity_, 1.0f,
                                cosT1 * -1,
                                &Vrefl, &Vtran,
                                &Crefl, &Ctran);
            //std::cout << " 2 to 1 ";
            Vector fTran;
            Ray rTran(intersectionPoint, intersectionPoint+Vtran);
            if (material.scatterFactor_ == 0) {
                fTran = raytrace(scene, rTran, reflectionDepth);
            } else {
                fTran = raytraceDistributed(scene, rTran, reflectionDepth,material.scatterFactor_,material.scatterSamples_);
            }

            color = fTran;
        }
    }
    return color;
}


Vector Raytracer::traceReflection(Scene &scene, Ray ray, 
                        Vector intersectionPoint, Vector intersectionNormal,
                        Material &material,
                        int *reflectionDepth)
{
    Vector reflection;
    *reflectionDepth += 1;
    
    Ray reflectedRay = ray.getReflectedRay(intersectionPoint, intersectionNormal);

    if (*reflectionDepth < maxReflectionDepth_) {
        if (material.scatterFactor_ == 0) {
            //Plane Reflections
            reflection = raytrace(scene, 
                                    reflectedRay, 
                                    reflectionDepth);
        } else {
            //Scattered Reflections
            reflection = raytraceDistributed(scene, 
                                    reflectedRay, 
                                    reflectionDepth, 
                                    material.scatterFactor_,
                                    material.scatterSamples_);
        }
    }
    return reflection;
}

Vector Raytracer::raytraceDistributed(Scene &scene, Ray ray, int *reflectionDepth, 
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
        color += raytrace(scene, ray, reflectionDepth);
    }
    color /= float(scatterSamples);
    return color;
}


Vector Raytracer::raytrace(Scene &scene ,Ray ray, int *reflectionDepth) {
    float t;

    rayCount_ += 1;

	BaseObject *closestObject = getClosestIntersection(scene, ray, &t);
	
    if (closestObject != NULL) {
        //TODO: Optimize
        Material material = closestObject->material();

        Vector intPoint = closestObject->point(ray, t);
        Vector intPointLocal = closestObject->pointLocal(ray, t);

        Vector intNormal = closestObject->normal(intPointLocal);

        setLighting(scene, material, intPoint, intNormal, ray.direction_);

        Color reflection;

        if (material.reflectivity() > 0) {
            if (material.dielectric_) {
                reflection = traceFresnel(scene, ray, 
                                            intPoint, intNormal, 
                                            material, reflectionDepth);
            } else {
                reflection = traceReflection(scene, ray, 
                                            intPoint, intNormal, 
                                            material, reflectionDepth);
            }
        }

        material.addReflection(reflection);

        Color color = closestObject->color(intPointLocal, &material);

        return Vector(color.x, color.y, color.z);
	}
	
	return Vector(0, 0, 0);
}


Vector Raytracer::correctExposure(Vector color) {
    if (exposure_ == 0) {
        return color;
    } else {
        return Vector(
            1.0f - expf(color.x * exposure_),
            1.0f - expf(color.y * exposure_),
            1.0f - expf(color.z * exposure_));
    }
}


void Raytracer::renderLetterBox (Scene &scene, Image *image, 
                                        int x0, int y0, 
                                        int x1, int y1, float *done=NULL)
{
    if (x1 > image->width())
        return;
    if (y1 > image->height())
        return;

    float total = (y1-y0) * (x1-x0);
    float width = x1-x0;
	    
	for (float y = y0; y <= y1; y++) { cY = y;
		for (float x = x0; x <= x1; x++) { cX = x;
            Vector color;
            //std::cout << "(" << x << "," << y << ")" << std::endl;
            for (int ssx=0; ssx < subSamplesX_; ssx++) {
                float sx = x + (float(ssx)/float(subSamplesX_));
                for (int ssy=0; ssy < subSamplesY_; ssy++) {
                    float sy = y + (float(ssy)/float(subSamplesY_));
                    int reflectionDepth = 0;
                    
                    color += raytrace(
                                    scene,
                                    Ray(
                                        scene.camera_.position_,
                                        scene.camera_.getScreenPosition(sx,sy)),
                                    &reflectionDepth);
                }
            }
            
            color = color / float((subSamplesX_*subSamplesY_));

            color = correctExposure(color);

            image->setColor(x, y, color);
		}
        
        if (done != NULL) {
            *done = ((y-y0) * width)/total;
        }
	}
    *done = 1;
}


void Raytracer::renderLetterBoxThread (Scene &scene, 
                                       Image *image, int x0, int y0, int x1, int y1, 
                                       int threadId)
{
    renderLetterBox(scene, image, x0, y0, x1, y1, &(completedThreads_[threadId]));
}


void Raytracer::render (Scene& scene, Image& image)
{
    rayCount_ = 0;
    scene.transformVertices();
    scene.camera_.setScreenDimensions(image.width(), image.height());

    renderLetterBox(scene, &image, 1, 1, image.width(), image.height());
}


void Raytracer::statusDisplay() {
    boost::timer statusTime;

    std::stringstream ss;
    float done = 0.0f;
    float pdone = -1.0f;

    ss << "0%";
    std::cout << ss.str();
    while (done < 100.0f) {
        done = 0.0f;
        for (int i=0; i < 16; i++) {done += completedThreads_[i];}
        done = int(done / 16.0f * 100.0f);
        if (pdone != done) {
            pdone = done;
            for (int w = 0; w < ss.str().size(); w ++) { std::cout << "\b";}
            ss.str("");
            ss << done << "\% done, " << rayCount_ << " rays cast, " << int(statusTime.elapsed()) << " s elapsed";
            std::cout << ss.str() << std::flush;
        }
        usleep(100);
    }
    for (int i=0; i < 16; i++) {completedThreads_[i] = 0;}

    for (int w = 0; w < ss.str().size(); w ++) { std::cout << "\b";}
    for (int w = 0; w < ss.str().size(); w ++) { std::cout << " ";}
    for (int w = 0; w < ss.str().size(); w ++) { std::cout << "\b";}

    std::cout << rayCount_ << " rays cast, Completed in " << statusTime.elapsed() << " s" << std::endl;
    std::cout << "Done" << std::endl;
}


void Raytracer::renderThreaded (Scene& scene, Image& image)
{
    rayCount_ = 0;

    scene.transformVertices();
    scene.camera_.setScreenDimensions(image.width(), image.height());

    int height = 4;
    int width = 4;

    if (statusOn) {
        std::cout << "Rendering..." << std::endl;
        std::cout << "  size " << image.width() << "x" << image.height() << std::endl;
        std::cout << "  " << height*width << " threads" << std::endl;
        std::cout << "  " << std::flush;
    }

    int boxw = image.width() / width;
    int boxh = image.height() / height;

    boost::thread renderThread[width][height];

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int threadId = y*height + x;

            int x0 = x * boxw + 1;
            int y0 = y * boxh + 1;
            int x1 = x0 + boxw -1;
            int y1 = y0 + boxh -1;
            
            renderThread[x][y] = boost::thread(
                                       &Raytracer::renderLetterBoxThread, 
                                       this, 
                                       scene, &image, x0, y0, x1, y1, 
                                       threadId);

        }
    }
    
    if (statusOn) {
        boost::thread status(&Raytracer::statusDisplay, this);
        status.join();
    }

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            renderThread[x][y].join();
        }
    }
    
}


TiXmlElement* Raytracer::getXml() {
    TiXmlElement * root = new TiXmlElement(xmlName.c_str());

    root->SetAttribute("reflectiondepth", maxReflectionDepth_);
    root->SetAttribute("subsamplesx", subSamplesX_);
    root->SetAttribute("subsamplesy", subSamplesY_);
    root->SetDoubleAttribute("exposure", exposure_);

    return root;
}

bool Raytracer::loadXml(TiXmlHandle &hRoot) {
    TiXmlElement* root = hRoot.FirstChild( xmlName.c_str() ).Element();
    if (!root) return false;
    std::cout << " Setting up " << root->Value() << std::endl;

    root->QueryIntAttribute("reflectiondepth", &maxReflectionDepth_);
    root->QueryIntAttribute("subsamplesx", &subSamplesX_);
    root->QueryIntAttribute("subsamplesy", &subSamplesY_);
    root->QueryFloatAttribute("exposure", &exposure_);

    std::cout << "  " << subSamplesX_ * subSamplesY_ << " samples/pixel" << std::endl;
    std::cout << "  Reflection depth " << maxReflectionDepth_ << std::endl;
    std::cout << "  Exposure " << exposure_ << std::endl;
    std::cout << " Done" <<std::endl;


    return true;
}
