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

CServerSceneBuilder::CServerSceneBuilder()
: m_bErrorOccured( false )
{
  m_last_scene_uid = int(time(0)); //making uids unique
}

CServerSceneBuilder::~CServerSceneBuilder()
{}

void CServerSceneBuilder::Init(void)
{
  Empty();  
  ResetLineNumber();
  ResetErrorFlag();
  m_camera = 0;
  m_scene = 0;
}

//zero if successfull
int CServerSceneBuilder::AddSphere(const CVector& pos, double radius, const CVector& color
   , double Betta /*= 0.0*/, double nRefr /*= 1.0*/
   , double reflCoeff /*= 1.0*/, double smoothness /*= 1.0*/)
{
  return _AddSolid( new CSphere( pos, radius, color
            , Betta, nRefr, 0, 1,reflCoeff, smoothness ) );
//  Message("[PARSER]  Sphere pos = ( %.3f, %.3f, %.3f), radius = %.3f",  pos.x, pos.y, pos.z, radius );
}

int CServerSceneBuilder::AddPlane(const CVector& normal, double distance, const CVector& color
   , double Betta /*= 0.0*/, double nRefr /*= 1.0*/
   , double reflCoeff /*= 1.0*/, double smoothness /*= 1.0*/)
{
  return _AddSolid( new CPlane( normal, distance, color
                ,reflCoeff, smoothness, Betta, nRefr ) );
}

int CServerSceneBuilder::AddBox(const CVector &pos, const CVector &e0
    , const CVector &e1, const CVector &e2, const CVector &color
    , double Betta /*= 0.0*/, double nRefr /*= 1.0*/
    , double reflCoeff /*= 1.0*/, double smoothness /*= 1.0*/)
{
  return _AddSolid( new CBox( pos, e0, e1, e2, color 
    , Betta, nRefr, 0, 1,reflCoeff, smoothness ) );
}

int CServerSceneBuilder::AddTriangle(const CVector &a, const CVector &b
                                      , const CVector &c, const CVector &color
   , double Betta /*= 0.0*/, double nRefr /*= 1.0*/
   , double reflCoeff /*= 1.0*/, double smoothness /*= 1.0*/)
{
  return _AddSolid( new CTriangle(a, b, c, color
    ,reflCoeff, smoothness, Betta, nRefr ) );
}

int CServerSceneBuilder::AddCylinder(const CVector &base, const CVector &direction
                               , double length, double radius, const CVector &color
   , double Betta /*= 0.0*/, double nRefr /*= 1.0*/
   , double reflCoeff /*= 1.0*/, double smoothness /*= 1.0*/)

{
  return _AddSolid( new CCylinder( base, direction, length, radius, color
         ,reflCoeff, smoothness, Betta, nRefr ) );
}

//zero if successfull
int CServerSceneBuilder::AddLight(const CVector& pos, const CVector& color)
{
  _CreateScene();
  CLight* p_light = new CLight( color, pos );
  if ( !p_light->IsValid() ){ 
    ParserError( "Invalid light" );
    delete p_light;
    return ERROR_INVALID_PARAMS;
  }else{  
    m_scene->Add( p_light );
    return 0;
  }

//  Message("[PARSER]  Light pos = ( %.3f, %.3f, %.3f), color = ( %.3f, %.3f, %.3f)", pos.x, pos.y, pos.z, color.x, color.y, color.z );
}


//setups m_scene if it is zero
void CServerSceneBuilder::_CreateScene(void)
{
  if( !m_scene )
    m_scene = new CEnvironment();
}

//Checks p_solid->IsValid and adds valid object to the scene and returns zero
//or destroys invalid object and returns nonzero
int CServerSceneBuilder::_AddSolid(CSolid* p_solid)
{
  ASSERT( p_solid );
  _CreateScene();
  if ( !p_solid->IsValid() ){
    SetErrorFlag();
    ParserError("Invalid object parameters");

    delete p_solid;    
    return ERROR_INVALID_PARAMS;
  }else{  
    m_scene->Add( p_solid );
    return 0;
  }
}

//zero if successfull
int CServerSceneBuilder::SetAmbientColor( const CVector& color )
{
  _CreateScene();
  if( !color.IsNormalized() ){
    ParserError( "Invalid ambient color");
    SetErrorFlag();
    return ERROR_INVALID_PARAMS; 
  }else{
    m_scene->SetAmbientColor( color );
    return 0;
  }   
}

//zero if successfull
int CServerSceneBuilder::SetupCamera(const CVector &eyePoint
                                , const CVector &viewDir, const CVector &topDir)
{
  /*if (m_camera){
    ASSERT( m_scene || m_scene_list.GetSceneCount()>0 ); //first camera 
                            //must have nonempty associated scene    
    m_scene_list.Add(m_scene, m_camera);
  }else{
    ASSERT( m_scene_list.GetSceneCount() == 0 ); //first scene
  }*/
  
  m_camera = new CCamera(eyePoint, viewDir, topDir, 1, 1);  //we use 1,1 only to 
                                            //allow IsValid work successfully
  if( !m_camera->IsValid() ){
    ParserError( "Invalid camera parameters");
    SetErrorFlag();
    return ERROR_INVALID_PARAMS; 
  }
  if (m_scene)
    m_scene->SetSceneUID( GetNewSceneUID() );

  m_scene_list.Add(m_scene, m_camera);

  m_scene = 0;

  return 0;
}

void CServerSceneBuilder::AddComment(LPCSTR /*comment*/)
{
  //do nothing  
}

void CServerSceneBuilder::ParserError( LPCSTR error )
{
  SetErrorFlag();
  #ifdef _DEBUG
  ErrorMessage("[PARSER]  ERROR '%s'. Parser stoped at line %d", error, m_line_number );
  #endif//_DEBUG
}

//returns camera and associated scene
void CServerSceneBuilder::GetSceneAndCamera(int index, CEnvironment** p_scene, CCamera** p_camera)
{
  ASSERT( p_scene );
  ASSERT( p_camera );
  ASSERT( !m_bErrorOccured);
  ASSERT( index>=0 && index < GetSceneCount() );

  m_scene_list.GetScene(index, p_scene, p_camera);
}

//empties the list of scenes
void CServerSceneBuilder::Empty(void)
{
  m_scene_list.Empty();
}

