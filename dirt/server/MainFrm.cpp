//****************************************
//** MainFrm.cpp **
// Created By: KIRILL
// On :10/27/2003 01:45:08
// Comments: implementation of the CMainFrame class
//
//*********************************************************
// REVISION by KIRILL, on 1/17/2004 05:21:37
// Comments: OnUserAddLogMessage was added in order to ssupport 
// multithreaded logging.
//
//****************************************
// REVISION by Vader, on 2/02/2004
// Comments: Added saving resulting picture as bitmap file
//***********************************
// REVISION by ..., on ...
// Comments: ...
//***********************************


#include "stdafx.h"
#include "SERVER.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////////////////////////////////////////////////
// this gloabal variable is used tby lexer/parser
CServerSceneBuilder* glb_scene_builder;

/////////////////////////////////////////////////////////////////////////////
// CMainFrame


IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_COMMAND(ID_VIEW_LOGWINDOW, OnViewLogwindow)
	ON_COMMAND(ID_VIEW_OPTIONS, OnViewOptions)
	ON_COMMAND(ID_RUN, OnRun)
	ON_COMMAND(ID_OPEN_SCENE, OnOpenScene)	
	ON_WM_DESTROY()
	ON_COMMAND(ID_STOP, OnStop)
	ON_UPDATE_COMMAND_UI(ID_STOP, OnUpdateStop)
	ON_UPDATE_COMMAND_UI(ID_RUN, OnUpdateRun)
	ON_UPDATE_COMMAND_UI(ID_OPEN_SCENE, OnUpdateOpenScene)
  ON_MESSAGE( WM_USER_ADD_LOG_MSG, OnUserAddLogMessage )
  ON_MESSAGE( WM_SERVER_FINISHED_SCENE, OnServerFinishedScene )
  ON_MESSAGE( WM_SERVER_LINE_RENDERED, OnServerLineRendered )
	ON_UPDATE_COMMAND_UI(ID_VIEW_OPTIONS, OnUpdateViewOptions)
	//}}AFX_MSG_MAP
	// Global help commands
	ON_COMMAND(ID_HELP_FINDER, CFrameWnd::OnHelpFinder)
	ON_COMMAND(ID_HELP, CFrameWnd::OnHelp)
	ON_COMMAND(ID_CONTEXT_HELP, CFrameWnd::OnContextHelp)
	ON_COMMAND(ID_DEFAULT_HELP, CFrameWnd::OnHelpFinder)  
END_MESSAGE_MAP()


static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
  ID_PROGRESS_INDICATOR   //progress indicator pane
};

//our indicator is the second in the status bar
#define PROGRESS_INDICATOR_INDEX 1

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
: m_scene_builder( m_scene )
, m_srv_ctrl()
, m_settings(mainFrameSection, MAINFRAME_DEFAULT_LEFT, MAINFRAME_DEFAULT_TOP
                             , MAINFRAME_DEFAULT_WIDTH, MAINFRAME_DEFAULT_HEIGHT)
{	
  m_last_scene_uid = 1; //zero scene uid means that scene wasn't loaded
  m_scene.SetSceneUID( 0 ); 
  glb_scene_builder = &m_scene_builder;
	m_settings.SetSection(mainFrameSection);
  m_bServerStarted = false;  
}

