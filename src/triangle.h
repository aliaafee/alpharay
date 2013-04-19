/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */

#ifndef _TRIANGLE_H_
#define _TRIANGLE_H_

#include "baseobject.h"
#include "vertex.h"


class Triangle : public BaseObject, public XmlObjectIndexed
{
    public:
        void init() { BaseObject::init(); XmlObjectIndexed::init(); }

        Triangle(int index, BaseObject* parent) 
            : XmlObjectIndexed ("trig", index) 
            { init(); parent_ = parent; }

        ~Triangle() {};

        void set(Vertex*  v0, Vertex*  v1, Vertex*  v2, 
                 Normal*  n0, Normal*  n1, Normal*  n2, 
                 MapPoint2* m0, MapPoint2* m1, MapPoint2* m2, Vector face_n_);

        void transform();

        Bounds bounds();
        
        Vector normal(Vector point);
        Color color(Vector point, Material* material);

        Material material() { return parent_->material(); }

        BaseObject* intersection(Ray &ray, float *t, float limit);
        
        TiXmlElement* getXml();

    protected:
        Vertex* v[3];
        Normal* n[3];
        MapPoint2* m[3];

        Vector face_n;

        Vector edge[2];

        Vector transformPoint(Vector &point) { return parent_->transformPoint(point); }
        Vector transformPointInv(Vector &point) { return parent_->transformPointInv(point); }
        Vector transformNormal(Vector &point) { return parent_->transformNormal(point); }
        Vector transformNormalInv(Vector &point) { return parent_->transformNormalInv(point); }
        Vector transformDisplacement(Vector &point) { return parent_->transformDisplacement(point); };
    	Vector transformDisplacementInv(Vector &point) { return parent_->transformDisplacementInv(point); };

    private:
        void calculateWeights(Vector point, float *w0, float *w1, float *w2);
};

/*
#define EPSILON 0.000001

#include "vector.h"
#include "uvtriangle.h"
#include "vertex.h"
#include "ray.h"

#define RAY_TRIG(result, trig, Ro, Rd, t, pvec, tvec, qvec, det, inv_det, u, v) \
    result = false; \
    V_CROSS(pvec, Rd, trig->edge2); \
    det = V_DOT((trig->edge1), pvec); \
    if (!(det > -EPSILON && det < EPSILON)) { \
        inv_det = 1.0 / det; \
        V_SUB(tvec, Ro, trig->v[0]->p); \
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
        
        Vertex* v[3];
        Vector* n[3];
        Vector* m[3];

        UVTriangle* uvtriangle;

        Vector faceNormal;
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

            v[0] = v0_;
            v[1] = v1_;
            v[2] = v2_;

            uvtriangle = new UVTriangle(
                    v[0], uv0,
                    v[1], uv1,
                    v[2], uv2, &faceNormal);
            
            //average out vertex normals to get face normal
            faceNormal = (v[0]->n + v[1]->n + v[2]->n) / 3.0f;
        };

        void transform() {
            Vector e1 = v[1]->p - v[0]->p;
            Vector e2 = v[2]->p - v[0]->p;

            Vector n2 = e1 % e2;

            if (faceNormal*n2 >= 0) {
                faceNormal = n2;
                edge1 = e1;
                edge2 = e2;
            } else {
                faceNormal = n2 * -1;
                edge1 = e2;
                edge2 = e1;
            }
        }

        bool intersection(const Ray &ray, float *t);

        bool inbounds(Vector &min, Vector &max);
        void getbounds(Vector *min, Vector *max);

        virtual TiXmlElement* getXml() {
            TiXmlElement* root = new TiXmlElement("trig");

            root->SetAttribute("i", i);

            root->SetAttribute("n", faceNormal.str());

            root->SetAttribute("v0", v[0]->i);
            root->SetAttribute("v1", v[1]->i);
            root->SetAttribute("v2", v[2]->i);

            root->SetAttribute("m0", uvtriangle->m[0]->i);
            root->SetAttribute("m1", uvtriangle->m[1]->i);
            root->SetAttribute("m2", uvtriangle->m[2]->i);

            return root;
        } 
};
*/
#endif // _TRIANGLE_H_
