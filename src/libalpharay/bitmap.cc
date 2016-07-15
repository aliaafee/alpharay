/* vim: set ts=4 ss=4 sw=4 noet ai cindent : */

#include "bitmap.h"


void Bitmap::init() 
{  
    image_ = NULL;
	imageSize_ = 0;

	addEditable(new Editable<std::string>("filename", &filename_, ""));
}


Bitmap::~Bitmap()
{
    if (imageSize_ != 0) {
        delete[] image_;
    }
}


bool Bitmap::create(int width, int height)
{
    if (imageSize_ != 0) {
        delete[] image_;
    }

	width_ = width;
	height_ = height;
	imageSize_ = width_ * height_ * 3;
	image_ = new float[width_ * height_ * 3];

    return true;
}


bool Bitmap::load()
{
	return load(absfilename_);
}


bool Bitmap::load(std::string filename)
{
    if (imageSize_ != 0) {
        delete[] image_;
    }
#ifdef CIMG
	CImg<float>* loadedImage;
	cimg::exception_mode(0);
	try {
		loadedImage = new CImg<float>(filename.c_str());
	} catch (cimg_library::CImgException& e) {
		std::cerr << "Image file not found" << std::endl;
		return false;
	}

	width_ = loadedImage->width();
	height_ = loadedImage->height();
	imageSize_ = width_ * height_ * 3;
	image_ = new float[width_ * height_ * 3];

	unsigned int i;
	cimg_forXY(*loadedImage, x, y) {
		i = y * width_ * 3 + x * 3;
		image_[i] = (*loadedImage)(x, y, 0) / 255.0;
		image_[i+1] = (*loadedImage)(x, y, 1) / 255.0;
		image_[i+2] = (*loadedImage)(x, y, 2) / 255.0;
	}

	return true;
#else
	//Load PPM image
	char *t;
	char buf[256];
	int r;
	unsigned int w, h, d;
	
	FILE *fp = fopen(filename.c_str(), "rb");

	if (fp == NULL) { 
		std::cerr << "Image file not found" << std::endl;
		return false;
	}
	
	t = fgets(buf, 256, fp);
	if ( (t == NULL) || (strncmp(buf, "P6\n", 3) != 0) ) {
		std::cerr << "PPM Load: Invalid image format" << std::endl;
		return false;
	}

	do {
		t = fgets(buf, 256, fp);
		if (t == NULL) {
			std::cerr << "PPM Load: Invalid image file: ended unexpectadly" << std::endl;
			return false;
		}
	} while (strncmp(buf, "#", 1) == 0);

	r = sscanf(buf, "%u %u", &w, &h);
	if (r < 2) {
		std::cerr << "PPM Load: Invalid image file: bad dimensions" << std::endl;
		return false;
	}

	r= fscanf(fp, "%u", &d);
	if ( (r < 1) || (d != 255)) {
		std::cerr << "PPM Load: Invalid image file: unsupported image depth" << std::endl;
		return false;
	}

	fseek(fp, 1, SEEK_CUR);
	
	width_ = w;
	height_ = h;
	imageSize_ = width_ * height_ * 3;
	image_ = new float[width_ * height_ * 3];

	size_t rd;
	unsigned char pixel[3];
	for (unsigned int i = 0; i < imageSize_-3 && rd == 3; i += 3) {
		rd = fread(pixel, 1, 3, fp);
		image_[i] = float(pixel[0]) / 255.0;
		image_[i+1] = float(pixel[1]) / 255.0;
		image_[i+2] = float(pixel[2]) / 255.0;
	}

	refresh();

    return true;
#endif
}


