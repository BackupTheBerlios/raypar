//****************************************
//** environment.cpp **
// Created By: Tonic
// On: 12/11/2003
// Comments: Environment class methods implementation
//
//***********************************
// COMMENT by Tonic, on 12/11/2003
// initial revision, implemented data-centric properties only
//
//*********************************************************
// REVISION by KIRILL, on 1/9/2004 03:10:10
// Comments: I've modified header a little. My macroses are stupid 
// and don't understand your (TONIC's) idea of header formating :)
//
//*********************************************************
// REVISION by ..., on ...
// Comments: ...
//
//*********************************************************





#include "stdafx.h"
#include "environment.h"

Ray::Ray( CVector *origin, CVector *direction )
{
	ASSERT( origin != NULL);
	ASSERT( direction != NULL);

	this->origin = *origin;
	this->direction = *direction;
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

Light::Light( CVector *color )
{
	ASSERT( color != NULL);

	this->color = *color;
};

Light::Light( double r, double g, double b )
{
	ASSERT( 0.0 <= r && r <= 1.0);
	ASSERT( 0.0 <= g && g <= 1.0);
	ASSERT( 0.0 <= b && b <= 1.0);

	color.x = r;
	color.y = g;
	color.z = b;
};