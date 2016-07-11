/* vim: set ts=4 ss=4 sw=4 noet ai cindent : */

#include "renderer.h"


void Renderer::init()
{
    traceDepth_ = 10; 
        
    exposure_ = -1.0f;

    subSamplesX_ = 4;
    subSamplesY_ = 4;

    statusOn_ = true;

    threadCount_ = 8;

    cellCx = 4;
    cellCy = 4;

	rendering_ = false;
}


BaseObject* Renderer::closestIntersection(Scene &scene, Ray &ray, float *closest) 
{
	float distance;
	*closest = BIG_NUM;
	
	BaseObject *currentObject = NULL;
	BaseObject *closestObject = NULL;
	
	for (unsigned long i=0; i < scene.objects.size(); i++) {
			currentObject = scene.objects[i]->intersection(ray, &distance, BIG_NUM);   

			if (currentObject != NULL) {
                if (distance > SMALL_NUM) {
				    if (distance < *closest) {
                        closestObject = currentObject;
                        *closest = distance;
				    }
                }
			}
	}
	return closestObject;
}


void Renderer::getFresnelValues(Vector Vincident, Vector normal,
                        float n1, float n2,
                        float cosT1, 
                        Vector *Vreflect, Vector *Vrefract,
                        float *reflectionCoeff, float *transmissionCoeff )
{
    //cosT1 = normal * (Vincident*-1);

    float cosT2 = sqrt(1 - ( pow(n1/n2, 2) * (1 - pow(cosT1,2)) ) );

    *Vreflect = Vincident + (normal * (2 * cosT1));
    
    if (cosT1 > 0) {
        *Vrefract = ( Vincident * (n1/n2) ) + ( normal * ( (n1/n2 * cosT1) - cosT2) );
    } else {
        *Vrefract = ( Vincident * (n1/n2) ) + ( normal * ( (n1/n2 * cosT1) + cosT2) );
    }

    float T1 = acos(cosT1);
    float T2 = acos(cosT2);

    //float rI = tan(T1 - T2) / tan(T1 + T2);
    //float rT = sin(T1 - T2) / sin(T1 + T2);

    //float sinT2 = sin(T2);
    //float sinT1pT2 = sin(T1 + T2);

    //float tI = (2 * sinT2 * cosT1) / ( sinT1pT2 * cos(T1 - T2) );
    //float tT = (2 * sinT2 * cosT1) / ( sinT1pT2 );

    //Parallel case
    /*float rI = tan(T1 - T2) / tan(T1 + T2);
    *reflectionCoeff   = rI * rI;
    *transmissionCoeff = 1 - *reflectionCoeff;
    */

    float rI = tan(T1 - T2) / tan(T1 + T2);
    float rT = sin(T1 - T2) / sin(T1 + T2);
    float r = (rI + rT)/2;
    *reflectionCoeff   = r * r;
    *transmissionCoeff = 1 - *reflectionCoeff;

}


Color Renderer::trace(Scene &scene ,Ray ray, int depth)
{
    return Color(randf(0,1), randf(0,1), randf(0,1));
}

/*
void Renderer::correctExposure(Color &color) {
    if (exposure_ != 0) {
        color.x = 1.0f - expf(color.x * exposure_);
        color.y = 1.0f - expf(color.y * exposure_);
        color.z = 1.0f - expf(color.z * exposure_);
    }
}
*/

void Renderer::renderCell
            (Scene &scene, Image *image, int x0, int y0, int x1, int y1)
{
    //if (x0 < 1) return;
    //if (y0 < 1) return;
    //if (x1 > image->width()) return;
    //if (y1 > image->height()) return;

	if (cancel_ == true) {
		completedCells += 1;
		return;
	}

    float total = (y1-y0) * (x1-x0), width = x1-x0 + 1;

    float tsx = float(subSamplesX_), tsy = float(subSamplesY_);
    float ts = tsx * tsy;

    Vector2 point;
	    
	for (point.y = y0; point.y <= y1; point.y++) {
		for (point.x = x0; point.x <= x1; point.x++) {
            Color color;
            for (float ssx=0; ssx < tsx; ssx++) {
                float sx = point.x + (ssx/tsx);
                for (float ssy=0; ssy < tsy; ssy++) {
                    float sy = point.y + (ssy/tsy);
                    
                    color += trace(scene, scene.ray(sx,sy), 0);
                }
            }
            
            color /= ts;
            image->setColor(point, color);
			if (cancel_ == true) {
				completedCells += 1;
				return;
			}
		}
	}

	completedCells += 1;
}


bool Renderer::getNextCell(int &x0, int &y0, int &x1, int &y1, int width, int height)
{
    if (currentCell >= cells_.size()) return false;

	x0 = cells_[currentCell].x0;
	y0 = cells_[currentCell].y0;

	x1 = cells_[currentCell].x1;
	y1 = cells_[currentCell].y1;

	/*
    int cell = curCell;

    int ix = cell % cellCx;
    int iy = (cell - ix) / cellCx;

	x0 = cellW * ix;
	y0 = cellH * iy;

	if (ix == cellCx-1) {
		x1 = width - 1;
	} else {
		x1 = x0 + cellW;
	}

	if (iy == cellCy-1) {
		y1 = height - 1;
	} else {
		y1 = y0 + cellH;
	}
	*/

    currentCell ++;
    return true;
}


