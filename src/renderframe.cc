/* vim: set ts=4 ss=4 sw=4 noet ai cindent : */

#include "renderframe.h"

wxBEGIN_EVENT_TABLE(RenderFrame, wxFrame)
	EVT_CLOSE( RenderFrame::OnClose )
	EVT_PAINT( RenderFrame::OnPaint )
wxEND_EVENT_TABLE()


RenderFrame::RenderFrame(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &pos, const wxSize &size, long style, const wxString &name)
		: wxFrame(parent, id, title, pos, size, style, name)
{
	canvas_ = new wxAlpharayCanvas(this);
}


void RenderFrame::Render(Project* project) 
{
	SetClientSize(project->outWidth(), project->outHeight());
	
	wxRect windowRect = GetScreenRect();
	wxRect screeRect(0, 0, wxSystemSettings::GetMetric( wxSYS_SCREEN_X ), wxSystemSettings::GetMetric( wxSYS_SCREEN_Y ));
	windowRect = screeRect.Intersect(windowRect);
	SetSize(windowRect);


	canvas_->Setup(&bitmap_, project->outWidth(), project->outHeight());
	
	project->render(&bitmap_);
	
	Refresh();
}


void RenderFrame::OnDoneRender()
{
	std::cout << "done render" << std::endl;
}


void RenderFrame::OnPaint(wxPaintEvent& event)
{
	/*
	std::cout << "repainting" << std::endl;
	if (!bitmap_.IsOk()) return;
	
	wxClientDC dc(this);
	PrepareDC(dc);
	
	dc.DrawBitmap(bitmap_, 0, 0, false );*/
	event.Skip();
}





void RenderFrame::OnClose(wxCloseEvent& event)
{
	if ( event.CanVeto() ) {
		event.Veto();
		Hide();
		return;
	}
	event.Skip();
}
