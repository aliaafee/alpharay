/* vim: set ts=4 ss=4 sw=4 noet ai cindent : */

#include "xmlobject-named.h"


void XmlObjectNamed::init()  { 
	//XmlObject::init();

	addEditable(new Editable<std::string>("name", &name_, "noname"));
}


TiXmlElement* XmlObjectNamed::getXml() {
    TiXmlElement* root = XmlObject::getXml();

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


//bool XmlObjectNamed::loadXml(TiXmlElement* pElem, std::string path) {
bool XmlObjectNamed::loadXml(TiXmlElement* pElem, std::string path, LinkList* linkList) {
    XmlObject::loadXml(pElem, path);

	int queryResult;

	std::string value;
	for (unsigned long i = 0; i < editables.size(); i++) {
		queryResult = pElem->QueryStringAttribute((editables[i]->name()).c_str(), &value);
		if (queryResult == TIXML_SUCCESS) {
			editables[i]->set(value);
		}
	}

	std::string sourceName;
	for (unsigned long i = 0; i < editableLinks.size(); i++) {
		queryResult = pElem->QueryStringAttribute((editableLinks[i]->name()).c_str(), &sourceName);
		if (queryResult == TIXML_SUCCESS) {
			editableLinks[i]->setSource(sourceName);
			EditableLink<XmlObjectNamed>* el = static_cast <EditableLink<XmlObjectNamed>*> (editableLinks[i]);
			linkList->add(sourceName, el->target());
		}
	}
	
	return true;

	return true;
}
