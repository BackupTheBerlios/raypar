//****************************************
//** Vector.h **
// Created By: KIRILL
// On :11/4/2003 20:00:12
// Comments: interface for the CVector class.
//
//***********************************
// REVISION by Tonic on 12/11/2003
// Comments: Fixed a bug in CVector operator =, added missing z component assignment
//
//***********************************
// REVISION by Tonic on 11/01/2004
// Comments: Added Normalize() method
//
//*********************************************************
// REVISION by KIRILL, on 1/14/2004 20:25:06
// Comments: Length() error corrected - sqrt(.) added
//
//*********************************************************
// REVISION by Tonic, on 1/19/2004
// Comments: added approximate comparison functions for double values
// using VECTOR_EQUAL_EPS
//
//*********************************************************
// REVISION by KIRILL, on 1/23/2004 04:51:54
// Comments: Storing/loading operators >> & << added
//   debug storing operators removed
//
//*********************************************************
// REVISION by KIRILL, on 1/23/2004 04:57:08
// Comments: double comparison functions slightly modified
//
//*********************************************************
// REVISION by Vader on 1/23/2004
// Comments: vector multiplication added
//
//*********************************************************
// REVISION by KIRILL, on 1/24/2004 04:34:17
// Comments: IsNormalized() method added
//
//*********************************************************
// REVISION by ..., on ...
// Comments: ...
//
//*********************************************************



#if !defined(AFX_VECTOR_H__4212329F_586D_46D6_B1EB_423C5FAE9069__INCLUDED_)
#define AFX_VECTOR_H__4212329F_586D_46D6_B1EB_423C5FAE9069__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <math.h>
#define VECTOR_EQUAL_EPS (1E-7)

//nonzero if a == b
inline int eq(double a, double b)
{
  return ( fabs(a-b) < VECTOR_EQUAL_EPS );
};

//nonzero if a >= b
inline int geq(double a, double b)
{
  return ( a > b - VECTOR_EQUAL_EPS );    
};

//nonzero if a <= b
inline int leq(double a, double b)
{ 
  return ( a < b + VECTOR_EQUAL_EPS );    
};

class CVector
{
public:	
  double x, y, z;
  
public:
  CVector() : x(0), y(0), z(0) {}
  CVector(double _x, double _y, double _z) : x(_x), y(_y), z(_z) {}
  CVector(const CVector &v) : x(v.x), y(v.y), z(v.z) {}  
  
  //operators
  
  CVector operator+ (const CVector& v)  const
  {	return CVector(x + v.x, y + v.y, z+v.z); }
  
  CVector operator- (const CVector& v) const
  {	return CVector(x - v.x, y - v.y, z-v.z); }
  
  void operator+= (const CVector& v)
  {	x+=v.x;	y+=v.y;	z+=v.z; }
  
  void operator-= (const CVector& v) 
  {	x-=v.x;	y-=v.y;	z-=v.z; }
  
  void operator*= (double m)
  {	x*=m; y*=m; z*=m; }
  
  CVector operator* ( double m ) const
  {	return CVector(x*m, y*m, z*m); }
  
  //don't forget about devizion by ZERO !
  void operator/= ( double d )
  {	x/=d; y/=d; z/=d; }
  
  //scalar multiplication
  double operator* ( const  CVector& v ) const
  {	return v.x*x + v.y*y + v.z*z; }

  //vector multiplication
  CVector operator^ ( const  CVector& v ) const
  {	return CVector(v.z*y - v.y*z, v.x*z - v.z*x, v.y*x - v.x*y); }
  
  //*************
  //operator =
  //last modified by Tonic on 12/11/2003
  //bugfix, added missing z=v.z
  //*************
  CVector& operator= ( const CVector& v )
  {	x=v.x; y=v.y; z=v.z;	return *this;}
  
  //Be carefull with this operator!
  int operator == ( const CVector& v ) const
  { return ( ( fabs(v.x-x) < VECTOR_EQUAL_EPS) && 
  ( fabs(v.y-y) < VECTOR_EQUAL_EPS) && 
  ( fabs(v.z-z) < VECTOR_EQUAL_EPS ) ); }
  
  //Checks whether Vector equals to zero
  //Be careful with this function!
  int IsZero (void) const
  { return ( ( fabs(x) < VECTOR_EQUAL_EPS ) && 
  ( fabs(y) < VECTOR_EQUAL_EPS ) &&
  ( fabs(z) < VECTOR_EQUAL_EPS ) ); }
  
  double Length (void) const
  { return sqrt(x*x + y*y + z*z); }
  
  //sinonim for Length
  double Len (void) const
  { return Length(); }
  
  //normalization to the length of 1
  void  Normalize() {
    double len = Length();  
    if( len > VECTOR_EQUAL_EPS ){
      x /= len;  y /= len;  z /= len;
    }    
  }

  // checks whether all coords are in [0,1]
  int IsNormalized(void) const  {
     return ( geq(x, 0) && leq(x, 1) 
           && geq(y, 0) && leq(y, 1) 
           && geq(z, 0) && leq(z, 1) );
  }
  

    
//#ifdef _DEBUG
//  void Dump(CDumpContext& dc = afxDump);
//#endif//_DEBUG
};

//unary minus operator
inline CVector operator- (const CVector& v) 
{	return CVector(-v.x, -v.y, -v.z); }

inline CVector operator* ( double m, const CVector& v )
{	return CVector(v.x*m, v.y*m, v.z*m); }

//don't forget about devision by ZERO !
inline CVector operator/ ( const CVector& v, double d )
{	return CVector(v.x/d, v.y/d, v.z/d); }

//CDumpContext& operator << (CDumpContext& dc, const CVector& v);

//Storing and loading operators
CArchive& operator << (CArchive& ar, const CVector& v);
CArchive& operator >> (CArchive& ar, CVector& v);
#endif // !defined(AFX_VECTOR_H__4212329F_586D_46D6_B1EB_423C5FAE9069__INCLUDED_)
