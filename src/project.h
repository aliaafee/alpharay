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

#include "bitmap.h"

class Project {
    public:
        Scene scene;
        Renderer* renderer;
        Bitmap* preview;
        Bitmap* final;
        Vector2 previewSize;
        Vector2 finalSize;

        Project () { renderer = NULL; preview = NULL; final = NULL; }
        
        bool load(string filename);
        bool save(string filename);

        void setPreviewImage(Bitmap* image) 
            { preview = image; preview->create(previewSize.x, previewSize.y); }
        void setFinalImage(Bitmap* image) 
            { final = image; final->create(finalSize.x, finalSize.y); }

        void renderPreview(bool join = false) {
            if (renderer == NULL) return;
            if (preview == NULL) return;

            renderer->render(scene, preview, true);

            preview->correctExposure(renderer->exposure_);
        }

        void renderFinal() {
            if (renderer == NULL) return;
            if (final == NULL) return;

            renderer->render(scene, final, true);

            final->correctExposure(renderer->exposure_);


            /*
            cout << "Saving to " << filename << " ..." << endl;
            final->save(filename);
            cout << "Done" << endl;
            */
        }

    private:
        std::string pathBase(std::string path);

};



#endif // _PROJECT_H_
