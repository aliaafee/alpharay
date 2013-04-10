/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */

#ifndef _POINT_LIGHT_H_
#define _POINT_LIGHT_H_

#include "light.h"

class PointLight: public Light 
{
public:
    virtual void init() { xmlName="pointlight"; }

    PointLight() {init();};
	PointLight(std::string name, Vector position,Vector intensity) 
        : Light(name, position, intensity) { init(); }

    
    Vector getIntensity(std::vector<Object*>* objects, Vector &point, Object *ignore);
	//Vector getIntensity(Vector &point, float &distance);

    virtual bool loadXml(TiXmlElement* pElem);
    virtual TiXmlElement* getXml();
};

#endif // _POINT_LIGHT_H_
