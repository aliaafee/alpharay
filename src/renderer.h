/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */

#ifndef _RENDERER_H_
#define _RENDERER_H_

#include <boost/thread.hpp>

#include "xmlobject.h"
#include "scene.h"
#include "image.h"

class Renderer : public XmlObject 
{
    public:
        virtual void init();
        
        Renderer () 
            : XmlObject("renderer") 
            { init(); }

        virtual void render (Scene &scene, Image *image, bool join);

        virtual TiXmlElement* getXml();
        virtual bool loadXml(TiXmlElement* pElem, std::string path);

    protected:
        int traceDepth_; 
        
        float exposure_;

        int subSamplesX_;
        int subSamplesY_;

        bool statusOn_;

        unsigned long raysCast_;

        virtual Color trace(Scene &scene ,Ray ray, int depth);

        BaseObject* closestIntersection(Scene &scene, Ray &ray, float *closest);
        
        void getFresnelValues(Vector Vincident, Vector normal,
                            float n1, float n2,
                            float cosT1, 
                            Vector *Vreflect, Vector *Vrefract,
                            float *reflectionCoeff, float *transmissionCoeff );

        void correctExposure(Color &color);
    private:
        int threadCount_;

        int cellCx, cellCy, curCell, maxCell, cellW, cellH;

        double completed;

        virtual void renderCell
            (Scene &scene, Image *image, int x0, int y0, int x1, int y1);

        bool renderAllCells(Scene& scene, Image* image);

        bool statusDisplay();

        bool getNextCell(int &x0, int &y0, int &x1, int &y1, int width, int height);

        void resetCells(Image* image);
};


#endif // _RENDERER_H_
