/* vim: set ts=4 ss=4 sw=4 noet ai cindent : */

#ifndef _MESH_H_
#define _MESH_H_

#include <vector>
#include <fstream>
#include <sstream>
#include <chrono>

#include "object.h"
#include "vertex.h"
#include "triangle.h"
#include "octree.h"


class Mesh : virtual public Object, virtual public XmlObjectNamed
{
    public:
        void init();// { Object::init(); }

        Mesh(std::string name, BaseObject* parent=NULL) 
            : XmlObjectNamed ("mesh", name) 
            { init(); parent_ = parent; }

        ~Mesh() {};

        virtual void transform();

        virtual void genBounds();

        virtual Bounds bounds();

        virtual BaseObject* intersection(Ray &ray, float *t, float limit);

        virtual TiXmlElement* getXml();
        virtual bool loadXml(TiXmlElement* pElem, std::string path);

    protected:
		std::string filename_, absfilename_;
        Octree octree_;
		int octreeMaxDepth_; //Maximum depth of octree
		int octreeMaxTrig_; //Maximum number of triangles per left of octree

        std::vector<Vertex*>     vertexs;
        std::vector<Normal*>     normals;
        std::vector<MapPoint2*>  uvpoints;
        std::vector<Triangle*>   triangles;

        bool loadWavefrontObj(std::string filename, float scale, Vector position);
};

#endif // _MESH_H_
