//****************************************
//** environment.cpp **
// Created By: Tonic
// On: 12/11/2003
// Comments: Environment class methods implementation
//
//***********************************
// COMMENT by Tonic, on 12/11/2003
// initial revision, implemented data-centric properties only
//*********************************************************
// REVISION by KIRILL, on 1/9/2004 03:10:10
// Comments: I've modified header a little. My macroses are stupid 
// and don't understand your (TONIC's) idea of header formating :)
//***********************************
// REVISION by Tonic, on 14/11/2003
// Comments:  Adding functionality to Environment( Intersect method )
//***********************************
// REVISION by Tonic, on 12/01/2004
// Comments:  Getters and setters for Ray, Light
//        components, getter for Enviroment light sources
//        Added CVector position to Light, modified constructors
//        Added Ray constructor without parameters
//***********************************
// REVISION by Tonic, on 1/13/2004
// Comments: Added normalization to Ray::setDirection so that
// direction is always of length 1 inside the Ray
//*********************************************************
// REVISION by Tonic, on 01/15/2004
// Comments: Added Environment::AmbientLight, refactoring of class members names
// Added Solid::GetColor returning always white (no check of actual intersection
// with a given ray)
// Changed interfaces to get parameters as references
// instead of pointers
//*********************************************************
// REVISION by Tonic, on 01/16/2004
// Comments: CCamera class added
//*********************************************************
// REVISION by Tonic, on 01/16/2004
// Comments: Added Ray::getPoint
//*********************************************************
// REVISION by KIRILL, on 1/24/2004 03:42:00
// Comments: Some refactoring was done
//*********************************************************
// REVISION by KIRILL, on 1/25/2004 18:26:30
// Comments: I've modified checking of pointers
// in Environment::Add(CLight*) and Environment::Add(CSolid*) 
//*********************************************************
// REVISION by KIRILL, on 1/26/2004
// Comments: CSolid::IsValid added
//*********************************************************
// REVISION by KIRILL, on 1/28/2004 17:18:43
// Comments: Environment renamed to CEnvironment
//*********************************************************
// REVISION by Tonic, on 1/29/2004
// Comments: Added default refraction function to CSolid
//*********************************************************
// REVISION by KIRILL, on 1/29/2004 21:35:10
// Comments: Environment::Empty() in addition zeroes all scene parameters
//*********************************************************
// REVISION by ..., on ...
// Comments: ...
//*********************************************************

#include "stdafx.h"
#include "environment.h"
#include "geometry.h"
#include "common/msg.h"

//Such return means that an error occured in function
#define STORING_ERROR_RETURN 1
#define LOADING_ERROR_RETURN 2

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


///////////////////////////////////////////////////////////
// Medium operators

CArchive& operator << (CArchive& ar, const Medium& m)
{
  ar << m.nRefr;
  ar << m.Betta;
  return ar;
}

CArchive& operator >> (CArchive& ar, Medium& m)
{
  ar >> m.nRefr;
  ar >> m.Betta;
  return ar;
}

///////////////////////////////////////////////////////////
//  Ray  methods implementation
///////////////////////////////////////////////////////////


Ray::Ray( const CVector &origin, const CVector &direction )
{
  ASSERT( direction.Length() != 0);
  
  m_origin = origin;
  m_direction = direction;
  m_direction.Normalize();
};

void Ray::operator = (const Ray& r)
{
  m_origin = r.m_origin;
  m_direction = r.m_direction;
}

void Ray::getPoint( double distance, CVector &point) const
{
  point = m_origin + distance*m_direction;
};

void Ray::getDirection(CVector &direction) const
{
  // = is overridden operator, just makes the components the same
  direction = m_direction;
};

void Ray::setDirection( const CVector &direction)
{
  ASSERT( direction.Length() != 0);
  
  // = is overridden operator, just makes the components the same
  m_direction = direction;
  
  //make the direction length 1
  (m_direction).Normalize();
};

