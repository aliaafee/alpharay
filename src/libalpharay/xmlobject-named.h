/* vim: set ts=4 ss=4 sw=4 noet ai cindent : */

#ifndef _XML_OBJECT_NAMED_H_
#define _XML_OBJECT_NAMED_H_

#include "xmlobject.h"
#include "editable.h"
#include "linklist.h"


class XmlObjectNamed : public XmlObject
{
	friend class MainFrame;
	friend class PropertyEditor;
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

        virtual TiXmlElement* getXml();
		virtual bool loadXml(TiXmlElement* pElem, std::string path, LinkList* linkList);

        std::string name() { return name_; }
		std::string type() { return type_; }

		virtual void addEditable(BaseEditable* editable) {
			editables.push_back(editable);
		}

		virtual void addEditableLink(BaseEditableLink* editableLink) {
			editableLinks.push_back(editableLink);
		}
		
    protected:
        std::string name_;
		std::vector<BaseEditable*> editables;
		std::vector<BaseEditableLink*> editableLinks;

		std::string type_;
};


#endif // _XML_OBJECT_NAMED_H_
