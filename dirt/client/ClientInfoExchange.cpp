//*********************************************************
//** ClientInfoExchange.cpp **
// Created By: KIRILL
// On: 1/28/2004 17:11:24
// Comments: realization of ClientInfoExchange and it's return values.
//    ClientInfoExchange() provides all the inforamtuion exchange with server
//   for client.
//
//*********************************************************
// REVISION by ..., on ...
// Comments: ...
//
//*********************************************************

#include "stdafx.h"
#include "ClientInfoExchange.h"
#include "COMMON/protocol.h"
#include "common/msg.h"
#include "environment.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

///////////////////////////////////////////////////////////
// helper functions. return the same values as ClientInfoExchange (CIE_XXX).

//Initializes the connection with server and received session id.
int DoConnectionInit(CArchive& arIn, CArchive& arOut, int* p_session_id);

//Receives camera position, image line inforamtion
//and current scene id from the server.
int DoGetFrameData(CArchive& arIn, CArchive& arOut, int session_id
      , int* p_scene_uid, CImageLinesInfo* p_image_lines_info
      , CCameraInfo* p_camera_info );

//Receives scene from the server.
//*p_scene_changed == TRUE means that the scene with scene_uid is unavailable 
//on server (probably it was already finished and modified), so we should
//rerequest scene_id, lines and camera inforamtion (CMD_GET_FRAME_DATA)
int DoGetSceneData(CArchive& arIn, CArchive& arOut, int session_id
      , int scene_uid, BOOL* p_scene_changed, CEnvironment* p_scene );

//Sends line data to the server
int DoSendLine(CArchive& arIn, CArchive& arOut, int session_id
      , int scene_uid, int line_num, int line_width, void* p_data);

///////////////////////////////////////////////////////////
// ClientInfoExchange - does all the information exchange with server
//
// rendereded_line_number - number of rendered line, negative means no line were rendered
//                            so we don't send line_data to server
// COLORREF* line_data - redered line data
// socket  - socket, connected with server
// scene  - the scene, which could be loaded from server if need
// camera  - the camera, which position is received from the server
// line_number  - number of the line to render
int ClientInfoExchange(int rendereded_line_number, COLORREF* line_data
                       , CSocket& socket, CEnvironment& scene
                        , CCamera& camera, int& line_number )
{
  CSocketFile sock_file( &socket );
  CArchive arIn ( &sock_file, CArchive::load );
  CArchive arOut( &sock_file, CArchive::store );

  int session_id;

  int ret;
  ret = DoConnectionInit(arIn, arOut, &session_id);
  if (ret)  
    return ret; //error occured, terminating connection.

  if ( rendereded_line_number >= 0 ){ //we've rendered a line
    ASSERT( scene.GetSceneUID() >0 );
    ASSERT( line_data );

    int height, width;
    camera.GetHeight( height );
    camera.GetWidth ( width );
    ASSERT( rendereded_line_number < height );

    ret = DoSendLine(arIn, arOut, session_id, scene.GetSceneUID()
                      , rendereded_line_number, width, line_data);
    if (ret)  
       return ret; //error occured, terminating connection.
  }

  CCameraInfo camera_info;
  CImageLinesInfo image_lines_info;
  int scene_uid;
 
  ret = DoGetFrameData(arIn, arOut, session_id, &scene_uid,
     &image_lines_info, &camera_info);
  
  if (ret)  
    return ret; //error occured, terminating connection.

  camera.Init( camera_info.m_camera_pos   //?K? Needs correction?
              , camera_info.m_camera_z_axis 
              , camera_info.m_camera_y_axis
              , image_lines_info.m_image_width
              , image_lines_info.m_image_height );
  

  if ( image_lines_info.m_line_number >= image_lines_info.m_image_height )
  {
    ASSERT(0);
    ErrorMessage("Wrong data received from server. Terminating the connection.");
    return CIE_COMMUNICATION_ERROR;
  }

  //line number amy be negative. this means that the server don't want to 
  line_number = image_lines_info.m_line_number;
  
  if( line_number <0 )
    return CIE_SERVER_WAIT; //we should wait for a while

  if ( scene_uid != scene.GetSceneUID() ){
    //we should download scene from the server
    BOOL b_scene_changed = FALSE;
    ret = DoGetSceneData(arIn, arOut, session_id, scene_uid, &b_scene_changed, &scene);
    if (ret)  
      return ret; //error occured, terminating connection.

    ASSERT( !b_scene_changed );
    if ( b_scene_changed )
      return CIE_COMMUNICATION_ERROR;    
  }

  return CIE_NORMAL_RENDER_RETURN; //0
}