void Ray::getOrigin(CVector &origin) const
{
  // = is overridden operator, just makes the components the same
  origin = m_origin;
};

void Ray::setOrigin( const CVector &origin)
{
  // = is overridden operator, just makes the components the same
  m_origin = origin;
};

///////////////////////////////////////////////////////////
//  CSolid    - interface for a geometric object
///////////////////////////////////////////////////////////


// ?K? What is the purpose for this method to live?
// ?K? Maybe it must be killed and become =0 ?

void CSolid::GetColor( const Ray &falling, CVector &color) const 
{
  //do not check whether there is even a ray
  //just return white
  //more strict implementations in descendants
  color = m_color;
};

int CSolid::IsValid(void) const
{
  if ( !geq(medium.Betta,0) )
    return 0;
  
  if ( !geq(medium.nRefr,0) )
    return 0;
  
  if( leq(m_smoothness,0) )
    return 0;
  
  if ( leq(m_reflectionCoefficient,0) || (m_reflectionCoefficient > 1.0))
    return 0;
  
  if( !m_color.IsNormalized() )
    return 0;
  
  return 1;
};

void CSolid::Refract( const Ray &falling, Ray &refracted, Medium &refractedMedium, bool &outside) const 
{
  outside = true;
  CVector vector;
  falling.getDirection( vector );
  refracted.setDirection( vector );
  refractedMedium.Betta = medium.Betta;
  refractedMedium.nRefr = medium.nRefr;
  
  double distance = INFINITY;
  
  if( Intersect( falling, distance ) )
  {
    falling.getPoint( distance, vector);
  }
  else
    falling.getOrigin( vector );
  
  refracted.setOrigin( vector );
};

//writes class id to the archive
int CSolid::WriteThisClassId(CArchive& ar) const
{
  int id = GetObjectID( this );
  ASSERT( id > _None && id <_Last);
  ar << (int)id;
  return 0; //no error
}
 
//helper function - returns ObjectID by object pointer
int CSolid::GetObjectID(const CSolid* obj)
{
  ASSERT( obj );

  if ( dynamic_cast<const CSphere*>(obj) ) return Sphere;
  if ( dynamic_cast<const CPlane*>(obj) ) return Plane;
  if ( dynamic_cast<const CBox*>(obj) ) return Box;
  if ( dynamic_cast<const CTriangle*>(obj) ) return Triangle;
  if ( dynamic_cast<const CCylinder*>(obj) ) return Cylinder;
  
  ASSERT( 0 ); //unknown object type?
  return _None;
}

//helper function - creates and returns object be its ObjectID
CSolid* CSolid::NewObjectByID(int id)
{
  ASSERT( id > _None && id <_Last );

  switch (id) {
    case Sphere:   return new CSphere;
    case Plane:    return new CPlane;
    case Box:      return new CBox;
    case Triangle: return new CTriangle;
    case Cylinder: return new CCylinder;
    default: ASSERT(0); //unknown id
  }

  return 0;
}

//reads object from archive. First it reads object type and than 
//it creates the objects and reads it data. Returns pointer to the object
//or zero if an error occured.
CSolid* CSolid::readObject( CArchive& ar )
{
  int obj_id; 
  ar >> obj_id; //first we read object id

  ASSERT( obj_id > _None && obj_id < _Last );

  CSolid* solid = NewObjectByID( obj_id ); //second we create object of appropriate type
  ASSERT( solid );

  if (solid){ 
    int ret = solid->read(ar); //third we read the object data
    if ( ret || !solid->IsValid() ){
      delete solid; //if something is wrong we delete object and return zero
      solid = 0;
    }
  }
  return solid;
}


///////////////////////////////////////////////////////////
// CLight     - model of an abstract light source
///////////////////////////////////////////////////////////

CLight::CLight()
: m_color(0,0,0)
{}

CLight::CLight(  const CVector &color,  const CVector &position )
: m_color( color )
, m_position( position )
{
  ASSERT( IsValid() );  
}

