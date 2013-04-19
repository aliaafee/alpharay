/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */

#ifndef _SPOT_LIGHT_H_
#define _SPOT_LIGHT_H_

#include "light.h"


class SpotLight : public Light, virtual public XmlObjectNamed
{
    public:
        virtual void init();

        SpotLight( std::string name ) 
            : Light(name), XmlObjectNamed("spotlight", name)
            { init(); }

        ~SpotLight() { } ;

        virtual Color getIntensity(std::vector<Object*>* objects, Vector &point);

        virtual TiXmlElement* getXml();
        virtual bool loadXml(TiXmlElement* pElem, std::string path);

    //protected:
        Vector target_;
        float angle_;
        float angleFalloff_;

        Color getIntensityByAngle(Color intensity, Vector PO, Vector TO);
};


#endif // _SPOT_LIGHT_H_

