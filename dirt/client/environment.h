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


#if !defined(CLIENT_ENVIRONMENT_H_INCLUDED)
#define CLIENT_ENVIRONMENT_H_INCLUDED

#include "common/vector.h"

//define maximal number of elements in the environment
//using static memory allocation
#define	MAX_LIGHTS	10
#define	MAX_SOLIDS	100

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
	
	void setOrigin( CVector &origin);
	void setDirection( CVector &direction);
	
};


//interface for a geometric object
class	Solid
{
public:

	virtual int Intersect(  Ray &ray, double &distance)=0;
	virtual void Reflect( Ray &falling, Ray &reflected)=0;
	
	//returns the color of the point, in which given ray
	//intersects the solid
	//by default (in this class) always returns white
	//can be redefined returning to check whether there is actually
	//an intersection, to support colored and textured objects
	virtual void GetColor( Ray &falling, CVector &color);
}; 

class	Light			// model of an abstract light source
{
private:
	//RGB brightness, each component in [0;1]
	CVector m_color;
	CVector m_position;	
public:
	
	Light(); //default constructor initialized to white
	Light(double r, double g, double b, double x, double y, double z);
	Light( CVector &color,  CVector &position);

	void getPosition(CVector &position) ;
	void setPosition( CVector &position);
	void getColor(CVector &color) ;
	void setColor( CVector &color);
};

class	Environment
{
private:
	Light	*m_lights[MAX_LIGHTS];
	Solid	*m_solids[MAX_SOLIDS];
	int		m_lightsCount;
	int		m_solidsCount;
	CVector	m_AmbientColor;
	
public:
	
	Environment (); //initialize counters to zeros
	~Environment (){};	//do nothing as contained objects may be reused
	
	void	Add ( Light *light );
	void	Add ( Solid *solid );
	void	SetAmbientColor(  CVector &AmbientColor );
	void	GetAmbientColor( CVector &AmbientColor ) ;

	int		getLightsCount(void) 
	{ return m_lightsCount; };

	void getLightByNumber(int number, Light &light) ;
	
	//returns first intersected object and distance from ray origin point
	Solid *	Intersect (  Ray &ray, double &t ) ;
};

#endif // CLIENT_ENVIRONMENT_H_INCLUDED