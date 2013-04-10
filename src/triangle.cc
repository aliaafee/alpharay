/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */

#include "triangle.h"

Triangle* Triangle::intersection(
                Vector Ro,
                Vector Rd,
                Vector *intersectionPoint=NULL,
                Vector *intersectionNormal=NULL,
                UVTriangle **intersectionUVTriangle=NULL,
                float *distance=NULL)
{
    /*
       Ray Plane Intersection
       ======================
       source:
       http://www.cs.virginia.edu/~gfx/Courses/2003/ImageSynthesis/papers/Acceleration/Fast%20MinimumStorage%20RayTriangle%20Intersection.pdf

    */

    //Vector edge1 = v1->p - v0->p;
    //Vector edge2 = v2->p - v0->p;

    Vector pvec;// = Rd % edge2;
    V_CROSS(pvec, Rd, edge2)

    float det = V_DOT(edge1, pvec);

    if (det > -EPSILON && det < EPSILON) {
    //if (det < EPSILON) {
        return NULL;
    }

    float inv_det = 1.0 / det;
    
    Vector tvec;
    V_SUB(tvec, Ro, v0->p);

    float u = V_DOT(tvec, pvec) * inv_det;

    if (u < 0.0 || u > 1.0)
        return NULL;

    Vector qvec;
    V_CROSS(qvec, tvec, edge1);

    float v = V_DOT(Rd, qvec) * inv_det;

    float t = V_DOT(edge2, qvec) * inv_det;

    if (t < 0) {
        return NULL;
    }

    if (v < 0.0 || u + v > 1.0)
        return NULL;

    if (intersectionNormal != NULL) {
        *intersectionNormal = n;
    }

    if (intersectionPoint != NULL || distance != NULL) {
        if (intersectionPoint != NULL) {
            V_INT_POINT((*intersectionPoint), Ro, Rd, t);
        }
        if (distance != NULL)
            *distance = t;
    }

    if (intersectionUVTriangle != NULL)
        *intersectionUVTriangle = uvtriangle;

    return this;
}

