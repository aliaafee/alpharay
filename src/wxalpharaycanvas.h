/* vim: set ts=4 ss=4 sw=4 noet ai cindent : */

#ifndef WX_ALPHARAY_CANVAS_H
#define WX_ALPHARAY_CANVAS_H

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

#include "wxalpharaybitmap.h"


class wxAlpharayCanvas : public wxScrolledCanvas
{
	public:
		wxAlpharayCanvas(wxWindow *parent, wxWindowID id=-1, const wxPoint &pos=wxDefaultPosition, const wxSize &size=wxDefaultSize, long style=wxHSCROLL|wxVSCROLL, const wxString &name="scrolledWindow");
		
		//wxAlpharayCanvas(wxWindow *parent, wxWindowID id=wxID_ANY);

		void Setup(wxAlpharayBitmap* bitmap, int width, int height);

		void OnPaint(wxPaintEvent& event);

	private:
		wxAlpharayBitmap* bitmap_;

	wxDECLARE_EVENT_TABLE();
};


#endif // WX_ALPHARAY_CANVAS_H
