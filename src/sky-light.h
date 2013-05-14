/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */

#ifndef _SKY_LIGHT_H_
#define _SKY_LIGHT_H_

#include "light.h"
#include "map.h"


class SkyLight : public Light, virtual public XmlObjectNamed
{
    public:
        virtual void init();

        SkyLight( std::string name ) 
            : Light(name), XmlObjectNamed("skylight", name)
            { init(); }

        ~SkyLight() { } ;

        virtual void set(std::vector<Object*>* objects, Material &material, Vector &point, Vector &pointNormal, Vector &viewDirection);

        virtual TiXmlElement* getXml();
        virtual bool loadXml(TiXmlElement* pElem, std::string path, LinkList *linkList);

    protected:
       int samples_;

       Map* lightMap_;
};


#endif // _SKY_LIGHT_H_
