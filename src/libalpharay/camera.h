/* vim: set ts=4 ss=4 sw=4 noet ai cindent : */

#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "vector.h"
#include "ray.h"
#include "xmlobject-named.h"


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

        virtual void setScreen(float screenWidth, float screenHeight);
        virtual Ray ray(float x, float y);

        virtual void orbitX(float angle);
        virtual void orbitY(float angle);
        virtual void orbitZ(float angle);

        virtual TiXmlElement* getXml();
        virtual bool loadXml(TiXmlElement* pElem, std::string path, LinkList* linkList);
 
        Vector position_;
        Vector target_;
        float tilt_;
        float fOV_;

    protected:
        float realScreenWidth_,realScreenHeight_;
	    Vector screen_0,screen_1,screen_2,screen_w,screen_h;
};


class CameraPano : public Camera, virtual public XmlObjectNamed
{
    public:
        void init() { Camera::init(); }

        CameraPano() 
            : XmlObjectNamed ("camera", "unnamed") 
            { init(); }

        CameraPano(std::string name) 
            : XmlObjectNamed ("camera", name) 
            { init(); }

        ~CameraPano() {};

        virtual void setScreen(float screenWidth, float screenHeight);
        virtual Ray ray(float x, float y);

        virtual TiXmlElement* getXml();
};

#endif // _CAMERA_H_
