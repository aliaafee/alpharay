/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */

#ifndef _PATHTRACER_H_
#define _PATHTRACER_H_

#include "renderer.h"


class Pathtracer : public Renderer 
{
    public:
        virtual void init();
        
        Pathtracer () 
            : Renderer() 
            { init(); }

        virtual TiXmlElement* getXml();
        virtual bool loadXml(TiXmlElement* pElem, std::string path);

    protected:
        int samples_;

        virtual Color trace(Scene &scene ,Ray ray, int depth);
        virtual Color pathTrace(Scene &scene ,Ray &ray, int depth);

};

#endif // _PATHTRACER_H_