CLight::CLight( double r, double g, double b, double x, double y, double z)
: m_color(r,g,b)
, m_position(x,y,z)
{
  ASSERT( IsValid() );
};

int CLight::IsValid() const
{
  if ( !m_color.IsNormalized() ) return 0;
  
  return 1;
}

void CLight::getPosition(CVector &position) const
{
  position = m_position;
};

void CLight::setPosition( const CVector &position) 
{
  m_position = position;
};

void CLight::getColor(CVector &color) const
{
  color = m_color;
};

void CLight::setColor( const CVector &color) 
{
  ASSERT( color.IsNormalized() );  
  m_color = color;
};


//Storing routine
int CLight::write(CArchive& ar) const
{
  ASSERT( IsValid() );
  ar << m_position;
  ar << m_color;
  return 0;
}

//Loading routine. Nonzero if received invalid values.
int CLight::read (CArchive& ar)
{
  ar >> m_position;
  ar >> m_color;
  
  if (!IsValid()){
    ASSERT( 0 );
    return 1;
  }

  return 0;
}



////////////////////////////////////////////////////////////////
//  CEnvironment - methods impementation for scene storage class
////////////////////////////////////////////////////////////////

CEnvironment::CEnvironment( )
: m_AmbientColor(0,0,0)
{}

CEnvironment::~CEnvironment () 
{
  //empty Solids and Lights
  Empty();
}

void CEnvironment::Empty()
{
  m_solids.Empty();
  m_lights.Empty();
  m_AmbientColor = CVector(0,0,0);
  m_scene_uid = 0;
}

void CEnvironment::Add ( CLight *light )
{
  ASSERT( light != NULL );
  
  int lightsCount = m_lights.GetSize();
  
#ifdef _DEBUG
  //check if this element is already present
  //if it holds, do not add repeatedly, just return
  for( int i = 0; i < lightsCount; i++ )
    if( m_lights[i] == light )
      ASSERT(0);
#endif//_DEBUG
    
    //actually add light source to environment
    m_lights.Add(light);
};

void CEnvironment::Add ( CSolid *solid )
{
  ASSERT( solid != NULL );
  
  int solidsCount = m_solids.GetSize();
  
#ifdef _DEBUG
  //check if this element is already present
  //if it holds, do not add repeatedly, just return
  for(int i = 0; i < solidsCount; i++ )
    if( m_solids[i] == solid )
      ASSERT(0);
#endif//_DEBUG
    
    //actually add element to environment
    m_solids.Add(solid);
};


void  CEnvironment::SetAmbientColor( const CVector &AmbientColor )
{
  ASSERT( AmbientColor.IsNormalized() );  
  
  m_AmbientColor = AmbientColor;
};

void  CEnvironment::GetAmbientColor( CVector &AmbientColor ) const
{
  AmbientColor = m_AmbientColor;
}

void CEnvironment::getLightByNumber(int number, CLight &light) const
{
  //there is a light source with such a number
  ASSERT( (number >= 0) && (number < m_lights.GetSize()) );
  
  light = *(m_lights.GetAt(number));
};


//determines the closest intersected object and distance
CSolid * CEnvironment::Intersect (  const Ray &ray, double &t ) const
{
  CSolid*  closestObj = NULL;
  //do not allow intersections with objects
  //farther than t away
  double  closestDist = t;
  int solidsCount = m_solids.GetSize();
  
  //for every object
  for ( int i = 0; i < solidsCount; i++ )
  {
    //find intersection
    if ( m_solids.GetAt(i) -> Intersect ( ray, t ) != 0 )
    {
      //check distance
      if ( t < closestDist )
      {
        //update if closer
        closestDist = t;
        closestObj  = m_solids.GetAt(i);
      }
    };
  };
  
  t = closestDist;
  return closestObj;
};

