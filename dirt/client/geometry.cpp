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
// REVISION by Tonic, on 01/22/2004
// Comments: Added CCylinder
//*********************************************************
// REVISION by Tonic, on 01/25/2004
// Comments: Modified CPlane::Intersect to make it ignore 
// the rays beginning too close, fixed CSphere::Refract math
// (removed fabs usage)
//*********************************************************
// REVISION by Vader, on 01/23/2004
// Comments: Added CBox class (supports refraction)
//*********************************************************
// REVISION by KIRILL, on 1/24/2004 03:42:00
// Comments: Some refactoring was done
//*********************************************************
// REVISION by Tonic, on 1/26/2004 Modified CSphere::Intersect
// to fix artefacts with reflection/refractions
// Color support went to CSphere, CColorSphere eliminated
// IsValid added to CTriangle, CSphere, CCylinder
//*********************************************************
// REVISION by Vader, on 1/26/2004 
// Comments: IsValid() added for CBox and CPlane
//*********************************************************
// REVISION by Tonic, on 01/29/2004
// Comments: Modified CPlane, CTriangle constructors to 
// support transparency, modified CBox::IsValid
//*********************************************************
// REVISION by ..., on ...
// Comments: ...
//*********************************************************

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "stdafx.h"
#include "geometry.h"

void CCylinder::Refract( const Ray &falling, Ray &refracted, Medium &refractedMedium, bool &outside ) const
{
  Ray reflected;
  Reflect( falling, reflected );
  CVector fallingOrigin, fallingDirection, reflectedDirection, normale, reflectedOrigin;
  falling.getOrigin(fallingOrigin);
  reflected.getOrigin(reflectedOrigin);
  refracted.setOrigin( reflectedOrigin );
  falling.getDirection(fallingDirection);
  reflected.getDirection(reflectedDirection);
  normale = reflectedDirection - fallingDirection;
  normale.Normalize();
  
  double horizontalProjection = (fallingOrigin - m_base)*m_direction;
  if(horizontalProjection > 0 && 
    horizontalProjection < m_length && 
    (fallingOrigin - m_base)*(fallingOrigin - m_base) - horizontalProjection*horizontalProjection < m_radius*m_radius)
  {
    //the ray begins inside the cylinder
    CVector parallelComponent = (fallingDirection - (fallingDirection*normale) * normale)*m_innerMedium.nRefr/m_outerMedium.nRefr;
    if( geq(parallelComponent.Length(),1))
    {
      //full inner reflection
      refracted.setDirection( reflectedDirection );
      refractedMedium.Betta = m_innerMedium.Betta;
      refractedMedium.nRefr = m_innerMedium.nRefr;
      outside = false;
    }
    else
    {
      //refracted ray went outside of the cylinder
      CVector refractedDirection = parallelComponent - normale*sqrt( 1 - parallelComponent*parallelComponent);
      refracted.setDirection( refractedDirection );
      refractedMedium.Betta = m_outerMedium.Betta;
      refractedMedium.nRefr = m_outerMedium.nRefr;
      outside = true;
    }
  }
  else
  {
    //the ray begins outside
    CVector parallelComponent = (fallingDirection - (fallingDirection*normale) * normale)*m_outerMedium.nRefr/m_innerMedium.nRefr;
    if( geq(parallelComponent.Length(),1))
    {
      //full inner reflection. The ray stayed outside
      refracted.setDirection( reflectedDirection );
      refractedMedium.Betta = m_outerMedium.Betta;
      refractedMedium.nRefr = m_outerMedium.nRefr;
      outside = true;
    }
    else
    {
      //refracted ray went inside the cylinder
      CVector refractedDirection = parallelComponent - normale*sqrt( 1 - parallelComponent*parallelComponent);
      refracted.setDirection( refractedDirection );
      refractedMedium.Betta = m_innerMedium.Betta;
      refractedMedium.nRefr = m_innerMedium.nRefr;
      outside = false;
    }
  }
};

