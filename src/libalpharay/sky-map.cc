/* vim: set ts=4 ss=4 sw=4 noet ai cindent : */

#include "sky-map.h"


void SkyMap::init()
{
    Map::init();

	n = 1.0003; // refractive index of air
	N = 2.545E25; // number of molecules per unit volume for air at
				// 288.15K and 1013mb (sea level -45 celsius)
	pn = 0.035;	// depolatization factor for standard air

	// wavelength of used primaries, according to preetham
	lambda = Vector(680E-9, 550E-9, 450E-9);

	// mie stuff
	// K coefficient for the primaries
	K = Vector(0.686f, 0.678f, 0.666f);
	v = 4.0f;


	// optical length at zenith for molecules
	rayleighZenithLength = 8.4E3;
	mieZenithLength = 1.25E3;
	up = Vector(0, 0, 1);

	E = 1000.0f;
	sunAngularDiameterCos = 0.999956676946448443553574619906976478926848692873900859324f;

	// earth shadow hack
	cutoffAngle = M_PI/1.95f;
	steepness = 1.5f;

	//default cameraPosition
	cameraPosition = Vector(0,0,0);	

	//default values
	azimuth = 0.5;
	inclination = 0.25;
	reileighCoefficient = 2;
	mieCoefficient = 0.005;
	mieDirectionalG = 0.8;
	turbidity = 10;
}


Color SkyMap::color(Vector  point, Vector2 point2)
{
	Vector worldSpacePosition = point;

	float sunE = sunIntensity(sunDirection * up);

	// extinction (absorbtion + out scattering)
	// rayleigh coefficients
	Vector betaR = totalRayleigh(lambda) * reileighCoefficient;

	// mie coefficients
	Vector betaM = totalMie(lambda, K, turbidity) * mieCoefficient;

	// optical length
	// cutoff angle at 90 to avoid singularity in next formula.
	//float zenithAngle = acos(max(0, dot(up, normalize(worldSpacePosition - vec3(0, 0, 0)))));
	float zenithAngle = acos(fmax(0, up * worldSpacePosition));
	float sR = rayleighZenithLength / (cos(zenithAngle) + 0.15 * pow(93.885 - ((zenithAngle * 180.0f) / M_PI), -1.253));
	float sM = mieZenithLength / (cos(zenithAngle) + 0.15 * pow(93.885 - ((zenithAngle * 180.0f) / M_PI), -1.253));

	// combined extinction factor	
	//vec3 Fex = exp(-(betaR * sR + betaM * sM));
	Vector Fex;
	Fex.x = exp(-(betaR.x * sR + betaM.x * sM));
	Fex.y = exp(-(betaR.y * sR + betaM.y * sM));
	Fex.z = exp(-(betaR.z * sR + betaM.z * sM));

	// in scattering
	Vector direction = worldSpacePosition - cameraPosition;
	direction.normalize();
	float cosTheta = direction * sunDirection;

	float rPhase = rayleighPhase(cosTheta);
	Vector betaRTheta = betaR * rPhase;

	float mPhase = hgPhase(cosTheta, mieDirectionalG);
	Vector betaMTheta = betaM * mPhase;

	//vec3 Lin = sunE * ((betaRTheta + betaMTheta) / (betaR + betaM)) * (1.0f - Fex);
	Vector Lin;
	Lin.x = sunE * ((betaRTheta.x + betaMTheta.x) / (betaR.x + betaM.x)) * (1.0f - Fex.x);
	Lin.y = sunE * ((betaRTheta.y + betaMTheta.y) / (betaR.y + betaM.y)) * (1.0f - Fex.y);
	Lin.z = sunE * ((betaRTheta.z + betaMTheta.z) / (betaR.z + betaM.z)) * (1.0f - Fex.z);

	// nightsky
	//vec3 direction = normalize(worldSpacePosition - cameraPosition);
	float theta = acos(direction.y); // elevation --> y-axis, [-pi/2, pi/2]
	float phi = atan2(direction.z, direction.x); // azimuth --> x-axis [-pi/2, pi/2]
	
	//vec2 uv = vec2(phi, theta) / vec2(2*pi, pi) + vec2(0.5f, 0.0f);
	Vector2 uv;
	uv.x = phi / 2*M_PI + 0.5f;
	uv.y = theta / M_PI + 0.0f;

	//vec3 L0 = texture(sDiffuse, uv).rgb * Fex;
	//vec3 L0 = vec3(0.1) * Fex;
	Vector L0 = Fex * 0.1;

	// composition + solar disc
	if (cosTheta > sunAngularDiameterCos)
		L0.x += sunE * Fex.x;
		L0.y += sunE * Fex.y;
		L0.z += sunE * Fex.z;

	Color result = Color( 
							L0.x + Lin.x,
							L0.y + Lin.y,
							L0.z + Lin.z );

	return result;

	/*

	point.normalize();
	float height =  fabs(point.z);
	return Color(height, height, height);*/
}


