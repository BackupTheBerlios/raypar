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
// REVISION by Tonic, on 1/26/2004
// Comments: Added getters to CPlane
//*********************************************************
// REVISION by Vader, on 1/26/2004 
// Comments: IsValid() added for CBox and CPlane
//*********************************************************
// REVISION by Tonic, on 01/29/2004
// Comments: Modified CPlane, CTriangle constructors to support transparency
//*********************************************************
// REVISION by Tonic, on 01/29/2004
// Comments: Comments on class members added
//*********************************************************
// REVISION by Vader, on 1/29/2004
// Comments: Read and write methods for all geometrical objects 
//*********************************************************
// REVISION by ..., on ...
// Comments: ...
//*********************************************************

#if !defined(CLIENT_GEOMETRY_H_INCLUDED)
#define CLIENT_GEOMETRY_H_INCLUDED

#pragma once

#include "environment.h"

#define EPSILON (1E-9)

//in some methods of CBox class n_number is an index of plane, which
//intersects with current ray. This index corresponds numeration of
//m_n[3] class members (normals). 0,1,2 - planes, so -1 means no intersection
#define NO_INTERSECTION -1

///////////////////////////////////////////////////////////
// CSphere

class CSphere : public CSolid
{
  //center of the sphere
  CVector m_position;
  double  m_radius;
  Medium  m_innerMedium;
  Medium  m_outerMedium;
  
public:
  //constructors
  
  //innerMedium is air (Betta=0, nRefr = 1)
  //and the sphere is opaque by default
  CSphere();
  CSphere( const CVector &position
            , double radius, const CVector &color, double Betta = 0.0
            , double nRefr = 1.0 , bool isTransparent = false 
            , double outerBetta = 0.0, double outerRefr = 1.0
            , double reflectionCoefficient = 1.0
            , double smoothness = 1.0);
  
  void SetPosition( const CVector &position);
  void SetRadius(double radius);
  //checks whether the color is valid, radius is nonthero
  //there are no other setters, so do not check anything else
  virtual int  IsValid(void) const;

  virtual int Intersect(  const Ray &ray, double &distance) const;
  virtual void Reflect( const Ray &falling, Ray &reflected) const;
  virtual void Refract( const Ray &falling, Ray &refracted, Medium &refractedMedium, bool &outside) const;
  
  //stores object to  'ar'
  int write(CArchive& ar) const;

protected:
  //reads object from 'ar'
  int read (CArchive& ar); //this can be called only by CSolid::readObject
};

///////////////////////////////////////////////////////////////////
// CPlane 

  class CPlane : public CSolid
{
  CVector m_n;    // Plane is defined by equation
  double m_D;    // (n,r) + D = 0;   |n| = 1 !!!
  // D - is distance from center (may be <0)
    
public:
  CPlane();
  CPlane( const CVector &n, double D, const CVector color,
               double reflectionCoefficient = 1.0, double smoothness = 1.0, 
               bool isTransparent = false, double Betta = 0.0, double nRefr = 1.0);
  CPlane(double a, double b, double c, double d, const CVector color,
               double reflectionCoefficient = 1.0, double smoothness = 1.0, 
               bool isTransparent = false, double Betta = 0.0, double nRefr = 1.0); // for   ax + by + cz + d = 0
  
  void SetPosition( const CVector &n, double D);
  void SetPosition(double a, double b, double c, double d);
  
  void getNormale( CVector &normale ) const
  { normale = m_n; };

  void getDistance( double &distance ) const
  { distance = m_D; };

    
  virtual int Intersect( const Ray &ray, double &distance) const;
  virtual void Reflect( const Ray &falling, Ray &reflected) const;

  //this method checks whether the plane is correctly defined
  //1) non zero normale m_n and distance m_D should be defined
  //2) color and m_reflectionCoefficient (0<=..<=1) should be defined
  //3) material parameters are not necessary, as the object is flat
  //   meanwhile m_isTransparent should be false
  virtual int IsValid(void) const;

  //stores object to  'ar'
  int write(CArchive& ar) const;

protected:
  //reads object from 'ar'
  int read (CArchive& ar); //this can be called only by CSolid::readObject
};


///////////////////////////////////////////////////////////////////
// CBox

