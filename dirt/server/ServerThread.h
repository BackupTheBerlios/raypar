//*********************************************************
//** ServerThread.h **
// Created By: KIRILL
// On: 1/10/2004 22:58:14
// Comments: interface for Server thread functions 
//   This thread does all the work with network clients.
//
//*********************************************************
// REVISION by ..., on ...
// Comments: ...
//
//*********************************************************

#if !defined(AFX_SERVERTHREAD_H__17E9B23D_C29A_447E_B4FB_FCF5A36F0587__INCLUDED_)
#define AFX_SERVERTHREAD_H__17E9B23D_C29A_447E_B4FB_FCF5A36F0587__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

///////////////////////////////////////////////////////////
class  CServerControl;

class CServerSocket : public CSocket{
public:
  CServerSocket(CServerControl& m_srv_ctrl);

  virtual void OnAccept(int errorCode);
public:
  CServerControl& m_srv_ctrl;
};

///////////////////////////////////////////////////////////
class CServerControl {
public:
  CServerControl();
  //virtual ~CServerControl();

  int StartServer(int portNum);
  int StopServer();
  
protected:
  void AcceptClient(void);

  
protected:  
  CServerSocket m_srv_sock;
  CEvent m_stop_server_event; //signal means that all server threads must stop

  friend CServerSocket;
};


///////////////////////////////////////////////////////////

//void StartServerThread( void * param );

CWinThread* StartServerThread( void * param );
//void StopServerThread();











#endif // !defined(AFX_SERVERTHREAD_H__17E9B23D_C29A_447E_B4FB_FCF5A36F0587__INCLUDED_)
