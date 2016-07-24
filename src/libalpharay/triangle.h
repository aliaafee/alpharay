/* vim: set ts=4 ss=4 sw=4 noet ai cindent : */

#ifndef _TRIANGLE_H_
#define _TRIANGLE_H_

#include "baseobject.h"
#include "vertex.h"


class Triangle : public BaseObject
{
    public:
        void init() {}

        Triangle(unsigned long index, BaseObject* parent) { 
			init(); 
			parent_ = parent; 
			index_ = index;
		}

        ~Triangle() {};

        void set(Vertex*  v0, Vertex*  v1, Vertex*  v2, 
                 Normal*  n0, Normal*  n1, Normal*  n2, 
                 MapPoint2* m0, MapPoint2* m1, MapPoint2* m2, Vector face_n_);

        void transform();

        Bounds bounds();
        
        Vector normal(Vector point, Material* material);
        void setPoint(Vector point, Material* material);

        //Material material() { return parent_->material(); }
		virtual void copyMaterial(Material* target) {
			parent_->copyMaterial(target);
		}

        BaseObject* intersection(Ray &ray, float *t, float limit);

    //protected:
        Vertex* v[3];
        Normal* n[3];
        MapPoint2* m[3];

        Vector face_n;
		Vector face_t;
		Vector face_b;

        Vector transformPoint(Vector &point) { return parent_->transformPoint(point); }
        Vector transformPointInv(Vector &point) { return parent_->transformPointInv(point); }
        Vector transformNormal(Vector &point) { return parent_->transformNormal(point); }
        Vector transformNormalInv(Vector &point) { return parent_->transformNormalInv(point); }
        Vector transformDisplacement(Vector &point) { return parent_->transformDisplacement(point); };
    	Vector transformDisplacementInv(Vector &point) { return parent_->transformDisplacementInv(point); };

    private:
		unsigned long index_;

        void calculateWeights(Vector point, float *w0, float *w1, float *w2);

		Vector edge[2];

		Vector T[3];
		Vector B[3];
		//Vector e0;
		//Vector e1;
};


#endif // _TRIANGLE_H_
