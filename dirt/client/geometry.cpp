//*********************************************************
//** geometry.cpp **
// Created By: Vader
// On: 01/12/2004
// Comments: Geometry objects class methods implementation
//
//*********************************************************
// REVISION by KIRILL, on 1/14/2004 20:54:59
// Comments: error in Sphere::reflect and Plane::reflect
//  with ASSERT(.) corrected
//
//*********************************************************
// REVISION by ..., on ...
// Comments: ...
//
//*********************************************************

#include "stdafx.h"
#include "geometry.h"


////////////////////////////// Sphere methods //////////////////////////////

Sphere::Sphere()
{  
  position.x = 0;
	position.y = 0;
	position.z = 0;
	radius = 1;
	radius2 = 1;
}

Sphere::Sphere(CVector *position, double radius)
{
	ASSERT( position != NULL);
	ASSERT( radius > 0);
		
	// = is overridden operator for CVector, makes components the same
	this->position = *position;
	this->radius = radius;
	radius2=radius*radius;
};

void Sphere::setPosition(CVector *position)
{
	ASSERT( position != NULL);
	
	// = is overridden operator for CVector, makes components the same
	this->position = *position;
};

void Sphere::setRadius(double radius)
{
	ASSERT ( radius > 0);
	
	this->radius = radius;
	radius2 = radius*radius;
};

int Sphere::Intersect(Ray *ray, double *distance)
{
	CVector origin, direction;
	
	ray->getOrigin(&origin); 
	ray->getDirection(&direction);

	CVector l = position - origin; //vector from ray origin to center of Sphere
	double l2 = l * l;
	double proection = l * direction; //proection of l on ray direction
	
	if(proection<=0) //no intersection
	{
		return 0; 
	}

	double condition = proection * proection + radius2 - l2; //condition of intersection
															 // (simple Pifagor Th.)

	if(condition<=0)  //no intersection
	{
		return 0;
	}
	else
	{
		*distance = proection - sqrt(condition);
		return 1;
	}
};

void Sphere::reflect(Ray *falling, Ray *reflected)
{
	double distance;
	CVector fallingOrigin,fallingDirection;
	CVector reflectedOrigin,reflectedDirection;

  
  // 	ASSERT (Intersect(falling, &distance)); This does NOTHING in RELEASE version.
  //                                          Intersect() is NOT called.

  int ret = Intersect(falling, &distance); //gets the distance
	ASSERT (ret); 
	
	falling->getOrigin(&fallingOrigin);
	falling->getDirection(&fallingDirection);

	reflectedOrigin = fallingOrigin + distance * fallingDirection;
	reflected->setOrigin(&reflectedOrigin);

	CVector normal = (reflectedOrigin - position)/radius;

	//newDirection = oldDirection + delta( in the derection of normal to the surface)
	reflectedDirection = fallingDirection - 2*(normal * fallingDirection)*normal;
	reflected->setDirection(&reflectedDirection);
};


////////////////////////////// Plane methods //////////////////////////////


Plane::Plane()
{
	n.x = 1;
	n.y = 0;
	n.z = 0;
	D = 0;
};


Plane::Plane(CVector *n, double D)
{
	ASSERT (n != NULL);

	this->n = *n;
	this->n.Normalize();
	this->D = D;	
};

Plane::Plane(double a, double b, double c, double d)
{
	ASSERT (a!=0 || b!=0 || c!=0);

	n = CVector (a, b, c);
	double length = n.Length();

	n /= length;
	D = d/length;
};
	
void Plane::setPosition(CVector *n, double D)
{
	ASSERT (n != NULL);
	this->n = *n;
	this->D = D;
};

void Plane::setPosition(double a, double b, double c, double d)
{
	ASSERT (a!=0 || b!=0 || c!=0);

	n = CVector (a, b, c);
	double length = n.Length();

	n /= length;
	D = d/length;
};

int Plane::Intersects(Ray *ray, double *distance)
{
	CVector origin, direction;
	
	ray->getOrigin(&origin);
	ray->getDirection(&direction);

	double scalar = n * direction;
	double t;

	if (scalar == 0)  //ray is parallel to plane
	{
		return 0;
	}
	else 
	{
		//if equation  {origin + t * direction = r}, where r satisfies plane equation
		//has positive solutions, then intersection takes place
		t = - (D + n * origin) / scalar;
		
		if( t <= 0)  //no intersection
		{
			return 0;
		}
		else
		{
			*distance = t;
			return 1;
		}
	}
};

void Plane::reflect(Ray *falling, Ray *reflected)
{
	double distance;
	CVector fallingOrigin,fallingDirection;
	CVector reflectedOrigin,reflectedDirection;

	int ret = Intersect(falling, &distance); //gets the distance
	ASSERT (ret); 
  
  falling->getOrigin(&fallingOrigin);
	falling->getDirection(&fallingDirection);

	reflectedOrigin = fallingOrigin + distance * fallingDirection;
	reflected->setOrigin(&reflectedOrigin);

	//newDirection = oldDirection + delta( in the derection of normal to the surface)
	reflectedDirection = fallingDirection - 2 * (n * fallingDirection) * n;
	reflected->setDirection(&reflectedDirection);
};

