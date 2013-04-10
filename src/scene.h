/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */

#ifndef _SCENE_H_
#define _SCENE_H_

#include <vector>
#include <tinyxml.h>
#include <string>

#include <linklist.h>

#include "vector.h"

#include "camera.h"

#include "light.h"
#include "point-light.h"
#include "spot-light.h"
#include "area-light.h"

#include "image.h"
#include "cimg-image.h"

#include "map.h"

#include "material.h"

#include "object.h"
#include "sphere.h"
#include "mesh.h"
#include "plane.h"


class Scene {
public:
    std::string xmlName;
    Camera camera_;

    std::vector<Light*> lights;
    std::vector<Image*> images;
    std::vector<Map*> maps;
    std::vector<Material*> materials;
    std::vector<Object*> objects;

	Scene () { xmlName="scene"; };
	
    void transformVertices();

    Light* add(Light *light) 
		{lights.push_back(light); return light;}
    Light* getLight(std::string name)
        {return getByName <Light> (name, lights);}

    Image* add(Image *image)
		{images.push_back(image); return image;}
    Image* getImage(std::string name)
        {return getByName <Image> (name, images);}

    Map* add(Map *map)
		{maps.push_back(map); return map;}
    Map* getMap(std::string name)
        {return getByName <Map> (name, maps);}

    Material* add(Material *material)
		{materials.push_back(material); return material;}
    Material* getMaterial(std::string name)
        {return getByName <Material> (name, materials);}

	Object* add(Object *object) 
		{objects.push_back(object); return object;}
    Object* getObject(std::string name)
        {return getByName <Object> (name, objects);}

    TiXmlElement* getXml();
    bool loadXml(TiXmlHandle &hRoot);

private:
    LinkList <Image> linkImages;
    LinkList <Map> linkMaps;
    LinkList <Material> linkMaterials;
    LinkList <Object> linkObjects;

    template <typename T> T* getByName(std::string name, std::vector<T*> &list) {
        for (int i = 0; i < list.size(); i++) {
            if (list[i]->name_ == name) {
                return list[i];
            }
        }
        return NULL;
    }
    
    template <typename T> void linkList ( LinkList <T> list, std::vector<T*> &things) {
        for (int i = 0; i < list.size(); i++) {
            string source;
            T** target;
            list.get(i , &source, &target);
            *target = getByName <T> (source, things);
        }
    }

    void linkAll() {
        linkList <Image> (linkImages, images);
        linkList <Map> (linkMaps, maps);
        linkList <Material> (linkMaterials, materials);
        linkList <Object> (linkObjects, objects);
    }

    bool fromXml(TiXmlElement* pElem, Light** light);
    bool fromXml(TiXmlElement* pElem, Image** image);
    bool fromXml(TiXmlElement* pElem, Map** map);
    bool fromXml(TiXmlElement* pElem, Material** mat);
    bool fromXml(TiXmlElement* pElem, Object** object);

    template< typename T > void addFromXml(TiXmlElement* pElem);

};

#endif // _SCENE_H_
