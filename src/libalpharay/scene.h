/* vim: set ts=4 ss=4 sw=4 noet ai cindent : */

#ifndef _SCENE_H_
#define _SCENE_H_

#include <vector>
#include <tinyxml.h>
#include <string>

#include "linklist.h"

#include "xmlobject-named.h"

#include "vector.h"

#include "camera.h"

#include "light.h"
#include "spot-light.h"
#include "area-light.h"
#include "sky-light.h"
#include "sun-light.h"

#include "image.h"
#include "bitmap.h"
#include "canvas.h"

#include "map.h"
#include "sky-map.h"

#include "material.h"

#include "object.h"
#include "sphere.h"
#include "plane.h"
#include "cone.h"
#include "mesh.h"
#include "group.h"


class Scene : public XmlObjectNamed 
{
    friend class Renderer;
    friend class Raytracer;
	
	friend class MainFrame;
public:
    virtual void init();
    
	Scene () 
        : XmlObjectNamed("scene") 
        { init(); }
	
    void transform();

    void setScreen(float width, float height);
    Ray ray(float x, float y);

	void setRayLog(bool state) { rayLog_ = state; }
	bool rayLog() {return rayLog_; }

    Camera* camera() { return camera_; }

    Color envColor(const Ray &ray);

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

    virtual TiXmlElement* getXml();
    virtual bool loadXml(TiXmlElement* pElem, std::string path, LinkList* linkList);
	virtual bool loadXml(TiXmlElement* pElem, std::string path)
		{ return loadXml(pElem, path, &linkList_); }

	virtual void cancelLoad() { cancelLoad_ = true; }

protected:
    Camera* camera_;

    Color envColor_;
    Map* envMap_;

	bool cancelLoad_;

    std::vector<Light*> lights;
    std::vector<Image*> images;
    std::vector<Map*> maps;
    std::vector<Material*> materials;
    std::vector<Object*> objects;

	bool rayLog_;

private:
    LinkList linkList_;

    template <typename T> T* getByName(std::string name, std::vector<T*> &list) {
        for (unsigned long i = 0; i < list.size(); i++) {
            if (list[i]->name() == name) {
                return list[i];
            }
        }
        return NULL;
    }
    
    template <typename T> void linkList ( std::vector<T*> &things) {
        for (unsigned long i = 0; i < linkList_.size(); i++) {
            std::string source;
            T** target;
            T* thing;
            linkList_.get <T> (i , &source, &target);
            thing = getByName <T> (source, things);
            if (thing != NULL) {
                *target = thing;
            }
        }
    }
    
    void linkAll() {
        linkList <Image> (images);
        linkList <Map> (maps);
        linkList <Material> (materials);
        linkList <Object> (objects);
    }

    bool fromXml(TiXmlElement* pElem, Light** light, std::string path);
    bool fromXml(TiXmlElement* pElem, Image** image, std::string path);
    bool fromXml(TiXmlElement* pElem, Map** map, std::string path);
    bool fromXml(TiXmlElement* pElem, Material** mat, std::string path);
    bool fromXml(TiXmlElement* pElem, Object** object, std::string path);

    template< typename T > bool addFromXml(TiXmlElement* pElem, std::string path);

};

#endif // _SCENE_H_