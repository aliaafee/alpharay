/* vim: set ts=4 ss=4 sw=4 noet ai cindent : */

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

        void setPreviewImage(Bitmap* image) 
            { preview = image; preview->create(previewSize.x, previewSize.y); }
        void setFinalImage(Bitmap* image) 
            { final = image; final->create(finalSize.x, finalSize.y); }

        void renderPreview();
        void renderFinal();
        
        bool load(string filename);
        bool save(string filename);

    private:
        std::string pathBase(std::string path);

};



#endif // _PROJECT_H_
