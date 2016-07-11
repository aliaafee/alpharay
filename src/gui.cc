/* vim: set ts=4 ss=4 sw=4 noet ai cindent : */

#include "gui.h"


//Tiny File Dialogue 
char const * saveImagePatters[1] = { "*.*" };

Project *guiProject;
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
		guiProject->renderPreviewPixel(x,y);
	}
	glutPostRedisplay();
}


void keyboard(unsigned char key, int x, int y)
{
	std::cout << "Pressed " << int(key) << std::endl;
	if (key == 27)
		exit(0);

	if (key == 13) {
		//guiProject->renderPreview(&onDoneRender);
	}
	glutPostRedisplay();
}


void keyboardSpecial(int key, int x, int y)
{
	std::cout << "Pressed " << int(key) << std::endl;

	/*
	Object* obj = guiProject->scene.getObject("two");

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

	guiProject->renderPreview();
	*/

}


void idle(void)
{	
	if (guiProject->renderer->rendering() == false) {
		btnRender->enable();
		btnCancelRender->disable();
		lblStatus->set_text(guiProject->renderer->status().c_str());
	} else {
		btnRender->disable();
		btnCancelRender->enable();
		lblStatus->set_text(guiProject->renderer->status().c_str());

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
			guiProject->renderPreview(&onDoneRender);
			break;
		case GLUI_CMD_CANCEL_RENDER:
			guiProject->renderer->cancel();
			break;
		case GLUI_CMD_SAVE:
			std::string out = txtOutFile->get_text();
			guiProject->preview->save(out);
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
				std::string out = saveFileName;
				guiProject->preview->save(out);
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


void guiMain(Project* prj, int argc, char** argv) {
	guiProject = prj;
	guiProject->setPreviewImage(&preview);
    guiProject->renderPreview(&onDoneRender);
    initGlut(argc, argv);
    glutMainLoop();
}