bool Renderer::renderAllCells(Scene& scene, Image* image)
{
    int x0, y0, x1, y1;

    while (getNextCell(x0, y0, x1, y1, image->width(), image->height())) {
        renderCell(scene, image, x0, y0, x1, y1);
    } 

    return true;
}


bool Renderer::statusDisplay()
{
    std::stringstream ss;
    float done = 0.0f;
    float pdone = -1.0f;

    std::cout << "Rendering..." << std::endl;
    std::cout << " " ;

    ss << "0%";
    std::cout << ss.str();
    while (completedCells < cells_.size()) {
        for (unsigned long w = 0; w < ss.str().size(); w ++) { std::cout << "\b";}
        ss.str("");
        ss << int(float(completedCells) / float(cells_.size()) * 100.0) << "\% done, " << raysCast_ << " rays cast";
        std::cout << ss.str() << std::flush;
		status_ = ss.str();
        usleep(10);
    }

    for (unsigned long w = 0; w < ss.str().size(); w ++) { std::cout << "\b";}

	ss.str("");
	ss << raysCast_ << " rays cast";
	status_ = ss.str();
    std::cout << status_ << std::endl;
    std::cout << "Done" << std::endl;
	
	rendering_ = false;

	onDoneCallback_();
	
	return true;
}


void Renderer::resetCells(Image* image)
{
	currentCell = 0;
	completedCells = 0;

	int width = image->width();
	int height = image->height();
    int maxCell = cellCx * cellCy;
    int cellW = width / cellCx;
    int cellH = height / cellCy;
    
	cells_.clear();

	int x0, y0, x1, y1;
	for (int i = 0; i < maxCell; i++) {
		int ix = i % cellCx;
		int iy = (i - ix) / cellCx;

		x0 = cellW * ix;
		y0 = cellH * iy;

		if (ix == cellCx-1) {
			x1 = width - 1;
		} else {
			x1 = x0 + cellW;
		}

		if (iy == cellCy-1) {
			y1 = height - 1;
		} else {
			y1 = y0 + cellH;
		}

		cells_.push_back(RendererCell(x0, y0, x1, y1));
	}

	srand(unsigned(time(NULL)));
	std::random_shuffle(cells_.begin(), cells_.end());
}


void Renderer::render (Scene& scene, Image* image, std::function<void()> onDoneCallback) {
	render(scene, image, onDoneCallback, false);
}


void Renderer::render (Scene& scene, Image* image, std::function<void()> onDoneCallback, bool join)
{
	if (rendering_) {
		std::cout << "We are already rendering, cannot start" << std::endl;
		return;
	}

	rendering_ = true;
	cancel_ = false;
	
	onDoneCallback_ = onDoneCallback;
	
    //Setup the scene for render
    scene.transform();
    scene.setScreen(image->width(), image->height());

    raysCast_ = 0;
    
    resetCells(image);

#ifdef THREADING
    boost::thread *renderThread = new boost::thread[threadCount_];
    for (int i = 0; i < threadCount_; i++) {
        renderThread[i] = boost::thread(
                &Renderer::renderAllCells, this, scene, image);
    }

    boost::thread status = boost::thread( &Renderer::statusDisplay, this );

	if (!join) return;

    for (int i = 0; i < threadCount_; i++) {
        renderThread[i].join();
    }
    //completed = (cellW * cellH);

    status.join();
#else
    std::cout << "Rendering..." << std::flush;

    renderAllCells(scene, image);

    std::cout << "Done" << std::endl;

	rendering_ = false;
	onDoneCallback_();
#endif 
            
}


void Renderer::cancel() {
	cancel_ = true;
}


void Renderer::toneMap_exp(Color* color) {
	color->x = 1.0f - expf( color->x * exposure_);
	color->y = 1.0f - expf( color->y * exposure_);
	color->z = 1.0f - expf( color->z * exposure_);
}


void Renderer::renderST (Scene &scene, Image *image)
{
    //Setup the scene for render
    scene.transform();
    scene.setScreen(image->width(), image->height());
    
    raysCast_ = 0;
    
    resetCells(image);

    std::cout << "Rendering..." << std::flush;

    renderAllCells(scene, image);

    std::cout << "Done" << std::endl;
}


Color Renderer::renderPixel (Scene& scene, Image* image, int x, int y)
{
	//Setup the scene for render
    scene.transform();
    scene.setScreen(image->width(), image->height());

	float sx,sy;

	sx = x;
	sy = y;

	return trace(scene, scene.ray(sx,sy), 0);
}


bool Renderer::loadXml(TiXmlElement* pElem, std::string path) 
{
    init();

    XmlObject::loadXml(pElem, path);

    pElem->QueryIntAttribute("reflectiondepth", &traceDepth_);
    pElem->QueryFloatAttribute("exposure", &exposure_);
    pElem->QueryIntAttribute("subsamplesx", &subSamplesX_);
    pElem->QueryIntAttribute("subsamplesy", &subSamplesY_);
    pElem->QueryIntAttribute("subcellsx", &cellCx);
    pElem->QueryIntAttribute("subcellsy", &cellCy);

    return true;
}


TiXmlElement* Renderer::getXml() 
{
    TiXmlElement* root = XmlObject::getXml();

    root->SetAttribute("reflectiondepth", traceDepth_);
    root->SetDoubleAttribute("exposure", exposure_);
    root->SetAttribute("subsamplesx", subSamplesX_);
    root->SetAttribute("subsamplesy", subSamplesY_);
    root->SetAttribute("subcellsx", cellCx);
    root->SetAttribute("subcellsy", cellCy);


    return root;
}

