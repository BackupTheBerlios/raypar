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
//***********************************


#if !defined(AFX_VECTOR_H__4212329F_586D_46D6_B1EB_423C5FAE9069__INCLUDED_)
#define AFX_VECTOR_H__4212329F_586D_46D6_B1EB_423C5FAE9069__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <math.h>
#define VECTOR_EQUAL_EPS (1E-7)

class CVector
{
public:	
	double x, y, z;
	
public:
	CVector() : x(0), y(0), z(0) {}
	CVector(double _x, double _y, double _z) : x(_x), y(_y), z(_z) {}
	CVector(const CVector &v) : x(v.x), y(v.y), z(v.z) {}  
	
	//operators
	
	CVector operator+ (const CVector& v) 
	{	return CVector(x + v.x, y + v.y, z+v.z); }
	
	CVector operator- (const CVector& v) 
	{	return CVector(x - v.x, y - v.y, z-v.z); }
	
	void operator+= (const CVector& v)
	{	x+=v.x;	y+=v.y;	z+=v.z; }
	
	void operator-= (const CVector& v)
	{	x-=v.x;	y-=v.y;	z-=v.z; }
	
	void operator*= (double m)
	{	x*=m; y*=m; z*=m; }
	
	CVector operator* ( double m )
	{	return CVector(x*m, y*m, z*m); }
	
	//don't forget about devizion by ZERO !
	void operator/= ( double d )
	{	x/=d; y/=d; z/=d; }
	
	//scalar multiplication
	double operator* ( const  CVector& v )
	{	return v.x*x + v.y*y + v.z*z; }
	
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
	{ return x*x + y*y + z*z; }
	
	//sinonim for Length
	double Len (void) const
	{ return Length(); }
	
	//normalization to the length of 1
	void Normalize(void)
	{
		double len = Length();
		
		if( len > VECTOR_EQUAL_EPS )
		{
			x = x/len;
			y = y/len;
			z = z/len;
		}
	}
	
	
#ifdef _DEBUG
	void Dump(CDumpContext& dc = afxDump);
#endif//_DEBUG
};

//unary minus operator
inline CVector operator- (const CVector& v) 
{	return CVector(-v.x, -v.y, -v.z); }


inline CVector operator* ( double m, const CVector& v )
{	return CVector(v.x*m, v.y*m, v.z*m); }

//don't forget about devision by ZERO !
inline CVector operator/ ( const CVector& v, double d )
{	return CVector(v.x/d, v.y/d, v.z/d); }

CDumpContext& operator << (CDumpContext& dc, const CVector& v);


#endif // !defined(AFX_VECTOR_H__4212329F_586D_46D6_B1EB_423C5FAE9069__INCLUDED_)
