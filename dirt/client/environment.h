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
//
//*********************************************************
// REVISION by KIRILL, on 1/9/2004 03:10:10
// Comments: I've modified header a little. My macroses are stupid 
// and don't understand your (TONIC's) idea of header formating :)
//
//*********************************************************
// REVISION by ..., on ...
// Comments: ...
//
//*********************************************************


#include "common/vector.h"

//define maximal number of elements in the environment
//using static memory allocation
#define	MAX_LIGHTS	10
#define	MAX_SOLIDS	100

class	Ray
{
private:
	//this id esentially a point
	CVector	origin;
	// direction must be normalized
	CVector	direction;

public:

	Ray ( CVector *origin, CVector *direction ); 
};


//interface for a geometric object
class	Solid
{
}; 

class	Light			// model of an abstract light source
{
	//RGB brightness, each component in [0;1]
	CVector color;

public:
	
	Light(); //default constructor initialized to white
	Light(double r, double g, double b);
	Light(CVector *color);

	virtual	~Light() {};	// force virtual destructor in all descendants
};

class	Environment
{
private:
	Light	*lights[MAX_LIGHTS];
	Solid	*solids[MAX_SOLIDS];
	int		lightsCount;
	int		solidsCount;
	
	
public:
	
	Environment (); //initialize counters to zeros
	~Environment (){};	//do nothing as contained objects may be reused
	
	void	Add ( Light *light );
	void	Add ( Solid *solid );
	
	//returns first intersected object and distance from ray origin point
	Solid		*Intersect ( Ray *ray, double *t );
};
