//*********************************************************
//** environment.h **
// Created By: Tonic
// On: 12/11/2003
// Comments: Defines world primitives interfaces: ray, environment, and its 
//			abstract contents: Light and Solid
//
//*********************************************************
// COMMENT by Tonic, on 12/11/2003
// initial revision, implemented data-centric properties only
//*********************************************************
// REVISION by Tonic, on 14/11/2003
// Comments:	Adding functionality to Solid (virtual intersect, reflect methods), 
//				Environment( intersect ), defined INFINITY
//*********************************************************
// REVISION by KIRILL, on 1/9/2004 03:10:10
// Comments: I've modified header a little. My macroses are stupid 
// and don't understand your (TONIC's) idea of header formating :)
//*********************************************************
// REVISION by Tonic, on 12/01/2004
// Comments:	Added getters and setters for Ray, Light
//				components, getter for Enviroment light sources
//				Added CVector position to Light, modified constructors
//				Added Ray constructor without parameters
//*********************************************************
// REVISION by Vader on 01/14/2004
// Comments: Changes in the definition of Solid interface
//
//*********************************************************
// REVISION by Tonic, on 01/14/2004
// Comments: Added checking of previous includes of this file. 
// Multiple includes do not cause "type redefined" errors anymore
//*********************************************************
// REVISION by Tonic, on 01/15/2004
// Comments: Added Environment::AmbientLight, refactoring of class members names
// Changed interfaces to get parameters as references
// instead of pointers
//*********************************************************
// REVISION by Tonic, on 01/16/2004
// Comments: CCamera class added
//*********************************************************
// REVISION by Tonic, on 01/16/2004
// Comments: Added Ray::GetPoint
//*********************************************************
// REVISION by Vader, on 01/18/2004
// Comments: Light and Solid classes changed into CLight and CSolid
// Added ligths and solids storage classes CLightArray and CSolidArray
// (based on CPtrArray collection) instead of usual arrays of fixed length
// in Environment class  m_solidsCount and m_lightsCount are not
// necessary now, use CPtrArray.GetSize() instead.
//*********************************************************
// REVISION by Vader, on 01/18/2004
// Comments: GetSize now returns number of objects in the Array
// as the increment of size is 1 and there is no access to any
// Delete functions
//*********************************************************
// REVISION by Tonic, on 01/19/2004
// Comments: Added Solid::IsTransparent, Refract and GetInnerMedium
// Transparent objects are assumed __NOT__ to intersect!!
// otherwise it is impossible to compute refracted rays
//*********************************************************
// REVISION by Tonic, on 01/19/2004
// Comments: Solid - reflection coefficient support
//*********************************************************
// REVISION by Tonic, on 01/21/2004
// Comments: Added variable material smoothness support to CSolid
//*********************************************************
// REVISION by KIRILL, on 1/24/2004 03:42:00
// Comments: Some refactoring was done
//*********************************************************
// REVISION by KIRILL, on 1/25/2004 18:23:15
// Comments: CLightArray and CSolidArray moved to COMMON\SceneArray.h
//*********************************************************
// REVISION by KIRILL, on 1/25/2004 18:39:25
// Comments: IsValid() routine added in CLight, CSolid and 
//   its descendants which provides checking of its members.
//*********************************************************
// REVISION by KIRILL, on 1/28/2004 17:18:43
// Comments: Environment renamed to CEnvironment
//*********************************************************
// REVISION by KIRILL, on 1/28/2004 19:33:12
// Comments: m_scene_uid added to the CEnvironment for the 
// communication purpose. This is UID of the scene in the system.
//*********************************************************
// REVISION by KIRILL, on 1/28/2004 23:52:31
// Comments: read/write methods added to solid
//*********************************************************
// REVISION by Tonic, on 1/29/2004
// Comments: Added default refraction function to CSolid
//*********************************************************
// REVISION by ..., on ...
// Comments: ...
//*********************************************************

#if !defined(CLIENT_ENVIRONMENT_H_INCLUDED)
#define CLIENT_ENVIRONMENT_H_INCLUDED

#include "common/vector.h"
#include "common/SceneArray.h"

//geometry-related parameters
#define	INFINITY	30000 //maximal distance

///////////////////////////////////////////////////////////
//  Medium      - main properties of the medium

struct Medium				
{
  double	nRefr;			// refraction coefficient
  double	Betta;			// attenuation coefficient
}; 

CArchive& operator << (CArchive& ar, const Medium& m);
CArchive& operator >> (CArchive& ar, Medium& m);

///////////////////////////////////////////////////////////
//  Ray   defines a ray by its origin and direction.
//  All ray points can be represeted by (origin + t * direction), where
//  t>=0 is real. And vice versa all such points belong to ray
class	Ray
{
protected:
  CVector	m_origin; //Ray initial point
  CVector	m_direction; // direction must be normalized
  
public:
  
  Ray ( const CVector &origin,  const CVector &direction );
  
