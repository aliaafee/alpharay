/* vim: set ts=4 ss=4 sw=4 noet ai cindent : */

/*
#ifdef WXWIDGETS

#include "gui/app.h"

int main (int argc, char **argv) {
	appMain(argc, argv);

	return 0;
}

#else
*/

#include <iostream>
#include <string>
#include <sstream>
#include <unistd.h>

#include "libalpharay/project.h"
#include "libalpharay/bitmap.h"

using namespace std;

Project project;
Bitmap output("");

string projectFile, outFile, projectSaveFile;


void usage(string name)
{
    cerr << "Usage: " << name << " <options(s)> PROJECT" << endl
         << "Options: " << endl
         << "\t-h\tDisplay this help message" << endl
         << "\t-o\tOutput image filename" << endl
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

	if (projectSaveFile == "" && outFile == "") {
		cout << "Not output set" << endl;
		usage(argv[0]);
		return 0;
	}

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
        project.render(&output);

		cout << "Saving output to..." << outFile << endl;
		output.save(outFile);
		cout << "Done" << endl;
        
		return 0;
    }

    return 0;
}

//#endif
