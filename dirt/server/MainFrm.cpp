//****************************************
//** MainFrm.cpp **
// Created By: KIRILL
// On :10/27/2003 01:45:08
// Comments: implementation of the CMainFrame class
//
//****************************************
// REVISION by: ..., on: ...
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
	ON_COMMAND(ID_OPEN_CAMERA, OnOpenCamera)
	ON_WM_DESTROY()
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
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	
}

CMainFrame::~CMainFrame()
{
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

  if ( !m_log_wnd.Create(IDD_DIALOG_LOG, 0) ){
		TRACE0("Failed to create log window\n");
		return -1;      // fail to create
  }

  m_log_box.Attach( m_log_wnd.GetDlgItem( IDC_LOG_LIST )->m_hWnd );
	  
  m_log_wnd.ShowWindow( SW_NORMAL );

  //KIRILL: Dummy caption generator
  SetWindowText( "SERVER [FPS = 3] [ 4 clients ]" );

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
	ErrorMessageWithBox("There is no realization yet");
  
}

void CMainFrame::OnRun() 
{
  // TODO: Add your command handler code here
	//ErrorMessageWithBox("There is no realization yet");

  

}

void CMainFrame::OnOpenScene() 
{
  // TODO: Add your command handler code here
  ErrorMessageWithBox("There is no realization yet");

}

void CMainFrame::OnOpenCamera() 
{
  ErrorMessageWithBox("There is no realization yet");
  // TODO: Add your command handler code here

}

void CMainFrame::OnDestroy() 
{
	CFrameWnd::OnDestroy();
  m_log_box.Detach();	
	
}
