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

#include "stdafx.h"
#include "environment.h"

Light::Light( double r, double g, double b, double x, double y, double z)
{
	ASSERT( 0.0 <= r && r <= 1.0);
	ASSERT( 0.0 <= g && g <= 1.0);
	ASSERT( 0.0 <= b && b <= 1.0);

	color.x = r;
	color.y = g;
	color.z = b;
	position.x = x;
	position.y = y;
	position.z = z;

};

void Light::getPosition(CVector *position)
{
	ASSERT( position != NULL );

	*position = this->position;
};

void Light::setPosition(CVector *position)
{
	ASSERT( position != NULL );

	this->position = *position;
};

void Light::getColor(CVector *color)
{
	ASSERT( color != NULL );

	*color = this->color;
};

void Light::setColor(CVector *color)
{
	ASSERT( color != NULL);
	ASSERT( (color->x >= 0) && (color->x <= 1) );
	ASSERT( (color->y >= 0) && (color->y <= 1) );
	ASSERT( (color->z >= 0) && (color->z <= 1) );

	this->color = *color;
}

void Environment::getLightByNumber(int number, Light *light)
{
	//there is a light source with such a number
	ASSERT( (number >= 0) && (number < lightsCount) );

	*light = *(lights[number]);
}


void Ray::getDirection(CVector *direction)
{
	ASSERT( direction != NULL);

	// = is overridden operator, just makes the components the same
	*direction = this->direction;
}

void Ray::setDirection(CVector *direction)
{
	ASSERT( direction != NULL);
	ASSERT( direction->Length() != 0);

	// = is overridden operator, just makes the components the same
	this->direction = *direction;
	
	//make the direction length 1
	(this->direction).Normalize();
}


void Ray::getOrigin(CVector *origin)
{
	ASSERT( origin != NULL );

	// = is overridden operator, just makes the components the same
	*origin = this->origin;
}


void Ray::setOrigin(CVector *origin)
{
	ASSERT( origin != NULL );

	// = is overridden operator, just makes the components the same
	this->origin = *origin;
}

Ray::Ray( CVector *origin, CVector *direction )
{
	ASSERT( origin != NULL);
	ASSERT( direction != NULL);
	ASSERT( direction->Length() != 0);

	this->origin = *origin;
	this->direction = *direction;
	(this->direction).Normalize();
};

//determines the closest intersected object and distance
Solid * Environment::Intersect ( Ray *ray, double *t )
{
	ASSERT( ray != NULL );
	ASSERT( t != NULL );

	Solid*	closestObj = NULL;
	//do not allow intersections with objects
	//farther than t away
	double	closestDist = *t;

	//for every object
	for ( int i = 0; i < solidsCount; i++ )
	{
		//find intersection
		if ( solids [i] -> Intersect ( ray, t ) != NULL )
		{
			//check distance
			if ( *t < closestDist )
			{
				//update if closer
				closestDist = *t;
				closestObj  = solids [i];
			}
		};
	};

	*t = closestDist;
	return closestObj;
};


Environment::Environment( )
{
	lightsCount = solidsCount = 0;
};

void Environment::Add ( Light *light )
{
	ASSERT( light != NULL );
	
	int i;
	
	//check if the maximum number of elements was reached
	//if it holds, return
	if( lightsCount == MAX_LIGHTS )
		return;
	
	//check if this element is already present
	//if it holds, do not add repeatedly, just return
	for( i = 0; i < lightsCount; i++ )
	{
		if( lights[i] == light )
			return;
	}
	
	//actually add light source to environment
	lights[lightsCount] = light;
	lightsCount++;
};

void Environment::Add ( Solid *solid )
{
	ASSERT( solid != NULL );
	
	int i;
	
	//check if the maximum number of elements was reached
	//if it holds, return
	if( solidsCount == MAX_SOLIDS )
		return;
	
	//check if this element is already present
	//if it holds, do not add repeatedly, just return
	for( i = 0; i < solidsCount; i++ )
	{
		if( solids[i] == solid )
			return;
	}
	
	//actually add element to environment
	solids[solidsCount] = solid;
	solidsCount++;
};

Light::Light()
{
	color.x = color.y = color.z = 1.0;
};

Light::Light( CVector *color, CVector *position )
{
	ASSERT( color != NULL);
	ASSERT( position != NULL);
	ASSERT( (color->x >= 0) && (color->x <= 1) );
	ASSERT( (color->y >= 0) && (color->y <= 1) );
	ASSERT( (color->z >= 0) && (color->z <= 1) );

	this->color = *color;
	this->position = *position;
};