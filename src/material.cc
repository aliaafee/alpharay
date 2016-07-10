/* vim: set ts=4 ss=4 sw=4 noet ai cindent : */

#include "material.h"


void Material::init() { 
	addEditable(new Editable<Color>("diffusecolor", &diffuseColor_, Color(1,1,1)));
	addEditable(new Editable<Color>("highlightcolor", &highlightColor_, Color(1,1,1)));

	addEditable(new Editable<float>("reflectivity", &reflectivity_, 1.0));
	addEditable(new Editable<float>("opticdensity", &opticDensity_, 1.0));
	addEditable(new Editable<bool>("dielectric", &dielectric_, false));
	addEditable(new Editable<float>("scatterfactor", &scatterFactor_, 0));
	addEditable(new Editable<int>("scattersamples", &scatterSamples_, 1));
	addEditable(new Editable<float>("ambient", &ka_, 0.1));
	addEditable(new Editable<float>("roughness_", &roughness_, 0.1));
	addEditable(new Editable<float>("refractive-index", &refractiveIndex_, 3));
	addEditable(new Editable<bool>("flatshading", &flatShading_, false));
	addEditable(new Editable<Vector>("emission", &emission_, Color(0,0,0)));
	addEditable(new Editable<Vector>("reflectance", &reflectance_, Color(0.5,0.5,0.5)));

	addEditableLink(new EditableLink<Map>("diffusemap", &diffuseMap_));
	addEditableLink(new EditableLink<Map>("reflectivitymap", &reflectivityMap_));
	addEditableLink(new EditableLink<Map>("normalmap", &normalMap_));

	//Fix this
	//addEditable(new Editable<Vector>("reflectionType_", &reflectionType_, DIFF));
	reflectionType_ = DIFF;

	log_ = false;
}


void Material::transform() 
{
    //scatterFactor_ = roughness_ - 0.01;
	//if (scatterFactor_ < 0) { scatterFactor_ = 0; }

	//reflectivity_ = 1 - roughness_;
	//if (reflectivity_ < 0) { reflectivity_ = 0; }
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
                        Vector &V, 
                        Vector &point, 
                        Vector &N)
{
	/*
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
	*/
	

	//Microofacet BRDF Model 
	//---------------------
	//src: http://simonstechblog.blogspot.com/2011/12/microfacet-brdf.html
	
	//Roughness 
	//float m = roughness_;

	//refractive index
	//float n = refractiveIndex_;

	Vector L;
	V_SUB(L, lightPosition, point);
	L.normalize();

	//Vector V;
	//V = viewerDirection;

	N.normalize();

	Vector H;
	H = N + L;
	H.normalize();

	//Shclick approximation to the Fresnel equation
	float f0 = pow((1 - refractiveIndex_)/(1 + refractiveIndex_), 2);
	float fresnelTerm = f0 + (1 - f0) * pow((1 - L * H), 5);

	float m2 = pow(roughness_, 2);

	/*
	//Blinn Phong distribution function
	float a = (2 / m2) - 2;
	float distributionTermBlinn = ( (a + 2) / (2 * M_PI) ) * (N * H);
	*/

	//Beckmann Distibution function
	float NdH = N * H;
	
	float NdH2 = pow(NdH, 2);
	float NdH4 = pow(NdH, 4);
	
	float distributionTermBeck = exp( (NdH2 - 1) / (m2 * NdH2) ) / (M_PI * m2 * NdH4);

	float NdL = N * L;
	float NdV = N * V;
	float VdH = V * H;

	//Implicit Geometry Function 
	float geometryTermImplicit = (NdL) * (NdV);

	/*
	//Cook-Torrnce Geometry Function 
	float x = ((2 * NdH) / VdH);
	float cookA = x * NdV;
	float cookB = x * NdL;
	float minAB = std::min(cookA, cookB);
	float geometryTermCook = std::min(1.0f, minAB);
	*/
	
	//TO DO: Approximation of Smith's shadowing function
	
	//Microfacte BRDF
	float brdf = (fresnelTerm * distributionTermBeck * geometryTermImplicit) / (4 * NdL * NdV);
	float diffuseTerm = 1 - f0 + ((1 - f0) * pow(NdL, 5));

	if (NdL > 0.0) {
		highlightIntensity_ += lightIntensity * brdf;
		diffuseIntensity_ += lightIntensity * diffuseTerm * NdL;
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
	XmlObjectNamed::loadXml <LinkList> (pElem, path, linkList);

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
