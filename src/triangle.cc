/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */

#include "triangle.h"


void Triangle::set(Vertex*  v0, Vertex*  v1, Vertex*   v2, 
         Normal*  n0, Normal*  n1, Normal*  n2, 
         MapPoint2* m0, MapPoint2* m1, MapPoint2* m2, Vector face_n_) 
{
    v[0] = v0;
    v[1] = v1;
    v[2] = v2;

    n[0] = n0;
    n[1] = n1;
    n[2] = n2;

    m[0] = m0;
    m[1] = m1;
    m[2] = m2;

    face_n = face_n_;
}


void Triangle::transform()
{
    //Pre calculate edges for later intersection calculation
    Vector e0 = (*v[1]) - (*v[0]);
    Vector e1 = (*v[2]) - (*v[0]);

    Vector n2 = e0 % e1;

    if (face_n*n2 >= 0) {
        face_n = n2;
        edge[0] = e0;
        edge[1] = e1;
    } else {
        face_n = n2 * -1;
        edge[0] = e1;
        edge[1] = e0;
    }
}


Bounds Triangle::bounds() 
{
    //Get the bounding box for the triangle

    Bounds result;
    result.max = Vector(-BIG_NUM, -BIG_NUM, -BIG_NUM);
    result.min = Vector( BIG_NUM,  BIG_NUM,  BIG_NUM);

    for (int i = 0; i < 3; i++) {
        V_MIN_MAX( (*v[i]), result.min, result.max );
    }

    return result;
}


Vector Triangle::normal(Vector point)
{
    if (parent_->material().flatShading()) {
        return face_n;
    }

    float w0, w1, w2;
    calculateWeights(point, &w0, &w1, &w2);

    Vector localnormal = ((*n[0]) * w0) + ((*n[1]) * w1) + ((*n[2]) * w2);

    return transformNormal(localnormal);
}


Color Triangle::color(Vector point, Material* material)
{
    Vector2 point2d;

    if (m[0] != NULL && m[1] != NULL && m[2] != NULL) {
        float w0, w1, w2;
        calculateWeights(point, &w0, &w1, &w2);

        //std::cout << w0 << "," << w1 << "," << w2 << std::endl;

        point2d.x = ((*m[0]).x * w0) + ((*m[1]).x * w1) + ((*m[2]).x * w2);
        point2d.y = ((*m[0]).y * w0) + ((*m[1]).y * w1) + ((*m[2]).y * w2);

        //std::cout << point2d << std::endl;
    }

    return material->color(point, point2d);
}


BaseObject* Triangle::intersection(Ray &ray, float *t, float limit)
{
    /*
    Ray Triangle Intersection
    source:
    http://www.cs.virginia.edu/~gfx/Courses/2003/ImageSynthesis/papers/Acceleration
             /Fast%20MinimumStorage%20RayTriangle%20Intersection.pdf

    */

    //assumes that the ray provided is in parents object space
    
    Vector pvec;// = Rd % edge2;
    V_CROSS(pvec, ray.direction_, edge[1])

    float det = V_DOT(edge[0], pvec);

    if (det > -EPSILON && det < EPSILON) {
    //if (det < EPSILON) {
        return NULL;
    }

    float inv_det = 1.0 / det;
    
    Vector tvec;
    V_SUB(tvec, ray.position_, (*v[0]));

    float u = V_DOT(tvec, pvec) * inv_det;

    if (u < 0.0 || u > 1.0)
        return NULL;

    Vector qvec;
    V_CROSS(qvec, tvec, edge[0]);

    *t = V_DOT(edge[1], qvec) * inv_det;

    if (*t < 0) {
        return NULL;
    }

    float v = V_DOT(ray.direction_, qvec) * inv_det;

    if (v < 0.0 || u + v > 1.0)
        return NULL; 

    return this;
}


void Triangle::calculateWeights(Vector point, float *w0, float *w1, float *w2) {
    /*
    Calculate relative influence of vertices, at an arbitrary point
    This is done by calculating magnitude the cross products of the vectors
    from the point to each point (which is proportional to the area of the triangl
    they form)
    (Need to explain this a bit more)

    */
    Vector e0;
    V_SUB(e0, (*v[0]), (*v[1]));

    Vector e1;
    V_SUB(e1, (*v[0]), (*v[2]));
    
    Vector f0;
    V_SUB(f0, (*v[0]), point);

    Vector f1;
    V_SUB(f1, (*v[1]), point);

    Vector f2;
    V_SUB(f2, (*v[2]), point);

    Vector xa;
    V_CROSS(xa, e0, e1);

    Vector xa0;
    V_CROSS(xa0, f1, f2);

    Vector xa1;
    V_CROSS(xa1, f0, f2);

    Vector xa2;
    V_CROSS(xa2, f1, f0);

    float a = xa.magnitude2();
    *w0 = sqrt(xa0.magnitude2() / a);
    *w1 = sqrt(xa1.magnitude2() / a);
    *w2 = sqrt(xa2.magnitude2() / a);
}


inline TiXmlElement* Triangle::getXml() {
    TiXmlElement* root = XmlObjectIndexed::getXml();

    root->SetAttribute("i", index_);

    root->SetAttribute("v0", v[0]->i());
    root->SetAttribute("v1", v[1]->i());
    root->SetAttribute("v2", v[2]->i());

    root->SetAttribute("m0", m[0]->i());
    root->SetAttribute("m1", m[1]->i());
    root->SetAttribute("m2", m[2]->i());

    root->SetAttribute("n0", n[0]->i());
    root->SetAttribute("n1", n[1]->i());
    root->SetAttribute("n2", n[2]->i());

    return root;
}