  Ray() 
    : m_origin(0,0,0)
    , m_direction(0,0,0) 
  {}
  
  //in getters one must provide a pointer to the place
  //where a copy of a private member will be written
  void getOrigin(CVector &origin) const;
  void getDirection(CVector &direction) const;
  
  //return point distance away from origin
  //i.e. origin + distance*direction
  void getPoint(double distance, CVector &point) const;
  
  void setOrigin( const CVector &origin);
  void setDirection( const CVector &direction);
  
  void operator = (const Ray& r);  
  
protected:
  int  operator == (const Ray&) const; //declared, but not defined
};

///////////////////////////////////////////////////////////
//  CSolid    - interface for a geometric object

class	CSolid
{
protected:

  //m_smoothness is the cosine exponent in the lights computation expression
  //see SimpleTracer::processLights for usage
  double m_smoothness;

  //the colors of reflected and refracted rays are added to the point color
  //with the weights of m_reflectionCoefficient and (1 - m_reflectionCoefficient)
  //respectively
  double m_reflectionCoefficient;
  CVector m_color;

  //outer medium. used in refracted rays computation for flat objects
  Medium medium;

  //refracted rays are computed iff this is true
  bool m_isTransparent;
  
public:

  CSolid( double reflectionCoefficient = 1.0, double smoothness = 1.0
          , bool isTransparent = false, double Betta = 0.0, double nRefr = 1.0 ) 
  : m_color(0,0,0)
  {
    m_reflectionCoefficient =  reflectionCoefficient;
    m_smoothness = smoothness;
    m_isTransparent = isTransparent;
    medium.Betta = Betta;
    medium.nRefr = nRefr;
  
    ASSERT( IsValid() );
  };

  virtual ~CSolid()
  {}


  // Searches for intersection with 'ray', saves distance from ray origin
  // to the intersection in 'distance' and returns intersection status
  virtual int Intersect( const Ray &ray, double &distance) const = 0;

  // Gets the reflected ray and saves the result in 'reflected'
  virtual void Reflect( const Ray &falling, Ray &reflected) const = 0;

  virtual void Refract( const Ray &falling, Ray &refracted, Medium &refractedMedium, bool &outside) const;

  //returns the color of the point, in which given ray
  //intersects the solid
  //by default (in this class) always returns white
  //can be redefined returning to check whether there is actually
  //an intersection, to support colored and textured objects
  virtual void GetColor( const Ray &falling, CVector &color) const;

  //the refractable object may be opaque
  //which is a default value
  //if we change this, we should redefine
  //Refract and GetInnerMedium functions
  virtual bool IsTransparent(void) const
  {
    return m_isTransparent;
  };
  
  virtual double GetReflectionCoefficient(void) const
  {
    return m_reflectionCoefficient;
  };

  virtual void SetReflectionCoefficient(double reflectionCoefficient)
  {
    //do not touch ">=" here!!
    //reflectionCoefficient CANNOT be negative, even if
    //its absolute value is very small
    ASSERT( (reflectionCoefficient >= 0) && !(reflectionCoefficient > 1.0) );
    m_reflectionCoefficient = reflectionCoefficient;
  };

  //smoothness is the exponent of the cosine of
  //angle between normale direction and light direction
  //in computing the lighting. The more is the smoothness
  //the more focused spot is left by a light source
  //see SimpleTracer::ProcessLights for usage
  virtual double GetSmoothness(void) const
  {
    return m_smoothness;
  };

  virtual void SetSmoothness(double smoothness )
  {
    ASSERT( smoothness > VECTOR_EQUAL_EPS );
    m_smoothness = smoothness;
  };

  virtual void SetColor( const CVector &color )
  {
    ASSERT( color.IsNormalized() );
    m_color = color;
  };
  
  //checks wether the oject is valid
  virtual int IsValid(void) const;

  //stores/loads object from/to  'ar'
  virtual int write(CArchive& ar) const { return 0; }  //= 0; ?K? KIRILL: temporarily

  //reads object from archive. First it reads object type and than 
  //it creates the objects and reads it data. Returns pointer to the object
  //or zero if an error occured.
  static CSolid* readObject( CArchive& ar );

protected:

  enum ObjectsIDs{ 
      _None = 0
    , Sphere
    , Plane
    , Box
    , Triangle
    , Cylinder
    , _Last }; //_Last is used to identify bounds, insert new items BEFORE it.
  

  //writes class id to the archive
  int WriteThisClassId(CArchive& ar) const;

  //this method can be called only by CSolid::readObject
  virtual int read(CArchive& ar)        { return 0; }  //will be = 0; ?K? KIRILL: temporarily

  //helper function - returns ObjectID by object pointer
  static int GetObjectID(const CSolid* obj);

  //helper function - creates and returns object be its ObjectID
  static CSolid* NewObjectByID(int id);
}; 

///////////////////////////////////////////////////////////
// CLight     - model of an abstract light source

