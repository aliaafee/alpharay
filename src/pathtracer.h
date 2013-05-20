/* vim: set ts=4 ss=4 sw=4 noet ai cindent : */

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
