/* vim: set ts=4 ss=4 sw=4 noet ai cindent : */

#include <iostream>
#include <string>
#include <sstream>
#include <unistd.h>

#include "project.h"
#include "bitmap.h"

using namespace std;

Project project;
Bitmap final("");

string projectFile, outFile, projectSaveFile;

#ifdef OPENGL

#include <GL/freeglut.h>
#include <GL/glu.h>
#include "glui.h"
#include "tinyfiledialogs.h"
#include "gl-image.h"

//GLUI Command IDs
#define GLUI_CMD_RENDER 1
#define GLUI_CMD_CANCEL_RENDER 2
#define GLUI_CMD_SAVE 3

//Main Window Menu
enum MENU_TYPE
{
	MENU_SAVE_RENDER,
	MENU_EDIT_MATERIAL
};

//Tiny File Dialogue 
char const * saveImagePatters[1] = { "*.*" };


GLImage preview("");

int mainWindow;

int toolWindow;
GLUI_Button* btnRender;
GLUI_Button* btnCancelRender;
GLUI_StaticText* lblStatus;
GLUI_EditText* txtOutFile;

void display(void)
{	
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	preview.display();
		
	/*
	//Image overlay
	glLineWidth(1); 
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_LINES);
	glVertex2f(0.0, 0.0);
	glVertex2f(10, 10);
	glEnd();
	*/
	glutSwapBuffers();
}


void reshape(int width, int height)
{
    //glViewport(0, 0, width, height);
	//glMatrixMode (GL_PROJECTION);
	//glLoadIdentity ();
	//gluOrtho2D (0.0, width, 0.0, height);
	
	//int vx, vy, vw, vh;

	//GLUI_Master.get_viewport_area(&vx, &vy, &vw, &vh);

	//glViewport(vx,vy,vw,vh);

	glutPostRedisplay();
	//preview.postRedisplay();

	//std::cout<<vx <<","<<vy<<","<<vw<<","<<vh << std::endl;

	//GLUI_Master.auto_set_viewport();
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
		//project.renderPreview(&onDoneRender);
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
	if (project.renderer->rendering() == false) {
		btnRender->enable();
		btnCancelRender->disable();
		lblStatus->set_text(project.renderer->status().c_str());
	} else {
		btnRender->disable();
		btnCancelRender->enable();
		lblStatus->set_text(project.renderer->status().c_str());

	}

	glutSetWindow(mainWindow);
	glutPostRedisplay();

	usleep(100);
}


void onDoneRender()
{
	cout << "Done Render" << endl;
}


void gluiCommand(int ID)
{
	switch (ID) {
		case GLUI_CMD_RENDER:
			project.renderPreview(&onDoneRender);
			break;
		case GLUI_CMD_CANCEL_RENDER:
			project.renderer->cancel();
			break;
		case GLUI_CMD_SAVE:
			outFile = txtOutFile->get_text();
			project.preview->save(outFile);
			break;
	}
	
}



void menu(int ID)
{
	switch (ID) {
		case MENU_SAVE_RENDER:
			char const *saveFileName;
			saveFileName = tinyfd_saveFileDialog(
				"Save Image",
				"render.jpg",
				2,
				saveImagePatters,
				NULL);
			if (!saveFileName) {
				cout << "Bad Save Filename" << endl;
			} else {
				outFile = saveFileName;
				project.preview->save(outFile);
			}
			break;
		case MENU_EDIT_MATERIAL:
			break;
	}
}


void initGlut(int argc, char** argv)
{
	glutInit(&argc, argv);
	
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    
    glutInitWindowSize(preview.width(), preview.height());

    mainWindow = glutCreateWindow("Render Result");
    
    glClearColor (0.0, 0.0, 0.0, 0.0);
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	glOrtho(0.0, preview.width(),preview.height(),0.0, -1.0, 1.0);
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
	glutIdleFunc(idle);
	//glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(keyboardSpecial);

	glutCreateMenu(menu);
	glutAddMenuEntry("Save Image", MENU_SAVE_RENDER);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	//GLUI 
	GLUI *glui = GLUI_Master.create_glui( "GLUI", 0 , preview.width(), 0);
	//GLUI *glui = GLUI_Master.create_glui_subwindow(mainWindow, GLUI_SUBWINDOW_LEFT);
	lblStatus = glui->add_statictext("");
	GLUI_Panel *render_panel = glui->add_panel( "Render" );
	btnRender = glui->add_button_to_panel(render_panel, "Render", GLUI_CMD_RENDER, gluiCommand);
	btnCancelRender = glui->add_button_to_panel(render_panel, "Cancel", GLUI_CMD_CANCEL_RENDER, gluiCommand);
	//glui->add_separator_to_panel(render_panel);
	//txtOutFile = glui->add_edittext_to_panel(render_panel, "Filename");
	//glui->add_button_to_panel(render_panel, "Save", GLUI_CMD_SAVE, gluiCommand);

	glui->set_main_gfx_window( mainWindow );
	GLUI_Master.set_glutIdleFunc(idle);
}

#endif

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
    project.setPreviewImage(&preview);
    //preview.enableDisplay();
    project.renderPreview(&onDoneRender);
    initGlut(argc, argv);
    glutMainLoop();
#endif
    
    return 0;
}
