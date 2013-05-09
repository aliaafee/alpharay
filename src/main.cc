/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */

#include <iostream>
#include <string>
#include <unistd.h>

#include <GL/freeglut.h>
#include <GL/glu.h>

#include "project.h"
#include "bitmap.h"

using namespace std;

Project project;
Bitmap final("");

#ifdef opengl

#include "gl-image.h"
GLImage preview("");

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    preview.display();
    
    glutSwapBuffers();
    
}


void reshape(int width, int height)
{
    glViewport(0, 0, width, height);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluOrtho2D (0.0, width, 0.0, height);
}


void idle(void)
{
    glutPostRedisplay();
}


void initGlut(int argc, char** argv)
{
    glutInit(&argc, argv);
    
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    glutInitWindowSize(preview.width(), preview.height());

    glutCreateWindow("Render Result");
    
    glClearColor (0.0, 0.0, 0.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);
}

#endif

void usage(string name)
{
    cerr << "Usage: " << name << " <options(s)> PROJECT" << endl
         << "Options: " << endl
         << "\t-h\tDisplay this help message" << endl
         << "\t-o\tOutput image filename" << endl
         << endl;
}


int main (int argc, char **argv)
{
    string projectFile, outFile;

    int c;

    while ((c = getopt (argc, argv, "hp:o:")) != -1)
    switch (c)
    {
        case 'o':
            outFile = optarg;
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

#ifndef opengl
    if (outFile == "") {
        usage(argv[0]);
        cerr << "To see render preview compile --with-opengl" << endl;
        return 1;
    }
#endif

    bool loaded = project.load(projectFile);
    if (!loaded) {
        cerr << "Failed to load " << projectFile << endl;
    }

    if (outFile != "") {
        project.setFinalImage(&final);
        project.renderFinal(outFile);
        return 0;
    }

#ifdef opengl
    project.setPreviewImage(&preview);
    project.renderPreview();
    initGlut(argc, argv);
    glutMainLoop();
#endif
    
    return 0;
}
