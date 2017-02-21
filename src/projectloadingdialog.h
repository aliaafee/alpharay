/* vim: set ts=4 ss=4 sw=4 noet ai cindent : */

#ifndef PROJECT_LOADING_DIALOG_H
#define PROJECT_LOADING_DIALOG_H

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif
#include <wx/thread.h>

#include "libalpharay/project.h"


enum {
	LOADING_SUCCESS,
	LOADING_CANCEL,
	LOADING_FAIL
};
enum {
	ID_ThreadDone,
	ID_CancelLoad
};


class ProjectLoadingDialog;


class ProjectLoadingThread : public wxThread
{
public:
    ProjectLoadingThread(ProjectLoadingDialog *dlg, Project *project, wxString filename) : wxThread(wxTHREAD_JOINABLE)
    {
        m_dlg = dlg;
		project_ = project;
		filename_ = filename;
    }

    virtual ExitCode Entry();// wxOVERRIDE;

private:
    ProjectLoadingDialog *m_dlg;
	Project *project_;
	wxString filename_;
};


class ProjectLoadingDialog: public wxDialog
{
	friend class ProjectLoadingThread;
public:
    // ctor
    ProjectLoadingDialog(wxFrame *frame, Project *project, wxString filename);
    ~ProjectLoadingDialog();

	int result() 
		{ return result_; }

protected:
	int result_;
	
private:
	Project *project_;

	void OnDoneLoading(wxThreadEvent& event);
	void OnCancel(wxCommandEvent& event);
	void OnClose(wxCloseEvent& event);

    ProjectLoadingThread m_thread;
    int m_nCurrentProgress;

    wxDECLARE_EVENT_TABLE();
};



#endif // PROJECT_LOADING_DIALOG_H