class CBox : public CSolid
{
  CVector m_position;  //the first corner
  CVector m_e[3]; //3 main edges, that form orthogonal basis
  CVector m_n[3]; //normals to sides, sides are (r, m_n)+ m_di = 0;
  double m_d1[3],m_d2[3]; //distances in plane equation m_d1[i] < m_d2[i];
  Medium m_innerMedium, m_outerMedium;

public:
  CBox();
  //e1,e2,e3 should be orthogonal to each other (but not necessarily equal)
  CBox(const CVector &position, const CVector &e0
    , const CVector &e1, const CVector &e2, const CVector &color = CVector(0,0,0)
    , double Betta = 0.0, double nRefr = 1.0 
    , bool isTransparent = false, double outerBetta = 0.0
    , double outerRefr = 1.0, double reflectionCoefficient = 1.0, double smoothness = 0.0);

  void SetPosition(const CVector &position);
  void SetOrientation(const CVector &e1, const CVector &e2, const CVector &e3);

  int IsInside(const CVector &vector) const;   //checks whether a point lies inside box
  virtual int Intersect(  const Ray &ray, double &distance) const; 
     //returns 1 if ray intersects side with normal m_d[0];
     //returns 2 if ray intersects side with normal m_d[1];
     //returns 3 if ray intersects side with normal m_d[2];
     //returns 0 if there is no intersection

  virtual void Reflect( const Ray &falling, Ray &reflected) const;
  virtual void Refract( const Ray &falling, Ray &refracted, Medium &refractedMedium, bool &outside) const;
  
  //this method checks whether the plane is correctly defined
  //1) sides m_e[i] should be nonzero and orthogonal
  //2) normales m_n and distances should be initialized correctly this step is
  //   connected with first one, as normales are always initialized after
  //   the initialization of sides. If sides are correct (step 1), Normales
  //   are correct too
  //3) m_color and m_reflectionCoefficient should be correct
  //4) material parameters should be physically correct if the box is transparent
  virtual int IsValid(void) const;

  //stores object to  'ar'
  int write(CArchive& ar) const;

protected:
  void InitNormals();
  
  //reads object from 'ar'
  int read (CArchive& ar); //this can be called only by CSolid::readObject
};


///////////////////////////////////////////////////////////////////
// CTriangle

class CTriangle : public CSolid
{
private:
  //the vertices of the triangle
  //and the normale to the plane containing the triangle
  CVector m_a, m_b, m_c, m_normal;

  //distance - D parameter in the plane equation
  // (x,normale) + D = 0
  double m_distance;
    
public:
  CTriangle();
  CTriangle(const CVector &a, const CVector &b
            , const CVector &c, const CVector &color, double reflectionCoefficient = 1.0, double smoothness = 1.0, 
               bool isTransparent = false, double Betta = 0.0, double nRefr = 1.0);
  
  virtual int  Intersect( const Ray &ray, double &distance) const;
  virtual void Reflect  ( const Ray &falling, Ray &reflected) const;
  
  //all the validation is done in the constructor
  //no setters
  //so the triangle is always valid
  virtual int  IsValid(void) const;

  //stores object to  'ar'
  int write(CArchive& ar) const;

protected:
  //reads object from 'ar'
  int read (CArchive& ar); //this can be called only by CSolid::readObject

  //plane equation is (m_normal,x) = m_distance
  int planeIntersect( const Ray &ray, double &distance) const;    
};

///////////////////////////////////////////////////////////////////
// CCylinder

class CCylinder : public CSolid
{
private:
  //m_length is the distance between the top and bottom circles
  double m_length;

  //the radius of the circles
  double m_radius;

  //base is the center of the "bottom" circle
  //direction is the direction to the center of the
  //"upper" circle
  CVector m_base;
  CVector m_direction;

  //the medium inside and outside the cylinder
  //used in refraction computation
  Medium m_innerMedium;
  Medium m_outerMedium;

  //these are the planes containing the bottom and top
  //circles respectively. Used to ease the intersections computation
  CPlane m_bottom;
  CPlane m_top;
public:
  CCylinder();
  CCylinder( CVector &base, CVector &direction, double length, double radius, const CVector &color, double reflectionCoefficient = 1.0, double smoothness = 1.0, bool isTransparent = false, double Betta = 0.0, double nRefr = 1.0 , double outerBetta = 0.0, double outerRefr = 1.0 );
  virtual int Intersect( const Ray &ray, double &distance) const;
  virtual void Reflect( const Ray &falling, Ray &reflected) const;
  virtual void Refract( const Ray &falling, Ray &refracted, Medium &refractedMedium, bool &outside) const;

  virtual int  IsValid(void) const;
  
  //stores object to  'ar'
  int write(CArchive& ar) const;

protected:
  //reads object from 'ar'
  int read (CArchive& ar); //this can be called only by CSolid::readObject

};
#endif //CLIENT_GEOMETRY_H_INCLUDED