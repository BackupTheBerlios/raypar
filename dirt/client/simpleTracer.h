
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

#if !defined(CLIENT_SIMPLETRACER_H_INCLUDED)
#define CLIENT_SIMPLETRACER_H_INCLUDED


#include "trace.h"

class SimpleTracer : public Tracer
{
public:
	virtual void trace( Medium &curMed,  Ray &ray,  Environment &scene, double weight, CVector &resultColor);

	//determines what visible color will the combination of material color and falling light color
	//will produce
	static void VisibleColor(  CVector &LightColor,  CVector &MaterialColor, CVector &ResultColor );

	SimpleTracer(void)
	{
		defaultDepth = 200;
		shadeA = shadeB = shadeC = shadeRoD = shadeRoR = 1;
	};
private:
	//added maximum recursion depth to the parameter list
	void strace( Medium &curMed,  Ray &ray,  Environment &scene, double weight, CVector &resultColor, int depth);
	
	//maximum recursion depth
	int defaultDepth;

	//shading model coefficients
	// color = shadeRoD*(normalDir*lightDirection)/(shadeA + shadeB*dist + shadeC*dist^2)
	double shadeA, shadeB, shadeC, shadeRoD;

	//reflected ray component coefficient
	// color = color(light sources) + shadeRoR * color(trace(reflected ray))
	double shadeRoR;
};

//class to contain pixel color computing routine
//which takes scene, camera, tracer and pixel coords as parameters
class CRenderer
	{
	public:
		static void RenderPixel( Environment &scene, Medium &medium, CCamera &camera, Tracer &tracer, int x, int y, CVector &color);
	};

#endif //CLIENT_SIMPLETRACER_H_INCLUDED