/* vim: set ts=4 ss=4 sw=4 noet ai cindent : */

#include "wxalpharaybitmap.h"


wxAlpharayBitmap::wxAlpharayBitmap()
	: Bitmap("unnamed"), wxBitmap() {
	exposure_ = -1.0f;
}


bool wxAlpharayBitmap::create(int width, int height) {
	Bitmap::create(width, height);
	wxBitmap::Create(width, height, 32);
	
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
	unsigned int r, g, b;
	for (unsigned int y = 0; y < height(); ++y )
	{
		wxNativePixelData::Iterator rowStart = p;
		for (unsigned int x = 0; x < width(); ++x, ++p )
		{
			i = y * width_ * 3 + x * 3;
			r = int(image_[i]*255.0f);
			if (r > 255) r = 255;
			g = int(image_[i+1]*255.0f);
			if (g > 255) g = 255;
			b = int(image_[i+2]*255.0f);
			if (b > 255) b = 255;
			p.Red() = 255;
			p.Green() = 255;
			p.Blue() = 255;
		}
		p = rowStart;
		p.OffsetY(data, 1);
	}
}


void wxAlpharayBitmap::fallbackRefreshArea_(const wxRect& fixedRect) {
	std::cout << "wxAlpharayBitmap: Fallback Bitmap Draw, this is slow." << std::endl;

	wxMemoryDC temp_dc;
	temp_dc.SelectObject(*this);
	wxPen temp_pen;

	temp_dc.SetBackground(*wxWHITE_BRUSH);
	temp_dc.Clear();
	
	unsigned int ix, iy, i;
	int r, g, b;
	for (unsigned int y = 0; y < fixedRect.height; ++y )
	{
		iy = y + fixedRect.y;
		for (unsigned int x = 0; x < fixedRect.width; ++x)
		{
			ix = x + fixedRect.x;
			i = iy * width_ * 3 + ix * 3;
			r = int(image_[i]*255.0f);
			if (r > 255) r = 255;
			g = int(image_[i+1]*255.0f);
			if (g > 255) g = 255;
			b = int(image_[i+2]*255.0f);
			if (b > 255) b = 255;
			temp_pen.SetColour(wxColour(r,g,b));
			temp_dc.SetPen(temp_pen);
			temp_dc.DrawPoint(ix, iy);
			
			//assign rgb to bitmap
		}
	}
	temp_dc.SelectObject(wxNullBitmap);
}


void wxAlpharayBitmap::RefreshArea(const wxRect& rect) {
	if (!IsOk()) return;
	
	wxRect bitmapRect(0, 0, width(), height());
	wxRect fixedRect = bitmapRect.Intersect(rect);

	wxNativePixelData data(*this);
	
	if (!data) {
		std::cout << "wxAlpharayBitmap: Native Pixel Data access unavilable" << std::endl;
		fallbackRefreshArea_(fixedRect);
		return;
	}

	wxNativePixelData::Iterator p(data);
	
	p.Offset(data, fixedRect.x, fixedRect.y);

	unsigned int ix, iy, i;
	int r, g, b;
	for (unsigned int y = 0; y < fixedRect.height; ++y )
	{
		iy = y + fixedRect.y;
		wxNativePixelData::Iterator rowStart = p;
		for (unsigned int x = 0; x < fixedRect.width; ++x, ++p )
		{
			ix = x + fixedRect.x;
			i = iy * width_ * 3 + ix * 3;
			r = int(image_[i]*255.0f);
			if (r > 255) r = 255;
			g = int(image_[i+1]*255.0f);
			if (g > 255) g = 255;
			b = int(image_[i+2]*255.0f);
			if (b > 255) b = 255;
			p.Red()   = r;
			p.Green() = g;
			p.Blue()  = b;
		}
		p = rowStart;
		p.OffsetY(data, 1);
	}
}


bool wxAlpharayBitmap::setColor(Vector2 point, Color color)
{
	Bitmap::setColor(point, color);

	//This needs to be thread safe. So no GD Drawing operations here
	
	return true;
}

