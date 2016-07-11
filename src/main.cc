/* vim: set ts=4 ss=4 sw=4 noet ai cindent : */

#include <iostream>
#include <string>
#include <sstream>
#include <unistd.h>

#include "project.h"
#include "bitmap.h"

#ifdef OPENGL
#include "gui.h"
#endif

using namespace std;

Project project;
Bitmap final("");

string projectFile, outFile, projectSaveFile;


void usage(string name)
{
    cerr << "Usage: " << name << " <options(s)> PROJECT" << endl
         << "Options: " << endl
         << "\t-h\tDisplay this help message" << endl
#ifdef OPENGL
         << "\t-o\tOutput image filename (Optional)" << endl
#else
		 << "\t-o\tOutput image filename (Required)" << endl
#endif
		 << "\t-s\tSave project to a new file" << endl
         << endl;
}


int main (int argc, char **argv)
{
    int c;

    while ((c = getopt (argc, argv, "ho:s:")) != -1)
    switch (c)
    {
        case 'o':
            outFile = optarg;
            break;
		case 's':
			projectSaveFile = optarg;
			break;
        case 'h':
            usage(argv[0]);
            return 1;
            break;
    }

    if ( optind < argc ) {
        projectFile = argv[optind];
    }

    if (projectFile == "") {
        usage(argv[0]);
        return 1;
    }

#ifndef OPENGL
    if (outFile == "") {
        usage(argv[0]);
        cerr << "NOTE: Compile with freeglut to enable render preview" << endl;
        return 1;
    }
#endif

    bool loaded = project.load(projectFile);
    if (!loaded) {
        cerr << "Failed to load " << projectFile << endl;
		return 0;
    }

	if (projectSaveFile != "") {
		project.save(projectSaveFile);
		cout << "Saving Project to " << projectSaveFile << endl;
		return 0;
	}

    if (outFile != "") {
		project.setOutFile(outFile);
        project.setFinalImage(&final);
        project.renderFinal();
		//initGlut(argc, argv);
		//glutMainLoop();

        //cout << "Saving render result..." << outFile << endl;
        //final.save(outFile);
        return 0;
    }

#ifdef OPENGL
	guiMain(&project, argc, argv);
#endif
    
    return 0;
}
