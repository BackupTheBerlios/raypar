//****************************************
//** simpleTracer.cpp **
// Created By: Tonic
// On: 01/10/2004
// Comments: simple implementation of tracer class
//
//***********************************

#include "stdafx.h"
#include "simpleTracer.h"

void SimpleTracer::trace(Medium *curMed, Ray *ray, Environment *scene, double weight, CVector *resultColor)
{
	ASSERT(curMed != NULL);
	ASSERT(ray != NULL);
	ASSERT(scene != NULL);
	ASSERT(resultColor != NULL);
	
	strace(curMed, ray, scene, weight, resultColor, defaultDepth);
}

void SimpleTracer::strace(Medium *curMed, Ray *ray, Environment *scene, double weight, CVector *resultColor, int depth)
{
	//the function is called only by our class member, 
	//so there is no need to check pointers once again
	Solid	*nearestObject;
	
	double	  t = INFINITY;
	CVector	  Color;
	
	resultColor->x = 0;
	resultColor->y = 0;
	resultColor->z = 0;
	
	
	nearestObject = scene->Intersect(ray, &t);
	if ( nearestObject != NULL )	// ray hit some object
	{
		//now we
		// - calculate the color due to light unshaded sources
		// - calculate the direction of reflected ray and run the tracing routine on it
		// - (to be added later) process the refracted ray
		
		Ray reflectedRay, normal;
		CVector fallingDir, reflectedDir, normalDir, normalPos;
		//first compute the reflected ray direction
		nearestObject->reflect( ray, &reflectedRay );
		
		
		//origin of normal is that of reflected ray
		//this normalPos is used in the following loop!!
		//do not change it!
		reflectedRay.getOrigin( &normalPos );
		normal.setOrigin( &normalPos );
		
		//normal direction = reflected direction - falling direction
		//we need to normalize the direction vectors to get correct normal direction
		ray->getDirection( &fallingDir );
		reflectedRay.getDirection( &reflectedDir );
		fallingDir.Normalize();
		reflectedDir.Normalize();
		//this normalDir is used in the following loop!!
		//do not change it!
		normalDir = reflectedDir - fallingDir;
		normalDir.Normalize();
		normal.setDirection( &normalDir);
		
		//for all light sources
		for( int i = 0; i < scene->getLightsCount() ; i++)
		{
			Light light;
			CVector lightPosition, lightDirection;
			
			scene->getLightByNumber( i, &light );
			light.getPosition( &lightPosition );
			lightDirection = lightPosition - normalPos;
			
			//the light is on the proper side of the tangent plane
			if ( (lightPosition - normalPos)*(normalDir) > 0)
			{	
				double dist = lightDirection.Length();
				
				//there is no object between us and the light source
				//we can compute this source's contribution
				if(scene->Intersect(ray, &t) == NULL)
				{
					CVector lightColor;
					lightDirection.Normalize();
					light.getColor( &lightColor );
					lightColor /= (shadeA + shadeB*dist + shadeC*dist*dist);
					lightColor *= shadeRoD*(normalDir*lightDirection);
					*resultColor += lightColor;
				}
				
			}
		}

		//now we trace the reflected ray if the recursion depth
		//limit has not been exceeded
		if(depth > 0)
		{
			CVector reflectedLight(0,0,0);

			strace(curMed, &reflectedRay, scene, weight, &reflectedLight, depth - 1);
			*resultColor += reflectedLight;
		}

		//we can have some components of the color greater than 1
		//If this has happened, we normalize the color making
		//the maximum component equal to 1

		if( (resultColor->x > 1) || (resultColor->y > 1) || (resultColor->z > 1))
		{
			double maxComp = (resultColor->x > resultColor->y) ? resultColor->x : resultColor->y;
			maxComp = (maxComp > resultColor->z) ? maxComp : resultColor->z;
			*resultColor /= maxComp;
		}

		//finally compute the attenuation due to distance

		*resultColor *= exp ( -t * curMed->Betta ); 
	};

	//we do not take ambient light into account
	//it is to be added later
};