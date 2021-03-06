//*********************************************************
//** protocol.cpp **
// Created By: KIRILL
// On :1/8/2004 23:37:26
// Comments: protocol structures implementation
//  These are only structures which are able to write its data
//  to archive and to read data from archive. All the details
//  of their using are in protocol realisation of Server and Client
//
//*********************************************************
// REVISION by KIRILL, on 1/29/2004 23:32:43
// Comments: unneeded const_casts removed
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

CConnectionInit::Q::Q(int& client_protocol_version)
: m_client_protocol_version( (client_protocol_version) )
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

CConnectionInit::A::A(int& server_protocol_version, int& session_id)
: m_server_protocol_version( server_protocol_version )
, m_session_id( session_id )
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
 
CGetFrameData::Q::Q(int& session_id)
: m_session_id( session_id )
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

CGetFrameData::A::A(  int& session_id, int& scene_uid
                   , CCameraInfo& camera_info, CImageLinesInfo& image_info )
 : m_session_id ( session_id  ) 
 , m_scene_uid  ( scene_uid   ) 
 , m_camera_info( camera_info ) 
 , m_image_info ( image_info  ) 
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

CGetSceneData::Q::Q(int& session_id, int& scene_uid)
: m_session_id ( session_id ) 
, m_scene_uid  ( scene_uid  ) 
{}

CGetSceneData::Q::Q(int *p_session_id, int *p_scene_uid )
: m_session_id( *p_session_id )
, m_scene_uid ( *p_scene_uid  )
{}

int CGetSceneData::Q::write(CArchive& ar)
{ 
  ASSERT( ar.IsStoring() );
  ar << m_session_id;
  ar << m_scene_uid;
  return 0; 
}

int CGetSceneData::Q::read(CArchive& ar)
{ 
  ASSERT( ar.IsLoading() );
  ar >> m_session_id;
  ar >> m_scene_uid;
  return 0;
}

///////////////////////////////////////////////////////////
// CGetSceneData::A
///////////////////////////////////////////////////////////

CGetSceneData::A::A( int& session_id, BOOL& scene_changed
                      , CEnvironment& scene )
 : m_session_id    ( session_id    )
 , m_scene_changed ( scene_changed )
 , m_scene         ( scene         ) 
{}


CGetSceneData::A::A( int *p_session_id, BOOL* p_scene_changed
                      , CEnvironment* p_scene )
 : m_session_id    ( *p_session_id    ) 
 , m_scene_changed ( *p_scene_changed ) 
 , m_scene         ( *p_scene         ) 
{}


//m_scene_changed == TRUE means that scene with requested UID can't be sent
//so the m_scene shouldn't be not written 
int CGetSceneData::A::write(CArchive& ar)
{
  ASSERT( ar.IsStoring() );
  int ret = 0;

  ar << m_session_id;
  ar << m_scene_changed;
  if (!m_scene_changed){
    ASSERT( m_scene.IsValid() );
    ret = m_scene.write( ar );
    ASSERT( !ret );
  }
  
  return ret;
}

//m_scene_changed == TRUE means that scene with requested UID can't be sent
//so the m_scene shouldn't be not read
int CGetSceneData::A::read(CArchive& ar)
{
  ASSERT( ar.IsLoading() );
  
  ar >> m_session_id;
  ar >> m_scene_changed;

  if ( !m_scene_changed ){
    int ret = m_scene.read( ar );

    if ( ret || !m_scene.IsValid() ){
      ASSERT( 0 );
      return LOADING_ERROR_RETURN;
    }
  }
  
  return 0;
}



///////////////////////////////////////////////////////////
//  SendLineData::Q
///////////////////////////////////////////////////////////
    
CSendLineData::Q::Q(int& session_id, int& scene_uid, int& line_num
                    , int& pixels_count, void*& data)
: m_session_id ( session_id )
, m_scene_uid( scene_uid )
, m_line_num( line_num )
, m_pixels_count( pixels_count )
, m_data( data )
{}

CSendLineData::Q::Q(int *p_session_id, int* p_scene_uid, int* p_line_num
                    , int *p_pixels_count, void **p_data)
: m_session_id( *p_session_id )
, m_scene_uid( *p_scene_uid )
, m_line_num( *p_line_num )
, m_pixels_count( *p_pixels_count )
, m_data( *p_data )

{}

int CSendLineData::Q::write(CArchive& ar)
{ 
  ASSERT( ar.IsStoring() );
  ar << m_session_id;
  ar << m_scene_uid;
  ar << m_line_num;

  ASSERT( m_pixels_count > 0 && m_pixels_count <= MAX_PIXELS_COUNT );
  ar << m_pixels_count;
  ar.Write(m_data, sizeof(COLORREF)*m_pixels_count);

  return 0; 
}

int CSendLineData::Q::read(CArchive& ar)
{ 
  ASSERT( ar.IsLoading() );
  ar >> m_session_id;  
  ar >> m_scene_uid;
  ar >> m_line_num;
  ar >> m_pixels_count;

  if ( m_pixels_count < 1 || m_pixels_count > MAX_PIXELS_COUNT ){
    ASSERT( 0 ); //something is wrong
    return LOADING_ERROR_RETURN;
  }
  
  m_data = new (COLORREF[m_pixels_count]);
  ar.Read(m_data, sizeof(COLORREF)*m_pixels_count);  

  return 0;
}


///////////////////////////////////////////////////////////
//  SendLineData::A
///////////////////////////////////////////////////////////

CSendLineData::A::A(int& session_id)
: m_session_id ( session_id )
{}

CSendLineData::A::A(int *p_session_id )
: m_session_id( *p_session_id )
{}

int CSendLineData::A::write(CArchive& ar)
{ 
  ASSERT( ar.IsStoring() );
  ar << m_session_id;
  return 0; 
}

int CSendLineData::A::read(CArchive& ar)
{ 
  ASSERT( ar.IsLoading() );
  ar >> m_session_id;  
  return 0;
}
