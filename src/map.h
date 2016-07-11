/* vim: set ts=4 ss=4 sw=4 noet ai cindent : */

#ifndef _MAP_H_
#define _MAP_H_

#include "vector.h"
#include "matrix4.h"
#include "xmlobject-named.h"
#include "image.h"
#include "linklist.h"


class Map : virtual public XmlObjectNamed
{
    public:
        virtual void init() { ; }
        
        Map() 
            :XmlObjectNamed("map", "")
            { init(); }

        Map( std::string name ) 
            :XmlObjectNamed("map", name)
            { init(); }

        ~Map() { } ;

        virtual void transform() {};

        virtual Color color(Vector  point, Vector2 point2) { return Color(0, 0, 0); }

		virtual void getTangents(Vector& point, Vector* os_tangent, Vector* os_bitangent) {;}
};


class Map2d : public Map, virtual public XmlObjectNamed
{
    public:
        virtual void init();
        
        Map2d( std::string name ) 
            : XmlObjectNamed("map2d", name)
            { init(); }

        ~Map2d() { } ;

		virtual void transform() { Map::transform(); }

        virtual Color color(Vector  point, Vector2 point2);

    protected:
        Image* image_;
        Vector2 imageScale_;

        virtual Color color(Vector2 point2);
};


class Map2dPlane : public Map2d, virtual public XmlObjectNamed
{
    public:
        virtual void init();
        
        Map2dPlane( std::string name ) 
            : Map2d(name), XmlObjectNamed("map2dplane", name)
            { init(); }

        ~Map2dPlane() { } ;

        virtual void transform();

        virtual Color color(Vector  point, Vector2 point2);

		virtual void getTangents(Vector& point, Vector* os_tangent, Vector* os_bitangent);

    protected:
        Vector position_;
	    Vector rotation_;
	    Vector scale_;

		Vector os_tangent_;
		Vector os_bitangent_;

        Vector transformPoint(Vector &point);
        Vector transformPointInv(Vector &point);
		Vector transformNormal(Vector &normal);
		Vector transformNormalInv(Vector &normal);
        
    private:
        Matrix4 transMatrix; // TRS
	    Matrix4 transMatrixInv; // S^R^T^
		Matrix4 transMatrixNormal; // RS^
        Matrix4 transMatrixNormalInv; // SR^
};


class Map2dCylindrical : public Map2dPlane, virtual public XmlObjectNamed
{
    public:
        virtual void init() { ; }
        
        Map2dCylindrical( std::string name ) 
            : Map2dPlane(name), XmlObjectNamed("map2dcylindrical", name)
            { init(); }

        ~Map2dCylindrical() { } ;

		virtual void transform();

        virtual Color color(Vector  point, Vector2 point2);

		virtual void getTangents(Vector& point, Vector* os_tangent, Vector* os_bitangent);
};


class Map2dSpherical : public Map2dPlane, virtual public XmlObjectNamed
{
    public:
        virtual void init() { pole_ = Vector(0, 0, 1); }
        
        Map2dSpherical( std::string name ) 
            : Map2dPlane(name), XmlObjectNamed("map2dspherical", name)
            { init(); }

        ~Map2dSpherical() { } ;

        virtual Color color(Vector  point, Vector2 point2);

		virtual void getTangents(Vector& point, Vector* os_tangent, Vector* os_bitangent);
		
	private:
		Vector pole_;
};

#endif // _MAP_H_
