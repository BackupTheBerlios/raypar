
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

#if !defined(CLIENT_SIMPLETRACER_H_INCLUDED)
#define CLIENT_SIMPLETRACER_H_INCLUDED

#include "trace.h"

class SimpleTracer : public Tracer
{
public:
  //bool outside - whether the ray begins outside of all objects
  //used for tracing refracted rays
  virtual void trace( Medium &curMed,  Ray &ray,  Environment &scene, CVector &resultColor, bool outside);
  
  //determines what visible color will the combination of material color and falling light color
  //will produce
  static void VisibleColor(  CVector &LightColor,  CVector &MaterialColor, CVector &ResultColor );
  
  SimpleTracer(int defaultDepth = 5, double shadeA = 0.1, double shadeB = 0.1, double shadeC = 0.1, double shadeRoD = 1, double shadeRoReflected = 1, double shadeRoRefracted = 1)
  {
    ASSERT( defaultDepth > 0);
    ASSERT( shadeA > VECTOR_EQUAL_EPS );
    ASSERT( shadeB > 0 );
    ASSERT( shadeC > 0 );
    ASSERT( shadeRoD > VECTOR_EQUAL_EPS );
    ASSERT( shadeRoReflected > VECTOR_EQUAL_EPS );
    ASSERT( shadeRoRefracted > VECTOR_EQUAL_EPS );
    
    m_defaultDepth = defaultDepth;
    m_shadeA = shadeA;
    m_shadeB = shadeB;
    m_shadeC = shadeC;
    m_shadeRoD = shadeRoD;
    m_shadeRoReflected = shadeRoReflected;
    m_shadeRoRefracted = shadeRoRefracted;
  };
private:
  //added maximum recursion depth to the parameter list
  void strace( Medium &curMed,  Ray &ray,  Environment &scene, CVector &resultColor, int depth, bool outside);
  
  //computes the color in the given point due to light sources ONLY
  //RESETS color in the beginning, so do not put any valuable data
  //there as it will be erased
  void processLights( Medium &curMed, Environment &scene, Ray &normale, CVector &color, double smoothness );
  
  //maximum recursion depth
  int m_defaultDepth;
  
  //shading model coefficients
  // color = shadeRoD*(normalDir*lightDirection)/(shadeA + shadeB*dist + shadeC*dist^2) 
  // + m_shadeRoReflected*reflectedColor + m_shadeRoRefracted*refractedColor 
  double m_shadeA, m_shadeB, m_shadeC, m_shadeRoD, m_shadeRoReflected, m_shadeRoRefracted;
};

//class to contain pixel color computing routine
//which takes scene, camera, tracer and pixel coords as parameters
class CRenderer
{
public:
		static void RenderPixel( Environment &scene, Medium &medium, CCamera &camera, Tracer &tracer, int x, int y, CVector &color);
};

#endif //CLIENT_SIMPLETRACER_H_INCLUDED