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
	long benchmarkSamples_;

	void RunBenchmark(long samples);

	void processArgs();
};


IMPLEMENT_APP(Alpharay);


bool Alpharay::OnInit()
{
	processArgs();

	project_ = NULL;//new Project();

	if (projectFilename_ != "") {
		project_ = new Project();
		if (project_->load(projectFilename_.ToStdString())) {
			if (benchmarkSamples_ > 0) {
				RunBenchmark(benchmarkSamples_);
				return false;
			}
		}
	}

    MainFrame *frame = new MainFrame( "Alpharay", project_, wxDefaultPosition, wxSize(800, 600) );
	frame->Show( true );
	
    return true;
}

void Alpharay::processArgs()
{
	wxCmdLineParser parser (argc, argv);
	parser.SetLogo("Alpharay");
	parser.AddOption("p", "project", "Project file");
	parser.AddOption("s", "save", "Save project to a new file.");
	parser.AddOption("b", "benchmark", "Benchmark, number of samples", wxCMD_LINE_VAL_NUMBER );
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

	benchmarkSamples_ = 0;
	parser.Found( "b", &benchmarkSamples_);

}


void Alpharay::RunBenchmark(long samples) {
	Bitmap bitmap("");
	RenderStatus rs;
	long renderTimes[samples];

	for (int i = 0; i < samples; i++) {
		std::cout << "Benchmar sample " << i+1 << " of " << samples << std::endl;
		project_->render(&bitmap, &rs);
		renderTimes[i] = rs.renderTime();
	}

	std::cout << "Summary" << std::endl;
	std::cout << "sample\t" << "time(ns)\t" << std::endl;
	for (int i = 0; i < samples; i++) {
		std::cout << i+1 << "\t" << renderTimes[i] << std::endl;
	}
}
