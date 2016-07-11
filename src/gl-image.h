/* vim: set ts=4 ss=4 sw=4 noet ai cindent : */

#ifndef _GL_IMAGE_H_
#define _GL_IMAGE_H_

#include "bitmap.h"

#include <GL/freeglut.h>


class GLImage : public Bitmap, virtual public XmlObjectNamed
{
    public:
        virtual void init();
		
        GLImage( int width, int height ) 
            : Bitmap("unnamed"), XmlObjectNamed("glimage", "unnamed")
            { init(); create(width, height); }
        
        GLImage( std::string name ) 
            : Bitmap(name), XmlObjectNamed("glimage", name)
            { init(); }

        ~GLImage() { 
			if (imageSize_ != 0) delete[] glImage_; }


		virtual bool create(int width, int height);
        virtual bool load(std::string filename);
        //virtual bool save(std::string filename);

        //virtual Color getColor(Vector2 point);
        virtual bool setColor(Vector2 point, Color color);

		void refresh();
		void display();

		void setGlExposure(float exp) 
			{ glExposure_ = exp; }
    protected:
        GLubyte *glImage_;
		unsigned int glImageSize_;

		float glExposure_;
};

#endif // _GL_IMAGE_H_
