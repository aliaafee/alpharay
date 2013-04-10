#include "project.h"

bool Project::load(string filename)
{
    TiXmlDocument doc(filename);

    bool result = doc.LoadFile();
    
    if (!result) {
        cout << "Failed to load file: " << filename << endl;
        return false;
    }
    
    TiXmlHandle hDoc(&doc);
    
    TiXmlElement* pElem;
    
    TiXmlHandle hRoot(0);
    
    //Root
    pElem = hDoc.FirstChildElement().Element();
    if (!pElem) {
        cout << "Failed" << endl;
        return false;
    }
    cout << "Loading Project..." << endl;
    hRoot = TiXmlHandle(pElem);
    
    //Raytracer
    raytracer.loadXml(hRoot);
    raytracer.statusOn = true;

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
    preview = new GLImage(width, height);
    width = 0;
    height= 0;
    pElem = hRoot.FirstChild("final").Element();
    if (pElem) {
        pElem->QueryIntAttribute("width", &width);
        pElem->QueryIntAttribute("height", &height);
    }
    if (width == 0 || height == 0) { width = 320; height = 240; }
    final = new CimgImage(width, height);

    //Scene
    scene.loadXml(hRoot);

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
    root->LinkEndChild(raytracer.getXml());

    // scene
    root->LinkEndChild(scene.getXml());

    doc.SaveFile(filename);
    return true;
}

