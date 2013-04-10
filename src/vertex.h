#ifndef _VERTEX_H_
#define _VERTEX_H_

#include <tinyxml.h>
#include "vector.h"

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

#endif // _VERTEX_H_
