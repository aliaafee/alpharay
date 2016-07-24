/* vim: set ts=4 ss=4 sw=4 noet ai cindent : */

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

	if (m[0] != NULL && m[1] != NULL && m[2] != NULL) {
		Vector E0 = (*v[1]) - (*v[0]);
		Vector E1 = (*v[2]) - (*v[0]);

		Vector2 m0 = (*m[1]) - (*m[0]);
		Vector2 m1 = (*m[2]) - (*m[0]);

		float det = m0.x * m1.y - m1.x * m0.y;
		if (det != 0) {
			float q = 1/det;

			face_t.x =  m1.y * E0.x - m0.y * E1.x * q;
			face_b.x = -m1.x * E0.x + m0.x * E1.x * q;

			face_t.y =  m1.y * E0.y - m0.y * E1.y * q;
			face_b.y = -m1.x * E0.y + m0.x * E1.y * q;

			face_t.z =  m1.y * E0.z - m0.y * E1.z * q;
			face_b.z = -m1.x * E0.z + m0.x * E1.z * q;

			face_t.normalize();
			face_b.normalize();
			face_n.normalize();

			det = (face_t % face_b) * face_n;
			if (det < 0) {
				face_t *= -1;
				face_b *= -1;
			}
		}
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


Vector Triangle::normal(Vector point, Material* material)
{
    if (material->flatShading()) {
        return transformNormal(face_n);
    }

    float w0, w1, w2;
    calculateWeights(point, &w0, &w1, &w2);

	Vector os_normal = ((*n[0]) * w0) + ((*n[1]) * w1) + ((*n[2]) * w2);

	if (material->normalMap_ != NULL) {
		Vector os_tangent;
		Vector os_bitangent;

		if (m[0] != NULL && m[1] != NULL && m[2] != NULL) {
			os_tangent = ((*v[0]).t * w0) + ((*v[1]).t * w1) + ((*v[2]).t * w2);
			os_bitangent = ((*v[0]).b * w0) + ((*v[1]).b * w1) + ((*v[2]).b * w2);
		}
		material->normalMap_->getTangents(point, &os_tangent, &os_bitangent);

		os_normal = material->normalObjectSpace(os_normal, os_tangent, os_bitangent);
	}
	
    return transformNormal(os_normal);
}


void Triangle::setPoint(Vector point, Material* material)
{
    Vector2 point2d;

    if (m[0] != NULL && m[1] != NULL && m[2] != NULL) {
        float w0, w1, w2;
        calculateWeights(point, &w0, &w1, &w2);

        point2d.x = ((*m[0]).x * w0) + ((*m[1]).x * w1) + ((*m[2]).x * w2);
        point2d.y = ((*m[0]).y * w0) + ((*m[1]).y * w1) + ((*m[2]).y * w2);
    }

    material->setPoint(point, point2d);
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

    //if (det > -EPSILON && det < EPSILON) {
    if (det < EPSILON) {
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
    
    Vector f0;
    V_SUB(f0, (*v[0]), point);

    Vector f1;
    V_SUB(f1, (*v[1]), point);

    Vector f2;
    V_SUB(f2, (*v[2]), point);

    Vector xa;
    V_CROSS(xa, edge[0], edge[1]);

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
