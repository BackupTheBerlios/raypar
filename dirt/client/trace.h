//****************************************
//** trace.cpp **
// Created By: Tonic
// On: 01/10/2004
// Comments: Interface for tracer classes
//
//***********************************
// REVISION by Tonic, on 01/14/2004
// Comments: Added checking of previous includes of this file. 
// Multiple includes do not cause "type redefined" errors anymore
//*********************************************************

#if !defined(CLIENT_TRACE_H_INCLUDED)
#define CLIENT_TRACE_H_INCLUDED


#include "..\common\vector.h"
#include "environment.h"

class Tracer
{
public:
	//pure virtual function, needs to be redefined in all descendants.
	//the last argument is the pointer to object, in which the result is written
	virtual void trace(Medium *curMed, Ray *ray, Environment *scene, double weight, CVector *resultColor)=0;
};

#endif //CLIENT_TRACE_H_INCLUDED