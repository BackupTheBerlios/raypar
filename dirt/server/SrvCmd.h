//*********************************************************
//** SrvCmd.h **
// Created By: KIRILL
// On: 1/26/2004 00:50:55
// Comments: Server protocol commands declaration
//
//*********************************************************
// REVISION by ..., on ...
// Comments: ...
//
//*********************************************************

#ifndef _SRV_CMD_H
#define _SRV_CMD_H

//this return means that an error occured and the
//connection must be terminated
#define ERROR_MUST_TERMINATE 1

class CServerControl;

// Supports CMD_CONNECTION_INIT command.
// Checks the required server parameters
//
// arIn -  loading archive
// arOut - storing archive
// client_name - text client name, which is used to identify the client in logs
// current_session_id - current session id, which is to be sent to the client
int CmdConnectionInit(CArchive& arIn, CArchive& arOut,
                  LPCSTR client_name, int current_session_id);


// Suports CMD_GET_FRAME_DATA command
// Gives camera position and scene_id to the server
//
// arIn -  loading archive
// arOut - storing archive
// client_name - text client name, which is used to identify the client in logs
// current_session_id - current session id, we compare it with the session id,received from the client
// p_srv_ctrl - pointer to the CServerControl object, which supports scene operations
int CmdGetFrameData(CArchive& arIn, CArchive& arOut, LPCSTR client_name, 
               int current_session_id, CServerControl* p_srv_ctrl);


// Suports CMD_GET_FRAME_DATA command
// Sends scene to the client
//
// arIn -  loading archive
// arOut - storing archive
// client_name - text client name, which is used to identify the client in logs
// current_session_id - current session id, we compare it with the session id,received from the client
// p_srv_ctrl - pointer to the CServerControl object, which supports scene operations
int CmdGetSceneData(CArchive& arIn, CArchive& arOut, LPCSTR client_name,
               int current_session_id, CServerControl* p_srv_ctrl);


// Suports CMD_SEND_LINE_DATA command
// Receives image line from the client
//
// arIn -  loading archive
// arOut - storing archive
// client_name - text client name, which is used to identify the client in logs
// current_session_id - current session id, we compare it with the session id,received from the client
// p_srv_ctrl - pointer to the CServerControl object, which supports scene operations
int CmdSendLineData(CArchive& arIn, CArchive& arOut, LPCSTR client_name,
               int current_session_id, CServerControl* p_srv_ctrl);



#endif//_SRV_CMD_H