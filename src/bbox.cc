/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */

#include "bbox.h"

bool BBox::intersection(const Vector Ro, const Vector Rd) {
    /*
       Ray Box intersection
       --------------------
       Source: http://www.cs.utah.edu/~awilliam/box/box.pdf
    */

    //if ray starts or ends in box then its intersecting
    if (Ro.x < bounds[1].x && Ro.x > bounds[0].x) {
        if (Ro.y < bounds[1].y && Ro.y > bounds[0].y) {
            if (Ro.z < bounds[1].z && Ro.z > bounds[0].z) {
                return true;
            }
        }
    }

    float tmin, tmax, tymin, tymax, tzmin, tzmax;

    Vector inv_direction = Vector(1/Rd.x, 1/Rd.y, 1/Rd.z);

    int sign[3];

    sign[0] = inv_direction.x < 0;
    sign[1] = inv_direction.y < 0;
    sign[2] = inv_direction.z < 0;

    tmin = (bounds[sign[0]].x - Ro.x) * inv_direction.x;
    tmax = (bounds[1-sign[0]].x - Ro.x) * inv_direction.x;

    tymin = (bounds[sign[1]].y - Ro.y) * inv_direction.y;
    tymax = (bounds[1-sign[1]].y - Ro.y) * inv_direction.y;

    if ( (tmin > tymax) || (tymin > tmax) )
        return false;

    if (tymin > tmin)
        tmin = tymin;

    if (tymax < tmax)
        tmax = tymax;

    tzmin = (bounds[sign[2]].z - Ro.z) * inv_direction.z;
    tzmax = (bounds[1-sign[2]].z - Ro.z) * inv_direction.z;

    if ( (tmin > tzmax) || (tzmin > tmax) )
        return false;

    if ( tzmin > tmin )
        tmin = tzmin;
    if ( tzmax < tmax )
        tmax = tzmax;

    //only objs infront looked for
    if (tmin < 0)
        return false;

    return true;
    /*
    *tminr = tmin;
    *tmaxr = tmax;
    *

    return true;*/
}

