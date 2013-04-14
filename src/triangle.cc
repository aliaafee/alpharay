/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */

#include "triangle.h"

bool Triangle::inbounds(Vector &min, Vector &max) {
    Vector tmax, tmin;

    if (v0->p.x < tmin.x) tmin.x = v0->p.x;
    if (v0->p.x > tmax.x) tmax.x = v0->p.x;
    if (v1->p.x < tmin.x) tmin.x = v1->p.x;
    if (v1->p.x > tmax.x) tmax.x = v1->p.x;
    if (v2->p.x < tmin.x) tmin.x = v2->p.x;
    if (v2->p.x > tmax.x) tmax.x = v2->p.x;

    if (v0->p.y < tmin.y) tmin.y = v0->p.y;
    if (v0->p.y > tmax.y) tmax.y = v0->p.y;
    if (v1->p.y < tmin.y) tmin.y = v1->p.y;
    if (v1->p.y > tmax.y) tmax.y = v1->p.y;
    if (v2->p.y < tmin.y) tmin.y = v2->p.y;
    if (v2->p.y > tmax.y) tmax.y = v2->p.y;

    if (v0->p.z < tmin.z) tmin.z = v0->p.z;
    if (v0->p.z > tmax.z) tmax.z = v0->p.z;
    if (v1->p.z < tmin.z) tmin.z = v1->p.z;
    if (v1->p.z > tmax.z) tmax.z = v1->p.z;
    if (v2->p.z < tmin.z) tmin.z = v2->p.z;
    if (v2->p.z > tmax.z) tmax.z = v2->p.z;

    Vector t;
    Vector tsize = tmax-tmin;
    Vector bsize = max - min;

    //cout << "trig bounds" << tmin << tmax << endl;
    //cout << "bbox bounds" << min << max << endl;
    //cout << "trig box" << endl;
    for (int x=0; x < 2; x++) {
        for (int y=0; y < 2; y++) {
            for (int z=0; z < 2; z++) {
                t = tmin + Vector(tsize.x * float(x), tsize.y * float(y), tsize.z * float(z));
                //cout << t;
                if ( (t.x > min.x) && (t.x < max.x) &&
                     (t.y > min.y) && (t.y < max.y) &&
                     (t.z > min.z) && (t.z < max.z) ) { 
                    //cout << "in" << endl;
                    return true;
                } else {
                    ;//cout << (t.x > min.x) << (t.x < max.x) << (t.y > min.y) << (t.y < max.y) << (t.z > min.z) << (t.z < min.z) << endl;
                }
            }
        }
    }

    //cout << "bbox" << endl;

    for (int x=0; x < 2; x++) {
        for (int y=0; y < 2; y++) {
            for (int z=0; z < 2; z++) {
                t = min + Vector(bsize.x * float(x), bsize.y * float(y), bsize.z * float(z));
                //cout << t;
                if (t.x > tmin.x && t.x < tmax.x &&
                    t.y > tmin.y && t.y < tmax.y &&
                    t.z > tmin.z && t.z < tmax.z) {
                    //cout << "in" << endl;
                    return true;
                } else {
                    ;//cout << "out" << endl;
                }
            }
        }
    }


    return false;
}


bool Triangle::intersection(
                const Vector &Ro,
                const Vector &Rd,
                float *t)
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

    //if (det > -EPSILON && det < EPSILON) {
    if (det < EPSILON) {
        return false;
    }

    float inv_det = 1.0 / det;
    
    Vector tvec;
    V_SUB(tvec, Ro, v0->p);

    float u = V_DOT(tvec, pvec) * inv_det;

    if (u < 0.0 || u > 1.0)
        return false;

    Vector qvec;
    V_CROSS(qvec, tvec, edge1);

    *t = V_DOT(edge2, qvec) * inv_det;

    if (*t < 0) {
        return false;
    }

    float v = V_DOT(Rd, qvec) * inv_det;

    if (v < 0.0 || u + v > 1.0)
        return false; 

    return true;
}

