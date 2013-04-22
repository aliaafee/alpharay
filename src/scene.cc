/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */

#include "scene.h"

void Scene::transform() {
    for (int i = 0; i < maps.size(); i++) {
		(maps[i])->transform();
	}
    for (int i = 0; i < materials.size(); i++) {
		(materials[i])->transform();
	}
    for (int i = 0; i < objects.size(); i++) {
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
    }

    if (*light) {
        (*light)->loadXml(pElem, path);
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
    } else if (name == "cimgimage") {
        *image = new CimgImage("");
    }

    if (*image) {
        (*image)->loadXml(pElem, path, &linkImages);
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
        (*map)->loadXml(pElem, path, &linkMaps, &linkImages);
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
        (*mat)->loadXml(pElem, path, &linkMaps, &linkMaterials);
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
    } else if (name == "mesh") {
        *object = new Mesh("");
    }

    if (*object) {
        (*object)->loadXml(pElem, path, &linkMaterials);

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

    //camera
    root->LinkEndChild(camera_->getXml());

    //lights
    TiXmlElement* lights_e = new TiXmlElement("lights");
    root->LinkEndChild(lights_e);

    for (int i = 0; i < lights.size(); i++) {
        lights_e->LinkEndChild( lights[i]->getXml() );
    }

    //images
    TiXmlElement* images_e = new TiXmlElement("images");
    root->LinkEndChild(images_e);

    for (int i = 0; i < images.size(); i++) {
        images_e->LinkEndChild( images[i]->getXml() );
    }

    //maps
    TiXmlElement* maps_e = new TiXmlElement("maps");
    root->LinkEndChild(maps_e);

    for (int i = 0; i < maps.size(); i++) {
        maps_e->LinkEndChild( maps[i]->getXml() );
    }

    //materials
    TiXmlElement* materials_e = new TiXmlElement("materials");
    root->LinkEndChild(materials_e);

    for (int i = 0; i < materials.size(); i++) {
        materials_e->LinkEndChild( materials[i]->getXml() );
    }

    //objects
    TiXmlElement* objects_e = new TiXmlElement("objects");
    root->LinkEndChild(objects_e);

    for (int i = 0; i < objects.size(); i++) {
        objects_e->LinkEndChild( objects[i]->getXml() );
    }

    return root;
}


bool Scene::loadXml(TiXmlElement* pElem, std::string path) {
    XmlObject::loadXml(pElem, path);

    std::cout << " Loading Scene..." << std::endl;

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
    linkAll();

    std::cout << " Done" << std::endl;

    return true;
}
