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


void XmlObjectNamed::init()  { 
	//XmlObject::init();

	addEditable(new Editable<std::string>("name", &name_, "noname"));
}


TiXmlElement* XmlObjectNamed::getXml() {
    TiXmlElement* root = XmlObject::getXml();

	for (unsigned long i = 0; i < editables.size(); i++) {
		root->SetAttribute(editables[i]->name(), editables[i]->str());
	}

	for (unsigned long i = 0; i < editableLinks.size(); i++) {
		root->SetAttribute(editableLinks[i]->name(), editableLinks[i]->str());
	}

    return root;
}


bool XmlObjectNamed::loadXml(TiXmlElement* pElem, std::string path) {
    XmlObject::loadXml(pElem, path);

	std::string value;
	int queryResult;
	for (unsigned long i = 0; i < editables.size(); i++) {
		queryResult = pElem->QueryStringAttribute((editables[i]->name()).c_str(), &value);
		if (queryResult == TIXML_SUCCESS) {
			editables[i]->set(value);
		}
	}

	return true;
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

