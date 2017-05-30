/* vim: set ts=4 ss=4 sw=4 noet ai cindent : */

#ifndef _RENDERER_H_
#define _RENDERER_H_

#ifdef THREADING
#include <boost/thread.hpp>
#endif

#include <functional>
#include <vector>
#include <algorithm>
#include <unistd.h>

#include "xmlobject.h"
#include "scene.h"
#include "bitmap.h"


class RendererCell {
	public:
	int x0, y0, x1, y1;
	RendererCell(int x0_, int y0_, int x1_, int y1_) {
		x0 = x0_; y0 = y0_;
		x1 = x1_; y1 = y1_;
	}
};


class RenderStatus {
	friend class Renderer;
	friend class Project;
	public:
		int progress()
			{ return progress_; }
		int raysCast()
			{ return raysCast_; }
		std::string message()
			{ return message_; }
		long renderTime()
			{ return renderTime_; }
	protected:
		int progress_;
		int raysCast_;
		std::string message_;
		long renderTime_;
};


class Renderer : public XmlObject
{
    public:
        virtual void init();
        
        Renderer () 
            : XmlObject("renderer") 
            { init(); }

		virtual void render (Scene* scene, Bitmap* bitmap, RenderStatus* renderStatus=NULL);
		Color renderPixel (Scene* scene, Bitmap* bitmap, int x, int y);
        virtual void renderST (Scene* scene, Bitmap *bitmap);

        //virtual TiXmlElement* getXml();
        //virtual bool loadXml(TiXmlElement* pElem, std::string path);

        float exposure_;
		
		std::string status() 
			{ return status_;}

		void cancel();

		int outWidth() 
			{ return outWidth_; }
		int outHeight()
			{ return outHeight_; }

	
		int threadCount_;
		int cellCx, cellCy;
		int subSamplesX_;
        int subSamplesY_;
		int traceDepth_;
		int outWidth_;
		int outHeight_;
	protected:
		
        bool statusOn_;

        unsigned long raysCast_;

        virtual Color trace(Scene* scene ,Ray ray, int depth);

        BaseObject* closestIntersection(Scene* scene, Ray &ray, float *closest);
        
        void getFresnelValues(Vector Vincident, Vector normal,
                            float n1, float n2,
                            float cosT1, 
                            Vector *Vreflect, Vector *Vrefract,
                            float *reflectionCoeff, float *transmissionCoeff );

        void toneMap_exp(Color *color);
    private:
		RenderStatus* renderStatus_;
		bool cancel_;
		bool running_;

		std::vector<RendererCell> cells_;

		int currentCell;
        int completedCells;

		std::string status_;

        bool statusDisplay();

		bool renderAllCells(Scene* scene, Bitmap* bitmap);
		virtual void renderCell(Scene* scene, Bitmap *bitmap, int x0, int y0, int x1, int y1);
        bool getNextCell(int &x0, int &y0, int &x1, int &y1, int width, int height);
        void resetCells(Bitmap* bitmap);
};


#endif // _RENDERER_H_
