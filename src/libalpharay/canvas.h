/* vim: set ts=4 ss=4 sw=4 noet ai cindent : */

#ifndef _CANVAS_H_
#define _CANVAS_H_

#include <vector>
#include "image.h"
#include "object2d.h"

class Canvas : public Image, virtual public XmlObjectNamed
{
	public:
        virtual void init();

        Canvas( int width, int height ) 
            : Image("unnamed"), XmlObjectNamed("canvas", "unnamed")
            { init(); create(width, height); }
        
        Canvas( std::string name ) 
            : Image(name), XmlObjectNamed("canvas", name)
            { init(); }

        ~Canvas() {};

        virtual bool create(int width, int height) { width_ = width; height_ = height; return true; }

        virtual Color getColor(Vector2 point);

		virtual void transform();

        virtual TiXmlElement* getXml();
        virtual bool loadXml(TiXmlElement* pElem, std::string path);

		virtual std::vector<BaseEditableLink*>* getEditableLinksList() {
			return &combinedEditableLinks_;
		}
		
	protected:
		Color backgroundColor_;
		std::vector<Object2d*> objects;

		std::vector<BaseEditableLink*> combinedEditableLinks_;
};


#endif // _CANVAS_H_

