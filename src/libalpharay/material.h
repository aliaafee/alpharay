/* vim: set ts=4 ss=4 sw=4 noet ai cindent : */

#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include <algorithm>

#include "vector.h"
#include "xmlobject.h"
#include "map.h"


enum ReflectionType { 
    DIFF, 
    SPEC, 
    FRES 
};

class Material : public XmlObjectNamed
{
    public:
        virtual void init();
        
        Material( std::string name ) 
            :XmlObjectNamed("material", name)
            { init(); }

		Material()
			{;}

        virtual ~Material() { } ;

        virtual void transform();

        virtual ReflectionType reflectionType() { return reflectionType_; }
        virtual void addReflection(Vector &reflection);
        virtual void addTransmission(Vector &transmission);

        virtual void setFresnelCoeff(float rcoeff, float tcoeff);

        virtual void addLight(Vector &lightIntensity, Vector &lightPosition, Vector &rayDirection, Vector &point, Vector &normal);

        virtual void setPoint(Vector& point, Vector2& point2);

        virtual Color color();
        virtual Color diffuseColor() { return diffuseColor_; }
        virtual Color emission() { return emission_; }
        virtual float reflectivity() { return reflectivity_; }
        virtual bool flatShading() { return flatShading_; }

		virtual Vector normalObjectSpace(Vector& os_normal, Vector& os_tangent, Vector& os_bitangent);

        virtual TiXmlElement* getXml();
        virtual bool loadXml(TiXmlElement* pElem, std::string path);

		virtual void setLog(bool log) { log_ = log; }
		virtual bool log() { return log_; }
        
    //protected:
        Color diffuseColor_;
        Color highlightColor_;
        Color reflection_;
        Color transmission_;
		
		Vector ts_normal_;

        float reflectionCoeff_;
        float transmissionCoeff_;

        ReflectionType reflectionType_;

        float reflectivity_;
        float opticDensity_;

        bool dielectric_;

        float scatterFactor_;
        int scatterSamples_;

        float ka_; //ambient reflection coefficient
        
		//float kd_; //diffuse reflection coefficient
        //float ks_; //specular reflection coefficient
        //float alpha_; //specular highlight amount

		float roughness_;
		float refractiveIndex_;

        bool flatShading_;

		bool log_;

        //For path tracing
        Color emission_;
        Color reflectance_;

        Map *diffuseMap_;
        Map *reflectivityMap_;
        Map *normalMap_;

    private:
        Color diffuseIntensity_;
        Color highlightIntensity_;

		float microfacetBRDF_;
		//float fresnelTerm_;
		//float distributionTerm_;
		//float geometryTerm_;
};


template <> inline std::string EditableLink<Material>::type() {
	return "material";
}


/*
#include <tinyxml.h>
#include <string>

#include "vector.h"
#include "texture.h"
#include "map.h"

#include "linklist.h"

class Material
{
public:
    std::string xmlName;

    std::string name_;

    Vector diffuseColor_;
	Vector highlightColor_;
	float reflectivity_;
    float opticDensity_;
    bool dielectric_;
    float scatterFactor_;
    int scatterSamples_;
	float ka_; //ambient reflection coefficient
	float kd_; //diffuse reflection coefficient
	float ks_; //specular reflection coefficient
	float alpha_; //specular highlight amount

    bool flatShading_;

	Map *diffuseMap_;
    Map *normalMap_;

    virtual void init();

	Material();
	Material(
        std::string name, 
		Vector diffuseColor,
		Vector highlightColor,
	    float reflectivity,
		float ka,
		float kd,
		float ks,
		float alpha,
	    Map *diffuseMap,
        Map *normalMap
	);

    Material(Material*);

	virtual ~Material() {};

    virtual void transform() {
        if (diffuseMap_ != NULL)
            diffuseMap_->transform();
        if (normalMap_ != NULL)
            normalMap_->transform();
    };

	virtual Vector getColor(Vector &reflection);
	virtual Vector getColor(Vector &point, UVTriangle *uvtriangle, Vector &reflection);
    virtual Vector getReflection();

    virtual Vector getNormal(Vector &point, UVTriangle *uvtriangle);

	virtual void resetLights();
    virtual void addLight(Vector &lightIntensity, Vector &lightPosition, Vector &rayDirection, Vector &point, Vector &normal);

    virtual bool loadXml(TiXmlElement* pElem, LinkList <Map> *linkMaps, LinkList <Material> *linkMaterials);
    TiXmlElement* getXml();
private:
    Vector diffuseIntensity_;
    Vector highlightIntensity_;
};
*/
#endif // _MATERIAL_H_
