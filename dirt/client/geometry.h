//*********************************************************
//** geometry.h **
// Created By: Vader
// On: 01/12/2004
// Comments: Defines main geometrical objects, with implemented interface methods
//
//*********************************************************
// REVISION by Tonic, on 01/14/2004
// Comments: Added checking of previous includes of this file. 
// Multiple includes do not cause "type redefined" errors anymore
//*********************************************************
// REVISION by Tonic, on 01/15/2004
// Comments: Made Reflect and Intersect methods virtual so
// that they may be overriden in descendants.
// Changed reflect -> Reflect
// Changed interfaces to get parameters as references
// instead of pointers
//*********************************************************
// REVISION by VADER, on 01/15/2004
// Comments: Names changed according to standart naming conventions
//*********************************************************
// REVISION by Tonic, on 01/16/2004
// Comments: CColorSphere class - one-colour sphere
//*********************************************************
// REVISION by Tonic, on 01/17/2004
// Comments: Added CTriangle class
//*********************************************************
// REVISION by Tonic, on 01/19/2004
// Comments: Added transparency support for CSphere
//*********************************************************
// REVISION by Tonic, on 01/22/2004
// Comments: Added CCylinder
//*********************************************************
// REVISION by Vader, on 01/23/2004
// Comments: Added CBox class (supports refraction)
//*********************************************************
// REVISION by KIRILL, on 1/24/2004 03:42:00
// Comments: Some refactoring was done
//*********************************************************
// REVISION by Tonic, on 1/26/2004
// Comments: Color support went to CSphere, CColorSphere eliminated
// IsValid added to CTriangle, CSphere, CCylinder
//*********************************************************
// REVISION by ..., on ...
// Comments: ...
//*********************************************************

#if !defined(CLIENT_GEOMETRY_H_INCLUDED)
#define CLIENT_GEOMETRY_H_INCLUDED

#include "environment.h"

#define EPSILON (1E-9)

///////////////////////////////////////////////////////////
// CSphere

class CSphere : public CSolid
{
private:
  CVector m_position;
  double  m_radius;
  Medium  m_innerMedium;
  Medium  m_outerMedium;
  CVector m_color;
  
public:
  //constructors
  
  //innerMedium is air (Betta=0, nRefr = 1)
  //and the sphere is opaque by default
  CSphere();
  CSphere( const CVector &position
            , double radius, const CVector &color, double Betta = 0.0
            , double nRefr = 1.0 , bool isTransparent = false 
            , double outerBetta = 0.0, double outerRefr = 1.0
            , double reflectionCoefficient = 1.0);
  
  virtual bool IsTransparent(void) const 
  { return m_isTransparent; }
  
  virtual double GetReflectionCoefficient(void) const
  { return m_reflectionCoefficient; }

  virtual void GetColor( const Ray &falling, CVector &color) const;

  void SetPosition( const CVector &position);
  void SetRadius(double radius);
  void SetColor( const CVector &color );

  //checks whether the color is valid, radius is nonthero
  //there are no other setters, so do not check anything else
  virtual int  IsValid(void) const;

  virtual int Intersect(  const Ray &ray, double &distance) const;
  virtual void Reflect( const Ray &falling, Ray &reflected) const;
  virtual void Refract( const Ray &falling, Ray &refracted, Medium &refractedMedium) const;
};

///////////////////////////////////////////////////////////////////
// CPlane 

class CPlane : public CSolid
{
  CVector m_n;    // Plane is defined by equation
  double m_D;    // (n,r) + D = 0;   |n| = 1 !!!
  // D - is distance from center (may be <0)
  
public:
  //construction
  CPlane();
  CPlane( const CVector &n, double D);
  CPlane(double a, double b, double c, double d); // for   ax + by + cz + d = 0
  
  void SetPosition( const CVector &n, double D);
  void SetPosition(double a, double b, double c, double d);
  
  virtual int Intersect( const Ray &ray, double &distance) const;
  virtual void Reflect( const Ray &falling, Ray &reflected) const;
};

///////////////////////////////////////////////////////////////////
// CBox

class CBox : public CSolid
{
  CVector m_position;  //the first corner
  CVector m_e[3]; //3 main edges, that form orthogonal basis
  CVector m_n[3]; //normals to sides, sides are (r, m_n)+ m_di = 0;
  double m_d1[3],m_d2[3]; //distances in plane equation m_d1[i] < m_d2[i];
  double m_reflectionCoefficient;
  bool m_isTransparent;
  Medium m_innerMedium, m_outerMedium;

public:
  CBox();
  //e1,e2,e3 should be orthogonal to each other (but not necessarily equal)
  CBox(const CVector &position, const CVector &e1
    , const CVector &e2, const CVector &e3
    , double Betta = 0.0, double nRefr = 1.0 
    , bool isTransparent = false, double outerBetta = 0.0
    , double outerRefr = 1.0, double reflectionCoefficient = 1.0);

  void SetPosition(const CVector &position);
  void SetOrientation(const CVector &e1, const CVector &e2, const CVector &e3);

  virtual double GetReflectionCoefficient(void) const
  { return m_reflectionCoefficient; }

  virtual bool IsTransparent(void) const
  { return m_isTransparent; };

  int IsInside(const CVector &vector) const;   //checks whether point lies inside box
  virtual int Intersect(  const Ray &ray, double &distance) const; 
     //returns 1 if ray intersects side with normal m_d[0];
     //returns 2 if ray intersects side with normal m_d[1];
     //returns 3 if ray intersects side with normal m_d[2];
     //returns 0 if there is no intersection

  virtual void Reflect( const Ray &falling, Ray &reflected) const;
  virtual void Refract( const Ray &falling, Ray &refracted, Medium &refractedMedium) const;        

protected:
  void InitNormals();
};

///////////////////////////////////////////////////////////////////
// CTriangle

class CTriangle : public CSolid
{
private:
  CVector m_a, m_b, m_c, m_color, m_normal;
  double m_distance;
    
public:
  CTriangle(const CVector &a, const CVector &b
            , const CVector &c, const CVector &color);
  
  virtual void GetColor ( const Ray &falling, CVector &color) const;
  virtual int  Intersect( const Ray &ray, double &distance) const;
  virtual void Reflect  ( const Ray &falling, Ray &reflected) const;
  
  //all the validation is done in the constructor
  //no setters
  //so the triangle is always valid
  virtual int  IsValid(void) const
  {return 1;};

protected:
  //plane equation is (m_normal,x) = m_distance
  int planeIntersect( const Ray &ray, double &distance) const;    
};

class CCylinder : public CSolid
{
private:
  double m_length, m_radius;

  //base is the center of the "bottom" circle
  //direction is the direction to the center of the
  //"upper" circle
  CVector m_base, m_direction;
  Medium m_innerMedium, m_outerMedium;
  CPlane m_bottom, m_top;
public:
  CCylinder( Ray &axis, double length, double radius, double reflectionCoefficient = 1.0, double smoothness = 1.0, bool isTransparent = false, double Betta = 0.0, double nRefr = 1.0 , double outerBetta = 0.0, double outerRefr = 1.0 );
  virtual int Intersect( const Ray &ray, double &distance) const;
  virtual void Reflect( const Ray &falling, Ray &reflected) const;
  virtual void Refract( const Ray &falling, Ray &refracted, Medium &refractedMedium) const;

  //all the validation is done in the constructor
  //no setters
  //so the cylinder is always valid
  virtual int  IsValid(void) const
  {return 1;};
};
#endif //CLIENT_GEOMETRY_H_INCLUDED