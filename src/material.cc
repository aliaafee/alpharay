/* vim: set ts=4 ss=4 sw=4 noet ai cindent : */

#include "material.h"


void Material::init() { 
    XmlObjectNamed::init();

    //Generates a default material
	diffuseColor_ = Color(1,1,1);
	highlightColor_ = Color(1,1,1);

    reflectionType_ = DIFF;

	reflectivity_ = 1.0;
    opticDensity_ = 1.66f;
    dielectric_ = false;
    scatterFactor_ = 0;
    scatterSamples_ = 1;
	ka_ = 0.1;
	kd_ = 0.8;
	ks_ = 0.8; 
	alpha_ = 15; 

    flatShading_ = false;

    emission_ = Color(0, 0, 0);
    reflectance_ = Color(0.5, 0.5, 0.5);

	diffuseMap_ = NULL;
    reflectivityMap_ = NULL;
    normalMap_ = NULL;

	log_ = false;
}


void Material::transform() 
{
    ;
}


void Material::addReflection(Vector &reflection)
{
    reflection_ += reflection;
}


void Material::addTransmission(Vector &transmission)
{
    transmission_ += transmission;
}

void Material::setFresnelCoeff(float rcoeff, float tcoeff)
{
    reflectionCoeff_ = rcoeff;
    transmissionCoeff_ = tcoeff;
}


void Material::addLight(Vector &lightIntensity, 
                        Vector &lightPosition, 
                        Vector &viewerDirection, 
                        Vector &point, 
                        Vector &N)
{
    //Phong reflection model
    //
    //src: http://en.wikipedia.org/wiki/Phong_reflection_model

    Vector L;// = (lightPosition - point).getUnitVector();
	V_SUB(L, lightPosition, point);
	L.normalize();
    //N.normalize();
	Vector R = L.getReflection(N);
	//Vector V = viewerDirection;//.getUnitVector();

    double LdN = V_DOT(L, N);// = L*N;

	if (LdN > 0.0) {
        diffuseIntensity_ += lightIntensity * kd_ * LdN ;
		double RdV = V_DOT(R, viewerDirection);// = R*viewerDirection;
	    if (RdV > 0.0) {
            highlightIntensity_ += lightIntensity * ks_ * pow(RdV,alpha_);
        }
    }

}


Color Material::color()
{
    Color col = diffuseColor_;
    col.x = (col.x * ka_) + (col.x * diffuseIntensity_.x) ;
    col.y = (col.y * ka_) + (col.y * diffuseIntensity_.y) ;
    col.z = (col.z * ka_) + (col.z * diffuseIntensity_.z) ; 

    Color highlight;
    highlight.x = (highlightColor_.x * highlightIntensity_.x);
    highlight.y = (highlightColor_.y * highlightIntensity_.y);
    highlight.z = (highlightColor_.z * highlightIntensity_.z); 

    if ( reflectionType_ == FRES) {
        Color reflection = (reflection_ + highlight) * reflectionCoeff_ + transmission_ * transmissionCoeff_;
        return col * ( 1.0f - reflectivity_ ) + reflection * reflectivity_;
    } else if ( reflectionType_ == SPEC ) {
        return col * ( 1.0f - reflectivity_ ) + reflection_ * reflectivity_ + highlight;
    }
    return col + highlight;
}


Vector Material::normalObjectSpace(Vector& os_normal, Vector& os_tangent, Vector& os_bitangent)
{
	//if (normalMap_ != NULL) {
	return os_tangent * ts_normal_.x + os_bitangent * ts_normal_.y + os_normal * ts_normal_.z;
	//}
	//return Vector(0, 0, 0);
}


void Material::setPoint(Vector& point, Vector2& point2)
{
    if (diffuseMap_ != NULL) {
        diffuseColor_ = diffuseMap_->color(point, point2);
    }
    if (reflectivityMap_ != NULL) {
        reflectivity_ = reflectivityMap_->color(point, point2).intensity();
    }
	if (normalMap_ != NULL) {
		ts_normal_ = normalMap_->color(point, point2);
		ts_normal_ *= 2.0;
		ts_normal_.x -= 1.0;
		ts_normal_.y -= 1.0;
		ts_normal_.z -= 1.0;
	}
}


