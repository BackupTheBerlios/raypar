//*********************************************************
//** ServerThread.cpp **
// Created By: KIRILL
// On: 1/10/2004 22:59:41
// Comments: Server thread implementation file
//
//*********************************************************
// REVISION by ..., on ...
// Comments: ...
//
//*********************************************************


#include "stdafx.h"
#include "server.h"
#include "ServerThread.h"
#include "common/protocol.h"
#include "srvcmd.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//Such return means that an error occured in function
#define ERROR_RETURN 1

//Maximum number of clients waiting in queue for Accept
#define MAX_CLIENTS_IN_QUEUE 50  

///////////////////////////////////////////////////////////
//  SServerThreadParam
//

struct SServerThreadParam{
  SOCKET h_socket;
};


///////////////////////////////////////////////////////////
//  CServerSocket
//

CServerSocket::CServerSocket(CServerControl& srv_ctrl)
: m_srv_ctrl( srv_ctrl )
{
}

void CServerSocket::OnAccept(int errorCode)
{
  if (errorCode){
    CString err_str = GetErrorMessageByErrorCode(errorCode);
    ErrorMessage( "Error accepting client connection: '%s'", (LPCSTR) err_str );    
  }else{
    m_srv_ctrl.AcceptClient();

  }
}


///////////////////////////////////////////////////////////
//  CServerControl
//

#pragma warning( disable : 4355 )

CServerControl::CServerControl()
: m_srv_sock( *this )
{  
}


//  Creates socket and starts listening
//  returns 0 if successful
//
int CServerControl::StartServer(int portNum)
{
  int ret = m_srv_sock.Create( portNum );
  if ( !ret ){
    CString err_str = GetErrorMessageByErrorCode(ret);
    ErrorMessage("Can't create socket: '%s'", (LPCSTR)err_str);
    return ERROR_RETURN; 
  }
  m_srv_sock.Listen( MAX_CLIENTS_IN_QUEUE ); 
  return 0;
}


//Accepts client and builds server thread for this client
//
void CServerControl::AcceptClient()
{
  CSocket cl_sock;
  m_srv_sock.Accept( cl_sock );
  SServerThreadParam* sp = new SServerThreadParam; //this memory will have been 
                                                  // freed by the thread
  
  sp->h_socket = cl_sock;
  cl_sock.Detach(); 

  CWinThread* srv_thr = StartServerThread( sp );
  if ( !srv_thr ){
    ErrorMessageWithBox( "Unable to create server thread!" );
    delete sp; //we have to do this
  }
}

///////////////////////////////////////////////////////////
//  ServerThreadFunction
//

static CEvent server_thread_stop_event;

UINT ServerThreadFunction( void* param )
{  
  SServerThreadParam * sp = (SServerThreadParam*) param;
  ASSERT( AfxIsValidAddress(param, sizeof (SServerThreadParam ) ) );
  //server_thread_stop_event.ResetEvent();

  SOCKET _socket = sp->h_socket;
  delete sp; //we must free thread parameters in this thread!

  CSocket cl_sock;
  int ret = cl_sock.Attach( _socket );
  

  if (!ret){
    ASSERT( 0 );
    CString err_text = GetErrorMessageByErrorCode( GetLastError() );
    ErrorMessage( "Can't attach socket: '%s'", (LPCSTR)err_text );
    return ERROR_RETURN;
  }

  CString sock_name;
  UINT sock_port;
  ret = cl_sock.GetPeerName(sock_name, sock_port);

  if (!ret){
    CString err_text = GetErrorMessageByErrorCode( GetLastError() );
    ErrorMessage( "Can't get peer name: '%s'", (LPCSTR)err_text );
    return ERROR_RETURN;
  }

  Message("Client connected: '%s:%d'", (LPCSTR)sock_name, sock_port);

  CSocketFile sock_file( &cl_sock );
  CArchive arIn( &sock_file, CArchive::load );
  CArchive arOut( &sock_file, CArchive::store );

  int last_session_id = 1; //let it be here for a while

  bool bEnd = false;
  while (!bEnd) {
    int cmd_id;
    arIn >> cmd_id;

    switch(cmd_id){
      case CMD_CONNECTION_INIT: CmdConnectionInit(arIn, arOut, last_session_id); break; 
      case CMD_GET_FRAME_DATA:  break;
      case CMD_GET_SCENE_DATA:  break;
      default:{      
          ASSERT(0);
          ErrorMessage( "Unknown command received from client ['']! Terminating connection" );
          bEnd = true;
        }
    }
  }


  cl_sock.Close();
  
  return 0;
}



CWinThread* StartServerThread( void * param )
{  
  return AfxBeginThread( ServerThreadFunction, param );
}

void StopServerThread()
{  
  server_thread_stop_event.ResetEvent();
}

