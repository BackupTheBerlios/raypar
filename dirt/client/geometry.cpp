//*********************************************************
//** geometry.cpp **
// Created By: Vader
// On: 01/12/2004
// Comments: Geometry objects class methods implementation
//
//*********************************************************
// REVISION by KIRILL, on 1/14/2004 20:54:59
// Comments: error in Sphere::reflect and Plane::reflect
//  with ASSERT(.) corrected
//
//*********************************************************
// REVISION by Tonic, on 1/15/2004
// Comments: Changed reflect -> Reflect
// Changed interfaces to get parameters as references
// instead of pointers
//*********************************************************
// REVISION by VADER, on 1/15/2004
// Comments: Names changed according to standart naming conventions
//
//*********************************************************
// REVISION by Tonic, on 01/16/2004
// Comments: CColorSphere class - one-colour sphere
//*********************************************************
// REVISION by Tonic, on 01/17/2004
// Comments: Added CTriangle class
//*********************************************************
// REVISION by Tonic, on 01/19/2004
// Comments: Added transparency support for CSphere, CColorSphere
//*********************************************************
// REVISION by Tonic, on 01/19/2004
// Comments: CSphere - reflection coefficient support
//*********************************************************

#include "stdafx.h"
#include "geometry.h"

void CSphere::Refract( Ray &falling, Ray &refracted, Medium &refractedMedium)
{
  //check whether the falling ray begins outside of the sphere
  CVector fallingOrigin;
  falling.getOrigin( fallingOrigin );
  double distance = INFINITY;
  if( (fallingOrigin - m_position).Length() > m_radius )
  {
    //the ray begins outside
    //check whether it intersects us
    if( Intersect(falling, distance) )
    {
      CVector vector, normale;
      
      //compute refraction
      falling.getPoint( distance, vector );
      refracted.setOrigin( vector );
      
      normale = vector - m_position;
      normale.Normalize();
      falling.getDirection( vector );
      
      CVector parallelComponent = (vector + fabs(vector*normale) * normale)*m_outerMedium.nRefr/m_innerMedium.nRefr;
      double pcl = parallelComponent.Length();
      if( geq(pcl,1) )
      {
        //full inner reflection
        //so the medium is outer
        refractedMedium.Betta = m_outerMedium.Betta;
        refractedMedium.nRefr = m_outerMedium.nRefr;
        
        //recompute the refracted ray because it coincides
        //with the reflected one
        CVector reflectedDir = vector + 2*normale*(vector*normale);
        refracted.setDirection(reflectedDir);
      }
      else
      {
        //the medium is inner
        refractedMedium.Betta = m_innerMedium.Betta;
        refractedMedium.nRefr = m_innerMedium.nRefr;
        
        vector = parallelComponent - normale*sqrt( 1 - pcl*pcl);
        refracted.setDirection( vector );
      }
    }
    else
    {
      //no refraction, just return the same ray and medium
      refracted.setOrigin( fallingOrigin );
      //reuse fallingOrigin object
      falling.getDirection( fallingOrigin );
      refracted.setDirection( fallingOrigin );
      
      //the medium is outer
      refractedMedium.Betta = m_outerMedium.Betta;
      refractedMedium.nRefr = m_outerMedium.nRefr;
    };
  }
  else
  {
    //the ray begins from within the sphere
    //there is always an intersestion
    CVector vector, normale;
    
    Intersect(falling, distance);
    
    falling.getPoint(distance, vector);
    refracted.setOrigin( vector );
    normale = m_position - vector;
    normale.Normalize();
    falling.getDirection( vector );
    
    CVector parallelComponent = (vector + fabs(vector*normale) * normale)*m_innerMedium.nRefr/m_outerMedium.nRefr;
    double pcl = parallelComponent.Length();
    if( geq(pcl,1) )
    {
      //full inner reflection
      //so the medium is inner
      refractedMedium.Betta = m_innerMedium.Betta;
      refractedMedium.nRefr = m_innerMedium.nRefr;
      
      //recompute the refracted ray because it coincides
      //with the reflected one
      CVector reflectedDir = vector + 2*normale*(vector*normale);
      refracted.setDirection(reflectedDir);
    }
    else
    {
      //the medium is outer
      refractedMedium.Betta = m_outerMedium.Betta;
      refractedMedium.nRefr = m_outerMedium.nRefr;
      
      vector = parallelComponent - normale*sqrt( 1 - pcl*pcl);
      refracted.setDirection( vector );
    }
  }
};

CTriangle::CTriangle(CVector &a, CVector &b, CVector &c, CVector &color)
{
  //check whether color components are correct
  ASSERT( 0 <= color.x && color.x <= 1 );
  ASSERT( 0 <= color.y && color.y <= 1 );
  ASSERT( 0 <= color.z && color.z <= 1 );
  
  //compute two sides dot product
  //if it is really a triangle, it wil be nonzero
  CVector ab, ac;
  ab = b - a;
  ac = c - a;
  m_normal.x = ab.y*ac.z - ab.z*ac.y;
  m_normal.y = -(ab.x*ac.z - ab.z*ac.x);
  m_normal.z = ab.x*ac.y - ab.y*ac.x;
  double len = m_normal.Length();
  ASSERT( len > VECTOR_EQUAL_EPS );
  
  //checks were passed, init private members
  m_normal.Normalize();
  m_distance = a*m_normal;
  m_a = a;
  m_b = b;
  m_c = c;
  m_color = color;
};

