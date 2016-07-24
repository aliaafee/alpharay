/* vim: set ts=4 ss=4 sw=4 noet ai cindent : */

#ifndef _XML_OBJECT_NAMED_H_
#define _XML_OBJECT_NAMED_H_

#include "xmlobject.h"


class XmlObjectNamed : public XmlObject
{
    public:
        virtual void init();

		XmlObjectNamed( std::string xmlTag) 
            :XmlObject(xmlTag)
            { init(); name_ = ""; }
		
        XmlObjectNamed( std::string xmlTag, std::string name ) 
            :XmlObject(xmlTag)
            { init(); name_ = name; }

		XmlObjectNamed()
			{;}

        virtual ~XmlObjectNamed() {};

        //virtual TiXmlElement* getXml();
		//virtual bool loadXml(TiXmlElement* pElem, std::string path);

        std::string name() { return name_; }
		/*
		std::string type() { return type_; }

		void addEditable(BaseEditable* editable) {
			editables.push_back(editable);
		}

		void addEditableLink(BaseEditableLink* editableLink) {
			editableLinks.push_back(editableLink);
		}

		virtual std::vector<BaseEditableLink*>* getEditableLinksList() {
			return &editableLinks;
		}*/
		
    protected:
        std::string name_;
		/*
		std::vector<BaseEditable*> editables;
		std::vector<BaseEditableLink*> editableLinks;

		std::string type_;*/
};


#endif // _XML_OBJECT_NAMED_H_
