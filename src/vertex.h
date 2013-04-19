#ifndef _VERTEX_H_
#define _VERTEX_H_

#include <tinyxml.h>
#include "vector.h"
#include "xmlobject.h"

class Vertex : public Vector, public XmlObjectIndexed
{
    public:
        Vertex(int i, float x, float y, float z)
            : Vector (x,y,z), XmlObjectIndexed("vert", i)
            {};

        virtual TiXmlElement* getXml();
        virtual bool loadXml(TiXmlElement* pElem, std::string path);
};


inline TiXmlElement* Vertex::getXml() {
    TiXmlElement* root = XmlObjectIndexed::getXml();

    root->SetAttribute("p", str());

    return root;
}


inline bool Vertex::loadXml(TiXmlElement* pElem, std::string path) {
    XmlObjectIndexed::loadXml(pElem, path);

    x = 0; y = 0; z = 0;

    pElem->QueryValueAttribute <Vector> ("p", this);

    return true;
}

//////////////////////////////////////////////////////////////////////////

class Normal : public Vector, public XmlObjectIndexed
{
    public:
        Normal(int i, float x, float y, float z)
            : Vector (x,y,z), XmlObjectIndexed("norm", i)
            {};

        virtual TiXmlElement* getXml();
        virtual bool loadXml(TiXmlElement* pElem, std::string path);
};


inline TiXmlElement* Normal::getXml() {
    TiXmlElement* root = XmlObjectIndexed::getXml();

    root->SetAttribute("n", str());

    return root;
}


inline bool Normal::loadXml(TiXmlElement* pElem, std::string path) {
    XmlObjectIndexed::loadXml(pElem, path);

    x = 0; y = 0; z = 0;

    pElem->QueryValueAttribute <Vector> ("n", this);

    return true;
}

//////////////////////////////////////////////////////////////////////////

class MapPoint2 : public Vector2, public XmlObjectIndexed
{
    public:
        MapPoint2(int i, float x, float y)
            : Vector2 (x,y), XmlObjectIndexed("uvpt", i)
            {};

        virtual TiXmlElement* getXml();
        virtual bool loadXml(TiXmlElement* pElem, std::string path);
};


inline TiXmlElement* MapPoint2::getXml() {
    TiXmlElement* root = XmlObjectIndexed::getXml();

    root->SetAttribute("p", str());

    return root;
}


inline bool MapPoint2::loadXml(TiXmlElement* pElem, std::string path) {
    XmlObjectIndexed::loadXml(pElem, path);

    x = 0; y = 0;

    pElem->QueryValueAttribute <Vector2> ("p", this);

    return true;
}






/*
class Vertex
{
    public:
        int i;
        Vector p;
        Vector n;

        Vertex() {};

        Vertex(int index, Vector position, Vector normal) {
            i = index;
            p = position;
            n = normal;
        };

        bool loadXml(TiXmlElement* pElem) {
            i = 0;
            p = Vector(0,0,0);
            n = Vector(0,0,0);

            pElem->QueryIntAttribute ("i", &i);
            pElem->QueryValueAttribute <Vector> ("p", &p);
            pElem->QueryValueAttribute <Vector> ("n", &n);
        }

        TiXmlElement* getXml() {
            TiXmlElement* root = new TiXmlElement("vert");

            root->SetAttribute("i", i);
            root->SetAttribute("p", p.str());
            root->SetAttribute("n", n.str());

            return root;
        }
};
*/

#endif // _VERTEX_H_