//Initializes the connection with server and received session id.
static int DoConnectionInit(CArchive& arIn, CArchive& arOut, int* p_session_id)
{
  ASSERT( p_session_id );
  
  arOut << (int)CMD_CONNECTION_INIT;
  
  int client_protocol_version = PROTOCOL_VERSION;
  CConnectionInit::Q conn_init_Q( client_protocol_version );
  int ret = conn_init_Q.write( arOut );
  arOut.Flush();

  if ( ret ){ 
    //Communication error. We should terminate connection
    ASSERT( 0 );
    ErrorMessage("Communication error. Terminating the connection.");
    return CIE_COMMUNICATION_ERROR;
  }
   
  int server_protocol_version;
  CConnectionInit::A conn_init_A( &server_protocol_version, p_session_id );
  ret = conn_init_A.read( arIn );

  if ( ret ){ 
    //Communication error. We should terminate connection
    ASSERT( 0 );
    ErrorMessage("Error receiving data from server. Terminating the connection.");
    return CIE_COMMUNICATION_ERROR;
  }

  if ( PROTOCOL_VERSION != server_protocol_version ){ 
    //Server protocol differs from ours. We should terminate connection
    ASSERT( 0 );
    ErrorMessage("Server protocol differs from ours. Terminating the connection.");
    return CIE_WRONG_PROTOCOL_ERROR;
  }
  
  return 0;
}



//Receives camera position and current scene id from the server.
int DoGetFrameData(CArchive& arIn, CArchive& arOut, int session_id
      , int* p_scene_uid, CImageLinesInfo* p_image_lines_info
      , CCameraInfo* p_camera_info )
{
  ASSERT( p_scene_uid );
  ASSERT( p_image_lines_info );
  ASSERT( p_camera_info );

  arOut << (int) CMD_GET_FRAME_DATA;

  CGetFrameData::Q get_frame_Q( session_id );
  int ret = get_frame_Q.write( arOut );
  arOut.Flush();

  if ( ret ) {
    ASSERT( 0 );
    ErrorMessage("Error sending data to server. Terminating connection.");
    return CIE_COMMUNICATION_ERROR;
  }
  
  int server_session_id;
   
  CGetFrameData::A get_frame_A( &server_session_id, p_scene_uid
                              , p_camera_info, p_image_lines_info);
  ret = get_frame_A.read( arIn );

  if ( ret ){
    ASSERT( 0 );
    ErrorMessage("Error receiving data from server. Terminating connection.");
    return CIE_COMMUNICATION_ERROR;
  }

  if ( server_session_id != session_id ){ 
    //Server sent unknown session id. Very strange. We should terminate connection
    ASSERT( 0 );
    ErrorMessage("Wrong session id received from server. Terminating the connection.");
    return CIE_COMMUNICATION_ERROR;
  }

  return 0;
}


//Receives scene from the server.
//*p_scene_changed == TRUE means that the scene with scene_uid is unavailable 
//on server (probably it was already finished and modified), so we should
//rerequest scene_id, lines and camera inforamtion (CMD_GET_FRAME_DATA)

int DoGetSceneData(CArchive& arIn, CArchive& arOut, int session_id
      , int scene_uid, BOOL* p_scene_changed, CEnvironment* p_scene )
{

  ASSERT( p_scene_changed );
  ASSERT( p_scene );  

  arOut << (int) CMD_GET_SCENE_DATA;

  CGetSceneData::Q get_frame_Q( session_id, scene_uid );
  int ret = get_frame_Q.write( arOut );
  arOut.Flush();

  if ( ret ) {
    ASSERT( 0 );
    ErrorMessage("Error sending data to server. Terminating connection.");
    return CIE_COMMUNICATION_ERROR;
  }
  
  int server_session_id;
   
  //if *p_scene_changed == TRUE than p_scene isn't read.
  CGetSceneData::A get_scene_A( &server_session_id, p_scene_changed, p_scene);
  ret = get_scene_A.read( arIn );
  
  ASSERT( !*p_scene_changed ); //TEMP!!! ?K?

  p_scene->SetSceneUID(scene_uid);

  if ( ret ){
    ASSERT( 0 );
    ErrorMessage("Error receiving data from server. Terminating connection.");
    return CIE_COMMUNICATION_ERROR;
  }

  if ( server_session_id != session_id ){ 
    //Server sent unknown session id. Very strange. We should terminate connection
    ASSERT( 0 );
    ErrorMessage("Wrong session id received from server. Terminating the connection.");
    return CIE_COMMUNICATION_ERROR;
  }
  
  return 0;
}


//Sends line data to the server
int DoSendLine(CArchive& arIn, CArchive& arOut, int session_id
      , int scene_uid, int line_num, int line_width, void* p_data)
{
  ASSERT( p_data );
  
  arOut << (int) CMD_SEND_LINE_DATA;
 
  CSendLineData::Q send_line_Q( session_id, scene_uid, line_num, 
                                  line_width, p_data);
  int ret = send_line_Q.write( arOut );
  arOut.Flush();

  if ( ret ){
    ASSERT( 0 );
    ErrorMessage("Error sending data to server. Terminating connection.");
    return CIE_COMMUNICATION_ERROR;
  }

  int server_session_id;
  CSendLineData::A send_line_A( &server_session_id );
  ret = send_line_A.read( arIn );

  if ( ret ){ 
    //Communication error. We should terminate connection
    ASSERT( 0 );
    ErrorMessage("Error receiving data from server. Terminating the connection.");
    return CIE_COMMUNICATION_ERROR;
  }

  if ( server_session_id != session_id ){ 
    //Server sent unknown session id. Very strange. We should terminate connection
    ASSERT( 0 );
    ErrorMessage("Wrong session id received from server. Terminating the connection.");
    return CIE_COMMUNICATION_ERROR;
  }
  
  return 0;
}
