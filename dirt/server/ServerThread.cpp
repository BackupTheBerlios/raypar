//*********************************************************
//** ServerThread.cpp **
// Created By: KIRILL
// On: 1/10/2004 22:59:41
// Comments: Server thread implementation file
//
//*********************************************************
// REVISION by KIRILL, on 1/30/2004 03:15:22
// Comments: ServerThreadFunction renamed to ServerThreadFunction
//
//*********************************************************
// REVISION by ..., on ...
// Comments: ...
//
//*********************************************************


#include "stdafx.h"
#include "ServerThread.h"
#include "common/protocol.h"
#include "srvcmd.h"
#include "common/protocol.h"
#include "client/geometry.h"   //?K? temp
#include "server.h"
#include "mainfrm.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "server.h"

//Such return means that an error occured in function
#define ERROR_RETURN 1

//Maximum number of clients waiting in queue for Accept
#define MAX_CLIENTS_IN_QUEUE 50  


///////////////////////////////////////////////////////////
//  SServerThreadParam
//

struct SServerThreadParam{  
  SOCKET h_socket;
  CServerControl* p_srv_ctrl;
};


///////////////////////////////////////////////////////////
//  CLinesController  - supports lines distribution among clients
//

CLinesController::CLinesController()
: m_line_width(0)
, m_lines_count(0)
, m_search_step(0)
, m_bCompleted(0)
, m_lines_info(0)
, m_scene_uid(0)
, m_rendered_count(0)
{}

CLinesController::~CLinesController()
{
  Free(); //we have to free allocated memory
}

//[Re]Initializes th object
// search step is used in GetNeaxtLine2Render algorithm
void CLinesController::Init( int scene_uid, int lines_count
                            , int line_width, int search_step /*= 7*/ )
{
  ASSERT( lines_count > 0 );
  ASSERT( line_width > 0 );
  ASSERT( search_step > 0 );
  ASSERT( scene_uid > 0); //zero or negative means that the scene 
                          //wasn't loaded
                          
  delete [] m_lines_info;

  m_lines_count = lines_count;
  m_line_width  = line_width;
  m_search_step = search_step;
  m_bCompleted  = false;
  m_scene_uid   = scene_uid;
  m_rendered_count = 0;

  if (m_search_step > m_lines_count)
    m_search_step = 1;

  m_lines_info = new CLineItem[m_lines_count] ;
  for(int i=0; i<m_lines_count; i++ )
    m_lines_info[i].AllocateData( m_line_width );
}

//Frees the allocated memory
void CLinesController::Free()
{   
  for(int i=0; i<m_lines_count; i++ )
    m_lines_info[i].FreeData( m_line_width );
  delete[] m_lines_info;
  m_lines_info = 0;
  m_lines_count = 0;
  m_line_width = 0;
  m_search_step = 0;
  m_bCompleted = 0;
  m_scene_uid = 0;
  m_rendered_count = 0;
}

//Gives next line to render to client
//Negative if the image was alredy finished
int  CLinesController::GetLine2Render(void)
{  
  if (m_bCompleted)
    return -1; //image finished

  ASSERT( m_lines_info );
  ASSERT( m_search_step <= m_lines_count );

  int search_step = m_search_step;

  int start = rand() % search_step;
  int line_num;

  //First we are trying to find nongiven line itereating with decreasing step
  //If we have found nothing 

  bool bFound=false, bEverythingGiven = false;
  for ( int i=start; !bFound && !bEverythingGiven; ){        
    if (! m_lines_info[i].m_bGiven ){
      m_lines_info[i].m_bGiven = true;
      m_lines_info[i].m_timestamp_given = GetTickCount();
      bFound = true;
      line_num = i;
    }else{
      int ni = i + search_step;
      if ( ni >= m_lines_count ){
        ni %= m_lines_count;
        search_step--;
        if ( 0==search_step )
          bEverythingGiven = true;
        if ( 1==m_search_step )
          ni = 0; //we must be careful not to jump over 0
      }
      i = ni;
    }
  }

  if (bFound){
    ASSERT( 0 <= line_num && line_num < m_lines_count );
    return line_num;
  }

  DWORD min_timestamp_given = 0;
  line_num = -1;

  //All lines were already given and we search the one which was given the first 
  //out of those which were not rendered yet.
  for( i=0; i<m_lines_count; i++ ){
    CLineItem& li = m_lines_info[i];
    
    ASSERT( li.m_bGiven ); // We have alredy checked that 
                                        //all lines were given away at lease once!
    if ( !li.m_bReceived &&
       ( line_num < 0 || li.m_timestamp_given < min_timestamp_given) ){
        min_timestamp_given = li.m_timestamp_given;
        line_num = i;
      }
  }

  if (line_num < 0){    
    ASSERT( 0 ); //all lines completed!
    m_bCompleted = true;
    return -1; //negative - no lines to render
  }
  
  ASSERT( line_num >=0 && line_num < m_lines_count );
  return line_num;
}

