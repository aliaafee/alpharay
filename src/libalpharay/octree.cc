/* vim: set ts=4 ss=4 sw=4 noet ai cindent : */

#include "octree.h"


bool Octree::add(std::vector<Triangle*> *newtriangles, unsigned long currentDepth, unsigned long *leaves) {
    if (!newtriangles) {
        return false;
    }

    //std::cout << "Depth " << currentDepth << " trigs " << triangles->size() << endl;

    //if too deep or reached threshold number of trigs store the trigs and return;
    if (currentDepth >= maxDepth_ || newtriangles->size() <= threshold_) {
        *leaves += 1;
        triangles = newtriangles;
        //std::cout << "     leaf trigs " << triangles->size() << std::endl;
        return true;
    }

    //calculate the sub bbox bounds and allocate trigs to subtrees
    Vector submin;
    Vector submax;
    std::vector<Triangle*> *subtrig;

    Vector size = (bounds[1]-bounds[0]) / 2.0f;
    Vector center = bounds[0] + size;

    for (float x = -1; x < 1; x++) {
        for (float y = -1; y < 1; y++) {
            for (float z = -1; z < 1; z++) {
                submin = center  + Vector(size.x * x, size.y * y, size.z * z);
                submax = submin + size;

                subtrig = new std::vector<Triangle*>;
                for (unsigned long i=0; i < newtriangles->size(); i++) {
                    if ( (*newtriangles)[i]->inBounds(Bounds(submin, submax)) ) {
                        subtrig->push_back( (*newtriangles)[i] );
                    }
                }

                //std::cout << "[" << x << "," << y << "," << z << "]" << "=\t" << submin << 
                //                        "->\t" << submin << " trigs" << subtrig->size() << endl;
                int xi = x + 1;
                int yi = y + 1;
                int zi = z + 1;
                if (subtrig->size() > 0) {
                    children[xi][yi][zi] = new Octree(submin, submax, maxDepth_, threshold_);
                    (children[xi][yi][zi])->add(subtrig, currentDepth + 1, leaves);
                } else {
                    children[xi][yi][zi] = NULL;
                    delete subtrig;
                }
            }
        }
    }
    if (currentDepth > 0) {
        //cout << currentDepth << endl;
        delete newtriangles;
    }
	
	return true;
}


BaseObject* Octree::intersection(Ray &ray, float *t, float limitMax) {
    //Check for intersection with this bounding volume

    if (!BBox::intersection(ray, limitMax)) {
        return NULL;
    }

    BaseObject* curI=NULL;
    float curt = 0;

    BaseObject* cloI=NULL;
    float clot = 0;

    if (triangles != NULL) {
        //if triangles list available, then process as leaf

        BaseObject *currentTrig = NULL;

        BaseObject *closestTrig = NULL;

        clot = BIG_NUM;

        for (unsigned long i=0; i < triangles->size(); i++) {
            currentTrig = (*triangles)[i]->intersection(ray, &curt, limitMax); 
            if (currentTrig != NULL) {
                if (curt > 0.0001 && curt < clot) {
                    if (curt < limitMax) {
                        closestTrig = currentTrig;
                        clot = curt;
                        
                        if (ray.shadowRay_) {
                            *t = clot;
                            return closestTrig;
                        }
                    }
                }
            }
        }

        *t = clot;

        return closestTrig;
    } else {
        //Check for intersection with twigs
        clot = BIG_NUM;
        curI = NULL;

        for (int x = 0; x < 2; x++) 
        for (int y = 0; y < 2; y++) 
        for (int z = 0; z < 2; z++)
        {
            if (children[x][y][z] != NULL) {
                curI = (children[x][y][z])->intersection(ray, &curt, limitMax);
                //Pick the closest  intersection
                if (curI != NULL) {
                    if (curt < clot) {
                        cloI = curI;
                        clot = curt;
                        
                        if (ray.shadowRay_) {
                            *t = clot;
                            return cloI;
                        }
                    }
                }
            }

        }
            
        

        *t = clot;
        return cloI;
    }
}
