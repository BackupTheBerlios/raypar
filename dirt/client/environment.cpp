//****************************************
//** environment.cpp **
// Created By: Tonic
// On: 12/11/2003
// Comments: Environment class methods implementation
//
//***********************************
// COMMENT by Tonic, on 12/11/2003
// initial revision, implemented data-centric properties only
//*********************************************************
// REVISION by KIRILL, on 1/9/2004 03:10:10
// Comments: I've modified header a little. My macroses are stupid 
// and don't understand your (TONIC's) idea of header formating :)
//***********************************
// REVISION by Tonic, on 14/11/2003
// Comments:	Adding functionality to Environment( Intersect method )
//***********************************
// REVISION by Tonic, on 12/01/2004
// Comments:	Getters and setters for Ray, Light
//				components, getter for Enviroment light sources
//				Added CVector position to Light, modified constructors
//				Added Ray constructor without parameters
//***********************************
// REVISION by Tonic, on 1/13/2004
// Comments: Added normalization to Ray::setDirection so that
// direction is always of length 1 inside the Ray
//*********************************************************
// REVISION by Tonic, on 01/15/2004
// Comments: Added Environment::AmbientLight, refactoring of class members names
// Added Solid::GetColor returning always white (no check of actual intersection
// with a given ray)
// Changed interfaces to get parameters as references
// instead of pointers
//*********************************************************

#include "stdafx.h"
#include "environment.h"

void Solid::GetColor( Ray &falling, CVector &color)
	{
	//do not check whether there is even a ray
	//just return white
	//more strict implementations in descendants
	color.x = color.y = color.z = 1;
	};

void	Environment::SetAmbientColor(  CVector &AmbientColor )
	{
	ASSERT( 0.0 <= AmbientColor.x && AmbientColor.x <= 1.0);
	ASSERT( 0.0 <= AmbientColor.y && AmbientColor.y <= 1.0);
	ASSERT( 0.0 <= AmbientColor.z && AmbientColor.z <= 1.0);

	m_AmbientColor = AmbientColor;
	};

void	Environment::GetAmbientColor( CVector &AmbientColor )
	{
	ASSERT( &AmbientColor != NULL);

	AmbientColor = m_AmbientColor;
	}

Light::Light( double r, double g, double b, double x, double y, double z)
	{
	ASSERT( 0.0 <= r && r <= 1.0);
	ASSERT( 0.0 <= g && g <= 1.0);
	ASSERT( 0.0 <= b && b <= 1.0);

	m_color.x = r;
	m_color.y = g;
	m_color.z = b;
	m_position.x = x;
	m_position.y = y;
	m_position.z = z;

	};

void Light::getPosition(CVector &position)
	{
	position = m_position;
	};

void Light::setPosition( CVector &position)
	{
	m_position = position;
	};

void Light::getColor(CVector &color)
	{
	color = m_color;
	};

void Light::setColor( CVector &color)
	{
	ASSERT( (color.x >= 0) && (color.x <= 1) );
	ASSERT( (color.y >= 0) && (color.y <= 1) );
	ASSERT( (color.z >= 0) && (color.z <= 1) );

	m_color = color;
	}

void Environment::getLightByNumber(int number, Light &light)
	{
	//there is a light source with such a number
	ASSERT( (number >= 0) && (number < m_lightsCount) );

	light = *(m_lights[number]);
	}


void Ray::getDirection(CVector &direction)
	{
	// = is overridden operator, just makes the components the same
	direction = m_direction;
	}

void Ray::setDirection( CVector &direction)
	{
	ASSERT( direction.Length() != 0);

	// = is overridden operator, just makes the components the same
	m_direction = direction;

	//make the direction length 1
	(m_direction).Normalize();
	}


void Ray::getOrigin(CVector &origin)
	{
	// = is overridden operator, just makes the components the same
	origin = m_origin;
	}


void Ray::setOrigin( CVector &origin)
	{
	// = is overridden operator, just makes the components the same
	m_origin = origin;
	}

Ray::Ray(  CVector &origin,  CVector &direction )
	{
	ASSERT( direction.Length() != 0);

	m_origin = origin;
	m_direction = direction;
	(m_direction).Normalize();
	};

//determines the closest intersected object and distance
Solid * Environment::Intersect (  Ray &ray, double &t )
	{
	Solid*	closestObj = NULL;
	//do not allow intersections with objects
	//farther than t away
	double	closestDist = t;

	//for every object
	for ( int i = 0; i < m_solidsCount; i++ )
		{
		//find intersection
		if ( m_solids [i] -> Intersect ( ray, t ) != 0 )
			{
			//check distance
			if ( t < closestDist )
				{
				//update if closer
				closestDist = t;
				closestObj  = m_solids [i];
				}
			};
		};

	t = closestDist;
	return closestObj;
	};


Environment::Environment( )
	{
	m_lightsCount = m_solidsCount = 0;
	m_AmbientColor.x = m_AmbientColor.y = m_AmbientColor.z = 0.2;
	};

void Environment::Add ( Light *light )
	{
	ASSERT( light != NULL );

	int i;

	//check if the maximum number of elements was reached
	//if it holds, return
	if( m_lightsCount == MAX_LIGHTS )
		return;

	//check if this element is already present
	//if it holds, do not add repeatedly, just return
	for( i = 0; i < m_lightsCount; i++ )
		{
		if( m_lights[i] == light )
			return;
		}

	//actually add light source to environment
	m_lights[m_lightsCount] = light;
	m_lightsCount++;
	};

void Environment::Add ( Solid *solid )
	{
	ASSERT( solid != NULL );

	int i;

	//check if the maximum number of elements was reached
	//if it holds, return
	if( m_solidsCount == MAX_SOLIDS )
		return;

	//check if this element is already present
	//if it holds, do not add repeatedly, just return
	for( i = 0; i < m_solidsCount; i++ )
		{
		if( m_solids[i] == solid )
			return;
		}

	//actually add element to environment
	m_solids[m_solidsCount] = solid;
	m_solidsCount++;
	};

Light::Light()
	{
	m_color.x = m_color.y = m_color.z = 1.0;
	};

Light::Light(  CVector &color,  CVector &position )
	{
	ASSERT( (color.x >= 0) && (color.x <= 1) );
	ASSERT( (color.y >= 0) && (color.y <= 1) );
	ASSERT( (color.z >= 0) && (color.z <= 1) );

	m_color = color;
	m_position = position;
	};