//Stores image line information
//Nonzero if the image is completed
int CLinesController::LineWasRendered(int line_num, COLORREF* line_data)
{
  ASSERT( line_num >= 0 && line_num < m_lines_count ); 
  ASSERT( AfxIsValidAddress(line_data, sizeof(COLORREF)*m_line_width ) );
  ASSERT( m_lines_info );

  Message("Line was rendered '%d'", line_num);

  CLineItem& li = m_lines_info[line_num];  
  
  if ( li.m_bReceived )   
    return m_bCompleted; //we have already received this line do nothing changed.
  
  // We store line_data and modify LineItem
  ASSERT( AfxIsValidAddress(li.m_data, sizeof(COLORREF)*m_line_width ) );
  memcpy( li.m_data, line_data, sizeof(COLORREF)*m_line_width );
  li.m_bReceived = true;
  m_rendered_count++;
  
  //Now we check whether we have finished the picture or no;
  bool bFoundNotReceived = false;
  //we start from the end. It seems to be more efficient
  for(int i=m_lines_count; i>=0 && !bFoundNotReceived; i--)
    if (! m_lines_info[i].m_bReceived )
      bFoundNotReceived = true;

  if (bFoundNotReceived)
    return 0; //we haven't finished the image yet
  
  //image was finished
  m_bCompleted = true; 
  ASSERT( m_rendered_count = m_lines_count );

  return 1;  
}

//returns the percentage of rendered lines
int CLinesController::GetRenderedPercent(void) const
{
  return m_rendered_count*100/m_lines_count;
}

//allocates memry and create bitmap bits from 
//received image lines. must be called only when the scene is done
void* CLinesController::BuildBitmapBits(void) const
{
  ASSERT( m_bCompleted );  
  ASSERT( m_lines_info );

  COLORREF* m_data = new COLORREF[ m_lines_count * m_line_width ];

  COLORREF* cur_p = m_data;
  for(int i=0; i<m_lines_count; i++ ){
    ASSERT( m_lines_info[i].m_bReceived );
    memcpy( cur_p, m_lines_info[i].m_data, sizeof(COLORREF)*m_line_width );
    cur_p += m_line_width;
  }

  return m_data;
}

///////////////////////////////////////////////////////////
//  CServerSocket    - server socket class
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

CServerControl::CServerControl(CEnvironment& scene)
: m_srv_sock( *this )
, m_last_session_id( 0 )
, m_scene( scene )
, m_p_frame( 0 )
{ 
}

CServerControl::~CServerControl()
{  
  m_lines.Free();
}


//  Creates socket and starts listening
//  returns 0 if successful
int CServerControl::StartServer(CWnd* p_frame, int portNum, int imageWidth, int imageHeight)
{
  ASSERT( p_frame );
  m_p_frame = p_frame;
  
  int ret = m_srv_sock.Create( portNum );
  if ( !ret ){
    CString err_str = GetErrorMessageByErrorCode(ret);
    ErrorMessage("Can't create socket: '%s'", (LPCSTR)err_str);
    return ERROR_RETURN; 
  }

  //KIRILL: temp:
  int scene_uid = m_scene.GetSceneUID();
  m_lines.Init( scene_uid, imageWidth, imageHeight );
  m_srv_sock.Listen( MAX_CLIENTS_IN_QUEUE ); 

  return 0;
}

int CServerControl::StopServer()
{
  m_srv_sock.Close();
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
  sp->p_srv_ctrl = this;

  CWinThread* srv_thr = StartServerThread( sp );
  if ( !srv_thr ){
    ErrorMessageWithBox( "Unable to create server thread!" );
    delete sp; //we have to do this
  }
}

//Generates unique session id
int CServerControl::GetNewSessionId(void)
{
  CSingleLock cs_lock(&m_session_id_change_cs, TRUE);
  return ++m_last_session_id;
}

//this functions can be called from different threads!
//Fills required scene parameters
//Zero if successful
//p_image_lines_info->m_line_number may be negative
//negative means that the image was completed
//and there is no line to render.
int CServerControl::FillSceneParameters( int* p_scene_id,
                         CImageLinesInfo* p_image_lines_info,
                         CCameraInfo*  p_camera_info )
{
  ASSERT( p_scene_id );
  ASSERT( p_image_lines_info );
  ASSERT( p_camera_info );

  m_lines_change_cs.Lock(); //we lock the access to the lines info

  
  p_camera_info->m_camera_pos = CVector(0,0,0);
  p_camera_info->m_camera_y_axis = CVector(0,1,0);
  p_camera_info->m_camera_z_axis = CVector(0,0,1);
  
  *p_scene_id = m_lines.GetSceneUID(); 
  ASSERT( *p_scene_id > 0);//zero or negative scene id means that 
                           //the scene wasn't loaded
  
  p_image_lines_info->m_image_height = m_lines.GetHeight();
  p_image_lines_info->m_image_width = m_lines.GetWidth();
  p_image_lines_info->m_line_number = m_lines.GetLine2Render();//may be negative
                        //negative means that the image was completed
                        //and there is no line to render.
  
  m_lines_change_cs.Unlock();

  return 0;
}


//returns pointer to the scene
CEnvironment* CServerControl::GetScene(void)
{  
  return &m_scene;
}

