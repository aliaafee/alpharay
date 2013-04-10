/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */

#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <tinyxml.h>

#include "vector.h"

class Camera
{
friend std::ostream& operator<<(std::ostream& os, Camera& camera);
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

inline std::ostream& operator<<(std::ostream& os, Camera& camera){
	os << "camera["<< camera.position_ << camera.target_ <<camera.screen_0 << camera.screen_1 << camera.screen_2  << camera.screen_w  <<camera.screen_h <<"]";
	return os;
}
#endif // _CAMERA_H_