int CEnvironment::IsValid(void) const
{
  if ( !m_AmbientColor.IsNormalized() ) return 0;
  if ( !m_lights.IsValid() ) return 0;
  if ( !m_lights.IsValid() ) return 0;
  return 1;
}

int CEnvironment::write(CArchive& ar) const
{
  ASSERT( IsValid() );
  
  ar << m_AmbientColor;
  if ( m_lights.write( ar ) )
    return STORING_ERROR_RETURN;
  
  if ( m_solids.write( ar ) )
    return STORING_ERROR_RETURN;
  
  return 0;
}

int CEnvironment::read (CArchive& ar)
{
  Empty(); //empty scene first
  
  ar >> m_AmbientColor;
  
  int ret = m_lights.read( ar );
  if ( ret || !m_lights.IsValid() ){
    m_lights.Empty();
    return LOADING_ERROR_RETURN;
  }

  ret = m_solids.read( ar );
  if ( ret || !m_solids.IsValid() ){
    m_solids.Empty();
    return LOADING_ERROR_RETURN;
  }

  ASSERT( IsValid() );

  Message("[Environment::read] We have %d solids and %d lights."
        , m_solids.GetSize(), m_lights.GetSize() );
  
  return 0;
}

///////////////////////////////////////////////////////////
//CCamera    - this class is fully responsible for transformation
//              between pixel coordinates of the picture to traced rays
//
///////////////////////////////////////////////////////////

CCamera::CCamera()
: m_eyePoint(0,0,0)
, m_viewDir(0,0,0)
, m_topDir(0,0,0)
, m_horDir(0,0,0)
, m_width(0)
, m_height(0)
, m_horizontalAngle(0)
, m_verticalAngle(0)
, m_minViewAngle(0)
, m_maxViewAngle(0)
{}

CCamera::CCamera( const CVector &eyePoint, const CVector &viewDir, 
                 const CVector &topDir, int width, int height )
{
  Init(eyePoint, viewDir, topDir, width, height );
};

void CCamera::Init(const CVector &eyePoint, const CVector &viewDir 
                   , const CVector &topDir, int width, int height )
{
  //Check that width and height are both positive
  ASSERT( width > 0 && height >0);
  
  //check that view direction and top direction are not parallel
  double lengthProduct = viewDir.Length() * topDir.Length();
  double scalarProduct = fabs( viewDir * topDir );
  ASSERT( scalarProduct < lengthProduct - VECTOR_EQUAL_EPS );  // ?K? Are you sure ???
  // ?K? Maybe you mean "+ VECTOR_EQUALS_EPSILON"?
  
  m_width = width;
  m_height = height;
  m_eyePoint = eyePoint;
  m_viewDir = viewDir;
  m_topDir = topDir;
  m_minViewAngle = 0.1;  // ?K? Why? 
  m_maxViewAngle = 3.0;  // ?K? Why?
  m_verticalAngle = m_horizontalAngle = 1.57; // ?K? Why? 
  
  m_viewDir.Normalize();
  m_topDir.Normalize();
  
  //now make top direction orthogonal to the viewing direction
  m_topDir = m_topDir - m_viewDir * (( m_viewDir * m_topDir ) / m_topDir.Length());
  m_topDir.Normalize();
  
  UpdateHorizontalDir();
}

void CCamera::Move(double length)
{
  m_eyePoint += m_viewDir*length;
};

void CCamera::Shift(double length)
{
  m_eyePoint += m_horDir*length;
};

void CCamera::Yaw(double angle)
{
  //view direction == y
  //hor direction == x
  //y_new = y*cos - x*sin
  //x_new = x*cos + y*sin
  
  double sinus = sin( angle);
  double cosinus = cos( angle );
  CVector viewDir = (m_viewDir * cosinus) - (m_horDir * sinus);
  CVector horDir  = (m_horDir * cosinus) + (m_viewDir * sinus);
  m_viewDir = viewDir;
  m_horDir = horDir;
  m_viewDir.Normalize();
  m_horDir.Normalize();
};

