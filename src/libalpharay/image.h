/* vim: set ts=4 ss=4 sw=4 noet ai cindent : */

#ifndef _IMAGE_H_
#define _IMAGE_H_

#include "vector.h"
#include "xmlobject-named.h"


class Image : virtual public XmlObjectNamed
{
    public:
        virtual void init() { type_ = "image"; width_ = 0; height_ = 0; }
        
        Image( std::string name ) 
            : XmlObjectNamed("image", name)
            { init(); }

        virtual ~Image() {} ;

        virtual bool create(int width, int height) { return true; }
        virtual bool load(std::string filename) { return true; }
        virtual bool save(std::string filename) { return true; }
		
		virtual bool load() {return true;}

        virtual void copyTo(Image* target);

        virtual Color getColor(Vector2 point) { return Color(0, 0, 0); }
        virtual bool setColor(Vector2 point, Color color) { return true; }

        virtual int width()  { return width_; }
        virtual int height() { return height_; }

		virtual void transform() {};
    protected:
        int width_;
        int height_;
};


template <> inline std::string EditableLink<Image>::type() {
	return "image";
}


class Checker2d : public Image, virtual public XmlObjectNamed
{
    public:
        virtual void init();
		
        Checker2d( std::string name ) 
            : Image(name), XmlObjectNamed("checker2d", name)
            { init(); }

        ~Checker2d() { } ;

        virtual Color getColor(Vector2 point);

    protected:
        Color color1_;
        Color color2_;
};

#endif // _IMAGE_H_

