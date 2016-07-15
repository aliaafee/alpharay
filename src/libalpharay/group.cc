/* vim: set ts=4 ss=4 sw=4 noet ai cindent : */

#include "group.h"

void Group::init() {
	;
}

void Group::transform() {
	Object::transform();
	
	for (unsigned long i = 0; i < lights.size(); i++) {
		(lights[i])->transform();
		/*
		(lights[i])->transMatrix = transMatrix * (lights[i])->transMatrix;
		(lights[i])->transMatrixInv = transMatrixInv * (lights[i])->transMatrixInv;
		(lights[i])->transMatrixNDInv = transMatrixNDInv * (lights[i])->transMatrixNDInv;
		(lights[i])->transMatrixND = transMatrixND * (lights[i])->transMatrixND;
		(lights[i])->transMatrixNormalInv = transMatrixNormalInv * (lights[i])->transMatrixNormalInv;
		(lights[i])->transMatrixNormal = transMatrixNormal * (lights[i])->transMatrixNormal;
		*/
		(lights[i])->transMatrix =  (lights[i])->transMatrix * transMatrix;
		(lights[i])->transMatrixInv =  (lights[i])->transMatrixInv * transMatrixInv;
		(lights[i])->transMatrixNDInv =  (lights[i])->transMatrixNDInv * transMatrixNDInv;
		(lights[i])->transMatrixND = (lights[i])->transMatrixND * transMatrixND;
		(lights[i])->transMatrixNormalInv =  (lights[i])->transMatrixNormalInv * transMatrixNormalInv;
		(lights[i])->transMatrixNormal =  (lights[i])->transMatrixNormal * transMatrixNormal;
	}
	
    for (unsigned long i = 0; i < objects.size(); i++) {
		(objects[i])->transform();
		/*
		(objects[i])->transMatrix = transMatrix * (objects[i])->transMatrix;
		(objects[i])->transMatrixInv = transMatrixInv * (objects[i])->transMatrixInv;
		(objects[i])->transMatrixNDInv = transMatrixNDInv * (objects[i])->transMatrixNDInv;
		(objects[i])->transMatrixND = transMatrixND * (objects[i])->transMatrixND;
		(objects[i])->transMatrixNormalInv = transMatrixNormalInv * (objects[i])->transMatrixNormalInv;
		(objects[i])->transMatrixNormal = transMatrixNormal * (objects[i])->transMatrixNormal;
		*/
		
		(objects[i])->transMatrix =  (objects[i])->transMatrix * transMatrix;
		(objects[i])->transMatrixInv =  (objects[i])->transMatrixInv * transMatrixInv;
		(objects[i])->transMatrixNDInv =  (objects[i])->transMatrixNDInv * transMatrixNDInv;
		(objects[i])->transMatrixND = (objects[i])->transMatrixND * transMatrixND;
		(objects[i])->transMatrixNormalInv =  (objects[i])->transMatrixNormalInv * transMatrixNormalInv;
		(objects[i])->transMatrixNormal =  (objects[i])->transMatrixNormal * transMatrixNormal;
	}
	
}


BaseObject* Group::intersection(Ray &ray, float *distance, float limit)
{
	
	/*
	//if triangles list available, then process as leaf

	BaseObject *currentTrig = NULL;

	BaseObject *closestTrig = NULL;

	float curt = 0;
	float clot = BIG_NUM;

	for (unsigned long i=0; i < objects.size(); i++) {
		currentTrig = objects[i]->intersection(ray, &curt, BIG_NUM); 
		if (currentTrig != NULL) {
			if (curt > 0.0001 && curt < clot) {
				//if (curt < limitMax) {
					closestTrig = currentTrig;
					clot = curt;
					
					if (ray.shadowRay_) {
						*distance = clot;
						return closestTrig;
					}
				//}
			}
		}
	}

	*distance = clot;

	return closestTrig;
	*/
	float d;
	float closest = BIG_NUM;

	Ray rayt = ray;

	/*
	Ray rayt = transformRay(ray);
	rayt.calculateInverse();
	*/
	
	BaseObject *currentObject = NULL;
	BaseObject *closestObject = NULL;
	//std::cout << "M1" << std::endl;
	for (unsigned long i=0; i < objects.size(); i++) {
			currentObject = objects[i]->intersection(rayt, &d, BIG_NUM);   
			
			if (currentObject != NULL) {
                if (d > 0.0000001) {
				    if (d < closest) {
                        closestObject = currentObject;
                        closest = d;
				    }

					if (rayt.shadowRay_) {
						*distance = closest;
						return closestObject;
					}
                }
			}
	}
	
	*distance = closest;
	return closestObject;
}


