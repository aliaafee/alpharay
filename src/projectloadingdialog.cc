/* vim: set ts=4 ss=4 sw=4 noet ai cindent : */

#include "projectloadingdialog.h"


wxThread::ExitCode ProjectLoadingThread::Entry()
{
	std::cout << "Loading Project" << std::endl;
	bool loadresult = project_->load(filename_.ToStdString());
	
	if (m_dlg->result_ != LOADING_CANCEL) {
		if (loadresult == true) {
			m_dlg->result_ = LOADING_SUCCESS;
		} else {
			m_dlg->result_ = LOADING_FAIL;
		}
	}

	wxThreadEvent event( wxEVT_THREAD, ID_ThreadDone );
	wxQueueEvent( m_dlg, event.Clone() );

    return (ExitCode)0;
}


wxBEGIN_EVENT_TABLE(ProjectLoadingDialog, wxDialog)
	EVT_THREAD(ID_ThreadDone, ProjectLoadingDialog::OnDoneLoading)
	EVT_BUTTON ( ID_CancelLoad, ProjectLoadingDialog::OnCancel )
	EVT_CLOSE( ProjectLoadingDialog::OnClose )
wxEND_EVENT_TABLE()
	

ProjectLoadingDialog::ProjectLoadingDialog(wxFrame *parent, Project *project, wxString filename)
    : wxDialog(parent, wxID_ANY, "Loading Project", wxDefaultPosition, wxSize(200,50)*1.5, wxDEFAULT_DIALOG_STYLE),
      m_thread(this, project, filename)
{
	project_ = project;
	
	wxButton * c = new wxButton( this, ID_CancelLoad, "Cancel", wxDefaultPosition, wxDefaultSize, 0 );

    if ( m_thread.Create() != wxTHREAD_NO_ERROR ||
         m_thread.Run() != wxTHREAD_NO_ERROR )
    {
        wxLogError("Can't create/run loading thread!");
        return;
    }
}


ProjectLoadingDialog::~ProjectLoadingDialog()
{
    m_thread.Delete();
}


void ProjectLoadingDialog::OnDoneLoading(wxThreadEvent& event)
{
    Refresh();
		
	Close( true );
}


void ProjectLoadingDialog::OnCancel(wxCommandEvent& event)
{
	result_ = LOADING_CANCEL;
	
	project_->cancelLoad();
	std::cout << "cancelling" << std::endl;
}


void ProjectLoadingDialog::OnClose(wxCloseEvent& event)
{
	if ( event.CanVeto() ) {
		result_ = LOADING_CANCEL;
		
		project_->cancelLoad();

		std::cout << "cancelling" << std::endl;
		
		event.Veto();
		return;
	} 
	event.Skip();
}
