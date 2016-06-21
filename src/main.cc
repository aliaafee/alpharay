/* vim: set ts=4 ss=4 sw=4 noet ai cindent : */

#include <iostream>
#include <string>
#include <unistd.h>

#include "project.h"
#include "bitmap.h"

using namespace std;

Project project;
Bitmap final("");

#ifdef OPENGL

#include <GL/freeglut.h>
#include <GL/glu.h>

Bitmap preview("");


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


void mouse(int button, int state, int x, int y)
{
	if (button ==  GLUT_LEFT_BUTTON && state == GLUT_UP) {
		std::cout << "Mouse is up at" << ",(" << x << "," << y << ")" << std::endl;
		project.renderPreviewPixel(x,y);
	}
	glutPostRedisplay();
}


void keyboard(unsigned char key, int x, int y)
{
	std::cout << "Pressed " << int(key) << std::endl;
	if (key == 27)
		exit(0);

	if (key == 13) {
		project.renderPreview();
	}
	glutPostRedisplay();
}


void keyboardSpecial(int key, int x, int y)
{
	std::cout << "Pressed " << int(key) << std::endl;

	/*
	Object* obj = project.scene.getObject("two");

	if (key == GLUT_KEY_UP) {
		obj->position_.x += 0.1;;
	}

	if (key == GLUT_KEY_DOWN) {
		obj->position_.x -= 0.1;;
	}

	if (key == GLUT_KEY_LEFT) {
		obj->position_.y += 0.1;;
	}

	if (key == GLUT_KEY_RIGHT) {
		obj->position_.y -= 0.1;;
	}

	project.renderPreview();
	*/

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
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(keyboardSpecial);
}

#endif

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
    string projectFile, outFile, projectSaveFile;

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
        cerr << "Compile with freeglut to enable render preview" << endl;
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
        project.setFinalImage(&final);
        project.renderFinal();

        cout << "Saving render result..." << outFile << endl;
        final.save(outFile);
        return 0;
    }

#ifdef OPENGL
    project.setPreviewImage(&preview);
    preview.enableDisplay();
    project.renderPreview();
    initGlut(argc, argv);
    glutMainLoop();
#endif
    
    return 0;
}