void CCamera::Pitch( double angle )
{
  //view direction == x
  //top direction == y
  //y_new = y*cos - x*sin
  //x_new = x*cos + y*sin
  
  double sinus = sin( angle);
  double cosinus = cos( angle );
  CVector topDir = (m_topDir * cosinus) - (m_viewDir * sinus);
  CVector viewDir  = (m_viewDir * cosinus) + (m_topDir * sinus);
  m_viewDir = viewDir;
  m_topDir = topDir;
  m_viewDir.Normalize();
  m_topDir.Normalize();
};

void CCamera::SetVerticalAngle(double angle)
{
  ASSERT( (m_minViewAngle < angle) && (angle < m_maxViewAngle));
  m_verticalAngle = angle;
};

void CCamera::SetHorizontalAngle(double angle)
{
  ASSERT( (m_minViewAngle < angle) && (angle < m_maxViewAngle));
  m_horizontalAngle = angle;
};

void CCamera::SetHeight(int height)
{
  ASSERT( height > 0 );
  m_height = height;
};

void CCamera::SetWidth(int width)
{
  ASSERT( width > 0 );
  m_width = width;
};

void CCamera::GetVerticalAngle(double &verticalAngle) const
{
  verticalAngle = m_verticalAngle;
};

void CCamera::GetEyePoint( CVector &eyePoint ) const
{
  eyePoint = m_eyePoint;
};

void CCamera::GetViewDir( CVector &viewDir ) const
{
  viewDir = m_viewDir;
};

void CCamera::GetTopDir( CVector &topDir ) const
{
  topDir = m_topDir;
};

void CCamera::GetWidth(int &width) const
{
  width = m_width;
};

void CCamera::GetHeight(int &height) const
{
  height = m_height;
};

void CCamera::GetHorizontalAngle(double &horizontalAngle) const
{
  horizontalAngle = m_horizontalAngle;
};

void CCamera::PixelRay(int x, int y, Ray &ray) const
{
  //check whether the pixel is actually
  //within picture bounds
  ASSERT( (x >= 0) && (x < m_width) );
  ASSERT( (y >= 0) && (y < m_height) );
  
  double sinus = sin( m_horizontalAngle/2 );
  double cosinus = cos( m_horizontalAngle/2 );
  
  //getting the rotation of view direction
  //around the top to horAngle/2
  //this is the left boundary
  CVector bound = cosinus*m_viewDir - sinus*m_horDir;
  
  //take into account only x offset
  CVector horizontalNewPoint = bound + ( (bound*m_viewDir)*m_viewDir - bound )*((double)2)*((double) x)/((double) m_width);
  horizontalNewPoint /= (bound*m_viewDir);
  
  //getting the rotation of view direction
  //around the hor to verAngle/2
  //this is the top boundary
  sinus = sin( m_verticalAngle/2 );
  cosinus = cos( m_verticalAngle/2 );
  bound = cosinus*m_viewDir + sinus*m_topDir;
  CVector verticalNewPoint = bound + ( (bound*m_viewDir)*m_viewDir - bound )*((double)2)*((double) y)/((double) m_height);
  verticalNewPoint /= (bound*m_viewDir);
  
  ray.setOrigin( m_eyePoint );
  ray.setDirection( verticalNewPoint + horizontalNewPoint - m_viewDir);
};

void CCamera::UpdateHorizontalDir(void)
{
  //compute the vector product [viewDir x topDir]
  //?K?:  Why don't we use vector product form CVector here?
  
  m_horDir.x = m_viewDir.y*m_topDir.z - m_viewDir.z*m_topDir.y;
  m_horDir.y = -(m_viewDir.x*m_topDir.z - m_viewDir.z*m_topDir.x);
  m_horDir.z = m_viewDir.x*m_topDir.y - m_viewDir.y*m_topDir.x;
  m_horDir.Normalize();
};