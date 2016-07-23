/* vim: set ts=4 ss=4 sw=4 noet ai cindent : */

#include "group.h"

void Group::init() {
	combinedEditableLinks_.reserve(editableLinks.size());
	combinedEditableLinks_.insert(combinedEditableLinks_.end(), editableLinks.begin(), editableLinks.end());
}

void Group::transform() {
	Object::transform();
	
	for (unsigned long i = 0; i < lights.size(); i++) {
		(lights[i])->transform();
		
		(lights[i])->transMatrix =  (lights[i])->transMatrix * transMatrix;
		(lights[i])->transMatrixInv =  (lights[i])->transMatrixInv * transMatrixInv;
		(lights[i])->transMatrixNDInv =  (lights[i])->transMatrixNDInv * transMatrixNDInv;
		(lights[i])->transMatrixND = (lights[i])->transMatrixND * transMatrixND;
		(lights[i])->transMatrixNormalInv =  (lights[i])->transMatrixNormalInv * transMatrixNormalInv;
		(lights[i])->transMatrixNormal =  (lights[i])->transMatrixNormal * transMatrixNormal;
	}
	
    for (unsigned long i = 0; i < objects.size(); i++) {
		(objects[i])->transform();
				
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
	float d;
	float closest = BIG_NUM;

	Ray rayt = ray;
	
	BaseObject *currentObject = NULL;
	BaseObject *closestObject = NULL;
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


Light* Group::add(Light *light) 
{
	lights.push_back(light);
	
	std::vector<BaseEditableLink*>* editableLinks = light->getEditableLinksList();
	combinedEditableLinks_.reserve(editableLinks->size());
	combinedEditableLinks_.insert(combinedEditableLinks_.end(), editableLinks->begin(), editableLinks->end());

	return light;
}


Object* Group::add(Object *object) 
{
	objects.push_back(object); 

	std::vector<BaseEditableLink*>* editableLinks = object->getEditableLinksList();
	combinedEditableLinks_.reserve(editableLinks->size());
	combinedEditableLinks_.insert(combinedEditableLinks_.end(), editableLinks->begin(), editableLinks->end());

	return object;
}


bool Group::fromXml(TiXmlElement* pElem, Light** light, std::string path) {
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
        (*light)->loadXml(pElem, path);		
        return true;
    }
    return false;
}


bool Group::fromXml(TiXmlElement* pElem, Object** object, std::string path) {
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
        (*object)->loadXml(pElem, path);
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


bool Group::loadXml(TiXmlElement* pElem, std::string path) {
    Object::loadXml(pElem, path);

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

    std::cout << "  Done" << std::endl;	

    return true;
}
