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

#include "client/environment.h" 

struct CImageLinesInfo;
struct CCameraInfo;


///////////////////////////////////////////////////////////
//  CLinesController  - supports lines distribution among clients
//                    and gathers rendered lines

class CLinesController
{
protected:
  class CLineItem{ //this class stores all information about single line
  public:
    CLineItem();
    ~CLineItem();
    
    void AllocateData(int line_width); //allocates memry for data
    void FreeData(int line_width);  //frees allocated memory
    void Reset(void); //resets line state to ungiven

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

  void Init( int scene_uid, int lines_count, int line_width );

  //Frees the allocated memory and sets parameters to zero
  void Free();

  //Gives next line to render to client
  //Negative if the scene was alredy finished
  int GetLine2Render(void);

  //Stores image line information
  //Nonzero if the image is completed
  int LineWasRendered(int line_num, COLORREF* line_data);

  //getters
  int GetWidth(void) const { return m_line_width; }
  int GetHeight(void) const { return m_lines_count; }
  int GetSceneUID(void) const { return m_scene_uid; }
  bool IsCompleted(void) const { return m_bCompleted; }

  void SetSceneUID(int scene_uid) { ASSERT( m_scene_uid >0 ); m_scene_uid = scene_uid; }

  //returns the percentage of rendered lines
  int GetRenderedPercent(void) const;

  //resets all lines
  void ResetAllLines(void);

  //allocates memry and create bitmap bits from 
  //received image lines. must be called only when the scene is done
  void* CLinesController::BuildBitmapBits(void) const;

protected:
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
  CServerControl();
  ~CServerControl();
  
  //  Creates socket and starts listening
  //  returns 0 if successful
  int CServerControl::StartServer(CWnd* p_frame, int portNum
                                  , CEnvironment* p_scene, CCamera* p_camera );

  //Stops server and closes listening socket
  int StopServer();

  //Setups new scene info. Thread safe.
  void SetNewScene(CEnvironment* p_scene, CCamera* p_camera);
  
  //this functions can be called from different threads!
  //Generate client session id.
  int GetNewSessionId(void);
  
  //this functions can be called from different threads!
  //Fills required scene parameters
  //Zero if successful
  int FillSceneParameters( int* p_scene_id,
                           CImageLinesInfo* p_image_lines_info,
                           CCameraInfo*  p_camera_info );

  //gives current scene
  CEnvironment* GetScene(void);

  // return line number which the client should render.
  // negative means that there is nothing to render
  int GetLineToRender(void);

  //processes image line, received from the client
  // scene_id  - scene_id, which client rendered
  // line_num  - number of the line which client rendered
  // pixel_count - number of pixels? rendered by client
  // line_data - image line data
  void LineReceived(int scene_id, int line_num, int pixel_count, COLORREF* line_data);

  //allocates memry and create bitmap bits from 
  //received image lines. must be called only when the scene is done
  void* BuildBitmapBits(void) const;

  //current rendering image width and height getters
  int GetWidth(void) const { return m_lines.GetWidth(); }
  int GetHeight(void) const { return m_lines.GetHeight(); }

  //return the value of m_b_server_should_stop flag
  //if returned true client should terminate thread
  bool ShouldStop(void) { return m_b_server_should_stop; }

  //client should call this before reading the scene
  //this may block
  void WantStartReadScene();

  //client should call this after it finished reading the scene
  void FinishedReadingScene();

protected:
  void AcceptClient(void);
  
protected:  
  CServerSocket m_srv_sock;  //server socket - is used for Listen()  

  CCriticalSection m_lines_change_cs; //lock this if work with lines
  CCriticalSection m_session_id_change_cs; //lock this when you read or modify 
                                //m_last_session_id.  Used in GetNewSessionId();

  bool m_b_server_should_stop; //true means that all clients should terminate

  int m_last_session_id;    //is used to generate unique session ids
  CLinesController m_lines;    //all the information about the lines is stored here
  CWnd* m_p_frame;    //we send some notification messages to this window

  //all scene inforamtaion
  CEnvironment* m_scene;        //current scene
  CCamera* m_camera;            //carrent camera info

  //these members are used for thread safe scene reading/modifying
  int m_reading_clients_count; //number of clients, reading the scene

  CCriticalSection m_reading_count_cs; //m_reading_clients_count can be read or modifyed
                                    //only in this critical section



  CEvent m_can_modify_scene_event; //signal means that the server may modify scene

  CEvent m_can_read_scene_event; //signal means that client thread may read scene
                                 //this is manual reset event

  

  friend CServerSocket;
};


///////////////////////////////////////////////////////////

CWinThread* StartServerThread( void * param );
//void StopServerThread();




#endif // !defined(AFX_SERVERTHREAD_H__17E9B23D_C29A_447E_B4FB_FCF5A36F0587__INCLUDED_)
