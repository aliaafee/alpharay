#ifndef _PROJECT_H_
#define _PROJECT_H_

#include <tinyxml.h>
#include <string>
#include <iostream>

using namespace std;

#include "scene.h"
#include "raytracer.h"

#include "gl-image.h"
#include "cimg-image.h"
#include "float-image.h"

class Project {
    public:
        Raytracer raytracer;
        Scene scene;
        GLImage* preview;
        CimgImage* final;

        Project () {};
        
        bool load(string filename);
        bool save(string filename);

        void renderPreview() {
            /*
            FloatImage temp(preview->width(), preview->height());
            raytracer.renderThreaded(scene, temp);
            temp.glow(10);
            temp.copyTo(preview);
            */
            raytracer.renderThreaded(scene, *preview);
            //raytracer.render(scene, *preview);
        }

        void renderFinal(string filename) {
            raytracer.renderThreaded(scene, *final);
            cout << endl;
            cout << "Saving to " << filename << " ..." << endl;
            final->save(filename);
            cout << "Done" << endl;
        }
};



#endif // _PROJECT_H_
