#ifndef _UV_TRIANGLE_H_
#define _UV_TRIANGLE_H_

#include "vector.h"
#include "vertex.h"

class UVTriangle {
    public:
        Vertex* v0;
        Vertex* v1;
        Vertex* v2;

        Vector* n;

        Vector2* uv0;
        Vector2* uv1;
        Vector2* uv2;

        float weight0;
        float weight1;
        float weight2;

        Vector currentPoint;
            
        UVTriangle (Vertex* v0_, Vector2* uv0_,
                    Vertex* v1_, Vector2* uv1_,
                    Vertex* v2_, Vector2* uv2_, Vector* n_) {
            v0 = v0_;
            uv0 = uv0_;

            v1 = v1_;
            uv1 = uv1_;

            v2 = v2_;
            uv2 = uv2_;

            n = n_;

            weight0 = 0.0f;
            weight1 = 0.0f;
            weight2 = 0.0f;
        };

        UVTriangle (UVTriangle* source) {
            if (source) {
                v0 = source->v0;
                uv0 = source->uv0;

                v1 = source->v1;
                uv1 = source->uv1;

                v2 = source->v2;
                uv2 = source->uv2;

                n = source->n;

                weight0 = 0.0f;
                weight1 = 0.0f;
                weight2 = 0.0f;
            }
        };

        bool isNull () {
            if (v0 == NULL || v1 == NULL || v2 == NULL || 
                uv0== NULL || uv1== NULL || uv2== NULL) {
                return true;
            }
            return false;
        };

        Vector getNormal(Vector point, bool interpolated);
        Vector2 getUV(Vector point);
        
        void calculateWeights(Vector point);
};


#endif // _UV_TRIANGLE_H_
