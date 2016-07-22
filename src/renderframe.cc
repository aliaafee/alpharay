/* vim: set ts=4 ss=4 sw=4 noet ai cindent : */

#include "renderframe.h"


wxThread::ExitCode RenderThread::Entry()
{
	project_->render(bitmap_, &(renderFrame_->renderStatus_));

	wxThreadEvent event( wxEVT_THREAD, ID_RenderDone );
	wxQueueEvent( renderFrame_, event.Clone() );

    return (ExitCode)0;
}


void RenderThread::CancelRender()
{
	project_->cancelRender();
}


wxBEGIN_EVENT_TABLE(RenderFrame, wxFrame)
	EVT_MENU	(ID_SaveRender, RenderFrame::OnSaveRender)
	EVT_CLOSE( RenderFrame::OnClose )
	EVT_PAINT( RenderFrame::OnPaint )
	EVT_THREAD(ID_RenderDone, RenderFrame::OnDoneRender)
	EVT_TIMER(ID_Timer, RenderFrame::OnTimer)
wxEND_EVENT_TABLE()


RenderFrame::RenderFrame(Project* project, wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &pos, const wxSize &size, long style, const wxString &name)
		: wxFrame(parent, id, title, pos, size, style, name), timer_(this, ID_Timer), renderThread_(this, project, &bitmap_)
{
	//Menu Bar
    wxMenu *menuFile = new wxMenu;
	menuFile->Append(ID_SaveRender, "&Save Image\tCtrl-S",
                     "Save render result to file");
	wxMenuBar *menuBar = new wxMenuBar;
	menuBar->Append( menuFile, "&File" );
	SetMenuBar( menuBar );

	wxBoxSizer* sizer = new wxBoxSizer( wxVERTICAL);
	
	canvas_ = new wxAlpharayCanvas(this);
	sizer->Add(canvas_, 1, wxEXPAND|wxALL,0);

	progressBar_ = new wxGauge(this, ID_ProgressBar, 100);
	sizer->Add(progressBar_, 0, wxEXPAND|wxALL,0);

	SetSizer(sizer);

	CreateStatusBar();
    SetStatusText( "" );

	project_ = project;
	
	SetClientSize(project->outWidth(), project->outHeight() + (progressBar_->GetSize()).GetHeight());
	
	wxRect windowRect = GetScreenRect();
	wxRect screeRect(0, 0, wxSystemSettings::GetMetric( wxSYS_SCREEN_X ), wxSystemSettings::GetMetric( wxSYS_SCREEN_Y ));
	windowRect = screeRect.Intersect(windowRect);
	SetSize(windowRect);

	canvas_->Setup(&bitmap_, project->outWidth(), project->outHeight());

	if ( renderThread_.Create() != wxTHREAD_NO_ERROR ||
         renderThread_.Run() != wxTHREAD_NO_ERROR )
    {
        wxLogError("Can't create/run render thread!");
        return;
    }

	timer_.Start(100);
}


RenderFrame::~RenderFrame()
{
	renderThread_.Delete();
}


void RenderFrame::OnDoneRender(wxThreadEvent& event)
{
	std::cout << "Render Thread Exited" << std::endl;
	
	SetStatusText(wxString(renderStatus_.message()));
	progressBar_->SetValue(renderStatus_.progress());
	Refresh();

	timer_.Stop();
}


void RenderFrame::OnPaint(wxPaintEvent& event)
{
	event.Skip();
}


void RenderFrame::OnTimer(wxTimerEvent& event)
{
	SetStatusText(wxString(renderStatus_.message()));
	progressBar_->SetValue(renderStatus_.progress());
	Refresh();
}


void RenderFrame::OnSaveRender(wxCommandEvent& event)
{
	wxFileDialog saveFileDialog(this, _("Save Render Result"), "", "",
                       "Image files (*.*)|*.*", wxFD_SAVE|wxFD_OVERWRITE_PROMPT);

	if (saveFileDialog.ShowModal() == wxID_CANCEL)
		return;

	bitmap_.save(saveFileDialog.GetPath().ToStdString());
}


void RenderFrame::OnClose(wxCloseEvent& event)
{
	std::cout << "Got Close Event" << std::endl;
	//if (renderThread_ != NULL) {
	renderThread_.CancelRender();
	renderThread_.Wait();
	//}

	/*
	if ( event.CanVeto() ) {
		event.Veto();
		Hide();
		return;
	}*/
	event.Skip();
}
