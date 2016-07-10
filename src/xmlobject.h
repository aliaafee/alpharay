/* vim: set ts=4 ss=4 sw=4 noet ai cindent : */

#ifndef _XML_OBJECT_H_
#define _XML_OBJECT_H_

#include <tinyxml.h>
#include <string>
#include <vector>

#include "editable.h"


class XmlObject
{
    public:
        virtual void init()  {};
        XmlObject( std::string xmlTag ) { init(); xmlTag_ = xmlTag; }
		XmlObject() {;}
		
        ~XmlObject() { } ;

        std::string xmlTag() { return xmlTag_; }

        virtual TiXmlElement* getXml() { return new TiXmlElement(xmlTag_); }
        virtual bool loadXml(TiXmlElement* pElem, std::string path) { return true; }

        std::string pathJoin(std::string path, std::string filename);
    protected:
        std::string xmlTag_;
};


class XmlObjectNamed : public XmlObject
{
    public:
        virtual void init();
		
        XmlObjectNamed( std::string xmlTag, std::string name ) 
            :XmlObject(xmlTag)
            { init(); name_ = name; }

		XmlObjectNamed()
			{;}

        ~XmlObjectNamed() { } ;

        virtual TiXmlElement* getXml();
        virtual bool loadXml(TiXmlElement* pElem, std::string path);

		template <typename T> bool loadXml(TiXmlElement* pElem, std::string path, T* linkList) {
			loadXml(pElem, path);
			
			std::string sourceName;
			int queryResult;
			for (unsigned long i = 0; i < editableLinks.size(); i++) {
				queryResult = pElem->QueryStringAttribute((editableLinks[i]->name()).c_str(), &sourceName);
				if (queryResult == TIXML_SUCCESS) {
					editableLinks[i]->setSource(sourceName);
					EditableLink<XmlObjectNamed>* el = reinterpret_cast <EditableLink<XmlObjectNamed>*> (editableLinks[i]);
					linkList->add(sourceName, el->target());
				}
			}
			
			return true;
		};

        std::string name() { return name_; }

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
};


class XmlObjectIndexed : public XmlObject
{
    public:
        virtual void init() {};//  { XmlObject::init(); }
        
        XmlObjectIndexed( std::string xmlTag, int index )
            :XmlObject(xmlTag)
            { init(); index_ = index; }

        ~XmlObjectIndexed() { } ;

        virtual TiXmlElement* getXml();
        virtual bool loadXml(TiXmlElement* pElem, std::string path);

        int i() { return index_; }
    protected:
        int index_;
};


#endif // _XML_OBJECT_H_
