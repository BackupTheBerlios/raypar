//*********************************************************
//** protocol.cpp **
// Created By: KIRILL
// On :1/8/2004 23:37:26
// Comments: protocol structures implementation
// NOTE: const_cast is used only for the purpose of useabilty
//
//*********************************************************
// REVISION by ..., on ...
// Comments: ...
//
//*********************************************************

#include "stdafx.h"
#include "protocol.h"
#include "msg.h"
#include "vector.h"
#include "client/environment.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////
// CCameraInfo operators
///////////////////////////////////////////////////////////

//Storing routine. Zero if successful
int CCameraInfo::write(CArchive& ar) const
{
  ASSERT( ar.IsStoring() );
  ar << m_camera_pos;
  ar << m_camera_z_axis;
  ar << m_camera_y_axis;
  return 0;
}

//Loading routine. Zero if successful
int CCameraInfo::read(CArchive& ar)
{
  ASSERT( ar.IsLoading() );
  ar >> m_camera_pos;
  ar >> m_camera_z_axis;
  ar >> m_camera_y_axis;
  return 0;
}


/////////////////////////////////////////////////
// CCameraInfo
///////////////////////////////////////////////////////////

//Storing routine. Zero if successful
int CImageLinesInfo::write(CArchive& ar) const

{  
  ASSERT( ar.IsStoring());

  ar << m_line_number;
  ar << m_image_width;
  ar << m_image_height;

  return 0;
}

//Loading routine. Zero if successful
int CImageLinesInfo::read(CArchive& ar)
{
  ASSERT( ar.IsLoading());

  ar >> m_line_number;
  ar >> m_image_width;
  ar >> m_image_height;

  return 0;
}



/////////////////////////////////////////////////
// CConnectionInit::Q
/////////////////////////////////////////////////

CConnectionInit::Q::Q(const int& client_protocol_version)
: m_client_protocol_version( const_cast<int&>(client_protocol_version) )
{}

CConnectionInit::Q::Q(int* p_client_protocol_version)
: m_client_protocol_version( *p_client_protocol_version )
{}

int CConnectionInit::Q::read(CArchive& ar)
{
  int ret = 0; //return code
  ASSERT( ar.IsLoading() );
  ar >> m_client_protocol_version;
  return 0;
}

int CConnectionInit::Q::write(CArchive& ar)
{
  int ret = 0; //return code
  //ASSERT( ar.IsStoring() );
  //TRY
  //{  
    ar << m_client_protocol_version;
  //}
  //CATCH_ALL(pEx)
  //{  
  //  ErrorMessageFromException(pEx);
  //  ret = 1;
  //}
  //END_CATCH_ALL

  return 0;
}

///////////////////////////////////////////////////////////
// CConnectionInit::A
///////////////////////////////////////////////////////////

CConnectionInit::A::A(const int& server_protocol_version, const int& session_id)
: m_server_protocol_version( const_cast<int&>(server_protocol_version) )
, m_session_id( const_cast<int&>(session_id) )
{}

CConnectionInit::A::A(int* p_server_protocol_version, int* p_session_id)
: m_server_protocol_version( *p_server_protocol_version )
, m_session_id( *p_session_id )
{}

int CConnectionInit::A::read(CArchive& ar)
{
  int ret = 0; //return code
  ASSERT( ar.IsLoading() );
  ar >> m_server_protocol_version;
  ar >> m_session_id;
  return 0;
}

int CConnectionInit::A::write(CArchive& ar)
{
  int ret = 0; //return code
  ASSERT( ar.IsStoring() );
  ar << m_server_protocol_version;
  ar << m_session_id;
  return ret;
}


///////////////////////////////////////////////////////////
// CGetFrameData::Q
///////////////////////////////////////////////////////////
 
CGetFrameData::Q::Q(const int& session_id)
: m_session_id( const_cast<int&>(session_id) )
{}

CGetFrameData::Q::Q(int *p_session_id)
: m_session_id( *p_session_id )
{}

