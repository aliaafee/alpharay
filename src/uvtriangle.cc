#include "uvtriangle.h"

void UVTriangle::calculateTangentSpace() {
    Vector ve1 = v[1]->p - v[0]->p;
    Vector ve2 = v[2]->p - v[0]->p;

    Vector2 me1 = *m[1] - *m[0];
    Vector2 me2 = *m[2] - *m[0];

    float det = divide(1, (me1.x * me2.y) - (me2.x * me1.y));

    T.x = det * (me2.y * ve1.x - me1.y * ve2.x);
    T.y = det * (me2.y * ve1.y - me1.y * ve2.y);
    T.z = det * (me2.y * ve1.z - me1.y * ve2.z);

    B.x = det * ( me1.x * ve2.x - me2.x * ve1.x );
    B.y = det * ( me1.x * ve2.y - me2.x * ve1.y );
    B.z = det * ( me1.x * ve2.z - me2.x * ve1.z );

    B.normalize();
    T.normalize();
    //(*n).normalize();
}

void UVTriangle::calculateWeights(Vector point) {
    if ( V_EQUAL(currentPoint, point) ) {
        return;
    }

    currentPoint = point;

    if (isNull()) {
        return;
    }

    Vector e0;
    V_SUB(e0, v[0]->p, v[1]->p);

    Vector e1;
    V_SUB(e1, v[0]->p, v[2]->p);
    
    Vector f0;// = v0->p - point;
    V_SUB(f0, v[0]->p, point);

    Vector f1;// = v1->p - point;
    V_SUB(f1, v[1]->p, point);

    Vector f2;// = v2->p - point;
    V_SUB(f2, v[2]->p, point);

    Vector xa;
    V_CROSS(xa, e0, e1);

    Vector xa0;
    V_CROSS(xa0, f1, f2);

    Vector xa1;
    V_CROSS(xa1, f0, f2);

    Vector xa2;
    V_CROSS(xa2, f1, f0);

    float a = xa.magnitude2();
    weight[0] = sqrt(xa0.magnitude2() / a);
    weight[1] = sqrt(xa1.magnitude2() / a);
    weight[2] = sqrt(xa2.magnitude2() / a);
}

Vector UVTriangle::getNormal(Vector point, bool interpolated = true) {
    if (interpolated) {
        calculateWeights(point);
        return ((v[0]->n) * weight[0]) + ((v[1]->n) * weight[1]) + ((v[2]->n) * weight[2]);
    } else {
        return *n;
    }
}

Vector UVTriangle::getNormal(Vector point, Vector normalFromMap, bool interpolated = true) {
    /*
       Tangent space to object space conversion
       ----------------------------------------
       I = image normal
       N = face normal
       T = tangent
       B = bitangent

       [ I.x I.y I.z ] * | T.x  B.x  N.x |
                         | T.y  B.y  N.y |
                         | T.z  B.z  N.z |

    */
    Vector N = getNormal(point, false);
    N.normalize();

    Vector I = normalFromMap;

    Vector r;

    r.x = (I.x * T.x + I.y * T.y + I.z * T.z);
    r.y = (I.x * B.x + I.y * B.y + I.z * B.z);
    r.z = (I.x * N.x + I.y * N.y + I.z * N.z);

    return r;
}

Vector2 UVTriangle::getUV(Vector point) {
    calculateWeights(point);
    return ((*m[0]) * weight[0]) + ((*m[1]) * weight[1]) + ((*m[2]) * weight[2]);
};

