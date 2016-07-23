/* vim: set ts=4 ss=4 sw=4 noet ai cindent : */

#ifndef _SKY_MAP_H_
#define _SKY_MAP_H_

#include "map.h"


/* Sky Map
 * =======
 * Base on Preetham Model, http://www.cs.utah.edu/~shirley/papers/sunsky/sunsky.pdf
 * and this implementation ported from Open GLSL implementation by Simon Waller
 * http://simonwallner.at/project/atmospheric-scattering/, which was used to implement for
 * tree.js by http://twitter.com/blurspline, located at 
 * https://github.com/mrdoob/three.js/blob/master/examples/js/SkyShader.js
 *
 * The colors are not quite right yet, i guess tone mapping needs to be done as done in the 
 * tree.js implementation.
 * 
 */


class SkyMap : public Map, virtual public XmlObjectNamed
{
	public:
        virtual void init();
        
        SkyMap( std::string name ) 
            : XmlObjectNamed("skymap", name)
            { init(); }

        ~SkyMap() { } ;

		virtual void transform() { Map::transform(); }

        virtual Color color(Vector  point, Vector2 point2);

        virtual TiXmlElement* getXml();
        virtual bool loadXml(TiXmlElement* pElem, std::string path);

		float azimuth;
		float inclination;
		float reileighCoefficient;
		float mieCoefficient;
		float mieDirectionalG;
		float turbidity;
	private:
		float n;
		float N;
		float pn;
		Vector lambda;
		Vector K;
		float v;
		float rayleighZenithLength;
		float mieZenithLength;
		Vector up;
		float E;
		float sunAngularDiameterCos;
		float cutoffAngle;
		float steepness;
		Vector cameraPosition;

		Vector totalRayleigh(Vector lambda);
		float rayleighPhase(float cosTheta);
		Vector totalMie(Vector lambda, Vector K, float T);
		float hgPhase(float cosTheta, float g);
		float sunIntensity(float zenithAngleCos);
		float logLuminance(Color c);

		Vector sunDirection;
};

#endif // _SKY_MAP_H_
