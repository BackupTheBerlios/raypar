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


#if !defined(CLIENT_ENVIRONMENT_H_INCLUDED)
#define CLIENT_ENVIRONMENT_H_INCLUDED

#include "common/vector.h"


//geometry-related parameters
#define	INFINITY	30000 //maximal distance

struct	Medium				// main properties of the medium
{
	double	nRefr;			// refraction coefficient
	double	Betta;			// attenuation coefficient
}; 


class	Ray
{
private:
	//this id esentially a point
	CVector	m_origin;
	// direction must be normalized
	CVector	m_direction;
	
public:
	
	Ray (  CVector &origin,  CVector &direction );
	Ray(void)
	{	
		m_origin.x = 0;
		m_origin.y = 0;
		m_origin.z = 0;
		m_direction.x = 1;
		m_direction.y= 1;
		m_direction.z= 1;
	}

	//in getters one must provide a pointer to the place
	//where a copy of a private member will be written
	void getOrigin(CVector &origin) ;
	void getDirection(CVector &direction) ;
	
	//return point distance away from origin
	//i.e. origin + distance*direction
	void getPoint(double distance, CVector &point);

	void setOrigin( CVector &origin);
	void setDirection( CVector &direction);
	
};


//interface for a geometric object
class	CSolid
{
public:

	virtual int Intersect(  Ray &ray, double &distance) = 0;
	virtual void Reflect( Ray &falling, Ray &reflected) = 0;
	
	//returns the color of the point, in which given ray
	//intersects the solid
	//by default (in this class) always returns white
	//can be redefined returning to check whether there is actually
	//an intersection, to support colored and textured objects
	virtual void GetColor( Ray &falling, CVector &color);
}; 

class	CLight			// model of an abstract light source
{
private:
	//RGB brightness, each component in [0;1]
	CVector m_color;
	CVector m_position;	
public:
	
	CLight(); //default constructor initialized to white
	CLight(double r, double g, double b, double x, double y, double z);
	CLight( CVector &color,  CVector &position);

	void getPosition(CVector &position) ;
	void setPosition( CVector &position);
	void getColor(CVector &color) ;
	void setColor( CVector &color);
};


//storage class for all geometrical objects (based on CArray collection)
class CSolidArray : protected CPtrArray{
public:
  CSolid* GetAt( int nIndex ) const {return (CSolid*) CPtrArray::GetAt(nIndex); }
  CSolid* operator[](int nIndex) const { return GetAt(nIndex); }
  int Add( CSolid* solid )  { return CPtrArray::Add( solid ); }
  int GetSize() {return CPtrArray::GetSize();}
};

//storage class for all lights (based on CArray collection)
class CLightArray : protected CPtrArray{
public:
  CLight* GetAt( int nIndex ) const {return (CLight*) CPtrArray::GetAt(nIndex); }
  CLight* operator[](int nIndex) const { return GetAt(nIndex); }
  int Add( CLight* light )  { return CPtrArray::Add( light ); }
  int GetSize() {return CPtrArray::GetSize();}
};


class	Environment
{
private:
	CLightArray m_lights;
	CSolidArray	m_solids;
	//int		m_lightsCount;
	//int		m_solidsCount;
	CVector	m_AmbientColor;
	
public:
	
	Environment (); //initialize CLightArray and CSolidArray params
	~Environment (){};	//do nothing as contained objects may be reused
	
	void	Add ( CLight *light );
	void	Add ( CSolid *solid );
	void	SetAmbientColor(  CVector &AmbientColor );
	void	GetAmbientColor( CVector &AmbientColor ) ;

	int		getLightsCount(void) 
	{ return m_lights.GetSize(); };

	void getLightByNumber(int number, CLight &light) ;
	
	//returns first intersected object and distance from ray origin point
	CSolid *	Intersect (  Ray &ray, double &t ) ;
};

//class fully responsible for transformation
//between pixel coordinates of the picture
//to traced rays
class CCamera
	{
	private:
		//view direction and top direction do not have to
		//be orthogonal. They just have not to be parallel
		//orthogonalization occurs in constructor
		CVector m_eyePoint, m_viewDir, m_topDir, m_horDir;

		//rendered picture resolution
		int m_width, m_height;

		//view angles
		double m_horizontalAngle, m_verticalAngle, m_minViewAngle, m_maxViewAngle;

		//called when wiew direction
		//or top direction are updated
		void UpdateHorizontalDir(void);
	public:
		CCamera( CVector &eyePoint, CVector &viewDir, CVector &topDir, int width, int height );
		
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
		//as usual with bitmaps
		//pixel (x,y) axes are directed (right, down)
		//BUT camera has its x and y axis directed
		//(right,up) as usual in geometry
		//so those who call this method should NOT convert
		//pixel coordinates in any way
		virtual void PixelRay(int x, int y, Ray &ray);

		//getters for everything
		virtual void GetEyePoint( CVector &eyePoint );
		virtual void GetViewDir( CVector &viewDir );
		virtual void GetTopDir( CVector &topDir );
		virtual void GetWidth(int &width);
		virtual void GetHeight(int &height);
		virtual void GetHorizontalAngle(double &horizontalAngle);
		virtual void GetVerticalAngle(double &verticalAngle);

		//setters for picture size and view angles
		virtual void SetWidth(int width);
		virtual void SetHeight(int height);
		virtual void SetHorizontalAngle(double horizontalAngle);
		virtual void SetVerticalAngle(double verticalAngle);

	};
#endif // CLIENT_ENVIRONMENT_H_INCLUDED