void CCylinder::Reflect( const Ray &falling, Ray &reflected) const
{
  double distance = INFINITY;
  
  if( !Intersect( falling, distance ) )
    return;
  
  CVector point, direction;
  falling.getDirection(direction);
  falling.getPoint( distance, point );
  reflected.setOrigin( point );
  
  //if the point is on one of the covers
  CVector normale;
  if ( eq( (point - m_base)*m_direction, 0) || eq( (point - m_base)*m_direction, m_length))
  {
    //if the point is on one of the covers
    normale = m_direction;
  }
  else
  {
    //the normale is the corresponding radius, normalized to the length of one
    normale = (point - m_base - ((point - m_base)*m_direction)*m_direction);
    normale.Normalize();
  };
  CVector newDirection = direction - 2*normale *(normale *direction);
  reflected.setDirection(newDirection);
};

int CCylinder::Intersect( const  Ray &ray, double &distance ) const
{
  ASSERT( distance > 0 );
  
  //first check intersection with the top and bottom covers
  double topDistance, bottomDistance;
  int topIntersection, bottomIntersection;
  topDistance = bottomDistance = distance;
  
  bottomIntersection = m_bottom.Intersect( ray, bottomDistance );
  topIntersection = m_top.Intersect( ray, topDistance );
  
  if( bottomIntersection && topIntersection)
  {
    //we intersect with poth planes
    //and the ray origin is therefore outside of the cylinder
    double newDistance = min(topDistance, bottomDistance);
    CVector point;
    ray.getPoint( newDistance, point );
    //distance to cover center in the nearest plane
    double planeDistance = (point - ((topDistance < bottomDistance) ? (m_base + m_length*m_direction) : m_base)).Length();
    if( planeDistance < m_radius )
    {
      //intersection with the cover
      distance = newDistance;
      return 1;
    }
    //may be an intersection with lateral surface, to be checked later
  }
  else if(bottomIntersection || topIntersection)
  {
    //the ray origin is between the covers containing planes
    double newDistance = min(topDistance, bottomDistance);
    bool isBottom = (topDistance > bottomDistance) ? true : false;
    CVector point, center;
    ray.getPoint( newDistance, point );
    center = isBottom ? m_base : (m_base + m_length*m_direction);
    double planeDistance;
    planeDistance = (point - center).Length();
    if( planeDistance < m_radius )
    {
      //there is an intersection with the cover
      //if the ray origin is inside, this is what we need
      CVector rayOrigin;
      ray.getOrigin(rayOrigin);
      if( ((rayOrigin - point)*(rayOrigin - point) - ((rayOrigin - point)*m_direction)*((rayOrigin - point)*m_direction)) < m_radius*m_radius )
      {
        distance = newDistance;
        return 1;
      };
    };
    //may be an intersection with lateral surface, to be checked later
  };
  
  //no intersection with the covers, so check for the intersection with the lateral surface
  CVector rayOrigin, rayDirection;
  ray.getOrigin(rayOrigin);
  ray.getDirection(rayDirection);
  
  //The equation is | point - cylOrigin - cylDirection(cylDirection, point - cylOrigin) | = r
  //it determines the points of ray intersection with the endless cylinder
  CVector a = rayOrigin - m_base - m_direction*(m_direction*(rayOrigin - m_base));
  CVector b = rayDirection - m_direction*(m_direction*rayDirection);
  
  //the equation is |a + bt| = r
  //equivalent to a^2 + 2(a,b)t + b^2t = r^2
  
  //first check whether |b| != 0
  if( leq( b.Length(), 0) )
  {
    //the ray is parallel to the axis
    //no intersection because possible covers intersection has been already investigated
  }
  else
  {
    //the equation is indeed quadratic
    double discriminant = pow( a*b, 2.0) - b*b*(a*a - m_radius*m_radius);
    
    //we have no more than 1 intersection with the endless
    //cylinder, so retun no intersection
    if( leq(discriminant,0) )
    {
      return 0;
    }
    else
    {
      double t1 = (-(a*b) + sqrt(discriminant))/(b*b);
      double t2 = (-(a*b) - sqrt(discriminant))/(b*b);
      double t;
      //now choose which root to check
      if(leq(min(t1,t2),0))
        t = max(t1,t2);
      else
        t = min(t1,t2);
      
      //we intersect the endless cylinder close enough to the ray origin
      //VECTOR_EQUAL_EPS - zatychka
      if( (t > VECTOR_EQUAL_EPS) && (t < distance))
      {
        //check if we are between the covers containing planes
        CVector point;
        ray.getPoint( t, point );
        double height = (point - m_base)*m_direction;
        if( (height > 0) && (height < m_length))
        {
          distance = t;
          return 1;
        };
      };
    };
  };
  //no intersection
  return 0;
};