TiXmlElement* SkyMap::getXml()
{
    TiXmlElement* root = Map::getXml();

	root->SetAttribute("azimuth", ftos(azimuth));
    root->SetAttribute("inclination", ftos(inclination));
	root->SetAttribute("reileigh-coefficient", ftos(reileighCoefficient));
    root->SetAttribute("mie-coefficient", ftos(mieCoefficient));
	root->SetAttribute("mie-directional-g", ftos(mieDirectionalG));
    root->SetAttribute("turbidity", ftos(turbidity));

	
    return root;
}


bool SkyMap::loadXml(TiXmlElement* pElem, std::string path) 
{
    init();

    Map::loadXml(pElem, path);

	
    pElem->QueryFloatAttribute ("azimuth", &azimuth);
	pElem->QueryFloatAttribute ("inclination", &inclination);
	pElem->QueryFloatAttribute ("reileigh-coefficient", &reileighCoefficient);
	pElem->QueryFloatAttribute ("mie-coefficient", &mieCoefficient);
	pElem->QueryFloatAttribute ("mie-directional-g", &mieDirectionalG);
	pElem->QueryFloatAttribute ("turbidity", &turbidity);

	//Calculate sun position from inclination and azimuth
	float distance = 400000.0;
	float theta = M_PI * (inclination);
	float phi = 2 * M_PI * (azimuth);

	sunDirection.x = cos( phi );
	sunDirection.y = sin( phi );
	sunDirection.z = cos(theta);
	sunDirection.normalize();

	return true;
}


Vector SkyMap::totalRayleigh(Vector lambda)
{
	/**
	 * Compute total rayleigh coefficient for a set of wavelengths (usually
	 * the tree primaries)
	 * @param lambda wavelength in m
	 */

	//return (8 * pow(pi, 3) * pow(pow(n, 2) - 1, 2) * (6 + 3 * pn)) / (3 * N * pow(lambda, vec3(4)) * (6 - 7 * pn));
	Vector result;
	result.x = (8 * pow(M_PI, 3) * pow(pow(n, 2) - 1, 2) * (6 + 3 * pn)) / (3 * N * pow(lambda.x, 4) * (6 - 7 * pn));
	result.y = (8 * pow(M_PI, 3) * pow(pow(n, 2) - 1, 2) * (6 + 3 * pn)) / (3 * N * pow(lambda.y, 4) * (6 - 7 * pn));
	result.z = (8 * pow(M_PI, 3) * pow(pow(n, 2) - 1, 2) * (6 + 3 * pn)) / (3 * N * pow(lambda.z, 4) * (6 - 7 * pn));
	return result;
}

float SkyMap::rayleighPhase(float cosTheta)
{
	/** Reileight phase function as a function of cos(theta)
	 * NOTE: There are a few scale factors for the phase funtion
	 * (1) as given bei Preetham, normalized over the sphere with 4pi sr
	 * (2) normalized to integral = 1
	 * (3) nasa: integrates to 9pi / 4, looks best
	 */
	 
//	return (3.0f / (16.0f*pi)) * (1.0f + pow(cosTheta, 2));
//	return (1.0f / (3.0f*pi)) * (1.0f + pow(cosTheta, 2));
	return (3.0f / 4.0f) * (1.0f + pow(cosTheta, 2));
}

Vector SkyMap::totalMie(Vector lambda, Vector K, float T)
{
	/**
	 * total mie scattering coefficient
	 * @param lambda set of wavelengths in m
	 * @param K corresponding scattering param
	 * @param T turbidity, somewhere in the range of 0 to 20
	 */

	// not the formula given py Preetham.
	float c = (0.2f * T ) * 10E-18;

	//return 0.434 * c * pi * pow((2 * pi) / lambda, vec3(v - 2)) * K;
	Vector result;
	result.x = 0.434 * c * M_PI * pow((2 * M_PI) / lambda.x, v - 2) * K.x;
	result.y = 0.434 * c * M_PI * pow((2 * M_PI) / lambda.y, v - 2) * K.y;
	result.z = 0.434 * c * M_PI * pow((2 * M_PI) / lambda.z, v - 2) * K.z;
	return result;
}


float SkyMap::hgPhase(float cosTheta, float g)
{
	/**
	 * Henyey-Greenstein approximation as a function of cos(theta)
	 * @param cosTheta 
	 * @param g goemetric constant that defines the shape of the ellipse.
	 */

	return (1.0f / (4.0f*M_PI)) * ((1.0f - pow(g, 2)) / pow(1.0f - 2.0f*g*cosTheta + pow(g, 2), 1.5));
}


float SkyMap::sunIntensity(float zenithAngleCos)
{
//	return E;
	return E * fmax(0.0f, 1.0f - exp(-((cutoffAngle - acos(zenithAngleCos))/steepness)));
}


float SkyMap::logLuminance(Color c)
{
	/**
	 * log luminance according to a formula from wikipedia
	 * http://en.wikipedia.org/wiki/Luminance_(relative)
	 */

//	return 5.3f;
	//return log(c.r * 0.2126f + c.g * 0.7152f + c.b * 0.0722f);
	return log(c.x * 0.2126f + c.y * 0.7152f + c.z * 0.0722f);
}

