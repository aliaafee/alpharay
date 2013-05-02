#ifndef _MAP_H_
#define _MAP_H_

#include "vector.h"
#include "matrix4.h"
#include "xmlobject.h"
#include "image.h"
#include "linklist.h"


class Map : virtual public XmlObjectNamed
{
    public:
        virtual void init() { XmlObjectNamed::init(); }
        
        Map() 
            :XmlObjectNamed("map", "")
            { init(); }

        Map( std::string name ) 
            :XmlObjectNamed("map", name)
            { init(); }

        ~Map() { } ;

        virtual void transform() {};

        virtual Color color(Vector  point, Vector2 point2) { return Color(0, 0, 0); }

        virtual TiXmlElement* getXml();
        virtual bool loadXml(TiXmlElement* pElem, std::string path, LinkList *linkList);

};


class Map2d : public Map, virtual public XmlObjectNamed
{
    public:
        virtual void init();
        
        Map2d( std::string name ) 
            : XmlObjectNamed("map2d", name)
            { init(); }

        ~Map2d() { } ;

        virtual Color color(Vector  point, Vector2 point2);

        virtual TiXmlElement* getXml();
        virtual bool loadXml(TiXmlElement* pElem, std::string path, LinkList *linkList);

    protected:
        Image* image_;
        Vector2 imageScale_;

        virtual Color color(Vector2 point2);
};


class Map2dPlane : public Map2d, virtual public XmlObjectNamed
{
    public:
        virtual void init();
        
        Map2dPlane( std::string name ) 
            : Map2d(name), XmlObjectNamed("map2dplane", name)
            { init(); }

        ~Map2dPlane() { } ;

        virtual void transform();

        virtual Color color(Vector  point, Vector2 point2);

        virtual TiXmlElement* getXml();
        virtual bool loadXml(TiXmlElement* pElem, std::string path, LinkList *linkList);

    protected:
        Vector position_;
	    Vector rotation_;
	    Vector scale_;

        Vector transformPoint(Vector point);
        Vector transformPointInv(Vector point);
        
    private:
        Matrix4 transMatrix; // TRS
	    Matrix4 transMatrixInv; // S^R^T^
};


class Map2dCylindrical : public Map2dPlane, virtual public XmlObjectNamed
{
    public:
        virtual void init() { Map2dPlane::init(); }
        
        Map2dCylindrical( std::string name ) 
            : Map2dPlane(name), XmlObjectNamed("map2dcylindrical", name)
            { init(); }

        ~Map2dCylindrical() { } ;

        virtual Color color(Vector  point, Vector2 point2);
};


class Map2dSpherical : public Map2dPlane, virtual public XmlObjectNamed
{
    public:
        virtual void init() { Map2dPlane::init(); }
        
        Map2dSpherical( std::string name ) 
            : Map2dPlane(name), XmlObjectNamed("map2dcylindrical", name)
            { init(); }

        ~Map2dSpherical() { } ;

        virtual Color color(Vector  point, Vector2 point2);
};