CCylinder::CCylinder( Ray &axis, double length, double radius, const CVector &color,
                     double reflectionCoefficient, 
                     double smoothness, 
                     bool isTransparent, double Betta, double nRefr,
                     double outerBetta, double outerRefr) : CSolid(reflectionCoefficient, smoothness, isTransparent)
{
  axis.getOrigin(m_base);
  axis.getDirection(m_direction);
  m_direction.Normalize();
  m_length = length;
  m_radius = radius;
  m_bottom.SetPosition( m_direction , -m_base*m_direction );
  m_top.SetPosition( m_direction, -m_base*m_direction - m_length );
  
  m_innerMedium.Betta = Betta;
  m_innerMedium.nRefr = nRefr;
  m_outerMedium.Betta = outerBetta;
  m_outerMedium.nRefr = outerRefr;
  
  SetColor( color );
  
  ASSERT( IsValid() );
};

int CCylinder::IsValid(void) const
{
  if( !CSolid::IsValid() )
    return 0;
  
  if( (m_length < VECTOR_EQUAL_EPS) || (m_length > INFINITY))
    return 0;
  
  if( (m_radius < VECTOR_EQUAL_EPS) || (m_radius > INFINITY))
    return 0;
  
  if ( !geq(m_innerMedium.Betta,0) )
    return 0;
  
  if ( !geq(m_innerMedium.nRefr,0) )
    return 0;
  
  if ( !geq(m_outerMedium.Betta,0) )
    return 0;
  
  if ( !geq(m_outerMedium.nRefr,0) )
    return 0;
  
  CVector normale;
  m_bottom.getNormale( normale );
  if( !leq((normale - m_direction).Length(),0) )
    return 0;
  
  m_top.getNormale( normale );
  if( !leq((normale - m_direction).Length(),0) )
    return 0;
  
  double bottomDistance, topDistance;
  m_bottom.getDistance( bottomDistance );
  m_top.getDistance( topDistance );
  
  if( !leq(fabs(m_direction*m_base + bottomDistance), 0) )
    return 0;
  
  if( !leq(fabs(bottomDistance - topDistance - m_length),0) )
    return 0;
  
  return 1;
};

///////////////////////////////////////////////////////////
//  CSphere
///////////////////////////////////////////////////////////

CSphere::CSphere()
: m_position(0,0,0)
, m_radius(0)
{  }

CSphere::CSphere( const CVector &position, double radius, const CVector &color, double Betta, double nRefr, 
                 bool isTransparent, double outerBetta, double outerRefr, double reflectionCoefficient, double smoothness)
                 : CSolid(reflectionCoefficient, smoothness, isTransparent)
{
  SetColor( color );
  SetReflectionCoefficient(reflectionCoefficient);
  m_position = position;
  m_radius = radius;
  m_innerMedium.Betta = Betta;
  m_innerMedium.nRefr = nRefr;
  m_outerMedium.Betta = outerBetta;
  m_outerMedium.nRefr = outerRefr;
  
  ASSERT( IsValid() );
};

void CSphere::SetPosition( const CVector &position )
{
  m_position = position;
};

void CSphere::SetRadius(double radius)
{
  ASSERT ( radius > 0 + EPSILON );
  
  m_radius = radius;
};