TiXmlElement* Material::getXml()
{
    TiXmlElement* root = XmlObjectNamed::getXml();

    root->SetAttribute("diffusecolor", diffuseColor_.str());
    root->SetAttribute("highlightcolor", highlightColor_.str());
    root->SetAttribute("reflectivity", ftos(reflectivity_));
    root->SetAttribute("dielectric", dielectric_);
    root->SetAttribute("opticdensity", ftos(opticDensity_));
    root->SetAttribute("scatterfactor", ftos(scatterFactor_));
    root->SetAttribute("scattersamples", scatterSamples_);
    root->SetAttribute("ka", ftos(ka_));
    root->SetAttribute("kd", ftos(kd_));
    root->SetAttribute("ks", ftos(ks_));
    root->SetAttribute("alpha", ftos(alpha_));
    root->SetAttribute("flatshading", flatShading_);

    root->SetAttribute("emission", emission_.str());
    root->SetAttribute("reflectance", reflectance_.str());
    
    if (diffuseMap_ == NULL) {
        root->SetAttribute("diffusemap", "");
    } else {
        root->SetAttribute("diffusemap", diffuseMap_->name());
    }

    if (reflectivityMap_ == NULL) {
        root->SetAttribute("reflectivitymap", "");
    } else {
        root->SetAttribute("reflectivitymap", reflectivityMap_->name());
    }

    if (normalMap_ == NULL) {
        root->SetAttribute("normalmap", "");
    } else {
        root->SetAttribute("normalmap", normalMap_->name());
    }

    std::string rt="";
    if (reflectionType_ == FRES) {
        rt = "fresnel";
    } else if (reflectionType_ == SPEC) {
        rt = "specular";
    } else {
        rt = "diffuse";
    }
    root->SetAttribute("reflection", rt);

    return root;
}


bool Material::loadXml(TiXmlElement* pElem, std::string path, LinkList *linkList)
{
    init();

    XmlObjectNamed::loadXml(pElem, path);

    pElem->QueryValueAttribute <Vector> ("diffusecolor", &diffuseColor_);
    pElem->QueryValueAttribute <Vector> ("highlightcolor", &highlightColor_);
    pElem->QueryFloatAttribute ("reflectivity", &reflectivity_);
    pElem->QueryBoolAttribute ("dielectric", &dielectric_);
    pElem->QueryFloatAttribute("opticdensity", &opticDensity_);
    pElem->QueryFloatAttribute ("scatterfactor", &scatterFactor_);
    pElem->QueryIntAttribute ("scattersamples", &scatterSamples_);
    pElem->QueryFloatAttribute ("ka", &ka_);
    pElem->QueryFloatAttribute ("kd", &kd_);
    pElem->QueryFloatAttribute ("ks", &ks_);
    pElem->QueryFloatAttribute ("alpha", &alpha_);
    pElem->QueryBoolAttribute ("flatshading", &flatShading_);

    pElem->QueryValueAttribute <Vector> ("emission", &emission_);
    pElem->QueryValueAttribute <Vector> ("reflectance", &reflectance_);

    std::string mapname; 

    mapname = "";
    pElem->QueryStringAttribute ("diffusemap", &mapname);
    linkList->add(mapname, &diffuseMap_);

    mapname = "";
    pElem->QueryStringAttribute ("reflectivitymap", &mapname);
    linkList->add(mapname, &reflectivityMap_);

    mapname = "";
    pElem->QueryStringAttribute ("normalmap", &mapname);
    linkList->add(mapname, &normalMap_);

    std::string rt="";
    pElem->QueryStringAttribute ("reflection", &rt);
    if (rt == "fresnel") {
        reflectionType_ = FRES;
    } else if (rt == "specular") {
        reflectionType_ = SPEC;
    } else {
        reflectionType_ = DIFF;
    }

    return true;
}