CMainFrame::~CMainFrame()
{
  glb_scene_builder = 0;  
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	// create a view to occupy the client area of the frame
	if (!m_wndView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
		CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))
	{
		TRACE0("Failed to create view window\n");
		return -1;
	}
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

  m_wndStatusBar.SetPaneText(PROGRESS_INDICATOR_INDEX, "");  

  if ( !m_log_wnd.Create(IDD_DIALOG_LOG, 0) ){
		TRACE0("Failed to create log window\n");
		return -1;      // fail to create
  }

  m_toolbar_bmp_stop.LoadBitmap( IDB_TOOLBAR_BITMAP_STOP );
  
  m_log_box.Attach( m_log_wnd.GetDlgItem( IDC_LOG_LIST )->m_hWnd );
	  
  m_log_wnd.ShowWindow( SW_NORMAL );

  //Loading info from reg on create
  m_settings.LoadResizablePosition(this);
  SetWindowText( "DiRT server" );

  m_serverOptions.GetDataFromReg();

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers
void CMainFrame::OnSetFocus(CWnd* pOldWnd)
{
	// forward focus to the view window
	m_wndView.SetFocus();
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// let the view have first crack at the command
	if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	// otherwise, do default handling
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CMainFrame::OnViewLogwindow() 
{
	if ( !m_log_wnd.IsWindowVisible())
    m_log_wnd.ShowWindow(SW_SHOWNORMAL);
  else
    m_log_wnd.ShowWindow(SW_HIDE);
}

void CMainFrame::OnViewOptions() 
{
	// TODO: Add your command handler code here
	//ErrorMessage("There is no realization yet");
  m_serverOptions.GetDataFromReg();
  COptionsDialog dlg(NULL, &m_serverOptions);
  if(dlg.DoModal() == IDOK) m_serverOptions.SaveDataToReg();
  
}

void CMainFrame::OnRun() 
{
  m_camera.SetWidth(  m_serverOptions.GetImageWidth() );
  m_camera.SetHeight( m_serverOptions.GetImageHeight() );
  m_camera.SetEyePoint(CVector(0,0,0));//temp KIRILL

  int ret = m_srv_ctrl.StartServer( this, m_serverOptions.GetServerPort(),
                             &m_scene, &m_camera );

  if ( !ret ){
    m_bServerStarted = true;
    Message("Server started on port '%d'", m_serverOptions.GetServerPort());
  }else{
    ErrorMessage("Unable to start server!");
  }
  
}

extern FILE *yyin; //located in ray_lex.l.cpp    
extern int yyparse(void);

void CMainFrame::OnOpenScene() 
{  
  CFileDialog ofd( TRUE, 0, 0, 0, "Scene Files (*.sc)|*.sc|All Files (*.*)", this);
  int ret = ofd.DoModal();

  if ( ret == IDOK ){
    m_scene_builder.Init(); //this will clean the scene
               //and set it uid to 0 - that means that the scene isn't loaded
    
    CString fname = ofd.GetPathName();
    yyin = fopen( fname, "r");
    ASSERT( yyin );
    int ret = yyparse();
    if ( ret != 0 ){
      char err_buf[128];
      sprintf(err_buf, "Wrong scene. Parsing stopped on line %d", 
                        m_scene_builder.GetCurrentLineNumber() );
      ErrorMessageWithBox( err_buf );
      m_scene.Empty();
      m_wndStatusBar.SetPaneText(PROGRESS_INDICATOR_INDEX, "");      
    }else{    
      m_scene.SetSceneUID(GetNewSceneUID());
      ASSERT( m_scene.IsValid() );
      m_wndStatusBar.SetPaneText(PROGRESS_INDICATOR_INDEX, "Loaded");
    }
    Message( "'%s' parsed (%d lines)", (LPCSTR) fname, m_scene_builder.GetCurrentLineNumber() );
    fclose(yyin);
    yyin = 0;
  }else{
    //error occurred or Cancel was pressed
    int err = CommDlgExtendedError();
    if (err)
      ErrorMessage("Error open Dialog box - %d ", err);
  }
}

void CMainFrame::OnDestroy() 
{
  m_settings.SavePosition(this);
	CFrameWnd::OnDestroy();
  m_log_box.Detach();	
}


//this is used to handle WM_USER_ADD_LOG_MESSAGE which can be sent 
//by ServerLogMessage(.). Receives messages sent by
//ServerLogMessage(.) and adds the information to the list box. 
//This function will ALWAYS be called in the main thread.
//Params:
// WPARAM = MAKEWPARAM( USER_ADD_LOG_MSG_CODE, <meassage type> )
// LPARAM = pointer to the message text
//
	
LRESULT CMainFrame::OnUserAddLogMessage(WPARAM wParam, LPARAM lParam)
{
  ASSERT( AfxIsValidString((char*)lParam) );
  ASSERT( LOWORD(wParam) == USER_ADD_LOG_MSG_CODE );  //Unknown caller??? 
                                                    //Calling conventions were broken
  int ret = 0;

  int msg_type = HIWORD(wParam);
  if ( LOWORD(wParam) == USER_ADD_LOG_MSG_CODE ){  
    switch (msg_type) {
      case LOG_MSG_ERROR:   m_log_box.AddError((char*)lParam); break;
      case LOG_MSG_NORMAL:  m_log_box.AddMessage((char*)lParam); break;
      default: ASSERT(0); //unknown type!
    };
         
    // delete (char*)lParam; //we have to clean the memory
    ret = 1; //this means that we've processed the message
  }
  return ret; 
}

//Client threads use WM_SERVER_FINISHED_SCENE message to inform main thread 
//that the scene rendering was finished (it is sent once per scene)
LRESULT CMainFrame::OnServerFinishedScene(WPARAM wParam, LPARAM lParam)
{

  void* bitmap_lines = m_srv_ctrl.BuildBitmapBits();
  m_wndView.SetBitmapParams( m_srv_ctrl.GetWidth()
                             , m_srv_ctrl.GetHeight(), bitmap_lines );
    //save result in BMP file if needed
  if(m_serverOptions.DoSaveFile())
  {
    HANDLE hf;                 // file handle 
    BITMAPINFOHEADER bmih;
    BITMAPFILEHEADER hdr;       // bitmap file-header
    DWORD dwTmp;
    DWORD dwTotal;              // total count of bytes 
    DWORD cb;                   // incremental count of bytes


    bmih.biSize = sizeof(BITMAPINFOHEADER);
    bmih.biWidth = m_srv_ctrl.GetWidth();
    bmih.biHeight = m_srv_ctrl.GetHeight();
    bmih.biPlanes = 1;
    bmih.biBitCount = 32;
    bmih.biCompression = BI_RGB;
    bmih.biSizeImage = m_srv_ctrl.GetWidth() * m_srv_ctrl.GetHeight() * 4;
    bmih.biXPelsPerMeter = 0;
    bmih.biYPelsPerMeter = 0;
    bmih.biClrImportant = 0;
    bmih.biClrUsed = 0;



        // Create the .BMP file. 
    hf = CreateFile(m_serverOptions.GetFileName(), 
                     GENERIC_READ | GENERIC_WRITE, 
                     (DWORD) 0, 
                      NULL, 
                     CREATE_ALWAYS, 
                     FILE_ATTRIBUTE_NORMAL, 
                     (HANDLE) NULL); 
    if (hf == INVALID_HANDLE_VALUE) 
        ErrorMessage("Error creating file");
    hdr.bfType = 0x4d42;        // 0x42 = "B" 0x4d = "M" 
    // Compute the size of the entire file. 
    hdr.bfSize = (DWORD) (sizeof(BITMAPFILEHEADER) + 
                 bmih.biSize + bmih.biClrUsed 
                 * sizeof(RGBQUAD) + bmih.biSizeImage); 
    hdr.bfReserved1 = 0; 
    hdr.bfReserved2 = 0; 

    // Compute the offset to the array of color indices. 
    hdr.bfOffBits = (DWORD) sizeof(BITMAPFILEHEADER) + 
                    bmih.biSize + bmih.biClrUsed 
                    * sizeof (RGBQUAD); 
  
    // Copy the BITMAPFILEHEADER into the .BMP file. 
    if (!WriteFile(hf, (LPVOID) &hdr, sizeof(BITMAPFILEHEADER), 
        (LPDWORD) &dwTmp,  NULL)) 
    {
       ErrorMessage("Error writing BITMAPFILEHEADER into file");
    }
    // Copy the BITMAPINFOHEADER and RGBQUAD array into the file. 
    if (!WriteFile(hf, (LPVOID) &(bmih), sizeof(BITMAPINFOHEADER) 
                    + bmih.biClrUsed * sizeof (RGBQUAD), 
                    (LPDWORD) &dwTmp, ( NULL)))
    {
       ErrorMessage("Error writing BITMAPINFOHEADER and RGBQUAD array into file");
    }

    // Copy the array of color indices into the .BMP file. 
    dwTotal = cb = bmih.biSizeImage; 
    if (!WriteFile(hf, (LPSTR) bitmap_lines, (int) cb, (LPDWORD) &dwTmp,NULL))
    {
      ErrorMessage("Error writing Image into file");
    }

    // Close the .BMP file. 
    if (!CloseHandle(hf)) 
    {
      ErrorMessage("Error closing file");
    }
  }

  delete[] bitmap_lines;

  int ret = m_wndStatusBar.SetPaneText(PROGRESS_INDICATOR_INDEX, "Finished");
  if ( 1 ){
    m_srv_ctrl.StopServer();
    m_bServerStarted = false;	    
  }else{
    //temp    
    m_srv_ctrl.SetNewScene(&m_scene, &m_camera);    
  }

  m_wndView.Invalidate();  //we should repaint the window but we do it only
  m_wndView.UpdateWindow(); //here not to waste server time

  return 1; //this means that we've processed the message
}


//Client threads use WM_SERVER_LINE_RENDERED message to inform main thread that 
//new line was received from client and send percent of rendered lines
//in WPARAM.
LRESULT CMainFrame::OnServerLineRendered(WPARAM wParam, LPARAM lParam)
{
  ASSERT( wParam >=0 && wParam <= 100 ); //wParam is the percentage 
                                        //of completed lines
  char buf[20];
  sprintf(buf, "%d %%", wParam);
  int ret = m_wndStatusBar.SetPaneText(PROGRESS_INDICATOR_INDEX, buf);
  ASSERT( ret );
  return 1; //this means that we've processed the message
}


void CMainFrame::OnStop() 
{  
	m_srv_ctrl.StopServer();
  m_bServerStarted = false;
}

void CMainFrame::OnUpdateStop(CCmdUI* pCmdUI) 
{
	if ( !m_bServerStarted || m_scene.GetSceneUID() <= 0 )
    pCmdUI->Enable(FALSE);		
}

void CMainFrame::OnUpdateRun(CCmdUI* pCmdUI) 
{
	if ( m_bServerStarted || m_scene.GetSceneUID() <= 0 ) 
    pCmdUI->Enable(FALSE);	
}

void CMainFrame::OnUpdateOpenScene(CCmdUI* pCmdUI) 
{
	if ( m_bServerStarted ) 
    pCmdUI->Enable(FALSE);	
}

void CMainFrame::OnUpdateViewOptions(CCmdUI* pCmdUI) 
{
	if ( m_bServerStarted ) 
    pCmdUI->Enable(FALSE);	
}
