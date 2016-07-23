/* vim: set ts=4 ss=4 sw=4 noet ai cindent : */

#include "mesh.h"


void Mesh::init() { 
	addEditable(new Editable<int>("octree-depth", &octreeMaxDepth_, 10));
	addEditable(new Editable<int>("octree-treshold", &octreeMaxTrig_, 100));
}


void Mesh::transform()
{
    Object::transform();

    for (unsigned long i=0; i < triangles.size(); i++) {
        triangles[i]->transform();
    }

	//Calculate the tangents and bitangents for each vertex, store it in each vertex
	if (material_ == NULL) { return; }

	if (material_->normalMap_ == NULL) { return; }
	
	if (uvpoints.size() > 0) {
		for (unsigned long vi=0; vi < vertexs.size(); vi++) {
			Vector total_t;
			Vector total_b;
			float count = 0;
			for (unsigned long ti=0; ti < vertexs[vi]->triangleIndexes.size(); ti++) {
				unsigned long tti = vertexs[vi]->triangleIndexes[ti];
				total_t += triangles[tti]->face_t;
				total_b += triangles[tti]->face_b;
				count += 1.0;
			}
			vertexs[vi]->t = total_t / count;
			vertexs[vi]->b = total_b / count;
		}
	}
}


void Mesh::genBounds()
{
    Bounds b = bounds();

    std::cout << "   generating octree with bounds" << b.min << "to" << b.max << std::endl;

    unsigned long leafCount = 0;

	TIMER_START;
    octree_ = Octree(b.min, b.max, octreeMaxDepth_, octreeMaxTrig_);
    
    octree_.add(&triangles, 0, &leafCount);

    std::cout << "     leaves " << leafCount << ", generated in " << TIMER_ELAPSED << std::endl;
}


Bounds Mesh::bounds()
{
    Vector max(-BIG_NUM, -BIG_NUM, -BIG_NUM);
    Vector min( BIG_NUM,  BIG_NUM,  BIG_NUM);

    for (unsigned long i=0; i < vertexs.size(); i++) {
        if (vertexs[i]->x < min.x) min.x = vertexs[i]->x;
        if (vertexs[i]->x > max.x) max.x = vertexs[i]->x;

        if (vertexs[i]->y < min.y) min.y = vertexs[i]->y;
        if (vertexs[i]->y > max.y) max.y = vertexs[i]->y;

        if (vertexs[i]->z < min.z) min.z = vertexs[i]->z;
        if (vertexs[i]->z > max.z) max.z = vertexs[i]->z;
    }

    //This fixes a precision error
    min -= Vector(0.01,0.01,0.01);
    max += Vector(0.01,0.01,0.01);

    return Bounds(min, max);
}


BaseObject* Mesh::intersection(Ray &ray, float *distance, float limit)
{
    Ray rayt = transformRay(ray);

    rayt.calculateInverse();

    BaseObject* closest=NULL;

    closest = octree_.intersection(rayt, distance, limit);

	return closest;
}


TiXmlElement* Mesh::getXml()
{
    TiXmlElement* root = Object::getXml();

	TiXmlElement* file_e = new TiXmlElement("file");
	root->LinkEndChild(file_e);
	
	file_e->SetAttribute("type", "obj");
	file_e->SetAttribute("filename", filename_);

    return root;
}


bool Mesh::loadXml(TiXmlElement* pElem, std::string path)
{
    Object::loadXml(pElem, path);

    TiXmlHandle hRoot = TiXmlHandle(pElem);

    //if file is specified load mesh from file
    TiXmlDocument doc;
    pElem = hRoot.FirstChild( "file" ).Element();
    if (pElem) {
        //std::string filename("");
		filename_ = "";
        pElem->QueryStringAttribute ("filename", &filename_);
        absfilename_ = pathJoin(path, filename_);
        std::string type("");
        pElem->QueryStringAttribute ("type", &type);
        if (type == "" || type == "xml") {
            //Load an xml object file
            /*
            if (filename != "") {
                TiXmlHandle hDoc(&doc);
                if (doc.LoadFile(filename)) {
                    pElem = hDoc.FirstChildElement().Element();
                    if (pElem) {
                        hRoot = TiXmlHandle(pElem);
                    }
                }
            }
            */
        } else if (type == "obj") {
            //Load wavefront obj file
            Vector position(0,0,0);
            float scale=1;
            pElem->QueryValueAttribute <Vector> ("position", &position);
            pElem->QueryFloatAttribute("scale", &scale);
            return loadWavefrontObj(absfilename_, scale, position);
        }
    }

    //TODO: XML loading


    return true;
}


