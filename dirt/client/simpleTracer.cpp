//****************************************
//** simpleTracer.cpp **
// Created By: Tonic
// On: 01/10/2004
// Comments: simple implementation of tracer class
//
//***********************************
// REVISION by Tonic, on 1/15/2004
// Comments: bugfix in SipleTracer::strace
//*********************************************************
// REVISION by Tonic, on 01/14/2004
// Comments: Added SimpleTracer::VisibleColor to combine light colr with
// material color
// Changed interfaces to get parameters as references
// instead of pointers
//*********************************************************
// REVISION by Tonic, on 1/17/2004
// Comments: Added CRenderer class containing pixel color computing routine
//*********************************************************

#include "stdafx.h"
#include "simpleTracer.h"

void CRenderer::RenderPixel( Environment &scene, Medium &medium, CCamera &camera, Tracer &tracer, int x, int y, CVector &color)
	{
	ASSERT((x>=0) && (y>=0));

	Ray ray;
	camera.PixelRay(x,y,ray);
	tracer.trace( medium, ray, scene, 1.0, color );
	};

void SimpleTracer::VisibleColor( CVector &LightColor,  CVector &MaterialColor, CVector &resultColor)
	{
	//allow color components to be graeter than 1
	//normalization occures after this function
	ASSERT( 0 <= LightColor.x);
	ASSERT( 0 <= LightColor.y);
	ASSERT( 0 <= LightColor.z);
	ASSERT( 0 <= MaterialColor.x && MaterialColor.x <= 1.0);
	ASSERT( 0 <= MaterialColor.y && MaterialColor.y <= 1.0);
	ASSERT( 0 <= MaterialColor.z && MaterialColor.z <= 1.0);
	
	//the result color is just componentwise product of LightColor and MaterialColor
	resultColor.x = LightColor.x*MaterialColor.x;
	resultColor.y = LightColor.y*MaterialColor.y;
	resultColor.z = LightColor.z*MaterialColor.z;
	}

void SimpleTracer::trace( Medium &curMed,  Ray &ray,  Environment &scene, double weight, CVector &resultColor)
{
	strace(curMed, ray, scene, weight, resultColor, defaultDepth);
}

void SimpleTracer::strace( Medium &curMed,  Ray &ray,  Environment &scene, double weight, CVector &resultColor, int depth)
{
	//the function is called only by our class member, 
	//so there is no need to check pointers once again
	CSolid	*nearestObject;
	
	double	  t = INFINITY;
	CVector	  Color;
	
	resultColor.x = 0;
	resultColor.y = 0;
	resultColor.z = 0;
	
	
	nearestObject = scene.Intersect(ray, t);
	if ( nearestObject != NULL )	// ray hit some object
	{
		//now we
		// - calculate the color due to light unshaded sources
		// - calculate the direction of reflected ray and run the tracing routine on it
		// - (to be added later) process the refracted ray
		
		Ray reflectedRay, normal;
		CVector fallingDir, reflectedDir, normalDir, normalPos;
		//first compute the reflected ray direction
		nearestObject->Reflect( ray, reflectedRay );
		
		//origin of normal is that of reflected ray
		//this normalPos is used in the following loop!!
		//do not change it!
		reflectedRay.getOrigin( normalPos );
		normal.setOrigin( normalPos );
		
		//normal direction = reflected direction - falling direction
		//we need to normalize the direction vectors to get correct normal direction
		ray.getDirection( fallingDir );
		reflectedRay.getDirection( reflectedDir );
		fallingDir.Normalize();
		reflectedDir.Normalize();
		//this normalDir is used in the following loop!!
		//do not change it!
		normalDir = reflectedDir - fallingDir;
		normalDir.Normalize();
		normal.setDirection( normalDir);
		
		//for all light sources
		for( int i = 0; i < scene.getLightsCount() ; i++)
		{
			CLight light;
			CVector lightPosition, lightDirection;
			
			scene.getLightByNumber( i, light );
			light.getPosition( lightPosition );
			lightDirection = lightPosition - normalPos;
						
			//the light is on the proper side of the tangent plane
			if ( (lightDirection)*(normalDir) > 0)
			{	
				double dist = lightDirection.Length();
				
				//Tonic: bugfix : lightpos -> normalpos
				Ray		lightRay( normalPos, lightDirection );
				//there is no object between us and the light source
				//we can compute this source's contribution
				if(scene.Intersect(lightRay, dist) == NULL)
				{
					CVector lightColor;
					lightDirection.Normalize();
					light.getColor( lightColor );
					lightColor /= (shadeA + shadeB*dist + shadeC*dist*dist);
					lightColor *= shadeRoD*(normalDir*lightDirection);
					resultColor += lightColor;
				};
			};
		};

		//now we trace the reflected ray if the recursion depth
		//limit has not been exceeded
		if(depth > 0)
		{
			CVector reflectedLight(0,0,0);

			strace(curMed, reflectedRay, scene, weight, reflectedLight, depth - 1);
			resultColor += reflectedLight;
		}
	
		//allow for ambient light
		CVector AmbientColor;
		scene.GetAmbientColor( AmbientColor );
		resultColor += AmbientColor;

		//allow for material color
		CVector materialColor;
		nearestObject->GetColor( ray, materialColor );
		VisibleColor( resultColor, materialColor, resultColor );

		//we can have some components of the color greater than 1
		//If this has happened, we normalize the color making
		//the maximum component equal to 1

		if( (resultColor.x > 1) || (resultColor.y > 1) || (resultColor.z > 1))
		{
			double maxComp = (resultColor.x > resultColor.y) ? resultColor.x : resultColor.y;
			maxComp = (maxComp > resultColor.z) ? maxComp : resultColor.z;
			resultColor /= maxComp;
		}

		//finally compute the attenuation due to distance

		resultColor *= exp ( -t * curMed.Betta ); 
	};

};