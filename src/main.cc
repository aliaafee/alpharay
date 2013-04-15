#include <iostream>
#include <string>
#include <sstream>

#include <GL/freeglut.h>
#include <GL/glu.h>

#include <boost/timer.hpp>

using namespace std;

#include "project.h"

#include "bbox.h"

#include "octree.h"

Object *actor;
Vector *vActor;
Vector2 * vActor2;

Project project;


void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
    //glWindowPos2i(0, 0);
    project.preview->display();
    
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


void animate(int etc) {
    glutPostRedisplay();
    //project.scene.camera_.orbitZ(-1 * M_PI/90);
    if (actor != NULL) {
        //actor->rotation_.x += 0.05;
        actor->rotation_.z += 0.05;
        //actor->position_.z -= 0.01;
        //actor->position_.z += 0.01;
        //actor->scale_.x = 0.5;
    }

    if (vActor != NULL) {
        vActor->y += 0.01;
    }

    if (vActor2 != NULL) {
        vActor2->y += 0.005;
    }

    project.renderPreview();

    project.raytracer.statusOn = false;

    glutTimerFunc(10, animate, 0);
}


void initGlut(int argc, char** argv)
{
    glutInit(&argc, argv);
    
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    glutInitWindowSize(project.preview->width(), project.preview->height());

    glutCreateWindow("Raytracer A");
    
    glClearColor (0.0, 0.0, 0.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);
}

int test(Object *obj=NULL) {
    /*
    UVTriangle *intersectionUVTriangle;
	Vector intersectionPoint, intersectionPointLocal;
    float distance;

    Ray ray(Vector(0,0,0) , Vector(1,1,1));

    int count = 10000;
    boost::timer statusTime;
    cout << "doing " << count << " ints with " << obj->name_  << endl;
    for (int i = 0; i < count; i++) {
        obj->intersection(
                    ray,
                    &intersectionPoint,
                    &intersectionPointLocal, 
                    &intersectionUVTriangle,
                    &distance); 
    }
    cout << "completed in " << statusTime.elapsed() << " s" << endl;
    */
    /*
    BBox box(Vector(0,0,0), Vector(2,2,2));
    Ray ray(Vector(10,1,1) , Vector(-1,1,1));

    int count = 1000000000;
    boost::timer statusTime;
    bool result;
    float tmax = 0;
    float tmin = 0;
    for (int i = 0; i < count; i++) {
        result = box.intersection(ray.position_, ray.direction_); 
    }
    cout << "result " << result << " int at " << tmin << " & " << tmax << "from ray" << endl;
    cout << "completed in " << statusTime.elapsed() << " s" << endl;
    
    Octree w(Vector(0,0,0), Vector(2,2,2), 10, 10);

    w.add(NULL, 0);
    */

    Vertex v1( 0, Vector(0,0,10) , Vector(1,1,1) );
    Vertex v2( 1, Vector(0,1,10) , Vector(1,1,1) );
    Vertex v3( 2, Vector(1,0,10) , Vector(1,1,1) );

    Triangle trig( 0,&v1,&v2,&v3,NULL,NULL,NULL,1,Vector(0,0,0) );

    Vector min(0.5,0.5,0.5);
    Vector max(0.75,0.75,0.75);

    trig.inbounds( min, max );

}


int main(int argc, char** argv)
{
    //test(NULL); return 0;

    string filename = "";
    string outfile = "";

    if (argc > 1) {
        filename = argv[1];
    } else {
        cout << "No project file name given" << endl;
        return 0;
    }

    if (argc > 2) {
        outfile = argv[2];
    }

    cout << "Opening file: " << filename << endl;

    project.load(filename);

    //return 0;
    //test(project.scene.getObject("cube")); return 0;

    if (outfile != "") {
        //Render to file
        cout << endl;

        /*
        std::stringstream ss;
        actor = project.scene.getObject("cube");
        for (int i = 0; i < 10 ; i ++) {
            ss.str("");
            ss << outfile << i << ".png";
            project.renderFinal(ss.str());
            actor->rotation_.z += 0.62832;
        }*/

        project.renderFinal(outfile);
        return 0;
    }

    //project.renderPreview();

    //Display
    initGlut(argc, argv);

    actor = project.scene.getObject("cube");
    animate(0);
    
    glutMainLoop();

    return 0;
}