int CGetFrameData::Q::write(CArchive& ar)
{ 
  ASSERT( ar.IsStoring() );
  ar << m_session_id;
  return 0; 
}

int CGetFrameData::Q::read(CArchive& ar)
{ 
  ASSERT( ar.IsLoading() );
  ar >> m_session_id;
  return 0;
}

///////////////////////////////////////////////////////////
// CGetFrameData::A
///////////////////////////////////////////////////////////

CGetFrameData::A::A(  const int& session_id, const int& scene_uid
                   , const CCameraInfo& camera_info, const CImageLinesInfo& image_info )
 : m_session_id ( const_cast<int&>            ( session_id  ) )
 , m_scene_uid  ( const_cast<int&>            ( scene_uid   ) )
 , m_camera_info( const_cast<CCameraInfo&>     ( camera_info ) )
 , m_image_info ( const_cast<CImageLinesInfo&> ( image_info  ) )
{}


CGetFrameData::A::A( int *p_session_id, int* p_scene_uid
                  , CCameraInfo* p_camera_info, CImageLinesInfo* p_image_info )
 : m_session_id ( *p_session_id  ) 
 , m_scene_uid  ( *p_scene_uid   ) 
 , m_camera_info( *p_camera_info )
 , m_image_info ( *p_image_info  )
{}

int CGetFrameData::A::write(CArchive& ar)
{
  ASSERT( ar.IsStoring() );
  
  ar << m_session_id;
  ar << m_scene_uid;
  m_camera_info.write( ar );
  m_image_info.write( ar );
  
  return 0;
}

int CGetFrameData::A::read(CArchive& ar)
{
  ASSERT( ar.IsLoading() );
  
  ar >> m_session_id;
  ar >> m_scene_uid;
  m_camera_info.read( ar );
  m_image_info.read( ar );

  return 0;
}

///////////////////////////////////////////////////////////
// CGetSceneData::Q
///////////////////////////////////////////////////////////

CGetSceneData::Q::Q(const int& session_id)
: m_session_id( const_cast<int&>(session_id) )
{}

CGetSceneData::Q::Q(int *p_session_id)
: m_session_id( *p_session_id )
{}

int CGetSceneData::Q::write(CArchive& ar)
{ 
  ASSERT( ar.IsStoring() );
  ar << m_session_id;
  return 0; 
}

int CGetSceneData::Q::read(CArchive& ar)
{ 
  ASSERT( ar.IsLoading() );
  ar >> m_session_id;
  return 0;
}

///////////////////////////////////////////////////////////
// CGetSceneData::A
///////////////////////////////////////////////////////////

CGetSceneData::A::A(  const int& session_id, const int& scene_uid
                      , const Environment& scene )
 : m_session_id ( const_cast<int&>         ( session_id ) )
 , m_scene_uid  ( const_cast<int&>         ( scene_uid  ) )
 , m_scene      ( const_cast<Environment&> ( scene      ) ) 
{}


CGetSceneData::A::A( int *p_session_id, int* p_scene_uid
                      , Environment* p_scene )
 : m_session_id ( *p_session_id ) 
 , m_scene_uid  ( *p_scene_uid  ) 
 , m_scene      ( *p_scene      ) 
{}

int CGetSceneData::A::write(CArchive& ar)
{
  ASSERT( ar.IsStoring() );
 
  ar << m_session_id;
  ar << m_scene_uid;

  ASSERT( m_scene.IsValid() );
  int ret;
  ret = m_scene.write( ar );
  ASSERT( !ret );
  
  return ret;
}

int CGetSceneData::A::read(CArchive& ar)
{
  ASSERT( ar.IsLoading() );
  
  ar >> m_session_id;
  ar >> m_scene_uid;
  if ( !m_scene.read( ar ) || !m_scene.IsValid() ){
    ASSERT( 0 );
    return LOADING_ERROR_RETURN;
  }
  
  return 0;
}

