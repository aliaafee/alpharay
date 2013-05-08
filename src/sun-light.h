/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */

#ifndef _SUN_LIGHT_H_
#define _SUN_LIGHT_H_

#include "light.h"


class SunLight : public Light, virtual public XmlObjectNamed
{
    public:
        virtual void init();

        SunLight( std::string name ) 
            : Light(name), XmlObjectNamed("sunlight", name)
            { init(); }

        ~SunLight() { } ;

        virtual void set(std::vector<Object*>* objects, Material &material, Vector &point, Vector &pointNormal, Vector &viewDirection);

        virtual TiXmlElement* getXml();
        virtual bool loadXml(TiXmlElement* pElem, std::string path);

    protected:
       int samples_;

       float size_; //Size in radians
};


#endif // _SUN_LIGHT_H_
