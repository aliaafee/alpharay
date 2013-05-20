/* vim: set ts=4 ss=4 sw=4 noet ai cindent : */

#include "project.h"


std::string Project::pathBase(std::string path) {
    int end = path.rfind("/");
    if (end == 0)
        return path;
    return path.substr(0, end);
}


void Project::renderPreview() {
    if (renderer == NULL) return;
    if (preview == NULL) return;

    std::cout << "Preview " << preview->width() << "x" << preview->height() << std::endl;

    renderer->render(scene, preview, true);

    preview->bloom(20, 150);
    preview->toneMap_exp(renderer->exposure_);
}


void Project::renderFinal() {
    if (renderer == NULL) return;
    if (final == NULL) return;

    std::cout << "Final " << final->width() << "x" << final->height() << std::endl;

    renderer->render(scene, final, true);

    final->bloom(10, 150);
    final->toneMap_exp(renderer->exposure_);
}


bool Project::load(string filename)
{
    TiXmlDocument doc(filename);

    string filedir = pathBase(filename);

    bool result = doc.LoadFile();
    
    if (!result) {
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

    cout << "Loading Project..." << endl;
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

    //Preview and final
    int width, height;
    width = 0;
    height= 0;
    pElem = hRoot.FirstChild("preview").Element();
    if (pElem) {
        pElem->QueryIntAttribute("width", &width);
        pElem->QueryIntAttribute("height", &height);
    }
    if (width == 0 || height == 0) { width = 320; height = 240; }
    //preview = new GLImage(width, height);
    previewSize = Vector2(width, height);
    width = 0;
    height= 0;
    pElem = hRoot.FirstChild("final").Element();
    if (pElem) {
        pElem->QueryIntAttribute("width", &width);
        pElem->QueryIntAttribute("height", &height);
    }
    if (width == 0 || height == 0) { width = 320; height = 240; }
    //final = new CimgImage(width, height);
    finalSize = Vector2(width, height);

    //Scene
    pElem = hRoot.FirstChild("scene").Element();
    
    scene.loadXml(pElem, filedir);

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
    root->LinkEndChild(renderer->getXml());

    // scene
    root->LinkEndChild(scene.getXml());

    doc.SaveFile(filename);
    return true;
}

