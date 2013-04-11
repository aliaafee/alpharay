/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */

#include "scene.h"

void Scene::transformVertices() {
    for (int i = 0; i < objects.size(); i++) {
		(objects[i])->transform();
	}
}

bool Scene::fromXml(TiXmlElement* pElem, Light** light){
    //std::cout << "light " << pElem->Value() << std::endl;
    std::string name(pElem->Value());
    *light = NULL;

    if (name == "pointlight") {
        *light = new PointLight();
    } else if (name == "spotlight") {
        *light = new SpotLight();
    } else if (name == "arealight") {
        *light = new AreaLight();
    }

    if (*light) {
        (*light)->loadXml(pElem);
        return true;
    }
    return false;
}


bool Scene::fromXml(TiXmlElement* pElem, Image** image) {
    //std::cout << "image " << pElem->Value() << std::endl;
    std::string name(pElem->Value());
    *image = NULL;

    if (name == "checker2d") {
        *image = new Checker2d();
    } else if (name == "cimgimage") {
        *image = new CimgImage();
    }

    if (*image) {
        (*image)->loadXml(pElem);
        return true;
    }

    return false;
}


bool Scene::fromXml(TiXmlElement* pElem, Map** map) {
    //std::cout << "map " << pElem->Value() << std::endl;
    std::string name(pElem->Value());
    *map = NULL;

    if (name == "map2dplane") {
        *map = new Map2dPlane();
    } else if (name == "map2dcylindrical") {
        *map = new Map2dCylindrical;
    } else if (name == "map2dspherical") {
        *map = new Map2dSpherical();
    } else if (name == "uvmap") {
        *map = new UVMap;
    }

    if (*map) {
        (*map)->loadXml(pElem, &linkImages);
        return true;
    }

    return false;
}


bool Scene::fromXml(TiXmlElement* pElem, Material** mat) {
    //std::cout << "mat " << pElem->Value() << std::endl;
    std::string name(pElem->Value());
    *mat = NULL;

    if (name == "material") {
        *mat = new Material();
    }

    if (*mat) {
        (*mat)->loadXml(pElem, &linkMaps, &linkMaterials);
        return true;
    }

    return false;
}


bool Scene::fromXml(TiXmlElement* pElem, Object** object) {
    //std::cout << "object " << pElem->Value() << std::endl;
    std::string name(pElem->Value());
    *object = NULL;

    if (name == "sphere") {
        *object = new Sphere();
    } else if (name == "plane") {
        *object = new Plane();
    } else if (name == "mesh") {
        *object = new Mesh();
    }

    if (*object) {
        (*object)->loadXml(pElem, &linkMaterials);

        return true;
    }
    return false;
}


template< typename T > void Scene::addFromXml(TiXmlElement* pElem) {
    if (pElem) {
        for (pElem; pElem; pElem = pElem->NextSiblingElement()) {
            T* newthing;
            fromXml(pElem, &newthing);
            if (newthing) {
                add(newthing);
            }
        }
    }
}


TiXmlElement* Scene::getXml() {
    TiXmlElement* root = new TiXmlElement(xmlName.c_str()); 

    //camera
    root->LinkEndChild(camera_.getXml());

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


bool Scene::loadXml(TiXmlHandle &hTopRoot) {
    TiXmlElement* root = hTopRoot.FirstChild( xmlName.c_str() ).Element();
    if (!root) return false;
    std::cout << " Loading Scene..." << std::endl;

    TiXmlHandle hRoot = TiXmlHandle(root);
    TiXmlElement* pElem;

    //camera
    std::cout << "  Setting up camera" << std::endl; 
    camera_.loadXml(hRoot);

    //lights
    pElem = hRoot.FirstChild( "lights" ).FirstChild().Element();
    addFromXml <Light> (pElem);
    std::cout << "  " << lights.size() << " lights" << std::endl; 

    //images
    pElem = hRoot.FirstChild( "images" ).FirstChild().Element();
    addFromXml <Image> (pElem);
    std::cout << "  " << images.size() << " images" << std::endl;

    //maps
    pElem = hRoot.FirstChild( "maps" ).FirstChild().Element();
    addFromXml <Map> (pElem);
    std::cout << "  " << maps.size() << " maps" << std::endl;

    //materials
    pElem = hRoot.FirstChild( "materials" ).FirstChild().Element();
    addFromXml <Material> (pElem);
    std::cout << "  " << materials.size() << " materials" << std::endl;

    //objects
    pElem = hRoot.FirstChild( "objects" ).FirstChild().Element();
    addFromXml <Object> (pElem);
    std::cout << "  " << objects.size() << " objects" << std::endl;

    //Linking
    linkAll();

    std::cout << " Done" << std::endl;

    return true;
}