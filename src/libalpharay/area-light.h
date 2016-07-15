/* vim: set ts=4 ss=4 sw=4 noet ai cindent : */

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

        virtual void set(std::vector<Object*>* objects, Material &material, Vector &point, Vector &pointNormal, Vector &viewDirection);

    protected:
        float lightRadius_;
        int samples_;
};

#endif // _AREA_LIGHT_H_

