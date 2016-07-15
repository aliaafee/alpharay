/* vim: set ts=4 ss=4 sw=4 noet ai cindent : */

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif
#include <wx/cmdline.h>


#include "libalpharay/project.h"

#include "mainframe.h"


class Alpharay: public wxApp
{
public:
    virtual bool OnInit();
private:
	Project* project_;

	wxString projectFilename_;
	wxString outFilename_;
	wxString saveFilename_;

	void processArgs();
};


IMPLEMENT_APP(Alpharay);


bool Alpharay::OnInit()
{
	processArgs();

	project_ = new Project();

	if (projectFilename_ != "") {
		project_->load(projectFilename_.ToStdString());
	}

    MainFrame *frame = new MainFrame( "Alpharay", project_, wxPoint(50, 50), wxSize(450, 340) );
	frame->Show( true );
	
    return true;
}

void Alpharay::processArgs()
{
	wxCmdLineParser parser (argc, argv);
	parser.SetLogo("Alpharay");
	parser.AddOption("p", "project", "Project file");
	parser.AddOption("s", "save", "Save project to a new file.");
	parser.AddOption("o", "out", "Render output image file.");
	parser.AddSwitch("h", "help", "Display usage.");

	parser.Parse(false);

	if (parser.Found( "h" )) {
		parser.Usage();
		exit(0);
	}

	projectFilename_ = "";
	parser.Found( "p", &projectFilename_);

	outFilename_ = "";
	parser.Found( "o", &outFilename_);

	saveFilename_ = "";
	parser.Found( "s", &saveFilename_);

}
