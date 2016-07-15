/* vim: set ts=4 ss=4 sw=4 noet ai cindent : */

#include "scene.h"

void Scene::init() {
	rayLog_ = false;
	
	addEditable(new Editable<Color>("skycolor", &envColor_, Vector(0, 0, 0)));
	addEditableLink(new EditableLink<Map>("skymap", &envMap_));
}

Color Scene::envColor(const Ray &ray) {
    if (envMap_ == NULL) {
        return envColor_;
    }

    return envMap_->color(ray.direction_, ray.position2_);
}

void Scene::transform() {
	for (unsigned long i = 0; i < images.size(); i++) {
		(images[i])->transform();
	}
    for (unsigned long i = 0; i < maps.size(); i++) {
		(maps[i])->transform();
	}
    for (unsigned long i = 0; i < materials.size(); i++) {
		(materials[i])->transform();
	}
	for (unsigned long i = 0; i < lights.size(); i++) {
		(lights[i])->transform();
	}
    for (unsigned long i = 0; i < objects.size(); i++) {
		(objects[i])->transform();
	}
}

void Scene::setScreen(float width, float height)
{
    camera_->setScreen(width, height);
}

Ray Scene::ray(float x, float y)
{
    return camera_->ray(x, y);
}


bool Scene::fromXml(TiXmlElement* pElem, Light** light, std::string path){
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
    }

	return true;
}


bool Scene::fromXml(TiXmlElement* pElem, Image** image, std::string path) {
    //std::cout << "image " << pElem->Value() << std::endl;
    std::string name(pElem->Value());
    *image = NULL;

    if (name == "checker2d") {
        *image = new Checker2d("");
    } else if (name == "bitmap") {
        *image = new Bitmap("");
    } else if (name == "canvas") {
        *image = new Canvas("");
    }


    if (*image) {
        (*image)->loadXml(pElem, path, &linkList_);
		return (*image)->load();
    }

    return true;
}


bool Scene::fromXml(TiXmlElement* pElem, Map** map, std::string path) {
    //std::cout << "map " << pElem->Value() << std::endl;
    std::string name(pElem->Value());
    *map = NULL;

    if (name == "map2d") {
        *map = new Map2d("");
    } else if (name == "map2dplane") {
        *map = new Map2dPlane("");
    } else if (name == "map2dcylindrical") {
        *map = new Map2dCylindrical("");
    } else if (name == "map2dspherical") {
        *map = new Map2dSpherical("");
	} else if (name == "skymap") {
		*map = new SkyMap("");
    }

    if (*map) {
        (*map)->loadXml(pElem, path, &linkList_);
    }

    return true;
}


bool Scene::fromXml(TiXmlElement* pElem, Material** mat, std::string path) {
    //std::cout << "mat " << pElem->Value() << std::endl;
    std::string name(pElem->Value());
    *mat = NULL;

    if (name == "material") {
        *mat = new Material("");
    }

    if (*mat) {
        (*mat)->loadXml(pElem, path, &linkList_);
    }

    return true;
}


bool Scene::fromXml(TiXmlElement* pElem, Object** object, std::string path) {
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
    }
    
	return true;
}


template< typename T > bool Scene::addFromXml(TiXmlElement* pElem, std::string path) {
    if (pElem) {
        for (; pElem; pElem = pElem->NextSiblingElement()) {
            T* newthing;
            bool result = fromXml(pElem, &newthing, path);
			if (!result) {
				return false;
			}
            if (newthing) {
                add(newthing);
            }
			if (cancelLoad_) { return false; }
        }
    }
	return true;
}


TiXmlElement* Scene::getXml() {
    TiXmlElement* root = XmlObjectNamed::getXml();

    //camera
	if (camera_ != NULL) {
		root->LinkEndChild(camera_->getXml());
	}

    //lights
    TiXmlElement* lights_e = new TiXmlElement("lights");
    root->LinkEndChild(lights_e);

    for (unsigned long i = 0; i < lights.size(); i++) {
        lights_e->LinkEndChild( lights[i]->getXml() );
    }

    //images
    TiXmlElement* images_e = new TiXmlElement("images");
    root->LinkEndChild(images_e);

    for (unsigned long i = 0; i < images.size(); i++) {
        images_e->LinkEndChild( images[i]->getXml() );
    }

    //maps
    TiXmlElement* maps_e = new TiXmlElement("maps");
    root->LinkEndChild(maps_e);

    for (unsigned long i = 0; i < maps.size(); i++) {
        maps_e->LinkEndChild( maps[i]->getXml() );
    }

    //materials
    TiXmlElement* materials_e = new TiXmlElement("materials");
    root->LinkEndChild(materials_e);

    for (unsigned long i = 0; i < materials.size(); i++) {
        materials_e->LinkEndChild( materials[i]->getXml() );
    }

    //objects
    TiXmlElement* objects_e = new TiXmlElement("objects");
    root->LinkEndChild(objects_e);

    for (unsigned long i = 0; i < objects.size(); i++) {
        objects_e->LinkEndChild( objects[i]->getXml() );
    }

    return root;
}


bool Scene::loadXml(TiXmlElement* pElem, std::string path, LinkList* linkList) {
	cancelLoad_ = false;

    XmlObjectNamed::loadXml(pElem, path, linkList);

    std::cout << " Loading Scene..." << std::endl;

    //Get handle to element
    TiXmlHandle hRoot = TiXmlHandle(pElem);

    //camera
    std::cout << "  Setting up camera" << std::endl;
    pElem = hRoot.FirstChild( "camera" ).Element();
    if (pElem) {
        std::string type = "";
        pElem->QueryStringAttribute("type", &type);
        if (type == "pano") {
            camera_ = new CameraPano();   
        } else {
            camera_ = new Camera();
        }
        camera_->loadXml(pElem, path, &linkList_);
    } else {
        camera_ = new Camera();
    }

	bool result = true;

    //lights
    pElem = hRoot.FirstChild( "lights" ).FirstChild().Element();
    result = addFromXml <Light> (pElem, path);
	if (!result) { return false; }
    std::cout << "  " << lights.size() << " lights" << std::endl; 

	if (cancelLoad_) { return false; }

    //images
    pElem = hRoot.FirstChild( "images" ).FirstChild().Element();
    result = addFromXml <Image> (pElem, path);
	if (!result) { return false; }
    std::cout << "  " << images.size() << " images" << std::endl;

	if (cancelLoad_) { return false; }

    //maps
    pElem = hRoot.FirstChild( "maps" ).FirstChild().Element();
    result = addFromXml <Map> (pElem, path);
	if (!result) { return false; }
    std::cout << "  " << maps.size() << " maps" << std::endl;

	if (cancelLoad_) { return false; }

    //materials
    pElem = hRoot.FirstChild( "materials" ).FirstChild().Element();
    result = addFromXml <Material> (pElem, path);
	if (!result) { return false; }
    std::cout << "  " << materials.size() << " materials" << std::endl;

	if (cancelLoad_) { return false; }

    //objects
    pElem = hRoot.FirstChild( "objects" ).FirstChild().Element();
    result = addFromXml <Object> (pElem, path);
	if (!result) { return false; }
    std::cout << "  " << objects.size() << " objects" << std::endl;

	if (cancelLoad_) { return false; }

    //Linking
	std::cout << "  Linking...";
    linkAll();
	std::cout << "Done" << std::endl;

    std::cout << " Done" << std::endl;

    return true;
}
