//****************************************
//** simpleTracer.h **
// Created By: Tonic
// On: 01/10/2004
// Comments: simple implementation of tracer class
//
//***********************************

#include "trace.h"

class SimpleTracer : public Tracer
{
public:
	virtual void trace(Medium *curMed, Ray *ray, Environment *scene, double weight, CVector *resultColor);

	SimpleTracer(void)
	{
		defaultDepth = 200;
		shadeA = shadeB = shadeC = shadeRoD = shadeRoR = 1;
	};
private:
	//added maximum recursion depth to the parameter list
	void strace(Medium *curMed, Ray *ray, Environment *scene, double weight,  CVector *resultColor, int depth);
	
	//maximum recursion depth
	int defaultDepth;

	//shading model coefficients
	// color = shadeRoD*(normalDir*lightDirection)/(shadeA + shadeB*dist + shadeC*dist^2)
	double shadeA, shadeB, shadeC, shadeRoD;

	//reflected ray component coefficient
	// color = color(light sources) + shadeRoR * color(trace(reflected ray))
	double shadeRoR;
};