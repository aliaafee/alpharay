/* vim: set ts=4 ss=4 sw=4 noet ai cindent : */

#include "gl-image.h"


void GLImage::init() 
{ 
	Bitmap::init(); 
    
    glImage_ = NULL;
	glImageSize_ = 0;
}


bool GLImage::create(int width, int height)
{
	Bitmap::create(width, height);

	if (glImageSize_ != 0) {
        delete[] glImage_;
    }
	
	glImageSize_ = width_ * height_ * 3;
    glImage_ = new GLubyte[glImageSize_];

	for (unsigned int i = 0; i < glImageSize_; i++) {
		glImage_[i] = 0;
	}

    return true;

}


bool GLImage::load(std::string filename)
{
	return Bitmap::load(filename);
}


void GLImage::refresh()
{
	Bitmap::refresh();
	
	unsigned int i, gli;
	for (unsigned int x = 0; x < width_; x++) {
		for (unsigned int y = 0; y < height_; y++) {
			i = y * width_ * 3 + x * 3;
			gli = ((height_-1) - y) * width_ * 3 + x * 3;

			glImage_[gli] =  GLubyte(image_[i]*255.0f);
			glImage_[gli+1] =  GLubyte(image_[i+1]*255.0f);
			glImage_[gli+2] =  GLubyte(image_[i+2]*255.0f);
		}
	}
}


void GLImage::display()
{
    glDrawPixels(width_, height_, GL_RGB, GL_UNSIGNED_BYTE, glImage_ );
}


bool GLImage::setColor(Vector2 point, Color color)
{
	Bitmap::setColor(point, color);

	if (glImageSize_ == 0) return false;

	int u = int(point.x) % width_;
	int v = ((height_-1)-int(point.y)) % height_;

	int i = v * width_ * 3 + u * 3;
	
	color.capColor();

	glImage_[i] = GLubyte(color.x*255.0f);
    glImage_[i+1] = GLubyte(color.y*255.0f);
    glImage_[i+2] = GLubyte(color.z*255.0f);

	return true;
}
