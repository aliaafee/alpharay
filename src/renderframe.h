/* vim: set ts=4 ss=4 sw=4 noet ai cindent : */

#ifndef RENDER_FRAME_H
#define RENDER_FRAME_H

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif


#include "wxalpharaybitmap.h"
#include "wxalpharaycanvas.h"

#include "libalpharay/project.h"


class RenderFrame : public wxFrame
{
    public:
		RenderFrame(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &pos=wxDefaultPosition, const wxSize &size=wxDefaultSize, long style=wxDEFAULT_FRAME_STYLE, const wxString &name=wxFrameNameStr);
		
        ~RenderFrame() {};

		void Render(Project* project);
    private:
		wxAlpharayBitmap bitmap_;
		wxAlpharayCanvas* canvas_;


		void OnClose(wxCloseEvent& event);
		
		void OnPaint(wxPaintEvent& paint);
		
		void OnDoneRender();

		wxDECLARE_EVENT_TABLE();
};


#endif // RENDER_FRAME_H
