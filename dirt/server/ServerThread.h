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
//                    and gathers rendered lines

class CLinesController
{
protected:
  class CLineItem{ //this class stores all information about single lines
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
  // scene_uid - uid of current scene
  // lines_count - image height
  // line_width  - image width
  // search_step - this number is used in GetNextLine2Render algorithm. 
                  //must be positive and for better perfomance must be small enough.
                  //Read GetNextLine2Render for details
  void Init( int scene_uid, int lines_count, int line_width, int search_step = 7 );

  //Frees the allocated memory and sets parameters to zero
  void Free();

  //Gives next line to render to client
  //Negative if the scene was alredy finished
  int  GetLine2Render(void);

  //Stores image line information
  //Nonzero if the image is completed
  int LineWasRendered(int line_num, COLORREF* line_data);


  //getters
  int GetWidth(void) const { return m_line_width; }
  int GetHeight(void) const { return m_lines_count; }
  int GetSceneUID(void) const { return m_scene_uid; }

  //returns the percentage of rendered lines
  int GetRenderedPercent(void) const;

protected:
  int  m_search_step;   //search step - is used in GetNextLine2Render algorithm
  int  m_line_width;    //image width
  int  m_lines_count;   //number of lines (image height)
  bool m_bCompleted;    //was the image completed?
  int  m_scene_uid;    //unique id of current scene
  int  m_rendered_count;    //number of rendered lines

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

  int StartServer(CWnd* p_frame, int portNum);
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

  // return line number which the client should render.
  // negative means that there is nothing to render
  int GetLineToRender(void);

  //processes image line, received from the client
  // scene_id  - scene_id, which client rendered
  // line_num  - number of the line which client rendered
  // pixel_count - number of pixels? rendered by client
  // line_data - image line data
  void LineReceived(int scene_id, int line_num, int pixel_count, COLORREF* line_data);


protected:
  void AcceptClient(void);

  
protected:  
  CServerSocket m_srv_sock;  //server socket - is used for Listen()
  CEvent m_stop_server_event; //signal means that all server threads must stop

  CMutex m_scene_change_mutex; //lock this if work with the scene
  CCriticalSection m_lines_change_cs; //lock this if work with lines
                               

  int m_last_session_id;    //is used to generate unique session ids
  CLinesController m_lines;    //all the information about the lines is stored here
  CEnvironment& m_scene;        //current scene
  bool m_bSceneCompleted;

  CWnd* m_p_frame;    //we send some notification messages to this window

  friend CServerSocket;
};


///////////////////////////////////////////////////////////

//void StartServerThread( void * param );

CWinThread* StartClientThread( void * param );
//void StopServerThread();




#endif // !defined(AFX_SERVERTHREAD_H__17E9B23D_C29A_447E_B4FB_FCF5A36F0587__INCLUDED_)