void CTriangle::GetColor( Ray &falling, CVector &color)
{
  //do not check the intersection has place (otherwise in most cases we
  //will repeat the computaion already done
  //as every point of the sphere has the same color), just return it
  
  color = m_color;
};

int CTriangle::Intersect( Ray &ray, double &distance)
{
  //checking intersection with the containing plane
  double rayDistance = distance;
  if( !planeIntersect(ray, rayDistance))
    return 0;
  
  CVector intersectionPoint;
  ray.getPoint( rayDistance, intersectionPoint );
  
  CVector ab = m_b - m_a;
  CVector ac = m_c - m_a;
  CVector a_ip = intersectionPoint - m_a;
  //decompose a_intersectionPoint
  //using the basis (ab, ac)
  double denominator = ab.Length()*ab.Length()*ac.Length()*ac.Length() - (ab*ac)*(ab*ac);
  double abProj = ((ab*a_ip)*ac.Length()*ac.Length() - (ac*a_ip)*(ac*ab))/denominator;
  double acProj = ((ac*a_ip)*ab.Length()*ab.Length() - (ab*a_ip)*(ac*ab))/denominator;
  
  if( (abProj < 0) || (acProj < 0) || (abProj + acProj > 1))
    return 0;
  
  //there is an intersection, modify the distance and return 1
  distance = rayDistance;
  return 1;
};

int CTriangle::planeIntersect( Ray &ray, double &distance )
{
  CVector origin, direction;
  
  ray.getOrigin(origin);
  ray.getDirection(direction);
  
  double originDistance = origin*m_normal;
  
  //the origin is in our plane
  //the triangle is not visible anyway so
  //no intersection
  if( fabs( originDistance - m_distance ) < VECTOR_EQUAL_EPS )
    return 0;
  
  double specificDistance = direction*m_normal;
  
  //ray direction parallel to the plane
  //no intersection
  if( fabs(specificDistance) < VECTOR_EQUAL_EPS )
    return 0;
  
  //distance along the ray to the point where
  //the ray and plane intersect
  double rayDistance = (m_distance - originDistance)/specificDistance;
  
  //the intersection is too far
  //or on the negative side of the ray
  if((rayDistance > distance) || (rayDistance < 0))
    return 0;
  
  distance = rayDistance;
  return 1;
};

void CTriangle::Reflect( Ray &falling, Ray &reflected )
{
  //this function is assumed to be called after
  //the check of intersection is done. So do not
  //repeat it, just return a ray reflected from the
  //containing plane
  double distance = INFINITY;
  
  //get the distance to the intersection
  if( !planeIntersect(falling, distance) )
  {
    //no intersection -> no reflection
    reflected = falling;
  }
  else
  {
    CVector vector;
    falling.getPoint( distance, vector );
    reflected.setOrigin( vector );
    falling.getDirection( vector );
    double normalProjection = vector*m_normal;
    reflected.setDirection( -normalProjection*m_normal + 2*(vector - normalProjection*m_normal ));
  };
};

CColorSphere::CColorSphere()
{
  //set color to white
  m_color.x = m_color.y = m_color.z = 1;
};

CColorSphere::CColorSphere(CVector &position, double radius) : CSphere(position, radius)
{
  //set color to white
  m_color.x = m_color.y = m_color.z = 1;
  
};

CColorSphere::CColorSphere(CVector &position, double radius, CVector &color) : CSphere(position, radius)
{
  //check parameter value
  ASSERT( 0 <= color.x && color.x <= 1);
  ASSERT( 0 <= color.y && color.y <= 1);
  ASSERT( 0 <= color.z && color.z <= 1);
  
  //set color according to parameter
  m_color = color;
};

void CColorSphere::GetColor( Ray &falling, CVector &color)
{
  //do not check the intersection has place (otherwise in most cases we
  //will repeat the computaion already done
  //as every point of the sphere has the same color), just return it
  
  color = m_color;
};

////////////////////////////// Sphere methods //////////////////////////////
CSphere::CSphere()
{  
  m_position.x = 0;
  m_position.y = 0;
  m_position.z = 0;
  m_radius = 1;
  //radius2 = 1;
}

CSphere::CSphere( CVector &position, double radius, double Betta, double nRefr, bool isTransparent, double outerBetta, double outerRefr, double reflectionCoefficient)
{
  ASSERT( radius > 0);
  bool comparisonResult = geq( Betta, 0 );
  ASSERT( comparisonResult );
  ASSERT( nRefr > 0 );
  ASSERT( (0 <= reflectionCoefficient) && (reflectionCoefficient <= 1.0) );
  
  m_reflectionCoefficient = reflectionCoefficient;
  // = is overridden operator for CVector, makes components the same
  
  m_position = position;
  m_radius = radius;
  m_innerMedium.Betta = Betta;
  m_innerMedium.nRefr = nRefr;
  
  m_outerMedium.Betta = outerBetta;
  m_outerMedium.nRefr = outerRefr;
  m_isTransparent = isTransparent;
};