/*
#include <string>
#include <tinyxml.h>

#include "vector.h"
#include "vertex.h"

#include "uvtriangle.h"

#include "linklist.h"

#include "image.h"


class Map {
    public:
        std::string xmlName;
        std::string name_;

        Vector position_;
	    Vector rotation_;
	    Vector scale_;

        virtual void init() { 
            xmlName = "map";
            name_ = "map";

            position_ = Vector(0, 0, 0);
            rotation_ = Vector(0, 0, 0);
            scale_ = Vector(1, 1, 1);
        }

        Map() {init();}
        Map(std::string name) {
            init();
            name_ = name;

            position_ = Vector(0, 0, 0);
            rotation_ = Vector(0, 0, 0);
            scale_ = Vector(1, 1, 1);
        };

        virtual Vector getColor(Vector point, UVTriangle* uvtriangle) { return Vector(0,0,0); };

        Vector transformPoint(Vector point);
        Vector transformPointInv(Vector point);

        void transform();

        virtual bool loadXml(TiXmlElement* pElem, LinkList <Image> *linkImages) {
            name_ = "map";
            
            position_ = Vector(0, 0, 0);
            rotation_ = Vector(0, 0, 0);
            scale_ = Vector(1, 1, 1);

            pElem->QueryStringAttribute ("name", &name_);
            pElem->QueryValueAttribute <Vector> ("position", &position_);
            pElem->QueryValueAttribute <Vector> ("rotation", &rotation_);
            pElem->QueryValueAttribute <Vector> ("scale", &scale_);

            return true;
        }

        virtual TiXmlElement* getXml() {
            TiXmlElement* root = new TiXmlElement(xmlName.c_str());

            root->SetAttribute("name", name_);
            root->SetAttribute("position", position_.str());
            root->SetAttribute("rotation", rotation_.str());
            root->SetAttribute("scale", scale_.str());

            return root;
        }
    private:
        Matrix4 transMatrix; // TRS
	    Matrix4 transMatrixInv; // S^R^T^
};


class Map2dPlane : public Map {
    public:
        Image* image_;
        Vector2 imageScale_;

        virtual void init() { xmlName = "map2dplane";}

        Map2dPlane() {init();}
        Map2dPlane(std::string name, Image *image) : Map(name) {
            init();

            image_ = image;
            imageScale_ = Vector2(1, 1);
        };

        Vector getColor2d(float u, float v)  {
            u = u / imageScale_.x;
            v = v / imageScale_.y;

            u = u - float(int(u));
            v = v - float(int(v));

            if (u < 0) { u = 1.0f + u; }
            if (v < 0) { v = 1.0f + v; }
            
            u = u * float(image_->width());
            v = v * float(image_->height());
            
            return image_->getColor(u, v);
        };

        virtual Vector getColor(Vector point, UVTriangle* uvtriangle) {
            point = transformPointInv(point);

            float u = -0.5f - (point.x/2.0f);
            float v = -0.5f - (point.y/2.0f);

            return getColor2d(u, v);
        };

        virtual bool loadXml(TiXmlElement* pElem, LinkList <Image> *linkImages) {
            Map::loadXml(pElem, linkImages);

            pElem->QueryValueAttribute <Vector2> ("imagescale", &imageScale_);

            std::string imagename = "";
            pElem->QueryStringAttribute ("image", &imagename);
            linkImages->add(imagename, &image_);

            return true;
        }

        virtual TiXmlElement* getXml() {
            TiXmlElement* root = Map::getXml();

            root->SetAttribute("imagescale", imageScale_.str());

            if (image_ == NULL) {
                root->SetAttribute("image", "");
            } else {
                root->SetAttribute("image", image_->name_);
            }

            return root;
        }
};

class Map2dCylindrical : public Map2dPlane {
    public:
        virtual void init() { xmlName = "map2dcylindrical";}

        Map2dCylindrical() {init();}
        Map2dCylindrical(std::string name, Image *image) 
            : Map2dPlane(name, image) 
        {init();}

        virtual Vector getColor(Vector point, UVTriangle* uvtriangle) {
            point = transformPointInv(point);

            float u = atan2(point.y, point.x) / (M_PI*2) * -1;
            float v = -0.5 - (point.z/2);

            return getColor2d(u, v);
        };
};


class Map2dSpherical : public Map2dPlane {
    public:
        virtual void init() { xmlName = "map2dspherical";}

        Map2dSpherical() {init();}
        Map2dSpherical(std::string name, Image *image) 
            : Map2dPlane(name, image) 
        {init();}

        virtual Vector getColor(Vector point, UVTriangle* uvtriangle) {
            point = transformPointInv(point);

            point.toSpherical();
            float u = point.z / (M_PI*2) * -1;
    	    float v = point.y / (M_PI);     

            return getColor2d(u, v);
        };
};


class UVMap : public Map2dPlane {
    public:
        virtual void init() { xmlName = "uvmap";}

        UVMap() {init();}
        UVMap(std::string name, Image *image) 
            : Map2dPlane(name, image) 
        {init();}

        virtual Vector getColor(Vector point, UVTriangle* uvt) {
            if (uvt == NULL) {
                return getColor2d(0, 0);
            }

            Vector2 uv = uvt->getUV(point);

            return getColor2d(uv.x, uv.y);
        }
};*/
#endif // _MAP_H_
