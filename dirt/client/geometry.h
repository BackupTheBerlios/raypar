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
// REVISION by Vader, on 01/23/2004
// Comments: Added CBox class (supports refraction)
//*********************************************************

#if !defined(CLIENT_GEOMETRY_H_INCLUDED)
#define CLIENT_GEOMETRY_H_INCLUDED

#include "environment.h"

#define EPSILON (1E-9)

class CSphere : public CSolid
{
  CVector m_position;
  double m_radius, m_reflectionCoefficient;
  bool m_isTransparent;
  Medium m_innerMedium, m_outerMedium;
  
public:
  //constructors
  
  //innerMedium is air (Betta=0, nRefr = 1)
  //and the sphere is opaque by default
  CSphere();
  CSphere( CVector &position, double radius, double Betta = 0.0, double nRefr = 1.0 , bool isTransparent = false, double outerBetta = 0.0, double outerRefr = 1.0, double reflectionCoefficient = 1.0);
  
  virtual bool IsTransparent(void)
  {
    return m_isTransparent;
  };
  
  virtual double GetReflectionCoefficient(void)
  {
    return m_reflectionCoefficient;
  };
  
  void SetPosition( CVector &position);
  void SetRadius(double radius);
  
  virtual int Intersect(  Ray &ray, double &distance) ;
  virtual void Reflect( Ray &falling, Ray &reflected) ;
  virtual void Refract( Ray &falling, Ray &refracted, Medium &refractedMedium);
};


class CColorSphere : public CSphere
{
private:
		CVector m_color;
    
public:
    CColorSphere();
	CColorSphere(CVector &position, double radius, CVector &color);
    CColorSphere(CVector &position, double radius);
    virtual void GetColor( Ray &falling, CVector &color);
    void SetColor( CVector &color );
};


class CPlane : public CSolid
{
  CVector m_n;    // Plane is defined by equation
  double m_D;	  // (n,r) + D = 0;   |n| = 1 !!!
  // D - is distance from center (may be <0)
  
public:
  //construction
  CPlane();
  CPlane( CVector &n, double D);
  CPlane(double a, double b, double c, double d); // for   ax + by + cz + d = 0
  
  void SetPosition( CVector &n, double D);
  void SetPosition(double a, double b, double c, double d);
  
  virtual int Intersect(  Ray &ray, double &distance) ;
  virtual void Reflect( Ray &falling, Ray &reflected) ;
};


class CBox : public CSolid
{
private:
    CVector m_position;  //the first corner
    CVector m_e[3]; //3 main edges, that form orthogonal basis
    CVector m_n[3]; //normals to sides, sides are (r, m_n)+ m_di = 0;
    double m_d1[3],m_d2[3]; //distances in plane equation m_d1[i] < m_d2[i];
    double m_reflectionCoefficient;
    bool m_isTransparent;
    Medium m_innerMedium, m_outerMedium;

    void InitNormals();
   
public:
    CBox();
    //e1,e2,e3 should be orthogonal to each other (but not necessarily equal)
    CBox(CVector &position, CVector &e1, CVector &e2, CVector &e3, double Betta = 0.0, double nRefr = 1.0 , bool isTransparent = false, double outerBetta = 0.0, double outerRefr = 1.0, double reflectionCoefficient = 1.0);

    void SetPosition(CVector &position);
    void SetOrientation(CVector &e1, CVector &e2, CVector &e3);

    virtual double GetReflectionCoefficient(void)
    {
        return m_reflectionCoefficient;
    };
    virtual bool IsTransparent(void)
    {
        return m_isTransparent;
    };

    int IsInside(CVector &vector);   //checks whether point lies inside box
    virtual int Intersect(  Ray &ray, double &distance); //returns 1 if ray intersects side with normal m_d[0];
                                                         //returns 2 if ray intersects side with normal m_d[1];
                                                         //returns 3 if ray intersects side with normal m_d[2];
                                                         //returns 0 if there is no intersection
    virtual void Reflect( Ray &falling, Ray &reflected) ;
    virtual void Refract( Ray &falling, Ray &refracted, Medium &refractedMedium);        
};


class CTriangle : public CSolid
{
private:
		CVector m_a, m_b, m_c, m_color, m_normal;
    double m_distance;
    
    //plane equation is (m_normal,x) = m_distance
    int planeIntersect( Ray &ray, double &distance);
    
public:
  CTriangle(CVector &a, CVector &b, CVector &c, CVector &color);
		//white by default
		//CTriangle(CVector &a, CVector &b, CVector &c);
  
		virtual void GetColor( Ray &falling, CVector &color);
    virtual int Intersect(  Ray &ray, double &distance);
    virtual void Reflect( Ray &falling, Ray &reflected);
    
};

#endif //CLIENT_GEOMETRY_H_INCLUDED