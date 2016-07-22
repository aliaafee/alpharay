/* vim: set ts=4 ss=4 sw=4 noet ai cindent : */

#ifndef RENDER_FRAME_H
#define RENDER_FRAME_H

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif
#include <wx/thread.h>
#include <wx/gauge.h>

#include "wxalpharaybitmap.h"
#include "wxalpharaycanvas.h"

#include "libalpharay/project.h"


enum {
	ID_RenderDone,
	ID_RenderCancel,
	ID_Timer,
	ID_ProgressBar,
	ID_SaveRender
};


class RenderFrame;


class RenderThread : public wxThread
{
public:
    RenderThread(RenderFrame* renderFrame, Project *project, Bitmap* bitmap) : wxThread(wxTHREAD_JOINABLE)
    {
        renderFrame_ = renderFrame;
		project_ = project; 
		bitmap_ = bitmap;
    }

    virtual ExitCode Entry() wxOVERRIDE;

	void CancelRender();

	void OnSaveRender(wxCommandEvent& event);

private:
    RenderFrame *renderFrame_;
	Project *project_;
	Bitmap *bitmap_;
};


class RenderFrame : public wxFrame
{
	friend class RenderThread;
    public:
		RenderFrame(Project* project, wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &pos=wxDefaultPosition, const wxSize &size=wxDefaultSize, long style=wxDEFAULT_FRAME_STYLE, const wxString &name=wxFrameNameStr);
		
        ~RenderFrame();
	protected:
		RenderStatus renderStatus_;
    private:
		Project* project_;
		wxAlpharayBitmap bitmap_;
		wxAlpharayCanvas* canvas_;
		RenderThread renderThread_;
		wxTimer timer_;
		wxGauge* progressBar_;

		void OnSaveRender(wxCommandEvent& event);

		void OnClose(wxCloseEvent& event);
		
		void OnPaint(wxPaintEvent& paint);

		void OnTimer(wxTimerEvent& event);
		
		void OnDoneRender(wxThreadEvent& event);

		wxDECLARE_EVENT_TABLE();
};


#endif // RENDER_FRAME_H
