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
// REVISION by Tonic, on 01/15/2004
// Comments: Changed interface to get parameters as references
// instead of pointers
//*********************************************************
// REVISION by Tonic, on 01/19/2004
// Comments: Removed unused double weight from the trace parameters list
//*********************************************************
// REVISION by KIRILL, on 1/28/2004 17:28:29
// Comments: Unneeded includes removed. Some renamings done.
//*********************************************************


#if !defined(CLIENT_TRACE_H_INCLUDED)
#define CLIENT_TRACE_H_INCLUDED

struct Medium;
class  Ray;
class  CEnvironment;
class  CVector;

class Tracer
{
public:
  //pure virtual function, needs to be redefined in all descendants.
  //the last argument is the pointer to object, in which the result is written
  virtual void trace( const Medium &curMed, const Ray &ray, 
       const CEnvironment &scene, CVector &resultColor, bool outside) const =0;
};

#endif //CLIENT_TRACE_H_INCLUDED