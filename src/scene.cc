/* vim: set ts=4 ss=4 sw=4 noet ai cindent : */

#include "scene.h"

void Scene::init() {
    XmlObject::init();

    envColor_ = Color(0, 0, 0);
    envMap_ = NULL;
    /*
    std::cout << "List Size Max " << lights.max_size() << std::endl;
    std::cout << "int max, min " << std::numeric_limits<int>::min() << "," << std::numeric_limits<int>::max() << std::endl;
    std::cout << "unsigned int max, min " << std::numeric_limits<unsigned int>::min() << "," << std::numeric_limits<unsigned int>::max() << std::endl;
    std::cout << "long max, min " << std::numeric_limits<long>::min() << "," << std::numeric_limits<long>::max() << std::endl;
    std::cout << "unsigned long max, min " << std::numeric_limits<unsigned long>::min() << "," << std::numeric_limits<unsigned long>::max() << std::endl;

    std::cout << std::numeric_limits<unsigned long>::max() - lights.max_size() << std::endl;
    */
}

Color Scene::envColor(const Ray &ray) {
    if (envMap_ == NULL) {
        return envColor_;
    }

    return envMap_->color(ray.direction_, Vector2(0,0));
}

void Scene::transform() {
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
        return true;
    }
    return false;
}


bool Scene::fromXml(TiXmlElement* pElem, Image** image, std::string path) {
    //std::cout << "image " << pElem->Value() << std::endl;
    std::string name(pElem->Value());
    *image = NULL;

    if (name == "checker2d") {
        *image = new Checker2d("");
    } else if (name == "bitmap") {
        *image = new Bitmap("");
    }

    if (*image) {
        (*image)->loadXml(pElem, path, &linkList_);
        return true;
    }

    return false;
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
    }

    if (*map) {
        (*map)->loadXml(pElem, path, &linkList_);
        return true;
    }

    return false;
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
        return true;
    }

    return false;
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

        return true;
    }
    return false;
}


template< typename T > void Scene::addFromXml(TiXmlElement* pElem, std::string path) {
    if (pElem) {
        for (pElem; pElem; pElem = pElem->NextSiblingElement()) {
            T* newthing;
            fromXml(pElem, &newthing, path);
            if (newthing) {
                add(newthing);
            }
        }
    }
}


TiXmlElement* Scene::getXml() {
    TiXmlElement* root = XmlObject::getXml();

    //environment
    root->SetAttribute("skycolor", envColor_.str());
    if (envMap_ == NULL) {
        root->SetAttribute("skymap", "");
    } else {
        root->SetAttribute("skymap", envMap_->name());
    }

    //camera
    root->LinkEndChild(camera_->getXml());

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


bool Scene::loadXml(TiXmlElement* pElem, std::string path) {
    XmlObject::loadXml(pElem, path);

    std::cout << " Loading Scene..." << std::endl;

    //Environment
    pElem->QueryValueAttribute <Vector> ("skycolor", &envColor_);
    std::string mapname = "";
    pElem->QueryStringAttribute ("skymap", &mapname);
    linkList_.add(mapname, &envMap_);

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
        camera_->loadXml(pElem, path);
    } else {
        camera_ = new Camera();
    }

    //lights
    pElem = hRoot.FirstChild( "lights" ).FirstChild().Element();
    addFromXml <Light> (pElem, path);
    std::cout << "  " << lights.size() << " lights" << std::endl; 

    //images
    pElem = hRoot.FirstChild( "images" ).FirstChild().Element();
    addFromXml <Image> (pElem, path);
    std::cout << "  " << images.size() << " images" << std::endl;

    //maps
    pElem = hRoot.FirstChild( "maps" ).FirstChild().Element();
    addFromXml <Map> (pElem, path);
    std::cout << "  " << maps.size() << " maps" << std::endl;

    //materials
    pElem = hRoot.FirstChild( "materials" ).FirstChild().Element();
    addFromXml <Material> (pElem, path);
    std::cout << "  " << materials.size() << " materials" << std::endl;

    //objects
    pElem = hRoot.FirstChild( "objects" ).FirstChild().Element();
    addFromXml <Object> (pElem, path);
    std::cout << "  " << objects.size() << " objects" << std::endl;

    //Linking
	std::cout << "  Linking...";
    linkAll();
	std::cout << "Done" << std::endl;

    std::cout << " Done" << std::endl;

    return true;
}
