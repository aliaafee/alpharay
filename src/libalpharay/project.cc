/* vim: set ts=4 ss=4 sw=4 noet ai cindent : */

#include "project.h"


std::string Project::pathBase(std::string path) {
    int end = path.rfind("/");
    if (end == 0)
        return path;
    return path.substr(0, end);
}

/*
//void Project::renderPreview(std::function<void()> onDoneCallback) {
void Project::renderPreview() {
	std::cout << "hi" << std::endl;
    if (renderer == NULL) return;
    if (preview == NULL) return;
	if (renderer->rendering()) return;

	//onDonePreviewCallback_ = onDoneCallback;

	
    std::cout << "Preview " << preview->width() << "x" << preview->height() << std::endl;

	timerStart_ = std::chrono::high_resolution_clock::now();
    
	auto fp = std::bind(&Project::onDoneRenderPreview, this);
	
	renderer->render(scene, preview, fp, true);
}

void Project::onDoneRenderPreview() {
	std::cout << "Done Preview" << std::endl;
	
	//Post processing here
	//preview->bloom(30,0.5);
	preview->toneMap_exp(renderer->exposure_);
	
	std::cout << "Render time: " << durationDisplay(std::chrono::duration_cast<std::chrono::nanoseconds>( std::chrono::high_resolution_clock::now() - timerStart_ )) << std::endl;

	//onDonePreviewCallback_();
}


Color Project::renderPreviewPixel(int x, int y)
{
	Color result;

    if (renderer == NULL) return result;
    if (preview == NULL) return result;

	scene.setRayLog(true);

	std::cout << "Tracing pixel at (" << x << "," << y << ") on preview..." << std::endl;

	TIMER_START;

    result = renderer->renderPixel(scene, preview, x, y);

	std::cout << "Done. Result: " << result << ", duration" << TIMER_ELAPSED << std::endl;

	scene.setRayLog(false);

	return result;
}


void Project::renderFinal() {
    if (renderer == NULL) return;
    if (final == NULL) return;

    std::cout << "Final " << final->width() << "x" << final->height() << std::endl;

	timerStart_ = std::chrono::high_resolution_clock::now();

	auto fp = std::bind(&Project::onDoneRenderFinal, this);
    renderer->render(scene, final, fp, true);
}


void Project::onDoneRenderFinal() {
	std::cout << "Done Render Final" << std::endl;

	//Post processing here
	final->toneMap_exp(renderer->exposure_);
	
	std::cout << "Render time: " << durationDisplay(std::chrono::duration_cast<std::chrono::nanoseconds>( std::chrono::high_resolution_clock::now() - timerStart_ )) << std::endl;

	final->save(outFile_);
}
*/

void Project::render(Bitmap* bitmap)
{
	if (renderer == NULL) {
		std::cout << "No renderer set" << std::endl;
		return;
	}

	std::cout << "Rendering " << outWidth_ << "x" << outHeight_ << std::endl;

	std::cout << "Perparing bitmap..." << std::flush;
	std::cout << "Done" << std::endl;

	bitmap->create(outWidth_, outHeight_);

	renderer->render(scene, bitmap);

	//Post processing.
	bitmap->toneMap_exp(renderer->exposure_);
}


bool Project::load(string filename)
{
    TiXmlDocument doc(filename);

    string filedir = pathBase(filename);

    bool result;
	
	result = doc.LoadFile();
    if (!result) {
		std::cout << "bad file" << filename << std::endl;
        return false;
    }
    
    TiXmlHandle hDoc(&doc);
    
    TiXmlElement* pElem;
    
    TiXmlHandle hRoot(0);
    
    //Root
    pElem = hDoc.FirstChildElement().Element();
    if (!pElem) {
        return false;
    }

    if (std::string(pElem->Value()) != "project") {
        return false;
    }

    cout << "Loading Project \"" << filename << "\" ..." << endl;
    hRoot = TiXmlHandle(pElem);
    
    //Renderer
    pElem = hRoot.FirstChild("renderer").Element();
    if (pElem) {
        std::string type = "";
        pElem->QueryStringAttribute("type", &type);
        if (type == "pathtracer") {
            renderer = new Pathtracer();
        } else {
            renderer = new Raytracer();
        }
        renderer->loadXml(pElem, filedir);
    } else {
        renderer = new Raytracer();
    }
	
	/*
    //Preview and final
    int width, height;
    width = 320;
    height= 240;
    pElem = hRoot.FirstChild("preview").Element();
    if (pElem) {
        pElem->QueryIntAttribute("width", &width);
        pElem->QueryIntAttribute("height", &height);
    }
    if (width == 0 || height == 0) { width = 320; height = 240; }
    //preview = new GLImage(width, height);
    previewSize = Vector2(width, height);
    width = 320;
    height= 240;
    pElem = hRoot.FirstChild("final").Element();
    if (pElem) {
        pElem->QueryIntAttribute("width", &width);
        pElem->QueryIntAttribute("height", &height);
    }
    if (width == 0 || height == 0) { width = 320; height = 240; }
    //final = new CimgImage(width, height);
    finalSize = Vector2(width, height);
	*/

	pElem = hRoot.FirstChild("preview").Element();
	if (pElem) {
        pElem->QueryIntAttribute("width", &outWidth_);
        pElem->QueryIntAttribute("height", &outHeight_);
    }
	if (outWidth_ == 0 || outHeight_ == 0) { outWidth_ = 320; outHeight_ = 240; }

	pElem = hRoot.FirstChild("output").Element();
	if (pElem) {
        pElem->QueryIntAttribute("width", &outWidth_);
        pElem->QueryIntAttribute("height", &outHeight_);
    }
	if (outWidth_ == 0 || outHeight_ == 0) { outWidth_ = 320; outHeight_ = 240; }

    //Scene
    pElem = hRoot.FirstChild("scene").Element();
    
    result = scene.loadXml(pElem, filedir);
	if (!result) {
		return false;
	}

    cout << "Done" << endl;
    return true;
}


bool Project::save(string filename)
{
    string m_name="MyTest";

    TiXmlDocument doc;
    TiXmlDeclaration* decl = new TiXmlDeclaration( "1.0", "", "");
    doc.LinkEndChild(decl);

    //Root
    TiXmlElement* root = new TiXmlElement("project");
    doc.LinkEndChild(root);

    // Raytracer settings
	if (renderer != NULL) {
		root->LinkEndChild(renderer->getXml());
	}

    // scene
    root->LinkEndChild(scene.getXml());

    doc.SaveFile(filename);
    return true;
}