void CSphere::SetPosition( CVector &position)
{
  
  // = is overridden operator for CVector, makes components the same
  m_position = position;
};

void CSphere::SetRadius(double radius)
{
  ASSERT ( radius > 0);
  
  m_radius = radius;
  //radius2 = radius*radius;
};

int CSphere::Intersect(  Ray &ray, double &distance) 
{
  CVector origin, direction;
  
  ray.getOrigin(origin); 
  ray.getDirection(direction);
  CVector l = m_position - origin;
  
  if( l.Length() > m_radius ) 
  {
    //ray begins outside, maybe no intersection at all
    //leave Kostya's implementation intact because it works :)
    
    double l2 = l * l;
    double proection = l * direction; //proection of l on ray direction
    
    if(proection<=0) //no intersection
    {
      return 0; 
    }
    
    double condition = proection * proection + m_radius * m_radius - l2; //condition of intersection
    // (simple Pifagor Th.)
    
    if(condition<=0)  //no intersection
    {
      return 0;
    }
    else
    {
      distance = proection - sqrt(condition);
      return 1;
    }
  }
  else
  {
    //starting from the inside
    //we may later use the property of CRay of direction.Length() being always 1
    //to win some performance
    l *= -1.0;
    double discriminant =  (direction*l)*(direction*l) - (direction*direction)*( l*l - m_radius*m_radius );
    
    //it should never happen
    if( discriminant < 0 )
      return 0;
    
    double newDistance = (sqrt(discriminant) - direction*l)/(direction*direction);
    if( geq( newDistance , distance))
      return 0;
    else
    {
      distance = newDistance;
      return 1;
    };
  };
};

void CSphere::Reflect( Ray &falling, Ray &reflected)
{
  double distance = INFINITY;
  CVector fallingOrigin,fallingDirection;
  CVector reflectedOrigin,reflectedDirection;
  
  // 	ASSERT (Intersect(falling, &distance)); This does NOTHING in RELEASE version.
  //                                          Intersect() is NOT called.
  
  int ret = Intersect(falling, distance); //gets the distance
  ASSERT (ret); 
  
  falling.getOrigin(fallingOrigin);
  falling.getDirection(fallingDirection);
  
  reflectedOrigin = fallingOrigin + distance * fallingDirection;
  reflected.setOrigin(reflectedOrigin);
  
  CVector normal = (reflectedOrigin - m_position)/m_radius;
  
  //newDirection = oldDirection + delta( in the derection of normal to the surface)
  reflectedDirection = fallingDirection - 2*(normal * fallingDirection)*normal;
  reflected.setDirection(reflectedDirection);
};


////////////////////////////// Plane methods //////////////////////////////


CPlane::CPlane()
{
  m_n.x = 1;
  m_n.y = 0;
  m_n.z = 0;
  m_D = 0;
};


CPlane::CPlane( CVector &n, double D)
{
  m_n = n;
  m_n.Normalize();
  m_D = D;	
};

CPlane::CPlane(double a, double b, double c, double d)
{
  ASSERT (a!=0 || b!=0 || c!=0);
  
  m_n = CVector (a, b, c);
  double length = m_n.Length();
  
  m_n /= length;
  m_D = d/length;
};

void CPlane::SetPosition(CVector &n, double D)
{
  m_n = n;
  m_D = D;
};

void CPlane::SetPosition(double a, double b, double c, double d)
{
  ASSERT (a!=0 || b!=0 || c!=0);
  
  m_n = CVector (a, b, c);
  double length = m_n.Length();
  
  m_n /= length;
  m_D = d/length;
};

int CPlane::Intersect( Ray &ray, double &distance)
{
  CVector origin, direction;
  
  ray.getOrigin(origin);
  ray.getDirection(direction);
  
  double scalar = m_n * direction;
  double t;
  
  if (scalar == 0)  //ray is parallel to plane
  {
    return 0;
  }
  else 
  {
    //if equation  {origin + t * direction = r}, where r satisfies plane equation
    //has positive solutions, then intersection takes place
    t = - (m_D + m_n * origin) / scalar;
    
    if( t <= 0)  //no intersection
    {
      return 0;
    }
    else
    {
      distance = t;
      return 1;
    }
  }
};

void CPlane::Reflect(Ray &falling, Ray &reflected)
{
  double distance;
  CVector fallingOrigin,fallingDirection;
  CVector reflectedOrigin,reflectedDirection;
  
  int ret = Intersect(falling, distance); //gets the distance
  ASSERT (ret); 
  
  
  falling.getOrigin(fallingOrigin);
  falling.getDirection(fallingDirection);
  
  reflectedOrigin = fallingOrigin + distance * fallingDirection;
  reflected.setOrigin(reflectedOrigin);
  
  //newDirection = oldDirection + delta( in the derection of normal to the surface)
  reflectedDirection = fallingDirection - 2 * (m_n * fallingDirection) * m_n;
  reflected.setDirection(reflectedDirection);
};