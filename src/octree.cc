#include "octree.h"

bool Octree::add(std::vector<Triangle*> *newtriangles, int currentDepth) {
    if (!newtriangles) {
        return false;
    }

    //std::cout << "Depth " << currentDepth << " trigs " << triangles->size() << endl;

    //if too deep or reached threshold number of trigs store the trigs and return;
    if (currentDepth >= maxDepth_ || newtriangles->size() <= threshold_) {
        triangles = newtriangles;
        //std::cout << "Done Leaf, trigs " << triangles->size() << std::endl;
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
                for (int i=0; i < newtriangles->size(); i++) {
                    if ( (*newtriangles)[i]->inbounds(submin, submax) ) {
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
                    (children[xi][yi][zi])->add(subtrig, currentDepth + 1);
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
}


Triangle* Octree::intersection(const Vector Ro, const Vector Rd, float *t) {
    //Check for intersection with this bounding volume
    if (!BBox::intersection(Ro, Rd)) {
        return NULL;
    }

    Triangle* curI=NULL;
    float curt = 0;

    Triangle* cloI=NULL;
    float clot = 0;

    if (triangles != NULL) {
        //if triangles list available, then process as leaf

        bool result;

        int closestTrig = -1;

        clot = BIG_NUM;

        Vector pvec, tvec, qvec;
        float det, inv_det, u, v;

        for (int i=0; i < triangles->size(); i++) {
            //This is real ugly, but kinda fast. is it worth it?
            //RAY_TRIG(result, ((*triangles)[i]), Ro, Rd, curt, pvec, tvec, qvec, det, inv_det, u, v);

            //Or use the function alternative. Much nicer looking. But a teeny bit slower
            result = (*triangles)[i]->intersection(Ro, Rd, &curt);

            if (result) {
                if (curt > 0.0001 && curt < clot) {
                    closestTrig = i;
                    clot = curt;
                }
            }
        }


        if (closestTrig < 0) {
            return NULL;
        }

        *t = clot;
        return (*triangles)[closestTrig];
    } else {
        //Check for intersection with twigs
        clot = BIG_NUM;
        curI = NULL;

        for (int x = 0; x < 2; x++) {
            for (int y = 0; y < 2; y++) {
                for (int z = 0; z < 2; z++) {
                    if (children[x][y][z] != NULL) {
                        curI = (children[x][y][z])->intersection(Ro, Rd, &curt);
                        if (curI != NULL) {
                            if (curt < clot) {
                                cloI = curI;
                                clot = curt;
                            }
                        }
                    }

                }
            }
        }

        *t = clot;
        return cloI;
    }
}
