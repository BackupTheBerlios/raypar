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

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

int CmdConnectionInit(CArchive& arIn, CArchive& arOut, int& last_session_id)
{
  int client_protocol_version;
  CConnectionInit::Q conn_init_Q( &client_protocol_version );
  conn_init_Q.read( arIn );

  if ( client_protocol_version != PROTOCOL_VERSION ){ 
    //Client protocol differs from ours. We should terminate connection

    ASSERT( 0 );
    ErrorMessage("Client protocol differs from ours. Terminating the connection");
    return ERROR_MUST_TERMINATE;
  }

  ++last_session_id;
  int server_protocol_version = PROTOCOL_VERSION;
  CConnectionInit::A conn_init_A( server_protocol_version, last_session_id );
  conn_init_A.write( arOut );
  arOut.Flush();  

  return 0;
}