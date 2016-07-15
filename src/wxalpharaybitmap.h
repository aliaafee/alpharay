/* vim: set ts=4 ss=4 sw=4 noet ai cindent : */

#ifndef WX_ALPHARAY_BITMAP_H
#define WX_ALPHARAY_BITMAP_H

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif
#include <wx/rawbmp.h>

#include "libalpharay/bitmap.h"


class wxAlpharayBitmap : public Bitmap, public wxBitmap
{
	public:
		wxAlpharayBitmap();
		
		virtual bool create(int width, int height);

		virtual void refresh();
		virtual void RefreshArea(const wxRect& rect);
		virtual bool setColor(Vector2 point, Color color);
		
		void setExposure(float exp)
			{ exposure_ = exp; }

	private:
		float exposure_;
};

#endif // WX_ALPHARAY_BITMAP_H
