//*********************************************************
//** SrvCmd.cpp **
// Created By: KIRILL
// On: 1/26/2004 00:50:27
// Comments: Server protocol commands
//
//*********************************************************
// REVISION by ..., on ...
// Comments: ...
//
//*********************************************************

#include "stdafx.h"
#include "SrvCmd.h"
#include "common/protocol.h"
#include "common/msg.h"
#include "ServerThread.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// Supports CMD_CONNECTION_INIT command.
// Checks the required client parameters
//
// arIn -  loading archive
// arOut - storing archive
// client_name - text client name, which is used to identify the client in logs
// current_session_id - current session id, which is to be sent to the client
int CmdConnectionInit(CArchive& arIn, CArchive& arOut, 
                       LPCSTR client_name, int current_session_id)
{
  int client_protocol_version;
  CConnectionInit::Q conn_init_Q( &client_protocol_version );
  int ret = conn_init_Q.read( arIn );

  if ( ret ){
    ASSERT( 0 );
    ErrorMessage("CL[%s] Error receiving data from client. Terminating connection."
                    , client_name);
    return ERROR_MUST_TERMINATE;
  }

  if ( client_protocol_version != PROTOCOL_VERSION ){ 
    //Client protocol differs from ours. We should terminate connection

    ASSERT( 0 );
    ErrorMessage("CL[%s] Client protocol differs from ours. Terminating the connection",
          client_name );
    return ERROR_MUST_TERMINATE;
  }
  
  int server_protocol_version = PROTOCOL_VERSION;
  CConnectionInit::A conn_init_A( server_protocol_version, current_session_id );
  ret = conn_init_A.write( arOut );
  if ( ret ){
    ASSERT( 0 );
    ErrorMessage("CL[%s] Error sending data to client. Terminating connection.", client_name);
    return ERROR_MUST_TERMINATE;
  }

  arOut.Flush();

  return 0;
}


// Suports CMD_GET_FRAME_DATA command
// Sends camera position and scene_id to the client
//
// arIn -  loading archive
// arOut - storing archive
// client_name - text client name, which is used to identify the client in logs
// current_session_id - current session id, we compare it with the session id,received from the client
// p_srv_ctrl - pointer to the CServerControl object, which supports scene operations
int CmdGetFrameData(CArchive& arIn, CArchive& arOut, LPCSTR client_name,
               int current_session_id, CServerControl* p_srv_ctrl)
{
  ASSERT( p_srv_ctrl );
  
  int session_id;
  CGetFrameData::Q get_frame_Q( &session_id );
  int ret = get_frame_Q.read( arIn );

  if ( ret ){
    ASSERT( 0 );
    ErrorMessage("CL[%s] Error receiving data from client. Terminating connection."
                    , client_name);
    return ERROR_MUST_TERMINATE;
  }

  if ( session_id != current_session_id ){ 
    //Client sent unknown session id. We should terminate connection

    ASSERT( 0 );
    ErrorMessage("CL[%s]Wrong data received from client. Terminating the connection"
        , client_name);
    return ERROR_MUST_TERMINATE;
  }
  
  CImageLinesInfo image_lines_info;
  CCameraInfo  camera_info;
  int scene_uid;

  p_srv_ctrl->FillSceneParameters( &scene_uid, &image_lines_info, &camera_info );

  CGetFrameData::A get_frame_A( current_session_id, scene_uid
                              , camera_info, image_lines_info);
  ret = get_frame_A.write( arOut );

  if ( ret ){
    ASSERT( 0 );
    ErrorMessage("CL[%s] Error sending data to client. Terminating connection.", client_name);
    return ERROR_MUST_TERMINATE;
  }

  arOut.Flush();  
  return 0;
}


// Suports CMD_GET_FRAME_DATA command
// Sends scene to the client
//
// arIn -  loading archive
// arOut - storing archive
// client_name - text client name, which is used to identify the client in logs
// current_session_id - current session id, we compare it with the session id,received from the client
// p_srv_ctrl - pointer to the CServerControl object, which supports scene operations
CmdGetSceneData(CArchive& arIn, CArchive& arOut, LPCSTR client_name,
               int current_session_id, CServerControl* p_srv_ctrl)
{
  ASSERT( p_srv_ctrl );

  int session_id;
  int scene_uid;
  CGetSceneData::Q get_scene_Q( &session_id, &scene_uid );
  int ret = get_scene_Q.read( arIn );

  if ( ret ){
    ASSERT( 0 );
    ErrorMessage("CL[%s] Error receiving data from client. Terminating connection."
                    , client_name);
    return ERROR_MUST_TERMINATE;
  }

  if ( session_id != current_session_id ){ 
    //Client sent unknown session id. We should terminate connection

    ASSERT( 0 );
    ErrorMessage("CL[%s]Wrong data received from client. Terminating the connection"
        , client_name);
    return ERROR_MUST_TERMINATE;
  }

  CEnvironment* p_scene = 0;
  

  TRY{  //We should be very careful when we lock 
        //access to the scene object
    p_scene = p_srv_ctrl->GetAndLockScene();
    int current_scene_uid = p_scene->GetSceneUID();
    ASSERT( p_scene );

    BOOL bNoSuchScene = FALSE;

    if (p_scene->GetSceneUID() != scene_uid ){
      //We were asked for another scene!!!
      //Probably scene changed recently
      p_srv_ctrl->UnlockScene();

      p_scene = 0;  //we set p_scene to zero because bNoSuchScene == TRUE will 
           //will lead to sending to the client notification 
           //that it should rerequest scene_uid, lines and camera information 
           //(Client should send CMD_GET_FRAME_DATA again)
      bNoSuchScene = TRUE;
    }
    
    CGetSceneData::A get_scene_A( current_session_id, bNoSuchScene, *p_scene);
    ret = get_scene_A.write( arOut );

    if( p_scene ) {  //Haven't we unlocked the scene yet?
      p_srv_ctrl->UnlockScene();
      p_scene = 0;  
    }
  }
  CATCH_ALL( e )
  {
    if ( p_scene ) { //Haven't we unlocked the scene yet?
      p_srv_ctrl->UnlockScene();
      p_scene = 0;
    }
    THROW_LAST(); //we don't really wish to process this exception here
  }
  END_CATCH_ALL  

  if ( ret ){
    ASSERT( 0 );
    ErrorMessage("CL[%s] Error sending data to client. Terminating connection.", client_name);
    return ERROR_MUST_TERMINATE;
  }

  arOut.Flush();  
  return 0;
}
