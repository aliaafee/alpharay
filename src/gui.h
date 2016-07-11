/* vim: set ts=4 ss=4 sw=4 noet ai cindent : */

#ifndef _GUI_H_
#define _GUI_H_

#include <GL/freeglut.h>
#include <GL/glu.h>
#include "glui.h"
#include "tinyfiledialogs.h"
#include "gl-image.h"

#include "project.h"
#include "bitmap.h"

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


void display(void);
void reshape(int width, int height);
void mouse(int button, int state, int x, int y);
void keyboard(unsigned char key, int x, int y);
void keyboardSpecial(int key, int x, int y);
void idle(void);
void onDoneRender();
void gluiCommand(int ID);
void menu(int ID);
void initGlut(int argc, char** argv);
void guiMain(Project *prj, int argc, char** argv);


#endif // _GUI_H_
