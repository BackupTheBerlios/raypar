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
// Comments:	Adding functionality to Environment( Intersect method )
//***********************************
// REVISION by Tonic, on 12/01/2004
// Comments:	Getters and setters for Ray, Light
//				components, getter for Enviroment light sources
//				Added CVector position to Light, modified constructors
//				Added Ray constructor without parameters
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

#include "stdafx.h"
#include "environment.h"


void Ray::getPoint( double distance, CVector &point)
	{
	point = m_origin + distance*m_direction;
	};

CCamera::CCamera( CVector &eyePoint, CVector &viewDir, CVector &topDir, int width, int height )
	{
	//Check that width and height are both positive
	ASSERT( width > 0 && height >0);

	//check that view direction and top direction are not parallel
	double lengthProduct = viewDir.Length() * topDir.Length();
	double scalarProduct = fabs( viewDir * topDir );
	ASSERT( scalarProduct < lengthProduct - VECTOR_EQUAL_EPS );

	m_width = width;
	m_height = height;
	m_eyePoint = eyePoint;
	m_viewDir = viewDir;
	m_topDir = topDir;
	m_minViewAngle = 0.1;
	m_maxViewAngle = 3.0;
	m_verticalAngle = m_horizontalAngle = 1.57;

	m_viewDir.Normalize();
	m_topDir.Normalize();

	//now make top direction orthogonal to the viewing direction
	m_topDir = m_topDir - m_viewDir * (( m_viewDir * m_topDir ) / m_topDir.Length());
	m_topDir.Normalize();

	UpdateHorizontalDir();
	};

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

void CCamera::GetVerticalAngle(double &verticalAngle)
	{
	verticalAngle = m_verticalAngle;
	};

void CCamera::GetEyePoint( CVector &eyePoint )
	{
	eyePoint = m_eyePoint;
	};

void CCamera::GetViewDir( CVector &viewDir )
	{
	viewDir = m_viewDir;
	};

void CCamera::GetTopDir( CVector &topDir )
	{
	topDir = m_topDir;
	};

void CCamera::GetWidth(int &width)
	{
	width = m_width;
	};

void CCamera::GetHeight(int &height)
	{
	height = m_height;
	};

void CCamera::GetHorizontalAngle(double &horizontalAngle)
	{
	horizontalAngle = m_horizontalAngle;
	};

void CCamera::PixelRay(int x, int y, Ray &ray)
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
	//compute the dot product [viewDir x topDir]
	
	m_horDir.x = m_viewDir.y*m_topDir.z - m_viewDir.z*m_topDir.y;
	m_horDir.y = -(m_viewDir.x*m_topDir.z - m_viewDir.z*m_topDir.x);
	m_horDir.z = m_viewDir.x*m_topDir.y - m_viewDir.y*m_topDir.x;
	m_horDir.Normalize();
	};

void CSolid::GetColor( Ray &falling, CVector &color)
	{
	//do not check whether there is even a ray
	//just return white
	//more strict implementations in descendants
	color.x = color.y = color.z = 1;
	};

void	Environment::SetAmbientColor(  CVector &AmbientColor )
	{
	ASSERT( 0.0 <= AmbientColor.x && AmbientColor.x <= 1.0);
	ASSERT( 0.0 <= AmbientColor.y && AmbientColor.y <= 1.0);
	ASSERT( 0.0 <= AmbientColor.z && AmbientColor.z <= 1.0);

	m_AmbientColor = AmbientColor;
	};

void	Environment::GetAmbientColor( CVector &AmbientColor )
	{
	ASSERT( &AmbientColor != NULL);

	AmbientColor = m_AmbientColor;
	}

CLight::CLight( double r, double g, double b, double x, double y, double z)
	{
	ASSERT( 0.0 <= r && r <= 1.0);
	ASSERT( 0.0 <= g && g <= 1.0);
	ASSERT( 0.0 <= b && b <= 1.0);

	m_color.x = r;
	m_color.y = g;
	m_color.z = b;
	m_position.x = x;
	m_position.y = y;
	m_position.z = z;

	};

void CLight::getPosition(CVector &position)
	{
	position = m_position;
	};

void CLight::setPosition( CVector &position)
	{
	m_position = position;
	};

void CLight::getColor(CVector &color)
	{
	color = m_color;
	};

void CLight::setColor( CVector &color)
	{
	ASSERT( (color.x >= 0) && (color.x <= 1) );
	ASSERT( (color.y >= 0) && (color.y <= 1) );
	ASSERT( (color.z >= 0) && (color.z <= 1) );

	m_color = color;
	}

void Environment::getLightByNumber(int number, CLight &light)
	{
	//there is a light source with such a number
	ASSERT( (number >= 0) && (number < m_lights.GetSize()) );

	light = *(m_lights.GetAt(number));
	}


void Ray::getDirection(CVector &direction)
	{
	// = is overridden operator, just makes the components the same
	direction = m_direction;
	}

void Ray::setDirection( CVector &direction)
	{
	ASSERT( direction.Length() != 0);

	// = is overridden operator, just makes the components the same
	m_direction = direction;

	//make the direction length 1
	(m_direction).Normalize();
	}


void Ray::getOrigin(CVector &origin)
	{
	// = is overridden operator, just makes the components the same
	origin = m_origin;
	}


void Ray::setOrigin( CVector &origin)
	{
	// = is overridden operator, just makes the components the same
	m_origin = origin;
	}

Ray::Ray(  CVector &origin,  CVector &direction )
	{
	ASSERT( direction.Length() != 0);

	m_origin = origin;
	m_direction = direction;
	(m_direction).Normalize();
	};

//determines the closest intersected object and distance
CSolid * Environment::Intersect (  Ray &ray, double &t )
	{
	CSolid*	closestObj = NULL;
	//do not allow intersections with objects
	//farther than t away
	double	closestDist = t;
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


Environment::Environment( )
{
	m_solids.SetSize(10,5);
    m_lights.SetSize(10,5);
	m_AmbientColor.x = m_AmbientColor.y = m_AmbientColor.z = 0.2;
};

void Environment::Add ( CLight *light )
	{
	ASSERT( light != NULL );

	int i;
    int lightsCount = m_lights.GetSize();
    

	//check if this element is already present
	//if it holds, do not add repeatedly, just return
	for( i = 0; i < lightsCount; i++ )
		{
		if( m_lights.GetAt(i) == light )
			return;
		}

	//actually add light source to environment
	m_lights.Add(light);
	};

void Environment::Add ( CSolid *solid )
	{
	ASSERT( solid != NULL );

	int i;
    int solidsCount = m_solids.GetSize();
	
	//check if this element is already present
	//if it holds, do not add repeatedly, just return
	for( i = 0; i < solidsCount; i++ )
		{
		if( m_solids.GetAt(i) == solid )
			return;
		}

	//actually add element to environment
	m_solids.Add(solid);
	};

CLight::CLight()
	{
	m_color.x = m_color.y = m_color.z = 1.0;
	};

CLight::CLight(  CVector &color,  CVector &position )
	{
	ASSERT( (color.x >= 0) && (color.x <= 1) );
	ASSERT( (color.y >= 0) && (color.y <= 1) );
	ASSERT( (color.z >= 0) && (color.z <= 1) );

	m_color = color;
	m_position = position;
	};