/*
Material::Material() {
    init();
}

Material::Material(std::string name, Vector diffuseColor, Vector highlightColor,float reflectivity, float ka, float kd, float ks, float alpha, Map *diffuseMap=NULL, Map *normalMap=NULL) {
    init();
    name_ = name;

	diffuseColor_ = diffuseColor;
	highlightColor_ = highlightColor;
	reflectivity_ = reflectivity;
    opticDensity_ = 1.66f;
    dielectric_ = false;
    scatterFactor_ = 0;
    scatterSamples_ = 1;
	ka_ = ka;
	kd_ = kd;
	ks_ = ks; 
	alpha_ = alpha; 

    flatShading_ = false;

	diffuseMap_ = diffuseMap;
    normalMap_ = normalMap;
}

Material::Material(Material* source) {
    init();
    if (source != NULL) {
    name_ = source->name_;

	diffuseColor_ = source->diffuseColor_;
	highlightColor_ = source->highlightColor_;
	reflectivity_ = source->reflectivity_;
    opticDensity_ = source->opticDensity_;
    dielectric_ = source->dielectric_;
    scatterFactor_ = source->scatterFactor_;
    scatterSamples_ = source->scatterSamples_;
	ka_ = source->ka_;
	kd_ = source->kd_;
	ks_ = source->ks_; 
	alpha_ = source->alpha_; 

    flatShading_ = source->flatShading_;

	diffuseMap_ = source->diffuseMap_;
    normalMap_ = source->normalMap_;
    }
}

Vector Material::getReflection() {
    Vector color;

    color.x = (highlightColor_.x * highlightIntensity_.x);
    
    color.y = (highlightColor_.y * highlightIntensity_.y);

    color.z = (highlightColor_.z * highlightIntensity_.z);

    return color;
}

Vector Material::getColor(Vector &reflection) {
    Vector color;

    color = diffuseColor_;

    
    color.x = (color.x * ka_) + (color.x * diffuseIntensity_.x) ;
    
    color.y = (color.y * ka_) + (color.y * diffuseIntensity_.y) ;

    color.z = (color.z * ka_) + (color.z * diffuseIntensity_.z) ;

    if (reflectivity_ > 0) {
        color = color * ( 1.0f - reflectivity_ ) + reflection * reflectivity_;
    }

    if (!dielectric_) {
        color += getReflection();
    }
    
    return color;
}

Vector Material::getColor(Vector &point, UVTriangle *uvtriangle, Vector &reflection) {
	if (diffuseMap_ != NULL) {
		diffuseColor_   = diffuseMap_->getColor(point, uvtriangle);
	}

    return getColor(reflection);
    //return diffuseColor_;
}


Vector Material::getNormal(Vector &point, UVTriangle *uvtriangle) {
    Vector normal(0,0,0);
    if (normalMap_ != NULL) {
        normal = normalMap_->getColor(point, uvtriangle);
    }
    return normal;
}

void Material::resetLights() {
    diffuseIntensity_.setNull();
    highlightIntensity_.setNull();
}

void Material::addLight(Vector &lightIntensity, 
                        Vector &lightPosition, 
                        Vector &viewerDirection, 
                        Vector &point, 
                        Vector &N) 
{
	Vector L = (lightPosition - point).getUnitVector();
    N.normalize();
	Vector R = L.getReflection(N);
	Vector V = viewerDirection.getUnitVector();

    double LdN = L*N;
    double RdV = R*V;

	if (LdN > 0.0) {
        diffuseIntensity_ += lightIntensity * kd_ * LdN ;
	    if (RdV > 0.0) {
            highlightIntensity_ += lightIntensity * ks_ * pow(RdV,alpha_);
        }
    }

}

bool Material::loadXml(TiXmlElement* pElem, LinkList <Map> *linkMaps, LinkList <Material> *linkMaterials) {
    name_ = "material";

    diffuseColor_ = Vector(255,0,0);
	highlightColor_ = Vector(255,255,255);
	reflectivity_ = 0;
    opticDensity_ = 1.66f;
    dielectric_ = false;
    scatterFactor_ = 0;
    scatterSamples_ = 1;
	ka_ = 0.1;
	kd_ = 0.8;
	ks_ = 0.8; 
	alpha_ = 15; 

	diffuseMap_ = NULL;
    normalMap_ = NULL;

    flatShading_ = false;

    pElem->QueryStringAttribute ("name", &name_);
    pElem->QueryValueAttribute <Vector> ("diffusecolor", &diffuseColor_);
    pElem->QueryValueAttribute <Vector> ("highlightcolor", &highlightColor_);
    pElem->QueryFloatAttribute ("reflectivity", &reflectivity_);
    pElem->QueryBoolAttribute ("dielectric", &dielectric_);
    pElem->QueryFloatAttribute("opticdensity", &opticDensity_);
    pElem->QueryFloatAttribute ("scatterfactor", &scatterFactor_);
    pElem->QueryIntAttribute ("scattersamples", &scatterSamples_);
    pElem->QueryFloatAttribute ("ka", &ka_);
    pElem->QueryFloatAttribute ("kd", &kd_);
    pElem->QueryFloatAttribute ("ks", &ks_);
    pElem->QueryFloatAttribute ("alpha", &alpha_);
    pElem->QueryBoolAttribute ("flatshading", &flatShading_);

    std::string mapname; 

    mapname = "";
    pElem->QueryStringAttribute ("diffusemap", &mapname);
    linkMaps->add(mapname, &diffuseMap_);

    mapname = "";
    pElem->QueryStringAttribute ("normalmap", &mapname);
    linkMaps->add(mapname, &normalMap_);

    return true;
}


TiXmlElement* Material::getXml() {
    TiXmlElement* root = new TiXmlElement(xmlName.c_str());

    root->SetAttribute("name", name_);
    root->SetAttribute("diffusecolor", diffuseColor_.str());
    root->SetAttribute("highlightcolor", highlightColor_.str());
    root->SetAttribute("reflectivity", ftos(reflectivity_));
    root->SetAttribute("dielectric", dielectric_);
    root->SetAttribute("opticdensity", ftos(opticDensity_));
    root->SetAttribute("scatterfactor", ftos(scatterFactor_));
    root->SetAttribute("scattersamples", scatterSamples_);
    root->SetAttribute("ka", ftos(ka_));
    root->SetAttribute("kd", ftos(kd_));
    root->SetAttribute("ks", ftos(ks_));
    root->SetAttribute("alpha", ftos(alpha_));
    root->SetAttribute("flatshading", flatShading_);
    
    if (diffuseMap_ == NULL) {
        root->SetAttribute("diffusemap", "");
    } else {
        root->SetAttribute("diffusemap", diffuseMap_->name_);
    }

    if (normalMap_ == NULL) {
        root->SetAttribute("normalmap", "");
    } else {
        root->SetAttribute("normalmap", normalMap_->name_);
    }

    return root;
}

*/
