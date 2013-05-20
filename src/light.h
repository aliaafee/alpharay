/* vim: set ts=4 ss=4 sw=4 noet ai cindent : */

#ifndef _LIGHT_H_
#define _LIGHT_H_

#include <vector>

#include "vector.h"
#include "ray.h"
#include "xmlobject.h"
#include "baseobject.h"
#include "object.h"
#include "material.h"


class Light : virtual public XmlObjectNamed
{
    public:
        virtual void init();

        Light( std::string name ) 
            :XmlObjectNamed("pointlight", name)
            { init(); }

        ~Light() { } ;

        //Adds this light to the material
        virtual void set(std::vector<Object*>* objects, Material &material, Vector &point, Vector &pointNormal, Vector &viewDirection);

        virtual TiXmlElement* getXml();
        virtual bool loadXml(TiXmlElement* pElem, std::string path, LinkList *linkList);

    protected:
        float kIntensity_;
    	Vector position_;
    	Color intensity_;

        bool shadowsOn_;

        virtual Color getIntensityByDistance(Color intensity, float &distance);
        virtual BaseObject* getFirstIntersection(std::vector<Object*>* objects, Ray &ray, float rayLimit);
};



/*

#include <vector>
#include <string>
#include <tinyxml.h>



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
*/

#endif // _LIGHT_H_
