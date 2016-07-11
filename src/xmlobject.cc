/* vim: set ts=4 ss=4 sw=4 noet ai cindent : */

#include "xmlobject.h"


std::string XmlObject::pathJoin(std::string path, std::string filename) {
    if (filename == "") 
        return "";

    //begining with / is an absolute path
    if (filename[0] == '/')
        return filename;

    return path + "/" + filename;
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

