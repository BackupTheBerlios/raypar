//*********************************************************
//** protocol.cpp **
// Created By: KIRILL
// On :1/8/2004 23:37:26
// Comments: protocol structures implementation
// NOTE: const_cast is used only for the purpose of comfort
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


/////////////////////////////////////////////////
// CameraInfo

CArchive& CameraInfo::operator << (CArchive& ar)
{
  ASSERT( ar.IsStoring() );
  ar << m_camera_pos;
  ar << m_camera_z_axis;
  ar << m_camera_y_axis;
  return ar;
}
CArchive& CameraInfo::operator >> (CArchive& ar)
{
  ASSERT( ar.IsLoading() );
  ar >> m_camera_pos;
  ar >> m_camera_z_axis;
  ar >> m_camera_y_axis;
  return ar;
}



/////////////////////////////////////////////////
// ConnectionInit
/////////////////////////////////////////////////

ConnectionInit::Q::Q(const int& client_protocol_version)
: m_client_protocol_version( const_cast<int&>(client_protocol_version) )
{}

ConnectionInit::Q::Q(int* p_client_protocol_version)
: m_client_protocol_version( *p_client_protocol_version )
{}


int ConnectionInit::Q::read(CArchive& ar)
{
  int ret = 0; //return code
  ASSERT( ar.IsLoading() );
  ar >> m_client_protocol_version;
  return 0;
}

int ConnectionInit::Q::write(CArchive& ar)
{
  int ret = 0; //return code
  //ASSERT( ar.IsStoring() );
  //TRY
  //{  
    ar << m_client_protocol_version;
  //}
  //CATCH_ALL(pEx)
  //{  
//    ErrorMessageFromException(pEx);
//    ret = 1;
//  }
//  END_CATCH_ALL

  return 0;
}

///////////////////////////////////////////////////////////

ConnectionInit::A::A(const int& server_protocol_version, const int& session_id)
: m_server_protocol_version( const_cast<int&>(server_protocol_version) )
, m_session_id( const_cast<int&>(session_id) )
{}

ConnectionInit::A::A(int* p_server_protocol_version, int* p_session_id)
: m_server_protocol_version( *p_server_protocol_version )
, m_session_id( *p_session_id )
{}

int ConnectionInit::A::read(CArchive& ar)
{
  int ret = 0; //return code
  ASSERT( ar.IsLoading() );
  ar >> m_server_protocol_version;
  ar >> m_session_id;
  return 0;
}

int ConnectionInit::A::write(CArchive& ar)
{
  int ret = 0; //return code
  ASSERT( ar.IsStoring() );
  ar << m_server_protocol_version;
  ar << m_session_id;
  return ret;
}

///////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////
