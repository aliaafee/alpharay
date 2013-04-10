/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */

#include "boolean.h"


Boolean::Boolean (Object *A, char booleanOperator, Object *B) {
	material_ = NULL;

	position_ = Vector(0,0,0);
	rotation_ = Vector(0,0,0);
	scale_ = Vector(1,1,1);
	normalDirection_ = 1;
	parent_ = NULL;

	A_ = A;
	A_->parent_ = this;
	B_ = B;
	B_->parent_ = this;
	B_->flipNormal();
	booleanOperator_ = booleanOperator;
}

void Boolean::transform() {
	GenerateTransformationMatrix();
	A_->transform ();
	B_->transform ();
}

Vector Boolean::getColor(Vector &intersectionPoint) {
	return Vector(255,0,0);
}

bool Boolean::isInside(Vector point) {
	return A_->isInside(point) && !B_->isInside(point);
}

void Boolean::flipNormal() {
	A_->flipNormal();
	B_->flipNormal();
	normalDirection_ *= -1;
}
	
Object* Boolean::intersection(Ray &ray,Vector *intersectionPoint ,Vector *intersectionNormal,float *distance) {
	
	Vector intApoint,intANormal;
	float intAdistance;
	Object *intA = A_->intersection(ray,&intApoint,&intANormal,&intAdistance);

	Vector intBpoint,intBNormal;
	float intBdistance;
	Object *intB = B_->intersection(ray,&intBpoint,&intBNormal,&intBdistance);

	if (intB == NULL && intA == NULL)
		return NULL;
	
	if (intA == NULL && intB != NULL) {
		return NULL;
	}

	if (intA != NULL && intB == NULL) {
        //return NULL;
		*intersectionPoint = intApoint;
		*intersectionNormal = intANormal;
		*distance = intAdistance;
		return intA;
	}
    
    //for nots
    if (intA != NULL && intB != NULL) {
        bool pointBinA = A_->isInside(intBpoint);
        bool pointAinB = B_->isInside(intApoint);
        
        if (!pointAinB && pointBinA) {
            if (intAdistance < intBdistance) {
                *intersectionPoint = intApoint;
                *intersectionNormal = intANormal;
                *distance = intAdistance;
                return intA;
            } else {
                *intersectionPoint = intBpoint;
                *intersectionNormal = intBNormal;
                *distance = intBdistance;
                return intB;
            }
        }
        
        if (pointAinB && pointBinA) {
            *intersectionPoint = intBpoint;
            *intersectionNormal = intBNormal;
            *distance = intBdistance;
            return intB;
            
        }
        
        if (pointAinB && !pointBinA) {
            return NULL;
        }
        
        if (!pointAinB && !pointBinA) {
            *intersectionPoint = intApoint;
            *intersectionNormal = intANormal;
            *distance = intAdistance;
            return intA;
        }
        
        return NULL;
    }
    
    //This is for Intersections
    if (intA != NULL && intB != NULL) {
        bool pointBinA = A_->isInside(intBpoint);
        bool pointAinB = B_->isInside(intApoint);
        
        if (pointAinB || pointBinA) {
            if (intAdistance < intBdistance) {
                *intersectionPoint = intApoint;
                *intersectionNormal = intANormal;
                *distance = intAdistance;
                return intA;
            } else {
                *intersectionPoint = intBpoint;
                *intersectionNormal = intBNormal;
                *distance = intBdistance;
                return intB;
            }
        }
        return NULL;
    }
    
    
    if (intA != NULL && intB != NULL) { 
        bool pointBinA = A_->isInside(intBpoint);
        bool pointAinB = B_->isInside(intApoint);
        
        *intersectionPoint = intApoint;
        *intersectionNormal = intANormal;
        *distance = intAdistance;
        return intA;
        
        
        //if (pointBinA) {
            //if (!pointAinB) {
                *intersectionPoint = intBpoint;
                *intersectionNormal = intBNormal;
                *distance = intBdistance;
                return intB;
            //}
        //}
        
        return NULL;
        
        if (!pointAinB && !pointBinA) {
            *intersectionPoint = intApoint;
            *intersectionNormal = intANormal;
            *distance = intAdistance;
            return intA;
        }
        
        if (pointAinB && pointBinA) {
            *intersectionPoint = intBpoint;
            *intersectionNormal = intBNormal;
            *distance = intBdistance;
            return intB;
        }
        
        if (!pointAinB && pointBinA) {
            *intersectionPoint = intApoint;
            *intersectionNormal = intANormal;
            *distance = intAdistance;
            return intA;
        }
        
        /*
        if (pointBinA && pointAinB) {
            return NULL;
            *intersectionPoint = intBpoint;
            *intersectionNormal = intBNormal;
            *distance = intBdistance;
            return intB;
        }
        
        
        
        if (pointBinA && !pointAinB) {
            *intersectionPoint = intBpoint;
            *intersectionNormal = intBNormal;
            *distance = intBdistance;
            return intB;
        }
        
        if (!pointBinA || !pointAinB) {
            *intersectionPoint = intBpoint;
            *intersectionNormal = intBNormal;
            *distance = intBdistance;
            return intB;
        }*/
    }
    
    return NULL;

	//what to do when both are not null

	bool pointBinA = A_->isInside(intBpoint);
	bool pointAinB = B_->isInside(intApoint);
    
    if (!pointAinB) {
        return NULL;
        *intersectionPoint = intApoint;
		*intersectionNormal = intANormal;
		*distance = intAdistance;
		return intA;
    }
    
    if (pointAinB) {
        *intersectionPoint = intBpoint;
		*intersectionNormal = intBNormal;
		*distance = intBdistance;
		return intB;
    }
    
    /*

	if (pointBinA && pointAinB) {
		*intersectionPoint = intBpoint;
		*intersectionNormal = intBNormal;
		*distance = intBdistance;
		return intB;
	}
	if(pointBinA && !pointAinB) {
		*intersectionPoint = intApoint;
		*intersectionNormal = intANormal;
		*distance = intAdistance;
		return intA;
	}
	if(!pointBinA && pointAinB) {
        return NULL;
        *intersectionPoint = intBpoint;
		*intersectionNormal = intBNormal;
		*distance = intBdistance;
		return intB;
	}
	if(!pointBinA && !pointAinB) {
        return NULL;
		*intersectionPoint = intApoint;
		*intersectionNormal = intANormal;
		*distance = intAdistance;
		return intA;
	}*/
	/*
	*intersectionPoint  = ray.position_ + (ray.direction_ * t);
	*intersectionNormal = transformNormal(Ro + (Rd *t)) * normalDirection_;
	*distance = t;
	
	return this;*/
	return NULL;
}
