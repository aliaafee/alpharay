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


class Light : virtual public Object, virtual public XmlObjectNamed
{
    public:
        virtual void init();

        Light( std::string name ) 
            :Object(name, NULL), XmlObjectNamed("pointlight", name)
            { init(); }

        virtual ~Light() { } ;

		virtual void transform();

        //Adds this light to the material
        virtual void set(std::vector<Object*>* objects, Material &material, Vector &point, Vector &pointNormal, Vector &viewDirection);

    
        float kIntensity_;
		Vector tposition_; //Position of the light when the transformations have been applied


		Color color_;
		float energy_;
		
    	//Vector intensity_;

        bool shadowsOn_;
	protected:
        virtual Color getIntensityByDistance(Color intensity, float &distance);
		virtual float getEnergyAtDistance(const float &totalEnergy, const float &distance);
        virtual BaseObject* getFirstIntersection(std::vector<Object*>* objects, Ray &ray, float rayLimit);
};


#endif // _LIGHT_H_
