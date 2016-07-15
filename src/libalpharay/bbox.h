/* vim: set ts=4 ss=4 sw=4 noet ai cindent : */

#ifndef _B_BOX_H_
#define _B_BOX_H_

#include "vector.h"
#include "ray.h"

class BBox {
    public:
        Vector bounds[2];

        BBox() {};

        BBox(const Vector &min, const Vector &max) {
            bounds[0] = min;
            bounds[1] = max;
        }

        //bool intersection(const Vector Ro, const Vector Rd);
        //bool intersection(const Ray &ray, const float &max);
        bool intersection(const Ray &ray, const float &limitMax);
};


#endif //B_BOX
