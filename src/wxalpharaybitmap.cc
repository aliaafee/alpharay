/* vim: set ts=4 ss=4 sw=4 noet ai cindent : */

#include "wxalpharaybitmap.h"


wxAlpharayBitmap::wxAlpharayBitmap()
	: Bitmap("unnamed"), wxBitmap() {
	exposure_ = -1.0f;
}


bool wxAlpharayBitmap::create(int width, int height) {
	Bitmap::create(width, height);
	wxBitmap::Create(width, height);
	
	return true;
}


void wxAlpharayBitmap::refresh() {
	Bitmap::refresh();

	if (!IsOk()) return;

	wxNativePixelData data(*this);
	
	if (!data) return; 

	wxNativePixelData::Iterator p(data);
	
	p.Offset(data, 0, 0);
	
	unsigned int i;
	for (unsigned int y = 0; y < height(); ++y )
	{
		wxNativePixelData::Iterator rowStart = p;
		for (unsigned int x = 0; x < width(); ++x, ++p )
		{
			i = y * width_ * 3 + x * 3;
			p.Red()   = int(image_[i]*255.0f);
			p.Green() = int(image_[i+1]*255.0f);
			p.Blue()  = int(image_[i+2]*255.0f);
		}
		p = rowStart;
		p.OffsetY(data, 1);
	}
}


void wxAlpharayBitmap::RefreshArea(const wxRect& rect) {
	if (!IsOk()) return;

	wxNativePixelData data(*this);
	
	if (!data) return; 

	wxRect bitmapRect(0, 0, width(), height());
	wxRect fixedRect = bitmapRect.Intersect(rect);

	wxNativePixelData::Iterator p(data);
	
	p.Offset(data, fixedRect.x, fixedRect.y);
	
	unsigned int ix, iy, i;
	for (unsigned int y = 0; y < fixedRect.height; ++y )
	{
		iy = y + fixedRect.y;
		wxNativePixelData::Iterator rowStart = p;
		for (unsigned int x = 0; x < fixedRect.width; ++x, ++p )
		{
			ix = x + fixedRect.x;
			i = iy * width_ * 3 + ix * 3;
			p.Red()   = int(image_[i]*255.0f);
			p.Green() = int(image_[i+1]*255.0f);
			p.Blue()  = int(image_[i+2]*255.0f);
		}
		p = rowStart;
		p.OffsetY(data, 1);
	}
}


bool wxAlpharayBitmap::setColor(Vector2 point, Color color)
{
	Bitmap::setColor(point, color);

	//This needs to be thread safe. ?????? how how
	
	/*
	if (bitmap_ == NULL) return true;

	wxNativePixelData data(*bitmap_);

	if (!data) { return true; }

	color.x = 1.0f - expf(color.x * exposure_);
	color.y = 1.0f - expf(color.y * exposure_);
	color.z = 1.0f - expf(color.z * exposure_);

	color.capColor();

	wxNativePixelData::Iterator p(data);
	
	p.Offset(data, int(point.x), int(point.y));

	p.Red()   = int(color.x * 255.0);
	p.Green() = int(color.y * 255.0);
	p.Blue()  = int(color.x * 255.0);
	*/
	
	return true;
}

