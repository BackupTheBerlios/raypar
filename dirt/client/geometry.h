//*********************************************************
//** geometry.h **
// Created By: Vader
// On: 01/12/2004
// Comments: Defines main geometrical objects, with implemented interface methods
//
//*********************************************************
// REVISION by Tonic, on 01/14/2004
// Comments: Added checking of previous includes of this file. 
// Multiple includes do not cause "type redefined" errors anymore
//*********************************************************
// REVISION by Tonic, on 01/15/2004
// Comments: Made Reflect and Intersect methods virtual so
// that they may be overriden in descendants.
// Changed reflect -> Reflect
// Changed interfaces to get parameters as references
// instead of pointers
//*********************************************************
// REVISION by VADER, on 01/15/2004
// Comments: Names changed according to standart naming conventions
//*********************************************************
// REVISION by Tonic, on 01/16/2004
// Comments: CColorSphere class - one-colour sphere
//*********************************************************
// REVISION by Tonic, on 01/17/2004
// Comments: Added CTriangle class
//*********************************************************

#if !defined(CLIENT_GEOMETRY_H_INCLUDED)
#define CLIENT_GEOMETRY_H_INCLUDED

#include "environment.h"

class CSphere : public CSolid
{
	CVector m_position;
	double m_radius;
//	double radius2;

public:
	//constructors

	CSphere();
	CSphere( CVector &position, double radius);

	void SetPosition( CVector &position);
	void SetRadius(double radius);
	
	virtual int Intersect(  Ray &ray, double &distance) ;
	virtual void Reflect( Ray &falling, Ray &reflected) ;
};

class CColorSphere : public CSphere
	{
	private:
		CVector m_color;

	public:
        CColorSphere();
		CColorSphere(CVector &position, double radius, CVector &color);
		CColorSphere(CVector &position, double radius);
		virtual void GetColor( Ray &falling, CVector &color);
		void SetColor( CVector &color );
	};

class CPlane : public CSolid
{
	CVector m_n;    // Plane is defined by equation
	double m_D;	  // (n,r) + D = 0;   |n| = 1 !!!
				  // D - is distance from center (may be <0)

public:
	//construction
	CPlane();
	CPlane( CVector &n, double D);
	CPlane(double a, double b, double c, double d); // for   ax + by + cz + d = 0
	
	void SetPosition( CVector &n, double D);
	void SetPosition(double a, double b, double c, double d);

	virtual int Intersect(  Ray &ray, double &distance) ;
	virtual void Reflect( Ray &falling, Ray &reflected) ;
};

class CTriangle : public CSolid
	{
	private:
		CVector m_a, m_b, m_c, m_color, m_normal;
		double m_distance;

		//plane equation is (m_normal,x) = m_distance

		int planeIntersect( Ray &ray, double &distance);
	
	public:
        CTriangle(CVector &a, CVector &b, CVector &c, CVector &color);
		//white by default
		//CTriangle(CVector &a, CVector &b, CVector &c);

		virtual void GetColor( Ray &falling, CVector &color);
		virtual int Intersect(  Ray &ray, double &distance);
		virtual void Reflect( Ray &falling, Ray &reflected);

	};

#endif //CLIENT_GEOMETRY_H_INCLUDED