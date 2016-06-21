/* vim: set ts=4 ss=4 sw=4 noet ai cindent : */

#include "camera.h"

void Camera::init() 
{
    XmlObjectNamed::init();

    position_ = Vector(0, 0, 0);
	target_ = Vector(0, 1, 0);
	fOV_ = M_PI/4.0f;
	tilt_ = 0.0f; 
}


void Camera::setScreen(float screenWidth, float screenHeight) 
{
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


Ray Camera::ray(float x, float y) 
{
    Ray ray;
    
    //Plane Camera
	Vector target = 	screen_1 + (
			(screen_w * ((realScreenWidth_-x)/realScreenWidth_)) //Left Position
			+ 
			(screen_h * (y/realScreenHeight_)) //TopPosition
		);

    ray.direction_ = target - position_;

    ray.position_ = position_;
    ray.direction_.normalize();
	ray.position2_ = Vector2(x/realScreenWidth_,y/realScreenHeight_);
    

    return ray;
}


void Camera::orbitX(float angle) 
{
	Vector v = position_ - target_;
	v = v.rotateX(angle);
	position_ = v + target_;
}


void Camera::orbitY(float angle) 
{
	Vector v = position_ - target_;
	v = v.rotateY(angle);
	position_ = v + target_;
}


void Camera::orbitZ(float angle) 
{
	Vector v = position_ - target_;
	v = v.rotateZ(angle);
	position_ = v + target_;
}


bool Camera::loadXml(TiXmlElement* pElem, std::string path) 
{
    init();

    XmlObjectNamed::loadXml(pElem, path);

    pElem->QueryValueAttribute <Vector> ("position", &position_);
    pElem->QueryValueAttribute <Vector> ("target", &target_);
    pElem->QueryFloatAttribute("fov", &fOV_);
    pElem->QueryFloatAttribute("tilt", &tilt_);

    return true;
}


TiXmlElement* Camera::getXml() 
{
    TiXmlElement* root = XmlObjectNamed::getXml();

    root->SetAttribute("position", position_.str());
    root->SetAttribute("target", target_.str());
    root->SetDoubleAttribute("fov", fOV_);
    root->SetDoubleAttribute("tilt", tilt_);

    return root;
}


void CameraPano::setScreen(float screenWidth, float screenHeight) 
{
	//See the attached document for details
	realScreenHeight_ = screenHeight;
	realScreenWidth_ = screenWidth;
}


Ray CameraPano::ray(float x, float y) 
{
    Ray ray;
    
    //Panoramic camera
    float r = 1;
    float p = x/realScreenWidth_  * 2 * M_PI;
    float t = y/realScreenHeight_ * M_PI;

    ray.direction_ =  Vector( r * sin(t) * cos(p),
                               r * sin(t) * sin(p),
                               r * cos(t) );

    ray.position_ = position_;
    ray.direction_.normalize();

    return ray;
}


TiXmlElement* CameraPano::getXml() 
{
    TiXmlElement* root = Camera::getXml();

    root->SetAttribute("type", "pano");

    return root;
}