bool Mesh::loadWavefrontObj(std::string filename, float scale, Vector position) {
    /*
       Wavefront Obj loader
       --------------------
       Needs a bit of work (bit slow)
    */  

	std::stringstream displayss; displayss << "0 vertices 0 normals 0 uvps 0 triangles";

	std::cout << "  Loading obj file " << filename << std::endl;
	std::cout << "   " << displayss.str() << std::flush;

    std::string line;
    std::ifstream objfile (filename.c_str());

    uvpoints.push_back( new MapPoint2(0, 0, 0) );

    if (objfile.is_open()) {
        unsigned long v = 0;
        unsigned long m = 0;
        unsigned long n = 0;
        unsigned long t = 0;
        unsigned long pcount = 0;
		while (objfile.good()) {
			getline(objfile,line);
            
            std::stringstream ss(line);
            std::string token;

            token = "";
            getline(ss, token, ' ');
            if (token == "v") {
                //Vertex
                Vector p;
                token = "0"; getline(ss, token, ' ');
                p.x = stof(token) * scale;
                token = "0"; getline(ss, token, ' ');
                p.y = stof(token) * scale;
                token = "0"; getline(ss, token, ' ');
                p.z = stof(token) * scale;

                p += position;

                v += 1;

                vertexs.push_back( new Vertex(v, p.x, p.y, p.z) );
                
                
            } else if (token == "vn") {
                //Normal
                Vector p;
                token = "0"; getline(ss, token, ' ');
                p.x = stof(token);
                token = "0"; getline(ss, token, ' ');
                p.y = stof(token);
                token = "0"; getline(ss, token, ' ');
                p.z = stof(token);

                n += 1;
                
                normals.push_back( new Normal(n, p.x, p.y, p.z) );

            } else if (token == "vt") {
                //UVMapping
                Vector2 p;
                token = "0"; getline(ss, token, ' ');
                p.x = stof(token);
                token = "0"; getline(ss, token, ' ');
                p.y = 1.0f - stof(token);

                m += 1;

                uvpoints.push_back( new MapPoint2(v, p.x, p.y) );

            } else if (token == "f") {
                //Triangles
                std::string vert,map,norm;
                unsigned long v0, v1, v2;
                unsigned long m0, m1, m2;
                unsigned long n0, n1, n2;

                token = ""; getline(ss, token, ' '); 
                std::stringstream s0(token);
                vert = "0"; getline(s0, vert, '/'); 
                v0 = (stringtolong(vert)) -1;
                map = "0"; getline(s0, map, '/'); 
                m0 = (stringtolong(map));
                norm = "0"; getline(s0, norm, '/');
                n0 = (stringtolong(norm)) -1;

                token = ""; getline(ss, token, ' '); 
                std::stringstream s1(token);
                vert = "0"; getline(s1, vert, '/'); 
                v1 = (stringtolong(vert)) -1;
                map = "0"; getline(s1, map, '/'); 
                m1 = (stringtolong(map));
                norm = "0"; getline(s1, norm, '/');
                n1 = (stringtolong(norm)) -1;

                token = ""; getline(ss, token, ' '); 
                std::stringstream s2(token);
                vert = "0"; getline(s2, vert, '/'); 
                v2 = (stringtolong(vert)) -1;
                map = "0"; getline(s2, map, '/'); 
                m2 = (stringtolong(map));
                norm = "0"; getline(s2, norm, '/');
                n2 = (stringtolong(norm)) -1;

				//Face normal is the average of vertex normals
                Vector n = ((*normals[n0]) + (*normals[n1]) + (*normals[n2])) / 3.0;

                t += 1;

                Triangle* trig = new Triangle(t, this);

                trig->set( vertexs[v0],  vertexs[v1],  vertexs[v2],
                          normals[n0],  normals[n1],  normals[n2],
                          uvpoints[m0], uvpoints[m1], uvpoints[m2],
                          n );

                triangles.push_back(trig);

				//Add links to triangle to all vertices
				(vertexs[v0])->addTriangle(triangles.size()-1);
				(vertexs[v1])->addTriangle(triangles.size()-1);
				(vertexs[v2])->addTriangle(triangles.size()-1);
            }

            //status
            if ( v+n+m+t > pcount + 5000) {
                pcount = v+n+m+t;

				for (int w = 0; w < displayss.str().size(); w ++) { std::cout << "\b";}
				displayss.str("");
				displayss << v << " vertices "
						  << n << " normals "
						  << m << " uvps "
						  << t << " triangles";
                std::cout << displayss.str() << std::flush;
            }

		}
		objfile.close();

		for (int w = 0; w < displayss.str().size(); w ++) { std::cout << "\b";}
				displayss.str("");
				displayss << v << " vertices "
						  << n << " normals "
						  << m << " uvps "
						  << t << " triangles";
                std::cout << displayss.str() << std::endl;

        
        genBounds();
        
        return true;
	}
	
    std::cout << "... failed (" << filename << ")" << std::endl;
    return false;
}
