/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */

#include "camera.h"

Camera::Camera (Vector position, Vector target, double fOV, double tilt) {
    xmlName = "camera";

	position_ = position;
	target_ = target;
	fOV_ = fOV;
	tilt_ = tilt;
}

void Camera::setScreenDimensions(double screenWidth, double screenHeight) {
	//See the attached document for details
	realScreenHeight_ = screenHeight;
	realScreenWidth_ = screenWidth;

	Vector CA = (target_ - position_);
	CA.normalize();
	Vector C = position_ + CA;
	Vector CE = CA%Vector(0,0,1);
	if (CE.isNull()) { CE.y = 1; }
	CE.normalize();
	Vector CD = CE%CA;
	//CD.normalize(); not required as CE and CA are already perpedicular hence CD has a magnitude of 1

	CE = CE  * tan(fOV_/2.0);
	CD = CD * ((CE.magnitude() * screenHeight) / screenWidth);

	
	
	Vector F = C + CE + CD;
	Vector G = C + CD - CE;
	Vector I = C + CE - CD;

	F = F.rotateAboutAxis(CA,tilt_);
	G = G.rotateAboutAxis(CA,tilt_);
	I = I.rotateAboutAxis(CA,tilt_);
	
	screen_1 = F ;
	screen_h = I - F;
	screen_w = G - F;
}

Vector Camera::getScreenPosition(double x, double y) {
	return 	screen_1 + (
			(screen_w * (x/realScreenWidth_)) //Left Position
			+ 
			(screen_h * (y/realScreenHeight_)) //TopPosition
		);
}

void Camera::orbitX(float angle) {
	Vector v = position_ - target_;
	v = v.rotateX(angle);
	position_ = v + target_;
}

void Camera::orbitY(float angle) {
	Vector v = position_ - target_;
	v = v.rotateY(angle);
	position_ = v + target_;
}


void Camera::orbitZ(float angle) {
	Vector v = position_ - target_;
	v = v.rotateZ(angle);
	position_ = v + target_;
}

bool Camera::loadXml(TiXmlHandle &hTopRoot) {
    TiXmlElement* root = hTopRoot.FirstChild( xmlName.c_str() ).Element();
    if (!root) return false;

    root->QueryValueAttribute <Vector> ("position", &position_);
    root->QueryValueAttribute <Vector> ("target", &target_);
    root->QueryFloatAttribute("fov", &fOV_);
    root->QueryFloatAttribute("tilt", &tilt_);

    return true;
}

TiXmlElement* Camera::getXml() {
    TiXmlElement* root = new TiXmlElement(xmlName.c_str());

    root->SetAttribute("position", position_.str());
    root->SetAttribute("target", target_.str());
    root->SetDoubleAttribute("fov", fOV_);
    root->SetDoubleAttribute("tilt", tilt_);

    return root;
}
