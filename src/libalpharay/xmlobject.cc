/* vim: set ts=4 ss=4 sw=4 noet ai cindent : */

#include "xmlobject.h"


std::string XmlObject::pathJoin(std::string path, std::string filename) {
    if (filename == "") 
        return "";

    //begining with / is an absolute path
    if (filename[0] == '/')
        return filename;

	if (path == "")
		return filename;

    return path + "/" + filename;
}


TiXmlElement* XmlObject::getXml() {
    TiXmlElement* root = new TiXmlElement(xmlTag_);
	
	std::string value;

	for (unsigned long i = 0; i < editables.size(); i++) {
		value = editables[i]->str();
		if (value != "") {
			root->SetAttribute(editables[i]->name(), value);
		}
	}

	for (unsigned long i = 0; i < editableLinks.size(); i++) {
		value = editableLinks[i]->str();
		if (value != "") {
			root->SetAttribute(editableLinks[i]->name(), value);
		}
	}

    return root;
}


bool XmlObject::loadXml(TiXmlElement* pElem, std::string path) {
	int queryResult;

	std::string value;
	for (unsigned long i = 0; i < editables.size(); i++) {
		queryResult = pElem->QueryStringAttribute((editables[i]->name()).c_str(), &value);
		if (queryResult == TIXML_SUCCESS) {
			editables[i]->set(value);
			if (editables[i]->type() == "filename") {
				Editable<FileName>* editableFilename = static_cast <Editable<FileName>*> (editables[i]);
				FileName* filename = editableFilename->getPointerToValue();
				filename->setRootPath(path);
			}
		}
	}

	std::string sourceName;
	for (unsigned long i = 0; i < editableLinks.size(); i++) {
		queryResult = pElem->QueryStringAttribute((editableLinks[i]->name()).c_str(), &sourceName);
		if (queryResult == TIXML_SUCCESS) {
			editableLinks[i]->setSource(sourceName);
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

