/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */

#ifndef _AREA_LIGHT_H_
#define _AREA_LIGHT_H_

#include "spot-light.h"


class AreaLight : public SpotLight, virtual public XmlObjectNamed
{
    public:
        virtual void init();

        AreaLight( std::string name ) 
            : SpotLight(name), XmlObjectNamed("spotlight", name)
            { init(); }

        ~AreaLight() { } ;

        virtual Color getIntensity(std::vector<Object*>* objects, Vector &point);

        virtual TiXmlElement* getXml();
        virtual bool loadXml(TiXmlElement* pElem, std::string path);

    //protected:
        float lightRadius_;
        int samples_;
};


/*
class AreaLight: public SpotLight 
{
public:
    float lightRadius_;
    int samples_;

    virtual void init() { xmlName="arealight"; }

    AreaLight() {init();};
	AreaLight(std::string name, Vector position, Vector target, float radius, float angle, Vector intensity) 
        : SpotLight (name, position, target, angle, intensity) 
		{init(); lightRadius_ = radius; samples_ = 10;}

    virtual Vector getIntensity(std::vector<Object*>* objects, Vector &point, Object *ignore);
    
    virtual bool loadXml(TiXmlElement* pElem);
    TiXmlElement* getXml();
};
*/

#endif // _AREA_LIGHT_H_

