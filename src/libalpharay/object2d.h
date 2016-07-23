/* vim: set ts=4 ss=4 sw=4 noet ai cindent : */

#ifndef _OBJECT2D_H_
#define _OBJECT2D_H_

#include <sstream>

#include "vector.h"
#include "xmlobject.h"
#include "image.h"

class Object2d : virtual public XmlObjectNamed
{
	friend class Canvas;
	
	public:
		void init();
		
        Object2d() 
            : XmlObjectNamed ("object2d", "unnamed") 
            { init(); }

        Object2d(std::string name) 
            : XmlObjectNamed ("object2d", name) 
            { init(); }

        ~Object2d() {};

		virtual void transform();

		virtual Vector2 transformPoint(Vector2 &point);
        virtual Vector2 transformPointInv(Vector2 &point);

		virtual bool isInside(Vector2 &point) { return false; }
		virtual Color getColor(Vector2 &point) { return Color(fillColor_.x, fillColor_.y, fillColor_.z, opacity_); }

		//virtual TiXmlElement* getXml();
        //virtual bool loadXml(TiXmlElement* pElem, std::string path);

	protected:
		Vector2 position_;
		float rotation_;
		Vector2 scale_;

		Color fillColor_;
		Color strokeColor_;

		float opacity_;

	private:
		Matrix4 transMatrix; // TRS
        Matrix4 transMatrixInv; // S^R^T^
};


class Circle : virtual public Object2d, virtual public XmlObjectNamed
{
	public:
        void init() { ; }

        Circle(std::string name) 
            : XmlObjectNamed ("circle", name) 
            { init(); }

        ~Circle() {};

		virtual bool isInside(Vector2 &point);
};


class Square : virtual public Object2d, virtual public XmlObjectNamed
{
	public:
        void init() { ; }

        Square(std::string name) 
            : XmlObjectNamed ("square", name) 
            { init(); }

        ~Square() {};

		virtual bool isInside(Vector2 &point);
};


class Polygon : virtual public Object2d, virtual public XmlObjectNamed
{
	public:
        void init();

        Polygon(std::string name) 
            : XmlObjectNamed ("polygon", name) 
            { init(); }

        ~Polygon() {};

		virtual bool isInside(Vector2 &point);

		virtual bool loadXml(TiXmlElement* pElem, std::string path);
		virtual TiXmlElement* getXml();
		
	private:
		std::string strPoints_;
		std::vector<Vector2> points_;
};


class Picture : virtual public Object2d, virtual public XmlObjectNamed
{
	public:
        void init();

        Picture(std::string name) 
            : XmlObjectNamed ("picture", name) 
            { init(); }

        ~Picture() {};

		virtual bool isInside(Vector2 &point);
		virtual Color getColor(Vector2 &point);

	private:
		Image* image_;
		Image* mask_;
		Vector2 tile_;
};


#endif // _OBJECT2D_H_
