/* vim: set ts=4 ss=4 sw=4 noet ai cindent : */

#ifndef _PROJECT_H_
#define _PROJECT_H_

#include <tinyxml.h>
#include <string>
#include <iostream>
#include <functional>
//using namespace std::placeholders;

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
        Bitmap* out;
		int outWidth_;
		int outHeight_;
        //Bitmap* final;
        //Vector2 previewSize;
        //Vector2 finalSize;

        Project () { renderer = NULL; outWidth_ = 320; outHeight_ = 240; }

		/*

        void setPreviewImage(Bitmap* image) 
            { preview = image; std::cout << previewSize << std::endl; preview->create(previewSize.x, previewSize.y); }
        void setFinalImage(Bitmap* image) 
            { final = image; final->create(finalSize.x, finalSize.y); }

        //void renderPreview(std::function<void()> onDoneCallback);
		void renderPreview();
		
		void onDoneRenderPreview();

		Color renderPreviewPixel(int x, int y);

		void setOutFile(std::string filename) { outFile_ = filename; };
        void renderFinal();
		void onDoneRenderFinal();
		*/

		//void setOutBitmap(Bitmap* bitmap);

		void render(Bitmap* bitmap);
        
        bool load(string filename);
        bool save(string filename);

		int outWidth() 
			{ return outWidth_; }
		int outHeight()
			{ return outHeight_; }

		void cancelLoad() 
			{ scene.cancelLoad(); }
    private:
        std::string pathBase(std::string path);

		std::chrono::high_resolution_clock::time_point timerStart_;

		std::string outFile_;

		std::function<void()> onDonePreviewCallback_;
};



#endif // _PROJECT_H_
