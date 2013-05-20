/* vim: set ts=4 ss=4 sw=4 noet ai cindent : */

#ifndef _FLOAT_IMAGE_H_
#define _FLOAT_IMAGE_H_

#include "vector.h"
#include "image.h"

#define expfunc(x, e) (1.0f - expf(x * e))

class FloatImage : public Image
{
public:
    FloatImage(int width, int height);
	~FloatImage();
    
    int height();
    int width();
	
	Vector getColor(float u, float v);
    bool setColor(float u, float v, Vector color);

    Vector getColor(float u, float v, Vector* result);

    void glow(int radius);
    
private:
    float *image_;
};


#endif // _FLOAT_IMAGE_H_
