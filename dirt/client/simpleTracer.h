//*********************************************************
//** simpleTracer.h **
// Created By: Tonic
// On: 01/10/2004
// Comments: simple implementation of tracer class
//
//***********************************
// REVISION by Tonic, on 01/14/2004
// Comments: Added checking of previous includes of this file. 
// Multiple includes do not cause "type redefined" errors anymore
//*********************************************************
// REVISION by Tonic, on 01/15/2004
// Comments: Added SimpleTracer::VisibleColor to combine light colr with
// material color
// Changed interfaces to get parameters as references
// instead of pointers
//*********************************************************
// REVISION by Tonic, on 1/17/2004
// Comments: Added CRenderer class containing pixel color computing routine
//*********************************************************
// REVISION by Tonic, on 01/19/2004
// Comments: Removed unused double weight from the trace parameters list
//*********************************************************
// REVISION by Tonic, on 01/20/2004
// Comments: Moved lights processing cycle of SimpleTracer 
// to a separate private function SimpleTracer::processLights
//*********************************************************
// REVISION by Tonic, on 01/21/2004
// Comments: Added variable material smoothness support to ProcessLights
//*********************************************************
// REVISION by Tonic, on 01/26/2004
// Comments: Made SimpleTracer parameters explicit in the constructor
//*********************************************************
// REVISION by Tonic, on 01/28/2004
// Comments: Added background color support to SimpleTracer
//*********************************************************
// REVISION by KIRILL, on 1/28/2004 17:22:22
// Comments: CEnvironment changed to CCEnvironment
//*********************************************************
// REVISION by Tonic, on 2/2/2004 
// Comments: Tracer settings went to CSimpleTracerSettings class
//*********************************************************
// REVISION by ..., on ...
// Comments: ...
//*********************************************************



#if !defined(CLIENT_SIMPLETRACER_H_INCLUDED)
#define CLIENT_SIMPLETRACER_H_INCLUDED

#pragma once

#include "trace.h"
#include "common/vector.h"

struct Medium;
class CEnvironment;
class CCamera;

class CSimpleTracerSettings
{
public:
    //maximum recursion depth
  int m_defaultDepth;
  
  //shading model coefficients
  // color = shadeRoD*(normalDir*lightDirection)/(shadeA + shadeB*dist + shadeC*dist^2) 
  // + m_shadeRoReflected*reflectedColor + m_shadeRoRefracted*refractedColor 
  double m_shadeA, m_shadeB, m_shadeC, m_shadeRoD, m_shadeRoReflected, m_shadeRoRefracted;
  CVector m_backgroundColor;

  int IsValid(void) const;
  
  int write(CArchive& ar) const;
  int read (CArchive& ar);

  CSimpleTracerSettings()
    : m_backgroundColor(0,0,0)
  {
    m_defaultDepth = 0;
    m_shadeA = m_shadeB = m_shadeC = m_shadeRoD = m_shadeRoReflected = m_shadeRoRefracted = 0;
  };
};

class SimpleTracer : public Tracer
{
public:
  //bool outside - whether the ray begins outside of all objects
  //used for tracing refracted rays
  virtual void trace( const Medium &curMed, const Ray &ray, 
             const CEnvironment &scene, CVector &resultColor, bool outside) const;
  
  //determines what visible color will the combination of material color and falling light color
  //will produce
  static void VisibleColor( const CVector &LightColor, const CVector &MaterialColor, CVector &resultColor);
  
  SimpleTracer(const CSimpleTracerSettings &settings)
    : m_backgroundColor(0,0,0)
  {
    ASSERT( settings.IsValid() );
    
    m_defaultDepth = settings.m_defaultDepth;
    m_shadeA = settings.m_shadeA;
    m_shadeB = settings.m_shadeB;
    m_shadeC = settings.m_shadeC;
    m_shadeRoD = settings.m_shadeRoD;
    m_shadeRoReflected = settings.m_shadeRoReflected;
    m_shadeRoRefracted = settings.m_shadeRoRefracted;
    m_backgroundColor = settings.m_backgroundColor;
  };
  
  void SetBackgroundColor( const CVector &backgroundColor )
  {
    ASSERT( backgroundColor.IsNormalized() );
    m_backgroundColor = backgroundColor;
  };
  
  void GetBackgroundColor( CVector &backgroundColor )const
  {
    backgroundColor = m_backgroundColor;
  };

private:
  //added maximum recursion depth to the parameter list
  void strace( const Medium &curMed, const Ray &ray, const CEnvironment &scene, CVector &resultColor, int depth, bool outside) const;
  
  //computes the color in the given point due to light sources ONLY
  //RESETS color in the beginning, so do not put any valuable data
  //there as it will be erased
  void processLights( const Medium &curMed, const CEnvironment &scene, const Ray &normale, CVector &color, double smoothness ) const;

  //maximum recursion depth
  int m_defaultDepth;

  
  double m_shadeA, m_shadeB, m_shadeC, m_shadeRoD, m_shadeRoReflected, m_shadeRoRefracted;
  CVector m_backgroundColor;
};

//pixel color computing routine
//which takes scene, camera, tracer and pixel coords as parameters
void RenderPixel( const CEnvironment &scene, 
                  const Medium &medium, const CCamera &camera, 
                  const Tracer &tracer, int x, int y, CVector &color); 


#endif //CLIENT_SIMPLETRACER_H_INCLUDED