int CSphere::Intersect( const Ray &ray, double &distance) const
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
    
    if(leq(proection,0)) //no intersection
    {
      return 0; 
    }
    
    double condition = proection * proection + m_radius * m_radius - l2; //condition of intersection
    // (simple Pifagor Th.)
    
    if(leq(condition,0))  //no intersection
    {
      return 0;
    }
    else
    {
      double newDistance = proection - sqrt(condition);
      if( (newDistance > VECTOR_EQUAL_EPS) && (newDistance < distance))
      {
        distance = newDistance;
        return 1;
      }
      else
        return 0;
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
    if( leq(discriminant, 0) )
      return 0;
    
    double newDistance = (sqrt(discriminant) - direction*l)/(direction*direction);
    if( (newDistance > VECTOR_EQUAL_EPS) && (newDistance < distance))
    {
      distance = newDistance;
      return 1;
    }
    else
      return 0;
  };
};


void CSphere::Reflect( const Ray &falling, Ray &reflected) const
{
  double distance = INFINITY;
  CVector fallingOrigin,fallingDirection;
  CVector reflectedOrigin,reflectedDirection;
  
  //   ASSERT (Intersect(falling, &distance)); This does NOTHING in RELEASE version.
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


void CSphere::Refract( const Ray &falling, Ray &refracted, Medium &refractedMedium, bool &outside) const
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
      
      CVector parallelComponent = (vector - (vector*normale) * normale)*m_outerMedium.nRefr/m_innerMedium.nRefr;
      double pcl = parallelComponent.Length();
      if( geq(pcl,1) )
      {
        //full inner reflection
        //so the medium is outer
        refractedMedium.Betta = m_outerMedium.Betta;
        refractedMedium.nRefr = m_outerMedium.nRefr;
        outside = true;
        
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
        outside = false;
        
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
      outside = true;
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
    
    CVector parallelComponent = (vector - (vector*normale) * normale)*m_innerMedium.nRefr/m_outerMedium.nRefr;
    double pcl = parallelComponent.Length();
    if( geq(pcl,1) )
    {
      //full inner reflection
      //so the medium is inner
      refractedMedium.Betta = m_innerMedium.Betta;
      refractedMedium.nRefr = m_innerMedium.nRefr;
      outside = false;
      
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
      outside = true;
      
      vector = parallelComponent - normale*sqrt( 1 - pcl*pcl);
      refracted.setDirection( vector );
    }
  }
};

int  CSphere::IsValid(void) const
{
  if( !CSolid::IsValid() )
    return 0;
  
  if( !(m_radius > VECTOR_EQUAL_EPS) || !(m_radius < INFINITY) )
    return 0;
  
  if ( !geq(m_innerMedium.Betta,0) )
    return 0;
  
  if ( !geq(m_innerMedium.nRefr,0) )
    return 0;
  
  if ( !geq(m_outerMedium.Betta,0) )
    return 0;
  
  if ( !geq(m_outerMedium.nRefr,0) )
    return 0;
  
  return 1;
};

///////////////////////////////////////////////////////////////////
// CPlane 
///////////////////////////////////////////////////////////

CPlane::CPlane()
: m_n(0,0,0)
, m_D(0)
{
};


CPlane::CPlane( const CVector &n, double D, const CVector color,
               double reflectionCoefficient, double smoothness, 
               bool isTransparent, double Betta, double nRefr)
               : CSolid( reflectionCoefficient, smoothness, isTransparent, Betta, nRefr)
{
  m_n = n;
  m_D = D;
  double length = m_n.Length();
  m_n.Normalize();
  SetColor(color);
  
  ASSERT( IsValid() );
  
  m_D /= length;
};

CPlane::CPlane(double a, double b, double c, double d, const CVector color,
               double reflectionCoefficient, double smoothness, 
               bool isTransparent, double Betta, double nRefr)
               : CSolid( reflectionCoefficient, smoothness, isTransparent, Betta, nRefr )
{
  m_n = CVector (a, b, c);
  m_D = d;
  double length = m_n.Length();
  m_n.Normalize();
  SetColor(color);
  
  ASSERT( IsValid() );
  
  m_D /= length;
};

