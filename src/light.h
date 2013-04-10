/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */

#ifndef _LIGHT_H_
#define _LIGHT_H_

#include <vector>
#include <string>
#include <tinyxml.h>

#include "vector.h"
#include "ray.h"
#include "object.h"

class Light {
public:
    std::string xmlName;
    std::string name_;

    bool shadowsOn_;

	Vector position_;
	Vector intensity_;

    virtual void init() {xmlName="light";}
	
    Light() {init();};
	Light(std::string name, Vector position, Vector intensity) 
		{init(); name_ = name; position_ = position; intensity_ = intensity; shadowsOn_ = true;}
	
	virtual ~Light() {};

    Vector getIntensityByDistance(Vector &intensity, float &distance);

    virtual Vector getIntensity(std::vector<Object*>* objects, Vector &point, Object *ignore=NULL) {return Vector(0,0,0);}
	
    virtual Object* getFirstIntersection(std::vector<Object*>* objects, Ray &ray, float rayLimit, Object *ignore);

    virtual bool loadXml(TiXmlElement* pElem);
    virtual TiXmlElement* getXml();


    //virtual Vector getIntensity(Vector &point, float &distance) {return Vector(0,0,0);}
};


#endif // _LIGHT_H_
