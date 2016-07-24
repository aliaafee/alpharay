/* vim: set ts=4 ss=4 sw=4 noet ai cindent : */

#include "project.h"


std::string Project::pathBase(std::string path) {
    int end = path.rfind("/");
    if (end == 0)
        return path;
    return path.substr(0, end);
}


void Project::render(Bitmap* bitmap, RenderStatus* renderStatus)
{
	if (renderer == NULL) {
		std::cout << "No renderer set" << std::endl;
		return;
	}

	//Prepare the bitmap
	std::cout << "Perparing bitmap " << renderer->outWidth() << "x" << renderer->outWidth() << "..." << std::flush;
	bitmap->create(renderer->outWidth(), renderer->outHeight());
	std::cout << "Done" << std::endl;

	
	TIMER_START;

	std::cout << "Rendering.." << std::endl;
	renderer->render(&scene, bitmap, renderStatus);
	std::cout << "Done" << std::endl;

	//Post processing.
	bitmap->toneMap_exp(renderer->exposure_);

	if(renderStatus != NULL) {
		renderStatus->renderTime_ = TIMER_ELAPSED_NS;
		std::cout << "Render time: " << TIMER_ELAPSED << std::endl;
	} else {
		std::cout << "Render time: " << TIMER_ELAPSED << std::endl;
	}
}


TiXmlElement* Project::getXml() {
    TiXmlElement* root = XmlObject::getXml();

	// Raytracer settings
	if (renderer != NULL) {
		root->LinkEndChild(renderer->getXml());
	}

    // scene
    root->LinkEndChild(scene.getXml());

	return root;
}


bool Project::loadXml(TiXmlElement* pElem, std::string path) {
	XmlObject::loadXml(pElem, path);

	TiXmlHandle hRoot = TiXmlHandle(pElem);

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
        renderer->loadXml(pElem, path);
    } else {
        renderer = new Raytracer();
    }

	//Scene
    pElem = hRoot.FirstChild("scene").Element();

	bool result = scene.loadXml(pElem, path);
	if (!result) {
		return false;
	}

	cout << "Done" << endl;

	return true;
}


bool Project::load(string filename)
{
	cout << "Loading Project \"" << filename << "\" ..." << endl;
	
    TiXmlDocument doc(filename);

    string path = pathBase(filename);

    bool result = doc.LoadFile();
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

	return loadXml(pElem, path);
}


bool Project::save(string filename)
{
    TiXmlDocument doc;
    TiXmlDeclaration* decl = new TiXmlDeclaration( "1.0", "", "");
    doc.LinkEndChild(decl);

    //Root
    TiXmlElement* root = getXml();//new TiXmlElement("project");
    doc.LinkEndChild(root);

    doc.SaveFile(filename);
    return true;
}

