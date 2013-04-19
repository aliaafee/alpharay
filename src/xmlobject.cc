/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */

#include "xmlobject.h"


TiXmlElement* XmlObjectNamed::getXml() {
    TiXmlElement* root = XmlObject::getXml();

    root->SetAttribute("name", name_);

    return root;
}


bool XmlObjectNamed::loadXml(TiXmlElement* pElem, std::string path) {
    XmlObject::loadXml(pElem, path);

    name_ = "unnamed"; 

    pElem->QueryStringAttribute ("name", &name_);
}


TiXmlElement* XmlObjectIndexed::getXml() {
    TiXmlElement* root = XmlObject::getXml();

    root->SetAttribute("i", index_);

    return root;
}


bool XmlObjectIndexed::loadXml(TiXmlElement* pElem, std::string path) {
    XmlObject::loadXml(pElem, path);

    index_ = 0; 

    pElem->QueryIntAttribute ("i", &index_);

    return true;
}

