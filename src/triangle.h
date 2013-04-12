/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */

#ifndef _TRIANGLE_H_
#define _TRIANGLE_H_

#define EPSILON 0.000001

#include "vector.h"
#include "map.h"
#include "vertex.h"

#define RAY_TRIG(result, trig, Ro, Rd, t, pvec, tvec, qvec, det, inv_det, u, v) \
    result = false; \
    V_CROSS(pvec, Rd, trig->edge2); \
    det = V_DOT((trig->edge1), pvec); \
    if (!(det > -EPSILON && det < EPSILON)) { \
        inv_det = 1.0 / det; \
        V_SUB(tvec, Ro, trig->v0->p); \
        u = V_DOT(tvec, pvec) * inv_det; \
        if (! (u < 0.0 || u > 1.0) ) {\
            V_CROSS(qvec, tvec, trig->edge1); \
            v = V_DOT(Rd, qvec) * inv_det; \
            t = V_DOT(trig->edge2, qvec) * inv_det; \
            if (t >= 0) { \
                if (! (v < 0.0 || u + v > 1.0) ) { \
                    result = true; \
                } \
            } \
        } \
    }\


class Triangle
{
    public:
        int i;
        Vertex* v0;
        Vertex* v1;
        Vertex* v2;

        UVTriangle* uvtriangle;

        Vector n;
        Vector edge1;
        Vector edge2;

        Triangle(int index,
                Vertex* v0_, 
                Vertex* v1_, 
                Vertex* v2_, 
                Vector2* uv0=NULL,
                Vector2* uv1=NULL,
                Vector2* uv2=NULL,
                float normalDir=1, 
                Vector normal=Vector(0,0,0)) {
            i = index;

            v0 = v0_;
            v1 = v1_;
            v2 = v2_;

            uvtriangle = new UVTriangle(
                    v0, uv0,
                    v1, uv1,
                    v2, uv2, &n);
            
            //average out vertex normals to get face normal
            n = (v0_->n + v1_->n + v2_->n) / 3.0f;
        };

        void transform() {
            Vector e1 = v1->p - v0->p;
            Vector e2 = v2->p - v0->p;

            Vector n2 = e1 % e2;

            if (n*n2 >= 0) {
                n = n2;
                edge1 = e1;
                edge2 = e2;
            } else {
                n = n2 * -1;
                edge1 = e2;
                edge2 = e1;
            }
        }

        bool intersection(
                const Vector &Ro,
                const Vector &Rd,
                float *distance);

       virtual TiXmlElement* getXml() {
            TiXmlElement* root = new TiXmlElement("trig");

            root->SetAttribute("i", i);

            root->SetAttribute("n", n.str());

            root->SetAttribute("v0", v0->i);
            root->SetAttribute("v1", v1->i);
            root->SetAttribute("v2", v2->i);

            root->SetAttribute("m0", uvtriangle->uv0->i);
            root->SetAttribute("m1", uvtriangle->uv1->i);
            root->SetAttribute("m2", uvtriangle->uv2->i);

            return root;
        } 
};

#endif // _TRIANGLE_H_
