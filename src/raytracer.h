/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */

#ifndef _RAYTRACER_H_
#define _RAYTRACER_H_

#include "renderer.h"


class Raytracer : public Renderer 
{
    public:
        virtual void init();
        
        Raytracer () 
            : Renderer() 
            { init(); }

        virtual TiXmlElement* getXml();
        virtual bool loadXml(TiXmlElement* pElem, std::string path);

    protected:
        virtual Color trace(Scene &scene ,Ray ray, int depth);

    private:
        void setLighting(Scene &scene, Material &material, Vector &point, Vector &pointNormal, Vector &viewDirection);
        void traceFresnel(Scene &scene, Ray ray, 
                            Vector intersectionPoint, Vector intersectionNormal,
                            Material &material,
                            int depth);
        void traceReflection(Scene &scene, Ray ray, 
                            Vector intersectionPoint, Vector intersectionNormal,
                            Material &material,
                            int depth);
        Vector raytraceDistributed(Scene &scene, Ray ray, int depth, 
                                        float scatterFactor, int scatterSamples);


};

/*

#include "image.h"
#include "vector.h"
#include "scene.h"
#include "baseobject.h"
#include "ray.h"
#include "map.h"
#include "material.h"

#include <boost/thread.hpp>
#include <boost/timer.hpp>
#include <tinyxml.h>
#include <string>
#include <sstream> 

//#include <iomanip>

class Raytracer {
public:
    int maxReflectionDepth_;
    int subSamplesX_;
    int subSamplesY_;
    float exposure_;

    bool statusOn;

    std::string xmlName;
    

	Raytracer() {
        xmlName = "raytracer";

        reflectionOn_ = true; 
        lightingOn_ = true; 
        maxReflectionDepth_ = 3; 
        subSamplesX_ = 1;
        subSamplesY_ = 1;

        exposure_ = -1.0f;

        pathTraceDepth_ = 10;

        statusOn = false;
    }

	Raytracer(bool lightingOn,bool reflectionOn,int maxReflectionDepth) {
        reflectionOn_ = reflectionOn; 
        lightingOn_ = lightingOn; 
        maxReflectionDepth_ = maxReflectionDepth;
    }

	void render (Scene&, Image&);

    void renderThreaded (Scene&, Image&, bool join);

    TiXmlElement* getXml();
    bool loadXml(TiXmlHandle &hRoot);
private:
    void statusDisplay();
    void renderLetterBox (Scene &scene, Image *image, int x0, int y0, int x1, int y1, float* done);
    void renderLetterBoxThread (Scene &scene, Image *image, int x0, int y0, int x1, int y1, int threadId);//, boost::barrier* bar);

	BaseObject* getClosestIntersection(Scene &scene, Ray &ray, float *t);

    void setLighting(Scene &scene, Material &material, Vector &point, Vector &pointNormal, Vector &viewDirection);

    Color pathTrace(Scene &scene ,Ray ray, int depth);
	
    Vector raytrace(Scene &scene ,Ray ray, int *reflectionDepth);

    Vector raytraceDistributed(Scene &scene, Ray ray, int *reflectionDepth, 
                                        float scatterFactor, int scatterSamples);

    Vector correctExposure(Vector color);

    void getFresnelValues(Vector Vincident, Vector normal,
                        float n1, float n2,
                        float cosT1, 
                        Vector *Vreflect, Vector *Vrefract,
                        float *reflectionCoeff, float *transmissionCoeff );

    //Vector getReflection(Vector Vincident, Vector normal, float cosT1);

    void traceFresnel(Scene &scene, Ray ray, 
                        Vector intersectionPoint, Vector intersectionNormal,
                        Material &material,
                        int *reflectionDepth);

    void traceReflection(Scene &scene, Ray ray, 
                        Vector intersectionPoint, Vector intersectionNormal,
                        Material &material,
                        int *reflectionDepth);
	
    bool reflectionOn_;
	bool lightingOn_;

	unsigned int cX,cY;

    float completedThreads_[16];
    float rayCount_;


    int pathTraceDepth_;
};
*/

#endif // _RAYTRACER_H_
