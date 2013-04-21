/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */

#ifndef _OCTREE_H_
#define _OCTREE_H_

#include <vector>

#include "bbox.h"
#include "triangle.h"

class Octree : BBox {
    public:
        std::vector<Triangle*> *triangles;

        int maxDepth_;
        int threshold_;

        Octree *children[2][2][2];

        Octree() { triangles=NULL;};

        Octree(const Vector &min, const Vector &max, int maxDepth, int threshold) 
            : BBox(min, max) {
            maxDepth_ = maxDepth;
            threshold_ = threshold;
            triangles = NULL;
        }

        virtual bool add(std::vector<Triangle*> *newtriangles, int currentDepth, int *leaves);
        virtual BaseObject* intersection(Ray &ray, float *t, float limitMax);
};


#endif //_OCTREE_H_
