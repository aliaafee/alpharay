/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */

#include "triangle.h"

void Triangle::getbounds(Vector *min, Vector *max) {
    *max = Vector(-BIG_NUM, -BIG_NUM, -BIG_NUM);
    *min = Vector( BIG_NUM,  BIG_NUM,  BIG_NUM);

    for (i=0; i < 3; i++) {
        if (v[i]->p.x < min->x) min->x = v[i]->p.x;
        if (v[i]->p.x > max->x) max->x = v[i]->p.x;
        if (v[i]->p.y < min->y) min->y = v[i]->p.y;
        if (v[i]->p.y > max->y) max->y = v[i]->p.y;
        if (v[i]->p.z < min->z) min->z = v[i]->p.z;
        if (v[i]->p.z > max->z) max->z = v[i]->p.z;
    }
}

bool Triangle::inbounds(Vector &min, Vector &max) {
    Vector tmax(-BIG_NUM, -BIG_NUM, -BIG_NUM);
    Vector tmin( BIG_NUM,  BIG_NUM,  BIG_NUM);

    for (i=0; i < 3; i++) {
        if (v[i]->p.x < tmin.x) tmin.x = v[i]->p.x;
        if (v[i]->p.x > tmax.x) tmax.x = v[i]->p.x;
        if (v[i]->p.y < tmin.y) tmin.y = v[i]->p.y;
        if (v[i]->p.y > tmax.y) tmax.y = v[i]->p.y;
        if (v[i]->p.z < tmin.z) tmin.z = v[i]->p.z;
        if (v[i]->p.z > tmax.z) tmax.z = v[i]->p.z;
    }

    bool a = inRange(min.x, tmin.x, tmax.x) || inRange(max.x, tmin.x, tmax.x) || 
             inRange(tmin.x, min.x, max.x)  || inRange(tmax.x, min.x, max.x) ;

    bool b = inRange(min.y, tmin.y, tmax.y) || inRange(max.y, tmin.y, tmax.y) || 
             inRange(tmin.y, min.y, max.y)  || inRange(tmax.y, min.y, max.y) ;

    bool c = inRange(min.z, tmin.z, tmax.z) || inRange(max.z, tmin.z, tmax.z) || 
             inRange(tmin.z, min.z, max.z)  || inRange(tmax.z, min.z, max.z) ;

    /*
    cout << a << "=" << min.x << "," << max.x << " | " << tmin.x << "," << tmax.x <<  endl;
    cout << b << "=" << min.y << "," << max.y << " | " << tmin.y << "," << tmax.y <<  endl;
    cout << c << "=" << min.z << "," << max.z << " | " << tmin.z << "," << tmax.z <<  endl;
    */

    if ( a && b && c) {
        return true;
    }
    

    return false;
}


bool Triangle::intersection(const Ray &ray, float *t)
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
    V_CROSS(pvec, ray.direction_, edge2)

    float det = V_DOT(edge1, pvec);

    if (det > -EPSILON && det < EPSILON) {
    //if (det < EPSILON) {
        return false;
    }

    float inv_det = 1.0 / det;
    
    Vector tvec;
    V_SUB(tvec, ray.position_, v[0]->p);

    float u = V_DOT(tvec, pvec) * inv_det;

    if (u < 0.0 || u > 1.0)
        return false;

    Vector qvec;
    V_CROSS(qvec, tvec, edge1);

    *t = V_DOT(edge2, qvec) * inv_det;

    if (*t < 0) {
        return false;
    }

    float v = V_DOT(ray.direction_, qvec) * inv_det;

    if (v < 0.0 || u + v > 1.0)
        return false; 

    return true;
}

