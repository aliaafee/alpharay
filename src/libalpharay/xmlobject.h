/* vim: set ts=4 ss=4 sw=4 noet ai cindent : */

#ifndef _XML_OBJECT_H_
#define _XML_OBJECT_H_

#include <tinyxml.h>
#include <string>
#include <vector>

#include "editable.h"


class XmlObject
{
	friend class MainFrame;
	friend class PropertyEditor;
    public:
        virtual void init()  {};
        XmlObject( std::string xmlTag ) { init(); xmlTag_ = xmlTag; }
		XmlObject() {;}
		
        virtual ~XmlObject() { } ;

        std::string xmlTag() { return xmlTag_; }

        virtual TiXmlElement* getXml();
		virtual bool loadXml(TiXmlElement* pElem, std::string path);

        std::string pathJoin(std::string path, std::string filename);

		std::string type() { return type_; }

		void addEditable(BaseEditable* editable) {
			editables.push_back(editable);
		}

		void addEditableLink(BaseEditableLink* editableLink) {
			editableLinks.push_back(editableLink);
		}

		virtual std::vector<BaseEditableLink*>* getEditableLinksList() {
			return &editableLinks;
		}

    protected:
        std::string xmlTag_;
		
		std::vector<BaseEditable*> editables;
		std::vector<BaseEditableLink*> editableLinks;

		std::string type_;
};


class XmlObjectIndexed : public XmlObject
{
    public:
        virtual void init() {};//  { XmlObject::init(); }
        
        XmlObjectIndexed( std::string xmlTag, int index )
            :XmlObject(xmlTag)
            { init(); index_ = index; }

        virtual ~XmlObjectIndexed() { } ;

        virtual TiXmlElement* getXml();
        virtual bool loadXml(TiXmlElement* pElem, std::string path);

        int i() { return index_; }
    protected:
        int index_;
};


#endif // _XML_OBJECT_H_
