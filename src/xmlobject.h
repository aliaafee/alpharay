/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */

#ifndef _XML_OBJECT_H_
#define _XML_OBJECT_H_

#include <tinyxml.h>
#include <string>

class XmlObject
{
    public:
        virtual void init()  {};
        XmlObject( std::string xmlTag ) { init(); xmlTag_ = xmlTag; }
        ~XmlObject() { } ;

        virtual TiXmlElement* getXml() { return new TiXmlElement(xmlTag_); }
        virtual bool loadXml(TiXmlElement* pElem, std::string path) { return true; }

        std::string pathJoin(std::string path, std::string filename);
    protected:
        std::string xmlTag_;
};


class XmlObjectNamed : public XmlObject
{
    public:
        virtual void init()  { XmlObject::init(); }
        
        XmlObjectNamed( std::string xmlTag, std::string name ) 
            :XmlObject(xmlTag)
            { init(); name_ = name; }

        ~XmlObjectNamed() { } ;

        virtual TiXmlElement* getXml();
        virtual bool loadXml(TiXmlElement* pElem, std::string path);

        std::string name() { return name_; }
    protected:
        std::string name_;
};


class XmlObjectIndexed : public XmlObject
{
    public:
        virtual void init()  { XmlObject::init(); }
        
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
