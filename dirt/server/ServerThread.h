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

#include "client/environment.h" //?K? !!

struct CImageLinesInfo;
struct CCameraInfo;


///////////////////////////////////////////////////////////
//  CLinesController  - supports lines distribution among clients
//

class CLinesController
{
  class CLineItem{
  public:
    CLineItem()
      : m_bGiven( 0 )
      , m_bReceived( 0 )
      , m_timestamp_given( 0 )
      , m_line_width( 0 )
      , m_data(0)
    {}

    ~CLineItem()
    { 
      delete[] m_data;
    }
    
    void AllocateData(int line_width)
    {      
      ASSERT( line_width > 0 );
      m_line_width = line_width;
      ASSERT( m_data == NULL );
      m_data =  new COLORREF[m_line_width];
    }

    void FreeData(int line_width)
    {      
      delete[] m_data;
      m_data = 0;
    }

    int m_bGiven; //was the line already given?
    int m_bReceived; //was the line already received?
    DWORD m_timestamp_given; //when the line was given LAST time  
    int m_line_width; //image line width
    COLORREF* m_data; //image line data (picture)
  };

public:   
   CLinesController();
  ~CLinesController();
  //[Re]Initializes the object
  void Init( int lines_count, int line_width, int search_step = 7 );
  //Frees the allocated memory and sets parameters to zero
  void Free();

  //Gives next line to render to client
  //Negative if the scene was alredy finished
  int  GetLine2Render(void);

  //Stores image line information
  //Nonzero if the image is completed
  int LineWasRendered(int line_num, COLORREF* line_data);

  int GetWidth(void) const { return m_line_width; }
  int GetHeight(void) const { return m_lines_count; }


protected:
  int m_search_step;
  int m_line_width;
  int m_lines_count;
  bool m_bCompleted;

  CLineItem* m_lines_info;
};


///////////////////////////////////////////////////////////
//  CServerSocket - server socket class
//

class  CServerControl;

class CServerSocket : public CSocket{
public:
  CServerSocket(CServerControl& m_srv_ctrl);

  virtual void OnAccept(int errorCode);
public:
  CServerControl& m_srv_ctrl;
};


///////////////////////////////////////////////////////////
//  CServerControl
//  This realization supports only only client per time.
//

class CServerControl {
public:
  CServerControl(CEnvironment& scene);
  ~CServerControl();

  int StartServer(int portNum);
  int StopServer();
  
  
  //this functions can be called from different threads!
  //Generate client session id.
  int GetNewSessionId(void);
  
  //this functions can be called from different threads!
  //Fills required scene parameters
  //Zero if successful
  int FillSceneParameters( int* p_scene_id,
                           CImageLinesInfo* p_image_lines_info,
                           CCameraInfo*  p_camera_info );

  //If you need to operate with scene you have to block the access to it
  CEnvironment* GetAndLockScene(void);

  //When you finished your work you must free scene access
  void UnlockScene(void);


protected:
  void AcceptClient(void);

  
protected:  
  CServerSocket m_srv_sock;
  CEvent m_stop_server_event; //signal means that all server threads must stop

  CMutex m_scene_change_mutex; //lock this if work with the scene
  CMutex m_change_lines_mutex; //lock this if work with lines
                               

  int m_last_session_id;
  CLinesController m_lines;    //all the information about the lines is stored here

  CEnvironment& m_scene;        //current scene

  friend CServerSocket;
};


///////////////////////////////////////////////////////////

//void StartServerThread( void * param );

CWinThread* StartServerThread( void * param );
//void StopServerThread();











#endif // !defined(AFX_SERVERTHREAD_H__17E9B23D_C29A_447E_B4FB_FCF5A36F0587__INCLUDED_)
