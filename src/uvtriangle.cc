#include "uvtriangle.h"

void UVTriangle::calculateWeights(Vector point) {
    if (isNull()) {
        return;
    }

    Vector e0;
    V_SUB(e0, v0->p, v1->p);

    Vector e1;
    V_SUB(e1, v0->p, v2->p);
    
    Vector f0;// = v0->p - point;
    V_SUB(f0, v0->p, point);

    Vector f1;// = v1->p - point;
    V_SUB(f1, v1->p, point);

    Vector f2;// = v2->p - point;
    V_SUB(f2, v2->p, point);

    Vector xa;
    V_CROSS(xa, e0, e1);

    Vector xa0;
    V_CROSS(xa0, f1, f2);

    Vector xa1;
    V_CROSS(xa1, f0, f2);

    Vector xa2;
    V_CROSS(xa2, f1, f0);

    float a = xa.magnitude2();
    weight0 = sqrt(xa0.magnitude2() / a);
    weight1 = sqrt(xa1.magnitude2() / a);
    weight2 = sqrt(xa2.magnitude2() / a);
}

Vector UVTriangle::getNormal(Vector point) {
    return ((v0->n) * weight0) + ((v1->n) * weight1) + ((v2->n) * weight2);
    //return v0->n;
}

Vector2 UVTriangle::getUV(Vector point) {
    /*
        source:
        http://answers.unity3d.com/questions/383804/calculate-uv-coordinates-of-3d-point-on-plane-of-m.html
    */
    /*
        
    Vector e0;
    V_SUB(e0, v0->p, v1->p);

    Vector e1;
    V_SUB(e1, v0->p, v2->p);
    
    Vector f0;// = v0->p - point;
    V_SUB(f0, v0->p, point);

    Vector f1;// = v1->p - point;
    V_SUB(f1, v1->p, point);

    Vector f2;// = v2->p - point;
    V_SUB(f2, v2->p, point);

    Vector xa;
    V_CROSS(xa, e0, e1);

    Vector xa0;
    V_CROSS(xa0, f1, f2);

    Vector xa1;
    V_CROSS(xa1, f0, f2);

    Vector xa2;
    V_CROSS(xa2, f1, f0);

    float a = xa.magnitude2();
    float a0 = sqrt(xa0.magnitude2() / a);
    float a1 = sqrt(xa1.magnitude2() / a);
    float a2 = sqrt(xa2.magnitude2() / a);*/

    return ((*uv0) * weight0) + ((*uv1) * weight1) + ((*uv2) * weight2);
};

