//*********************************************************
//** geometry.h **
// Created By: Vader
// On: 01/12/2004
// Comments: Defines main geometrical objects, with implemented interface methods
//
//*********************************************************

#include "environment.h"

class Sphere : public Solid
{
	CVector position;
	double radius;
	double radius2;

public:
	//constructors
	Sphere();
	Sphere(CVector *position, double radius);
	
	void setPosition(CVector *position);
	void setRadius(double radius);
	
	int Intersect( Ray *ray, double *distance);
	void reflect(Ray *falling, Ray *reflected);
};



class Plane : public Solid
{
	CVector n;    // Plane is defined by equation
	double D;	  // (n,r) + D = 0;   |n| = 1 !!!
				  // D - is distance from center (may be <0)

public:
	//construction
	Plane();
	Plane(CVector *n, double D);
	Plane(double a, double b, double c, double d); // for   ax + by + cz + d = 0
	
	void setPosition(CVector *n, double D);
	void setPosition(double a, double b, double c, double d);

	int Intersects(Ray *ray, double* distance);
	void reflect(Ray *falling, Ray *reflected);
};


