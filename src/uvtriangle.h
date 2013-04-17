#ifndef _UV_TRIANGLE_H_
#define _UV_TRIANGLE_H_

#include "vector.h"
#include "vertex.h"

class UVTriangle {
    public:
        Vertex*  v[3];
        Vector2* m[3];

        Vector T;
        Vector B;

        float weight[3];

        Vector* n;

        Vector currentPoint;
            
        UVTriangle (Vertex* v0_, Vector2* uv0_,
                    Vertex* v1_, Vector2* uv1_,
                    Vertex* v2_, Vector2* uv2_, Vector* n_) {
            v[0] = v0_;
            m[0] = uv0_;

            v[1] = v1_;
            m[1] = uv1_;

            v[2] = v2_;
            m[2] = uv2_;

            n = n_;

            weight[0] = 0.0f;
            weight[1] = 0.0f;
            weight[2] = 0.0f;

            calculateTangentSpace();
        };

        UVTriangle (UVTriangle* source) {
            if (source) {
                for (int i = 0; i <  3; i++) {
                    v[i] = source->v[i];
                    m[i] = source->m[i];
                    weight[i] = 0;
                }

                n = source->n;

                calculateTangentSpace();
            }
        };

        bool isNull () {
            if (v[0] == NULL || v[1] == NULL || v[2] == NULL || 
                m[0] == NULL || m[1] == NULL || m[2] == NULL) {
                return true;
            }
            return false;
        };

        Vector getNormal(Vector point, bool interpolated);
        Vector getNormal(Vector point, Vector normalFromMap, bool interpolated);
        Vector getTangent(Vector point);
        Vector2 getUV(Vector point);
        
        void calculateTangentSpace();
        void calculateWeights(Vector point);
};


#endif // _UV_TRIANGLE_H_