bool Group::fromXml(TiXmlElement* pElem, Light** light, std::string path) {
    //std::cout << "light " << pElem->Value() << std::endl;
    std::string name(pElem->Value());
    *light = NULL;

    if (name == "pointlight") {
        *light = new Light("");
    } else if (name == "spotlight") {
        *light = new SpotLight("");
    } else if (name == "arealight") {
        *light = new AreaLight("");
    } else if (name == "skylight") {
        *light = new SkyLight("");
    } else if (name == "sunlight") {
        *light = new SunLight("");
    }

    if (*light) {
        (*light)->loadXml(pElem, path, &linkList_);
		//std::cout << *((*light)->getXml()) << std::endl;
		
        return true;
    }
    return false;
}


bool Group::fromXml(TiXmlElement* pElem, Object** object, std::string path) {
    //std::cout << "object " << pElem->Value() << std::endl;
    std::string name(pElem->Value());
    *object = NULL;

    if (name == "sphere") {
        *object = new Sphere("");
    } else if (name == "plane") {
        *object = new Plane("");
	} else if (name == "cone") {
        *object = new Cone("");
    } else if (name == "mesh") {
        *object = new Mesh("");
	} else if (name == "group") {
		*object = new Group("");
    }

    if (*object) {
        (*object)->loadXml(pElem, path, &linkList_);
		//std::cout << *((*object)->getXml()) << std::endl;

        return true;
    }
    return false;
}


template< typename T > void Group::addFromXml(TiXmlElement* pElem, std::string path) {
    if (pElem) {
        for (; pElem; pElem = pElem->NextSiblingElement()) {
            T* newthing;
            fromXml(pElem, &newthing, path);
            if (newthing) {
                add(newthing);
            }
        }
    }
}


TiXmlElement* Group::getXml() {
	TiXmlElement* root = Object::getXml();

    //lights
    TiXmlElement* lights_e = new TiXmlElement("lights");
    root->LinkEndChild(lights_e);

    for (unsigned long i = 0; i < lights.size(); i++) {
        lights_e->LinkEndChild( lights[i]->getXml() );
    }

    //objects
    TiXmlElement* objects_e = new TiXmlElement("objects");
    root->LinkEndChild(objects_e);

    for (unsigned long i = 0; i < objects.size(); i++) {
        objects_e->LinkEndChild( objects[i]->getXml() );
    }

    return root;
}


bool Group::loadXml(TiXmlElement* pElem, std::string path, LinkList *linkList) {
	init();

    Object::loadXml(pElem, path, linkList);

    std::cout << "  Loading Group..." << std::endl;

	//Get handle to element
    TiXmlHandle hRoot = TiXmlHandle(pElem);

    //lights
    pElem = hRoot.FirstChild( "lights" ).FirstChild().Element();
    addFromXml <Light> (pElem, path);
    std::cout << "   " << lights.size() << " lights" << std::endl; 

    //objects
    pElem = hRoot.FirstChild( "objects" ).FirstChild().Element();
    addFromXml <Object> (pElem, path);
    std::cout << "   " << objects.size() << " objects" << std::endl;

	linkList->append(linkList_);

    std::cout << "  Done" << std::endl;	

    return true;
}
