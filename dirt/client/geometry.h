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
//*********************************************************
// REVISION by VADER, on 01/15/2004
// Comments: Names changed according to standart naming conventions
//*********************************************************

#if !defined(CLIENT_GEOMETRY_H_INCLUDED)
#define CLIENT_GEOMETRY_H_INCLUDED


#include "environment.h"

class CSphere : public Solid
{
	CVector m_position;
	double m_radius;
//	double radius2;

public:
	//constructors
	CSphere();
	CSphere(CVector *position, double radius);
	
	void SetPosition(CVector *position);
	void SetRadius(double radius);
	
	int Intersect( Ray *ray, double *distance);
	void Reflect(Ray *falling, Ray *reflected);
};



class CPlane : public Solid
{
	CVector m_n;    // Plane is defined by equation
	double m_D;	  // (n,r) + D = 0;   |n| = 1 !!!
				  // D - is distance from center (may be <0)

public:
	//construction
	CPlane();
	CPlane(CVector *n, double D);
	CPlane(double a, double b, double c, double d); // for   ax + by + cz + d = 0
	
	void SetPosition(CVector *n, double D);
	void SetPosition(double a, double b, double c, double d);

	int Intersects(Ray *ray, double* distance);
	void Reflect(Ray *falling, Ray *reflected);
};

#endif //CLIENT_GEOMETRY_H_INCLUDED