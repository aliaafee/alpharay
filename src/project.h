#ifndef _PROJECT_H_
#define _PROJECT_H_

#include <tinyxml.h>
#include <string>
#include <iostream>

using namespace std;

#include "scene.h"

#include "renderer.h"

#include "raytracer.h"
#include "pathtracer.h"

#include "gl-image.h"
#include "cimg-image.h"
#include "float-image.h"

class Project {
    public:
        Renderer* renderer;
        Scene scene;
        GLImage* preview;
        CimgImage* final;

        Project () { renderer = NULL; }
        
        bool load(string filename);
        bool save(string filename);

        void renderPreview(bool join = false) {
            if (renderer == NULL) return;
            /*
            FloatImage temp(preview->width(), preview->height());
            raytracer.renderThreaded(scene, temp);
            temp.glow(10);
            temp.copyTo(preview);
            */
            renderer->render(scene, preview, join);
            //raytracer.render(scene, *preview);
        }

        void renderFinal(string filename) {
            if (renderer == NULL) return;
            renderer->render(scene, final, true);
            cout << endl;
            cout << "Saving to " << filename << " ..." << endl;
            final->save(filename);
            cout << "Done" << endl;
        }

    private:
        std::string pathBase(std::string path);
};



#endif // _PROJECT_H_
