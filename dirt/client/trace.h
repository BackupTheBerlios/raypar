//****************************************
//** trace.cpp **
// Created By: Tonic
// On: 01/10/2004
// Comments: Interface for tracer classes
//
//***********************************

#include "Vector.h"
#include "environment.h"

class Tracer
{
public:
	//pure virtual function, needs to be redefined in all descendants.
	//the last argument is the pointer to object, in which the result is written
	virtual void trace(Medium *curMed, Ray *ray, Environment *scene, double weight, CVector *resultColor);
};