// processes image line, received from the client
// scene_id  - scene_id, which client rendered
// line_num  - number of the line which client rendered
// pixel_count - number of pixels? rendered by client
// line_data - image line data
void CServerControl::LineReceived(int scene_id, int line_num
                                  , int pixel_count, COLORREF* line_data)
{
  m_lines_change_cs.Lock();

  if ( pixel_count = m_lines.GetWidth()  && scene_id == m_lines.GetSceneUID() 
       && line_num >= 0 && line_num < m_lines.GetHeight())  
     //client've rendered the scene which we are interested in. 
     //and number of pixels client rendered equals to the number we are interested in
     //and the line number received is appropriate.
  {
    if (! m_lines.IsCompleted() ){//we don't want to add lines 
                                  //to finished image
      int bCompleted = m_lines.LineWasRendered(line_num, line_data);
      if ( bCompleted ){       
        ASSERT( m_p_frame );
        ::PostMessage(*m_p_frame, WM_SERVER_FINISHED_SCENE, 0, 0);
      }else{
        int percent = m_lines.GetRenderedPercent();
        ::PostMessage(*m_p_frame, WM_SERVER_LINE_RENDERED, percent, 0);
      }
    }
    
  }

  m_lines_change_cs.Unlock();
}


//allocates memry and create bitmap bits from 
//received image lines. must be called only when the scene is done
void* CServerControl::BuildBitmapBits(void) const
{
  ASSERT( m_lines.IsCompleted() );
  return m_lines.BuildBitmapBits();
}

///////////////////////////////////////////////////////////
//  ServerThreadFunction
//

UINT ServerThreadFunction( void* param )
{  
  SServerThreadParam * sp = (SServerThreadParam*) param;
  ASSERT( AfxIsValidAddress(param, sizeof (SServerThreadParam ) ) );

  SOCKET _socket = sp->h_socket;
  CServerControl* p_srv_ctrl = sp->p_srv_ctrl;
  ASSERT( p_srv_ctrl );
  delete sp; //we must free thread parameters in this thread!

  TRY{
  

    CSocket cl_sock;
    int ret = cl_sock.Attach( _socket );

    if (!ret){
      ASSERT( 0 );
      CString err_text = GetErrorMessageByErrorCode(  );
      ErrorMessage( "Can't attach socket: '%s'", (LPCSTR)err_text );
      return ERROR_RETURN;
    }

    CString sock_name;
    UINT sock_port;
    ret = cl_sock.GetPeerName(sock_name, sock_port);

    if (!ret){
      CString err_text = GetErrorMessageByErrorCode( );
      ErrorMessage( "Can't get peer name: '%s'", (LPCSTR)err_text );
      return ERROR_RETURN;
    }

    CString client_name; //this name is used to identify the client in logs
    client_name.Format("%s:%d", (LPCSTR)sock_name, sock_port);

    Message("CL[%s] Client connected", client_name);

    CSocketFile sock_file( &cl_sock );
    CArchive arIn( &sock_file, CArchive::load );
    CArchive arOut( &sock_file, CArchive::store );

    int current_session_id = p_srv_ctrl->GetNewSessionId(); 

    bool bEnd = false;
    
    while (!bEnd) {
      int cmd_id;
      arIn >> cmd_id;

      switch(cmd_id){
        case CMD_CONNECTION_INIT: 
          {
            int ret = CmdConnectionInit(arIn, arOut, client_name, current_session_id); 
            if ( ERROR_MUST_TERMINATE == ret )
              bEnd = true;
            break; 
          }
        case CMD_GET_FRAME_DATA:
          {
            int ret = CmdGetFrameData(arIn, arOut, client_name, current_session_id, p_srv_ctrl);
            if ( ERROR_MUST_TERMINATE == ret )
              bEnd = true;
            break;
          }
        case CMD_GET_SCENE_DATA:
          {    
            int ret = CmdGetSceneData(arIn, arOut, client_name, current_session_id, p_srv_ctrl);
            if ( ERROR_MUST_TERMINATE == ret )
                bEnd = true;          
            bEnd = true; //KIRILL: temporarily 
            break;
          }
        case CMD_SEND_LINE_DATA:
          {    
            int ret = CmdSendLineData(arIn, arOut, client_name, current_session_id, p_srv_ctrl);
            if ( ERROR_MUST_TERMINATE == ret )
                bEnd = true;
            break;
          }
        default:{
            ASSERT(0);
            ErrorMessage( "CL[%s] Unknown command received from client! Terminating connection", client_name );
            bEnd = true;
          }
      }
    }

    cl_sock.Close();
  }
  CATCH(CArchiveException, e)
  {
    //this exception probably means that the connection was closed
  }AND_CATCH(CFileException, pEx){
    //this exception probably means that the connection was closed
    //so we ignore it
  }AND_CATCH_ALL(pEx){
    ErrorMessageFromException(pEx, TRUE);
  }
  END_CATCH_ALL
  

  return 0;
}


CWinThread* StartServerThread( void * param )
{  
  return AfxBeginThread( ServerThreadFunction, param );
}


