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
// REVISION by Vader, on 01/23/2004
// Comments: Added CBox class (supports refraction)
//*********************************************************
// REVISION by KIRILL, on 1/24/2004 03:42:00
// Comments: Some refactoring was done
//*********************************************************
// REVISION by ..., on ...
// Comments: ...
//*********************************************************


#include "stdafx.h"
#include "geometry.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////////////////////////////////////////////////
//  CSphere
///////////////////////////////////////////////////////////

CSphere::CSphere()
: m_position(0,0,0)
, m_radius(0)
{  }

CSphere::CSphere( const CVector &position, double radius, double Betta, double nRefr, 
              bool isTransparent, double outerBetta, double outerRefr, double reflectionCoefficient)
{
  ASSERT( radius > 0);
  int comparisonResult = geq( Betta, 0 );
  ASSERT( comparisonResult );
  ASSERT( nRefr > 0 );
  ASSERT( geq(reflectionCoefficient,0) && leq(reflectionCoefficient, 1) );
  
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


void CSphere::Refract( const Ray &falling, Ray &refracted, Medium &refractedMedium) const
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




///////////////////////////////////////////////////////////
// CColorSphere 
///////////////////////////////////////////////////////////


CColorSphere::CColorSphere()
: m_color(0,0,0) 
{}

CColorSphere::CColorSphere(const CVector &position, double radius) 
: CSphere(position, radius)
, m_color(0,0,0)
{}

CColorSphere::CColorSphere(const CVector &position, double radius, const CVector &color) 
: CSphere(position, radius)
, m_color( color )
{ 
  ASSERT( m_color.IsNormalized() ); 
}

void CColorSphere::GetColor( const Ray &falling, CVector &color) const
{
  //do not check the intersection has place (otherwise in most cases we
  //will repeat the computaion already done
  //as every point of the sphere has the same color), just return it
  
  color = m_color;
};



///////////////////////////////////////////////////////////////////
// CPlane 
///////////////////////////////////////////////////////////

CPlane::CPlane()
: m_n(0,0,0)
, m_D(0)
{}

CPlane::CPlane( const CVector &n, double D)
{
  ASSERT( n.Length() > EPSILON );
  m_n = n;
  m_n.Normalize();

  m_D = D;  
};

CPlane::CPlane(double a, double b, double c, double d)
{
  //ASSERT (a!=0 || b!=0 || c!=0);
  
  m_n = CVector (a, b, c);
  double length = m_n.Length();
  ASSERT( length > EPSILON );
  
  m_n /= length;
  m_D = d/length;
};

void CPlane::SetPosition(const CVector &n, double D)
{
  m_n = n;
  m_D = D;
};

void CPlane::SetPosition(double a, double b, double c, double d)
{
  //ASSERT (a!=0 || b!=0 || c!=0);
  
  m_n = CVector (a, b, c);
  double length = m_n.Length();
  ASSERT( length > EPSILON );
  
  m_n /= length;
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




///////////////////////////////////////////////////////////////////
// CBox
///////////////////////////////////////////////////////////

CBox::CBox()
{
  m_position = CVector(0,0,0);
  m_e[0] = CVector(0,0,0);
  m_e[1] = CVector(0,0,0);
  m_e[2] = CVector(0,0,0);
  //InitNormals();
};

CBox::CBox(const CVector &position, const CVector &e0
           , const CVector &e1, const CVector &e2
           , double Betta, double nRefr
           , bool isTransparent, double outerBetta
           , double outerRefr, double reflectionCoefficient)
{
    // edges should be orthogonal to each other
    ASSERT( (fabs(e0*e1) < EPSILON) && (fabs(e1*e2) < EPSILON) && (fabs(e2*e0) < EPSILON) );
    int comparisonResult = geq( Betta, 0 );
    ASSERT( comparisonResult );
    ASSERT( nRefr > 0 );
    ASSERT(   (0 <= reflectionCoefficient) && (reflectionCoefficient <= 1.0) );
  
    m_position = position;
    m_e[0] = e0;
    m_e[1] = e1;
    m_e[2] = e2;
    InitNormals();
    
    m_reflectionCoefficient = reflectionCoefficient;
    m_isTransparent = isTransparent;
    
    m_innerMedium.Betta = Betta;
    m_innerMedium.nRefr = nRefr;
  
    m_outerMedium.Betta = outerBetta;
    m_outerMedium.nRefr = outerRefr;
    m_isTransparent = isTransparent;
};

void CBox::InitNormals()
{
// ?K?  No ASSERTS ??????? Are you sure it's absolutly safe?
  

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
    InitNormals();
};

void CBox::SetOrientation(const CVector &e0, const CVector &e1, const CVector &e2)
{
    // edges should be orthogonal to each other
    ASSERT( (fabs(e0*e1) < VECTOR_EQUAL_EPS) && 
              (fabs(e1*e2) < VECTOR_EQUAL_EPS) && 
                (fabs(e2*e0) < VECTOR_EQUAL_EPS) );
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

void CBox::Refract( const Ray &falling, Ray &refracted, Medium &refractedMedium) const
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
      
      //just reuse fallingOrigin, do not create additional objects, here it
      //is supposed to be refractedDirection
      fallingOrigin = parallelComponent + normal*(fallingDirection*normal);
      refracted.setDirection( fallingOrigin );
    }
  }
};


///////////////////////////////////////////////////////////
// CTriangle
///////////////////////////////////////////////////////////

CTriangle::CTriangle(const CVector &a, const CVector &b, 
                        const CVector &c, const CVector &color)
{
  //check whether color components are correct
  ASSERT( color.IsNormalized() );
  
  //compute two sides dot product
  //if it is really a triangle, it wil be nonzero
  CVector ab, ac;
  ab = b - a;
  ac = c - a;
  m_normal.x = ab.y*ac.z - ab.z*ac.y;     //?K? There is vector multiplication 
  m_normal.y = -(ab.x*ac.z - ab.z*ac.x);   //?K?   operator ^  now. You may use it.
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

void CTriangle::GetColor( const Ray &falling, CVector &color) const
{
  //do not check the intersection has place (otherwise in most cases we
  //will repeat the computaion already done
  //as every point of the triangle has the same color), just return it
  
  color = m_color;
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