class	CLight			
{
protected:
  CVector m_position;	 // Light position  
  CVector m_color; //RGB brightness, each component is in [0;1]
  
public:
  CLight(); //default constructor
  CLight(double r, double g, double b, double x, double y, double z);
  CLight( const CVector &color,  const CVector &position);
  
  void getPosition(CVector &position) const ;
  void setPosition(const CVector &position);
  
  void getColor(CVector &color) const;
  void setColor(const CVector &color);
  
  //nonzero if the object has valid parameters, zero otherwise
  int IsValid(void) const;
  
  //storing and loading routines. Nonzero if received invalid values.
  //this may throw communication exceptions.
  int write(CArchive& ar) const;
  int read (CArchive& ar);
};


///////////////////////////////////////////////////////////
//  CEnvironment - storage class for rhe whole scene

class	CEnvironment
{
protected:
  CLightArray m_lights;
  CSolidArray	m_solids;
  CVector	m_AmbientColor;
  int m_scene_uid; //scene UID in the system. Scenes with equal IDs must be equal.
            //it is used by protocol and isn't required to be processed by read/write

public:
  
  CEnvironment (); //initialize CLightArray and CSolidArray params
  virtual ~CEnvironment () ;	//Empties solids and lights arrays on destuction

  void Empty(void); ////Empties solids and lights arrays

  void	Add ( CLight *light );
  void	Add ( CSolid *solid );
  void	SetAmbientColor( const CVector &AmbientColor );
  void	GetAmbientColor( CVector &AmbientColor ) const;
  
  int		getLightsCount(void) const 
  { return m_lights.GetSize(); };
  
  void getLightByNumber(int number, CLight &light) const;
  
  //returns first intersected object and distance from ray origin point
  CSolid *	Intersect ( const Ray &ray, double &t ) const;
  
  int IsValid(void) const;


  int  GetSceneUID(void) const { return m_scene_uid; }
  void SetSceneUID(int scene_uid) { m_scene_uid = scene_uid; }

  int write(CArchive& ar) const;
  int read (CArchive& ar);
};

///////////////////////////////////////////////////////////
//CCamera    - this class is fully responsible for transformation
//              between pixel coordinates of the picture to traced rays


class CCamera
{
protected:
  //view direction and top direction do not have to
  //be orthogonal. They just have not to be parallel
  //orthogonalization occurs in constructor
  CVector m_eyePoint;
  CVector m_viewDir;
  CVector m_topDir;
  CVector m_horDir;
  
  //rendered picture resolution
  int m_width;
  int m_height;
  
  //view angles  
  double m_horizontalAngle;  // ?K? Comments???
  double m_verticalAngle;
  double m_minViewAngle;
  double m_maxViewAngle;
  
public:
  CCamera();
  CCamera( const CVector &eyePoint, const CVector &viewDir, 
    const CVector &topDir, int width, int height );
  
  //setups camera internal parameters
  void Init(const CVector &eyePoint, const CVector &viewDir 
         , const CVector &topDir, int width, int height );

  //move along the viewing direction
  //distance can be both positive and negative
  //positive distance corresponds to moving forward
  virtual void Move(double length);
  
  //move along the horizontal direction
  //positive distance corresponds to shifting right
  //i.e. [viewDir x topDir] <- vector product
  virtual void Shift(double length);
  
  //rotating around the vertical axis
  //positive angle corresponds to rotating left
  virtual void Yaw(double angle);
  
  //rotating around the horizontal axis
  //positive angle corresponds to rotating up
  virtual void Pitch(double angle);
  
  //compute the ray originating from view point
  //and passing through a given pixel
  //!!! IMPORTANT !!!!
  //!!! pixel coordinates are (0,0) in the top-left corner
  //as is is usual in Windows
  //pixel (x,y) axes are directed (right, down)
  //BUT camera has its x and y axis directed
  //(right,up) as usual in geometry
  //so those who call this method should NOT convert
  //pixel coordinates in any way
  virtual void PixelRay(int x, int y, Ray &ray) const;
  
  //getters for everything
  virtual void GetEyePoint( CVector &eyePoint ) const;
  virtual void GetViewDir( CVector &viewDir ) const;
  virtual void GetTopDir( CVector &topDir ) const;
  virtual void GetWidth(int &width) const;
  virtual void GetHeight(int &height) const;
  virtual void GetHorizontalAngle(double &horizontalAngle) const;
  virtual void GetVerticalAngle(double &verticalAngle) const;
  
  //setters for picture size and view angles
  virtual void SetWidth(int width);
  virtual void SetHeight(int height);
  virtual void SetHorizontalAngle(double horizontalAngle);
  virtual void SetVerticalAngle(double verticalAngle);
  
protected:
  //called when view direction
  //or top direction are updated
  void UpdateHorizontalDir(void);
  
};
#endif // CLIENT_ENVIRONMENT_H_INCLUDED