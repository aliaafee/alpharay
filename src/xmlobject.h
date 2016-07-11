/* vim: set ts=4 ss=4 sw=4 noet ai cindent : */

#ifndef _XML_OBJECT_H_
#define _XML_OBJECT_H_

#include <tinyxml.h>
#include <string>
#include <vector>


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
