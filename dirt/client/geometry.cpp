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
// REVISION by Tonic, on 1/15/2004
// Comments: Changed reflect -> Reflect
// Changed interfaces to get parameters as references
// instead of pointers
//*********************************************************
// REVISION by VADER, on 1/15/2004
// Comments: Names changed according to standart naming conventions
//
//*********************************************************
// REVISION by ..., on ...
// Comments: ...
//
//*********************************************************

#include "stdafx.h"
#include "geometry.h"


////////////////////////////// Sphere methods //////////////////////////////

CSphere::CSphere()
{  
    m_position.x = 0;
	m_position.y = 0;
	m_position.z = 0;
	m_radius = 1;
	//radius2 = 1;
}

CSphere::CSphere( CVector &position, double radius)
{
	ASSERT( radius > 0);
		
	// = is overridden operator for CVector, makes components the same

	m_position = position;
	m_radius = radius;
	//radius2=radius*radius;
};

void CSphere::SetPosition( CVector &position)
{

	// = is overridden operator for CVector, makes components the same
	m_position = position;
};

void CSphere::SetRadius(double radius)
{
	ASSERT ( radius > 0);
	
	m_radius = radius;
	//radius2 = radius*radius;
};

int CSphere::Intersect(  Ray &ray, double &distance) 
{
	CVector origin, direction;
	
	ray.getOrigin(origin); 
	ray.getDirection(direction);

	CVector l = m_position - origin; //vector from ray origin to center of Sphere
	double l2 = l * l;
	double proection = l * direction; //proection of l on ray direction
	
	if(proection<=0) //no intersection
	{
		return 0; 
	}

	double condition = proection * proection + m_radius * m_radius - l2; //condition of intersection
															 // (simple Pifagor Th.)

	if(condition<=0)  //no intersection
	{
		return 0;
	}
	else
	{
		distance = proection - sqrt(condition);
		return 1;
	}
};

void CSphere::Reflect( Ray &falling, Ray &reflected)
{
	double distance;
	CVector fallingOrigin,fallingDirection;
	CVector reflectedOrigin,reflectedDirection;

  // 	ASSERT (Intersect(falling, &distance)); This does NOTHING in RELEASE version.
  //                                          Intersect() is NOT called.

  int ret = Intersect(falling, distance); //gets the distance
	ASSERT (ret); 
	
	falling.getOrigin(fallingOrigin);
	falling.getDirection(fallingDirection);

	reflectedOrigin = fallingOrigin + distance * fallingDirection;
	reflected.setOrigin(reflectedOrigin);

	CVector normal = (reflectedOrigin - m_position)/m_radius;

	//newDirection = oldDirection + delta( in the derection of normal to the surface)
	reflectedDirection = fallingDirection - 2*(normal * fallingDirection)*normal;
	reflected.setDirection(reflectedDirection);
};


////////////////////////////// Plane methods //////////////////////////////


CPlane::CPlane()
{
	m_n.x = 1;
	m_n.y = 0;
	m_n.z = 0;
	m_D = 0;
};


CPlane::CPlane( CVector &n, double D)
{
	m_n = n;
	m_n.Normalize();
	m_D = D;	
};

CPlane::CPlane(double a, double b, double c, double d)
{
	ASSERT (a!=0 || b!=0 || c!=0);

	m_n = CVector (a, b, c);
	double length = m_n.Length();

	m_n /= length;
	m_D = d/length;
};
	
void CPlane::SetPosition(CVector &n, double D)
{
	m_n = n;
	m_D = D;
};

void CPlane::SetPosition(double a, double b, double c, double d)
{
	ASSERT (a!=0 || b!=0 || c!=0);

	m_n = CVector (a, b, c);
	double length = m_n.Length();

	m_n /= length;
	m_D = d/length;
};

int CPlane::Intersect( Ray &ray, double &distance)
{
	CVector origin, direction;
	
	ray.getOrigin(origin);
	ray.getDirection(direction);

	double scalar = m_n * direction;
	double t;

	if (scalar == 0)  //ray is parallel to plane
	{
		return 0;
	}
	else 
	{
		//if equation  {origin + t * direction = r}, where r satisfies plane equation
		//has positive solutions, then intersection takes place
		t = - (m_D + m_n * origin) / scalar;
		
		if( t <= 0)  //no intersection
		{
			return 0;
		}
		else
		{
			distance = t;
			return 1;
		}
	}
};

void CPlane::Reflect(Ray &falling, Ray &reflected)
{
	double distance;
	CVector fallingOrigin,fallingDirection;
	CVector reflectedOrigin,reflectedDirection;

	int ret = Intersect(falling, distance); //gets the distance
	ASSERT (ret); 
  

  falling.getOrigin(fallingOrigin);
	falling.getDirection(fallingDirection);

	reflectedOrigin = fallingOrigin + distance * fallingDirection;
	reflected.setOrigin(reflectedOrigin);

	//newDirection = oldDirection + delta( in the derection of normal to the surface)
	reflectedDirection = fallingDirection - 2 * (m_n * fallingDirection) * m_n;
	reflected.setDirection(reflectedDirection);
};

