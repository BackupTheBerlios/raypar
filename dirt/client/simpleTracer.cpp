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
// REVISION by Tonic, on 1/19/2004
// Comments: Fixed multiple ambient light addition, added 
// refracted rays support, removed unused double weight 
// from the trace parameters list
//*********************************************************
// REVISION by Tonic, on 01/20/2004
// Comments: Moved lights processing cycle of SimpleTracer 
// to a separate private function SimpleTracer::processLights
// reflection coefficient support
//*********************************************************
// REVISION by Tonic, on 01/21/2004
// Comments: Added variable material smoothness support to ProcessLights
//*********************************************************
// REVISION by Tonic, on 01/28/2004
// Comments: Added background color support to SimpleTracer
//*********************************************************
// REVISION by Tonic, on 01/29/2004
// Comments: Added transparency support for flat objects
//*********************************************************
// REVISION by Tonic, on 01/29/2004
// Comments: Fixed a bug in detecting object transparency 
// via reflectionCoefficient
//*********************************************************

#include "stdafx.h"
#include "simpleTracer.h"
#include "COMMON/msg.h"
#include "environment.h"

void SimpleTracer::processLights( const Medium &curMed, const CEnvironment &scene, 
                                 const Ray &normale, CVector &color, double smoothness ) const
{
  ASSERT( smoothness > VECTOR_EQUAL_EPS );
  
  //reset color
  color.x = color.y = color.z = 0;
  CVector normalPos, normalDir;
  
  normale.getOrigin( normalPos );
  normale.getDirection( normalDir );
  
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
        lightColor /= (m_shadeA + m_shadeB*dist + m_shadeC*dist*dist);
        //testing exponential attenuation
        lightColor *= exp ( -dist * curMed.Betta ); 
        lightColor *= m_shadeRoD*pow((normalDir*lightDirection),smoothness);
        color += lightColor;
      };
    };
  };
};

void SimpleTracer::VisibleColor( const CVector &LightColor, const CVector &MaterialColor, CVector &resultColor)
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
};

void SimpleTracer::trace( const Medium &curMed, const Ray &ray, 
                         const CEnvironment &scene, CVector &resultColor, bool outside) const
{
  strace(curMed, ray, scene, resultColor, m_defaultDepth, outside);
};

void SimpleTracer::strace( const Medium &curMed, const Ray &ray, 
                          const CEnvironment &scene, CVector &resultColor, int depth, bool outside) const
{
  CSolid	*nearestObject;
  double	  t = INFINITY;
  CVector	  Color;
  
  resultColor.x = 0;
  resultColor.y = 0;
  resultColor.z = 0;
  nearestObject = scene.Intersect(ray, t);
  if ( nearestObject != NULL ) // ray hit some object
  {
    Ray reflected, normal;
    CVector fallingDir, reflectedDir, normalDir, normalPos;
    
    double smoothness = nearestObject->GetSmoothness();
    ASSERT( smoothness > VECTOR_EQUAL_EPS );
    
    double reflectionCoefficient = nearestObject->GetReflectionCoefficient();
    ASSERT( (0 <= reflectionCoefficient) && (reflectionCoefficient <= 1.0) );
    //first compute the reflected ray direction
    nearestObject->Reflect( ray, reflected );
    
    //origin of normal is that of reflected ray
    //this normalPos is used in the following loop!!
    //do not change it!
    reflected.getOrigin( normalPos );
    normal.setOrigin( normalPos );
    //    Message("%.2f %.2f", normalPos.x, normalPos.z);
    
    //normal direction = reflected direction - falling direction
    //we need to normalize the direction vectors to get correct normal direction
    ray.getDirection( fallingDir );
    reflected.getDirection( reflectedDir );
    fallingDir.Normalize();
    reflectedDir.Normalize();
    //this normalDir is used in the following loop!!
    //do not change it!
    normalDir = reflectedDir - fallingDir;
    normalDir.Normalize();
    normal.setDirection( normalDir);
    
    //compute lights even if we are inside
    CVector lightColor;
    processLights( curMed, scene, normal, lightColor, smoothness );
    resultColor += lightColor*reflectionCoefficient;
    
    if(depth > 0)
    {
      CVector color(0,0,0), newOrigin;
      
      reflected.setOrigin( normalPos + VECTOR_EQUAL_EPS*reflectedDir );
      strace(curMed, reflected, scene, color, depth - 1, true);
      resultColor += color*reflectionCoefficient*m_shadeRoReflected;
      
      //now process the refracted ray
      if( nearestObject->GetReflectionCoefficient() < 1 - VECTOR_EQUAL_EPS ) //solid is NOT opaque
      {
        Ray refracted;
        bool newOutside;
        
        Medium newMedium;
        CVector refractedDir, color(0,0,0);
        nearestObject->Refract( ray, refracted, newMedium, newOutside);
        refracted.getDirection( refractedDir );
        
        if( refractedDir*normalDir < 0)
        {
          //no full inner reflection
          
          //compute the light in the refracted ray origin point
          normalDir *= -1;
          normal.setDirection( normalDir );
          normal.setOrigin(normalPos + VECTOR_EQUAL_EPS*normalDir );
          CVector lightColor;
          processLights( newMedium, scene, normal, lightColor, smoothness );
          resultColor += lightColor*(1 - reflectionCoefficient);
          
          refracted.setOrigin( normalPos + VECTOR_EQUAL_EPS*refractedDir );
          strace(newMedium, refracted, scene, color, depth - 1, newOutside);
          resultColor += color*(1 - reflectionCoefficient)*m_shadeRoRefracted;
        };
      };
    };
    //allow for ambient light
    //Tonic:1/19/2004: bugfix: no more adding 
    //ambient light for steps other than 1
    if ( depth == m_defaultDepth )
    {
      CVector AmbientColor;
      scene.GetAmbientColor( AmbientColor );
      resultColor += AmbientColor*reflectionCoefficient;
    };
    
    //allow for material color
    CVector materialColor;
    nearestObject->GetColor( ray, materialColor );
    VisibleColor( resultColor, materialColor, resultColor );
    
    //we can have some components of the color greater than 1
    //If this has happened, we normalize the color making
    //the maximum component equal to 1
    
    if( (resultColor.x > 1) || (resultColor.y > 1) || (resultColor.z > 1))
    {      
      double maxComp = max (resultColor.x, resultColor.y);
      maxComp = max( maxComp, resultColor.z);
      resultColor /= maxComp;
    }
    
    //finally compute the attenuation due to distance
    
    resultColor *= exp ( -t * curMed.Betta ); 
  }
  else
    GetBackgroundColor( resultColor );
};


///////////////////////////////////////////////////////////
//  RenderPixel

void RenderPixel( const CEnvironment &scene, const Medium &medium, const CCamera &camera, const Tracer &tracer, int x, int y, CVector &color)
{
  ASSERT((x>=0) && (y>=0));
  
  Ray ray;
  camera.PixelRay(x,y,ray);
  tracer.trace( medium, ray, scene, color, true);
};
