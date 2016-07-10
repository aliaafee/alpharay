/* vim: set ts=4 ss=4 sw=4 noet ai cindent : */

#ifndef _SPOT_LIGHT_H_
#define _SPOT_LIGHT_H_

#include "light.h"


class SpotLight : public Light, virtual public XmlObjectNamed
{
    public:
        virtual void init();

        SpotLight( std::string name ) 
            : Light(name), XmlObjectNamed("spotlight", name)
            { init(); }

        ~SpotLight() { } ;

        virtual void set(std::vector<Object*>* objects, Material &material, Vector &point, Vector &pointNormal, Vector &viewDirection);

    protected:
        Vector target_;
        float angle_;
        float angleFalloff_;

        Color getIntensityByAngle(Color intensity, Vector PO, Vector TO);
};


#endif // _SPOT_LIGHT_H_

