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

#include "image.h"

class Project {
    public:
        Scene scene;
        Renderer* renderer;
        Image* preview;
        Image* final;
        Vector2 previewSize;
        Vector2 finalSize;

        Project () { renderer = NULL; preview = NULL; final = NULL; }
        
        bool load(string filename);
        bool save(string filename);

        void setPreviewImage(Image* image) 
            { preview = image; preview->create(previewSize.x, previewSize.y); }
        void setFinalImage(Image* image) 
            { final = image; final->create(finalSize.x, finalSize.y); }

        void renderPreview(bool join = false) {
            if (renderer == NULL) return;
            if (preview == NULL) return;

            renderer->render(scene, preview, join);
        }

        void renderFinal(string filename) {
            if (renderer == NULL) return;
            if (final == NULL) return;

            renderer->render(scene, final, true);

            cout << "Saving to " << filename << " ..." << endl;
            final->save(filename);
            cout << "Done" << endl;
        }

    private:
        std::string pathBase(std::string path);

};



#endif // _PROJECT_H_