void CPlane::SetPosition(const CVector &n, double D)
{
  m_n = n;
  m_n.Normalize();
  m_D = D;
  
  ASSERT( IsValid() );
};

void CPlane::SetPosition(double a, double b, double c, double d)
{
  //ASSERT (a!=0 || b!=0 || c!=0);
  
  m_n = CVector (a, b, c);
  double length = m_n.Length();
  m_n.Normalize();
  
  ASSERT( IsValid() );
  
  m_D = d/length;
};

int CPlane::Intersect( const Ray &ray, double &distance) const
{
  CVector origin, direction;
  
  ray.getOrigin(origin);
  ray.getDirection(direction);
  
  double scalar = m_n * direction;
  double t;
  
  if ( fabs( scalar ) < 0)  //ray is parallel to plane
  {
    return 0;
  }
  else 
  {
    //if equation  {origin + t * direction = r}, where r satisfies plane equation
    //has positive solutions, then intersection takes place
    t = - (m_D + m_n * origin) / scalar;
    
    if( t < EPSILON )  //no intersection
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

void CPlane::Reflect(const Ray &falling, Ray &reflected) const
{
  double distance;
  CVector fallingOrigin,fallingDirection;
  CVector reflectedOrigin,reflectedDirection;
  
  if (Intersect(falling, distance) == 0) //no intersection
  {
    //just return falling ray
    falling.getOrigin(fallingOrigin);
    falling.getDirection(fallingDirection);
    reflected.setDirection(fallingDirection);
    reflected.setOrigin(fallingOrigin);
  }
  else  //intersects
  {  
    falling.getOrigin(fallingOrigin);
    falling.getDirection(fallingDirection);
    
    reflectedOrigin = fallingOrigin + distance * fallingDirection;
    reflected.setOrigin(reflectedOrigin);
    
    //newDirection = oldDirection + delta( in the derection of normal to the surface)
    reflectedDirection = fallingDirection - 2 * (m_n * fallingDirection) * m_n;
    reflected.setDirection(reflectedDirection);
  }
};

int CPlane::IsValid(void) const
{
  if( !CSolid::IsValid() )
    return 0;
  
  if(fabs(m_n.Length() - 1) > EPSILON) return 0;
  
  return 1;
};

///////////////////////////////////////////////////////////////////
// CBox
///////////////////////////////////////////////////////////

CBox::CBox()
:
m_position(0,0,0)
{
  m_e[0].x = m_e[0].y = m_e[0].z = 0;
  m_e[1].x = m_e[1].y = m_e[1].z = 0;
  m_e[2].x = m_e[2].y = m_e[2].z = 0;
};

CBox::CBox(const CVector &position, const CVector &e0
           , const CVector &e1, const CVector &e2, const CVector &color
           , double Betta, double nRefr
           , bool isTransparent, double outerBetta
           , double outerRefr, double reflectionCoefficient, double smoothness)
           : CSolid( reflectionCoefficient, smoothness, isTransparent)
{
  m_position = position;
  m_e[0] = e0;
  m_e[1] = e1;
  m_e[2] = e2;
  InitNormals();
  
  SetColor(color);
  
  m_innerMedium.Betta = Betta;
  m_innerMedium.nRefr = nRefr;
  m_outerMedium.Betta = outerBetta;
  m_outerMedium.nRefr = outerRefr;
  
  ASSERT( IsValid() );
};

void CBox::InitNormals()
{
  // edges should be orthogonal to each other, but nonzero
  ASSERT( (m_e[0].Length() > VECTOR_EQUAL_EPS)
    && (m_e[1].Length() > VECTOR_EQUAL_EPS)
    && (m_e[2].Length() > VECTOR_EQUAL_EPS) );
  ASSERT( (fabs(m_e[0]*m_e[1]) < EPSILON)
    && (fabs(m_e[1]*m_e[2]) < EPSILON) 
    && (fabs(m_e[2]*m_e[0]) < EPSILON) );  
  
  m_n[0] = m_e[0] ^ m_e[1];
  m_n[0].Normalize();
  m_d1[0] = - (m_position * m_n[0]);
  m_d2[0] = - ((m_position + m_e[2]) * m_n[0]);
  
  m_n[1] = m_e[1] ^ m_e[2];
  m_n[1].Normalize();
  m_d1[1] = - (m_position * m_n[1]);
  m_d2[1] = - ((m_position + m_e[0]) * m_n[1]);
  
  m_n[2] = m_e[2] ^ m_e[0];
  m_n[2].Normalize();
  m_d1[2] = - (m_position * m_n[2]);
  m_d2[2] = - ((m_position + m_e[1]) * m_n[2]);
  
  //flip normals so that m_d1 < m_d2
  for(int i=0; i<3; i++)
  {
    if(m_d1[i] > m_d2 [i])
    {
      m_d1[i] = -m_d1[i];
      m_d2[i] = -m_d2[i];
      m_n[i] = -m_n[i];
    }
  }    
};

void CBox::SetPosition(const CVector &position)
{
  m_position = position;
};

void CBox::SetOrientation(const CVector &e0, const CVector &e1, const CVector &e2)
{
  
  // edges should be orthogonal to each other, but nonzero
  ASSERT( (m_e[0].Length() > VECTOR_EQUAL_EPS)
    && (m_e[1].Length() > VECTOR_EQUAL_EPS)
    && (m_e[2].Length() > VECTOR_EQUAL_EPS) );
  ASSERT( (fabs(m_e[0]*m_e[1]) < EPSILON)
    && (fabs(m_e[1]*m_e[2]) < EPSILON)
    && (fabs(m_e[2]*m_e[0]) < EPSILON) );
  m_e[0] = e0;
  m_e[1] = e1;
  m_e[2] = e2;
  InitNormals();
};

int CBox::Intersect(const Ray &ray, double &distance) const
{
  //if the ray goes through side, then no intersection
  
  CVector origin, direction;
  ray.getOrigin(origin);
  ray.getDirection(direction);
  
  int isInside = IsInside(origin);
  
  double dirP, orP; //direction and origin projections on m_n[i];
  double t, t1, t2;  //2 intersections with plane
  double intersection = INFINITY;
  
  for(int i=0;i<3;i++) //process each side
  {
    dirP = m_n[i] * direction;
    orP = m_n[i] * origin;
    
    if(dirP > EPSILON) // t1 < t2   //?K? What does this comment mean???
    {
      t1 = -(m_d2[i] + orP) / dirP;
      t2 = -(m_d1[i] + orP) / dirP;
    }
    else
    {
      if(dirP < EPSILON) // t1 < t2 //?K? What does this comment mean???
      {
        t1 = -(m_d1[i] + orP) / dirP;
        t2 = -(m_d2[i] + orP) / dirP;
      }
      else  // ray is parallel to sides
      {
        if(  (orP > m_d2[i]-EPSILON) || (orP < m_d1[i]+EPSILON) )
          return 0; //no intersection
        else
          continue;
      }
    }
    //check if intersection points belong to sides
    
    CVector toPoint;
    double x,y; //vector toPoint in basis e1,e2,e3 (one component is 0)
    int k,l; //edges that form side to check
    k = i;
    l = (i+1)%3;
    
    //if the ray origin is outside, we should check only t1
    //if it is inside, then only t2
    
    if(isInside) t = t2;
    else  t = t1;
    
    if(0+EPSILON<t)
    {
      toPoint = origin + t * direction - m_position - isInside * m_e[(i+2)%3];
      x = (toPoint * m_e[k]) / (m_e[k]*m_e[k]);
      y = (toPoint * m_e[l]) / (m_e[l]*m_e[l]);
      if( x>0+EPSILON && x<1-EPSILON && y>0+EPSILON && y<1-EPSILON)
      {
        distance = t;
        return i+1;
      }
    }
  }
  
  return 0;
};

int CBox::IsInside(const CVector &vector) const
{
  CVector delta = vector - m_position;
  double x,y,z; //coordinates of vector 'delta' in e1,e2,e3 basis
  x = (delta * m_e[0]) / (m_e[0] * m_e[0]);
  y = (delta * m_e[1]) / (m_e[1] * m_e[1]);
  z = (delta * m_e[2]) / (m_e[2] * m_e[2]);
  if ( EPSILON < x && x < 1-EPSILON &&
    EPSILON < y && y < 1-EPSILON &&
    EPSILON < z && z < 1-EPSILON) return 1;
  return 0;
};

void CBox::Reflect( const Ray &falling, Ray &reflected) const
{
  double distance = INFINITY; //distance from origin to intersection
  int n_number;               //number of normal to side of intersection
  //no matter inner or outer
  CVector normal;             //normal to side in the intersection point
  CVector fallingOrigin,fallingDirection;
  CVector reflectedOrigin,reflectedDirection;
  
  
  n_number = Intersect(falling, distance); //gets the distance
  ASSERT (n_number);
  
  
  falling.getOrigin(fallingOrigin);
  falling.getDirection(fallingDirection);
  
  reflectedOrigin = fallingOrigin + distance * fallingDirection;
  reflected.setOrigin(reflectedOrigin);
  
  normal = m_n[n_number-1];
  
  //newDirection = oldDirection + delta( in the derection of normal to the surface)
  reflectedDirection = fallingDirection - 2*(normal * fallingDirection)*normal;
  reflected.setDirection(reflectedDirection);
};

void CBox::Refract( const Ray &falling, Ray &refracted, Medium &refractedMedium, bool &outside) const
{
  double distance = INFINITY;
  int n_number;
  
  CVector normal;
  CVector fallingOrigin, fallingDirection;
  
  falling.getOrigin(fallingOrigin);
  falling.getDirection(fallingDirection);
  
  n_number = Intersect(falling,distance) - 1;
  
  if(IsInside(fallingOrigin)) //origin is outside
  {
    
    if(n_number != -1)  //there is intersection   //?K? MAGIC NUMBER!
      //?K? Why -1. Why not -117? Name your constants!!
    {
      refracted.setOrigin(fallingOrigin + distance * fallingDirection);
      normal = m_n[n_number];
      CVector parallelComponent = (fallingDirection-(fallingDirection*normal)*normal)*m_outerMedium.nRefr/m_innerMedium.nRefr;      
      double pcl = parallelComponent.Length();
      if( geq(pcl,1) )
      {
        //full inner reflection
        //so the medium is outer
        refractedMedium.Betta = m_outerMedium.Betta;
        refractedMedium.nRefr = m_outerMedium.nRefr;
        outside = true;
        
        //recompute the refracted ray because it coincides
        //with the reflected one
        CVector reflectedDir = fallingDirection - 2*normal*(fallingDirection*normal);
        refracted.setDirection(reflectedDir);
      }
      else
      {
        //the medium is inner
        refractedMedium.Betta = m_innerMedium.Betta;
        refractedMedium.nRefr = m_innerMedium.nRefr;
        outside = false;
        //just reuse fallingOrigin, do not create additional objects, here it
        //is supposed to be refractedDirection
        fallingOrigin = parallelComponent + normal*(fallingDirection*normal);
        refracted.setDirection( fallingOrigin );
      }
    }
    else  //no refraction, just return the same ray and medium
    {
      refracted.setOrigin( fallingOrigin );
      //reuse fallingOrigin object
      falling.getDirection( fallingOrigin );
      refracted.setDirection( fallingOrigin );
      
      //the medium is outer
      refractedMedium.Betta = m_outerMedium.Betta;
      refractedMedium.nRefr = m_outerMedium.nRefr;
      outside = true;
    }
  }
  else //origin is inside.
  {
    normal = m_n[n_number];
    refracted.setOrigin(fallingOrigin + distance * fallingDirection);
    CVector parallelComponent = (fallingDirection-(fallingDirection*normal)*normal)*m_innerMedium.nRefr/m_outerMedium.nRefr;      
    double pcl = parallelComponent.Length();
    if( geq(pcl,1) )
    {
      //full inner reflection
      //so the medium is inner
      refractedMedium.Betta = m_innerMedium.Betta;
      refractedMedium.nRefr = m_innerMedium.nRefr;
      outside = false;
      //recompute the refracted ray because it coincides
      //with the reflected one
      CVector reflectedDir = fallingDirection - 2*normal*(fallingDirection*normal);
      refracted.setDirection(reflectedDir);
    }
    else
    {
      //the medium is outer
      refractedMedium.Betta = m_outerMedium.Betta;
      refractedMedium.nRefr = m_outerMedium.nRefr;
      outside = true;
      //just reuse fallingOrigin, do not create additional objects, here it
      //is supposed to be refractedDirection
      fallingOrigin = parallelComponent + normal*(fallingDirection*normal);
      refracted.setDirection( fallingOrigin );
    }
  };
};

int CBox::IsValid(void) const
{
  if( !CSolid::IsValid() )
    return 0;
  
  
  
  
  // edges should be orthogonal to each other, but nonzero
  if( (m_e[0].Length() < VECTOR_EQUAL_EPS) 
    || (m_e[1].Length() < VECTOR_EQUAL_EPS) 
    || (m_e[2].Length() < VECTOR_EQUAL_EPS) ) return 0;
  if( (fabs(m_e[0]*m_e[1]) > EPSILON)
    || (fabs(m_e[1]*m_e[2]) > EPSILON)
    || (fabs(m_e[2]*m_e[0]) > EPSILON) ) return 0;
  
  if ( !geq(m_innerMedium.Betta,0) )
    return 0;
  
  if ( !geq(m_innerMedium.nRefr,0) )
    return 0;
  
  if ( !geq(m_outerMedium.Betta,0) )
    return 0;
  
  if ( !geq(m_outerMedium.nRefr,0) )
    return 0;
  
  return 1;
};


///////////////////////////////////////////////////////////
// CTriangle
///////////////////////////////////////////////////////////

CTriangle::CTriangle(const CVector &a, const CVector &b, 
                     const CVector &c, const CVector &color,
                     double reflectionCoefficient, 
                     double smoothness, 
                     bool isTransparent, double Betta, double nRefr) 
                     : CSolid(reflectionCoefficient, smoothness, isTransparent, Betta, nRefr)
{
  //compute two sides dot product
  //if it is really a triangle, it wil be nonzero
  m_normal = (b-a)^(c-a);
  m_normal.Normalize();
  m_distance = a*m_normal;
  m_a = a;
  m_b = b;
  m_c = c;
  SetColor(color);
  
  ASSERT( IsValid() );
};

int CTriangle::Intersect( const Ray &ray, double &distance) const
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
  if( (abProj < 0) || (acProj < 0) || (abProj + acProj > 1)) //?K? EPSILON!!!
    return 0;
  
  //there is an intersection, modify the distance and return 1
  distance = rayDistance;
  return 1;
};

int CTriangle::planeIntersect( const Ray &ray, double &distance ) const
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
  if((rayDistance > distance) || (rayDistance < 0))  //?K? EPSILON !!!
    return 0;
  
  distance = rayDistance;
  return 1;
};

void CTriangle::Reflect( const Ray &falling, Ray &reflected ) const
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

int CTriangle::IsValid(void) const
{
  if( !CSolid::IsValid() )
    return 0;
  
  CVector testNormale = (m_b - m_a)^(m_c - m_a);
  if(testNormale.Length() < VECTOR_EQUAL_EPS)
    return 0;
  
  testNormale.Normalize();
  if ( (testNormale - m_normal).Length() > VECTOR_EQUAL_EPS)
    return 0;
  
  if( fabs( m_a*m_normal - m_distance ) > VECTOR_EQUAL_EPS)
    return 0;
  
  return 1;
};