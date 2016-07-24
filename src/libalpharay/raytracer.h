/* vim: set ts=4 ss=4 sw=4 noet ai cindent : */

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
        virtual Color trace(Scene* scene ,Ray ray, int depth);

    private:
        void setLighting(Scene* scene, Material &material, Vector &point, Vector &pointNormal, Vector &viewDirection);
        void traceFresnel(Scene* scene, Ray ray, 
                            Vector intersectionPoint, Vector intersectionNormal,
                            Material &material,
                            int depth);
        void traceReflection(Scene* scene, Ray ray, 
                            Vector intersectionPoint, Vector intersectionNormal,
                            Material &material,
                            int depth);
        Vector raytraceDistributed(Scene* scene, Ray ray, int depth, 
                                        float scatterFactor, int scatterSamples);


};

#endif // _RAYTRACER_H_
