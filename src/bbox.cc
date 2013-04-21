/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */

#include "bbox.h"

//bool BBox::intersection(const Vector Ro, const Vector Rd) 
bool BBox::intersection(const Ray &ray, const float &limitMax)
{
    /*
       Ray Box intersection
       --------------------
       Source: http://www.cs.utah.edu/~awilliam/box/box.pdf
    */

    //if ray starts in box then its intersecting
    if (ray.position_.x < bounds[1].x && ray.position_.x > bounds[0].x) {
        if (ray.position_.y < bounds[1].y && ray.position_.y > bounds[0].y) {
            if (ray.position_.z < bounds[1].z && ray.position_.z > bounds[0].z) {
                return true;
            }
        }
    }

    float tmin, tmax, tymin, tymax, tzmin, tzmax;

    tmin = (bounds[ray.sign[0]].x - ray.position_.x) * ray.inv_direction.x;
    tmax = (bounds[1-ray.sign[0]].x - ray.position_.x) * ray.inv_direction.x;

    tymin = (bounds[ray.sign[1]].y - ray.position_.y) * ray.inv_direction.y;
    tymax = (bounds[1-ray.sign[1]].y - ray.position_.y) * ray.inv_direction.y;

    if ( (tmin > tymax) || (tymin > tmax) )
        return false;

    if (tymin > tmin)
        tmin = tymin;

    if (tymax < tmax)
        tmax = tymax;

    tzmin = (bounds[ray.sign[2]].z - ray.position_.z) * ray.inv_direction.z;
    tzmax = (bounds[1-ray.sign[2]].z - ray.position_.z) * ray.inv_direction.z;

    if ( (tmin > tzmax) || (tzmin > tmax) )
        return false;

    if ( tzmin > tmin )
        tmin = tzmin;
    if ( tzmax < tmax )
        tmax = tzmax;

    //only objs infront looked for
    if (tmin < 0)
        return false;

    //only objs with in specified range
    if (tmax > limitMax)
        return false;

    return true;
}