bool Bitmap::save(std::string filename)
{
    if (imageSize_ == 0) return false;

#ifdef CIMG
	CImg<float> outImage(width(), height(), 1, 3, 0);
	
	unsigned int i;
	cimg_forXY(outImage, x, y) {
		i = y * width_ * 3 + x * 3;
		outImage(x, y, 0) = image_[i] * 255.0;
		outImage(x, y, 1) = image_[i+1] * 255.0;
		outImage(x, y, 2) = image_[i+2] * 255.0;
	}

	outImage.save(filename.c_str());

	return true;
#else
	//Save image to PPM Format
	FILE *fp = fopen(filename.c_str(), "wb");
	(void) fprintf(fp, "P6\n%d %d\n255\n", width_, height_);
	for (unsigned int i = 0; i < imageSize_; i++) {
		static unsigned char color[3];
		color[0] = char(image_[i] * 255.0f);
		if (color[0] > 255) { color[0] = 255; }

		(void) fwrite(color, 1, 1, fp);
	}
	(void) fclose(fp);
	
	return true;
#endif
}


Color Bitmap::getColor(Vector2 point)
{
	Color result;

	if (imageSize_ == 0) return result;

	int u = int(point.x) % width_;
	int v = int(point.y) % height_;

	int i = v * width_ * 3 + u * 3;

	result.x = image_[i];
	result.y = image_[i+1];
	result.z = image_[i+2];

	return result;
}


bool Bitmap::setColor(Vector2 point, Color color)
{
	if (imageSize_ == 0) return false;

	int u = int(point.x) % width_;
	int v = int(point.y) % height_;

	int i = v * width_ * 3 + u * 3;

	image_[i] = color.x;
	image_[i+1] = color.y;
	image_[i+2] = color.z;

	return true;
}


int Bitmap::width()
{
	return width_;
}


int Bitmap::height()
{
	return height_;
}


void Bitmap::toneMap_simple()
{
	if (imageSize_ == 0) return;

	for (unsigned int i = 0; i < imageSize_; i++) {
		image_[i] = image_[i] / (image_[i] + 1.0f);
	}
	
	refresh();
}

void Bitmap::toneMap_gamma(float a, float gamma)
{
	// a > 0 and 0 < gamma < 1
    // converts from [0, 1/a^(1/gamma)] to [0,1]

	if (imageSize_ == 0) return;

	for (unsigned int i = 0; i < imageSize_; i++) {
		image_[i] = a * pow(image_[i], gamma);
	}

	refresh();
}

void Bitmap::toneMap_exp(float exposure)
{
	if (imageSize_ == 0) return;

	for (unsigned int i = 0; i < imageSize_; i++) {
		image_[i] = 1.0f - expf(image_[i] * exposure);
	}

	refresh();
}



void Bitmap::bloom(float radius, float highpass) 
{
	if (imageSize_ == 0) return;
#ifdef CIMG	
    //Creat a Copy and do a high pass filter
	CImg<float> blured(width(), height(), 1, 3, 0);
	unsigned int i;
	cimg_forXY(blured, x, y) {
		i = y * width_ * 3 + x * 3;
		if ( (image_[i] + image_[i+1] + image_[i+2])/3.0f > highpass) {
			blured(x, y, 0) = image_[i] * 255.0;
			blured(x, y, 1) = image_[i+1] * 255.0;
			blured(x, y, 2) = image_[i+2] * 255.0;
		} else {
			blured(x, y, 0) = 0;
			blured(x, y, 1) = 0;
			blured(x, y, 2) = 0;
		}
	}

    //Blur
    blured.blur(radius,radius,1);

    //Combine
	cimg_forXY(blured, x, y) {
		i = y * width_ * 3 + x * 3;
		image_[i] += blured(x, y, 0)/255.0;
		image_[i+1] += blured(x, y, 1)/255.0;
		image_[i+2] += blured(x, y, 2)/255.0;
	}
#else
	std::cout << "Bloom not awailable without CImg library" << std::endl;
#endif
}


bool Bitmap::loadXml(TiXmlElement* pElem, std::string path, LinkList *linkList)
{
    Image::loadXml(pElem, path, linkList);

    if (filename_ != "") {
        absfilename_ = pathJoin(path, filename_);
    }

    return false;
}
