// ServerSceneBuilder.cpp: implementation of the CServerSceneBuilder class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "server.h"
#include "ServerSceneBuilder.h"
#include "common/vector.h"
#include "client/geometry.h"
#include "client/environment.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define ERROR_INVALID_PARAMS 1

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CServerSceneBuilder::CServerSceneBuilder(CEnvironment& scene)
: m_scene( scene )
, m_bErrorOccured( false )
{}

CServerSceneBuilder::~CServerSceneBuilder()
{}

void CServerSceneBuilder::Init(void)
{
  m_scene.Empty();
  m_scene.SetSceneUID( 0 );
  ResetLineNumber();
  ResetErrorFlag();
}

//zero if successfull
int CServerSceneBuilder::AddSphere(const CVector& pos, double radius, const CVector& color)
{
  return _AddSolid( new CSphere( pos, radius, color ) );
//  Message("[PARSER]  Sphere pos = ( %.3f, %.3f, %.3f), radius = %.3f",  pos.x, pos.y, pos.z, radius );
}

//zero if successfull
int CServerSceneBuilder::AddLight(const CVector& pos, const CVector& color)
{
  CLight* p_light = new CLight( pos, color );
  if ( !p_light->IsValid() ){ 
    ParserError( "Invalid light!" );
    delete p_light;
    return ERROR_INVALID_PARAMS;
  }else{  
    m_scene.Add( p_light );
    return 0;
  }

//  Message("[PARSER]  Light pos = ( %.3f, %.3f, %.3f), color = ( %.3f, %.3f, %.3f)", pos.x, pos.y, pos.z, color.x, color.y, color.z );
}


//Checks p_solid->IsValid and adds valid object to the scene and returns zero
//or destroys invalid object and returns nonzero
int CServerSceneBuilder::_AddSolid(CSolid* p_solid)
{
  ASSERT( p_solid );
  if ( !p_solid->IsValid() ){
    ParserError( "Invalid object!" );
    delete p_solid;
    return ERROR_INVALID_PARAMS;
  }else{  
    m_scene.Add( p_solid );
    return 0;
  }
}

//zero if successfull
int CServerSceneBuilder::SetAmbientColor( const CVector& color )
{
  if( !color.IsNormalized() ){
    ParserError( "Invalid color!");
    return ERROR_INVALID_PARAMS; 
  }else{
    m_scene.SetAmbientColor( color );
    return 0;
  }   
}


void CServerSceneBuilder::AddComment(LPCSTR comment)
{
  //temp
  //Message("[PARSER]  Comment =  '%s'", comment );
}

void CServerSceneBuilder::ParserError( LPCSTR error )
{
  SetErrorFlag();
  ErrorMessage("[PARSER]  ERROR =  '%s'", error );
}
