/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */

#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "vector.h"
#include "xmlobject.h"


class Camera : virtual public XmlObjectNamed
{
    public:
        void init();

        Camera() 
            : XmlObjectNamed ("camera", "unnamed") 
            { init(); }

        Camera(std::string name) 
            : XmlObjectNamed ("camera", name) 
            { init(); }

        ~Camera() {};

        virtual void setScreenDimensions(double screenWidth, double screenHeight);
        virtual Vector getScreenPosition(double x, double y);
        virtual void orbitX(float angle);
        virtual void orbitY(float angle);
        virtual void orbitZ(float angle);

        virtual TiXmlElement* getXml();
        virtual bool loadXml(TiXmlElement* pElem, std::string path);

    //protected:
        Vector position_;
        Vector target_;
        float tilt_;
        float fOV_;

    private:
        float realScreenWidth_,realScreenHeight_;
	    Vector screen_0,screen_1,screen_2,screen_w,screen_h;
};

/*
#include <tinyxml.h>

#include "vector.h"

class Camera
{
public:
    std::string xmlName;
	Vector position_;
	Vector target_;
	float tilt_;
	float fOV_;
	
	Camera () {fOV_ = M_PI/4; tilt_ = 0; xmlName = "camera";}
	Camera (Vector position, Vector target, double fOV, double tilt);
	void setScreenDimensions(double screenWidth, double screenHeight);
	Vector getScreenPosition(double x, double y);
	void orbitX(float angle);
	void orbitY(float angle);
	void orbitZ(float angle);

    TiXmlElement* getXml();
    bool loadXml(TiXmlHandle &hTopRoot);
private:
	double realScreenWidth_,realScreenHeight_;
	Vector screen_0,screen_1,screen_2,screen_w,screen_h;
};
*/
#endif // _CAMERA_H_
