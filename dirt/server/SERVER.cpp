//*********************************************************
//** SERVER.cpp **
// Created By: KIRILL
// On :10/27/2003 01:50:43
// Comments: Defines the class behaviors for the application.
//
//*********************************************************
// REVISION by KIRILL, on 11/2/2003 01:01:39
// Comments: I've added the description of ServerLogMessage
//           Look at msg.h for details
//*********************************************************
// REVISION by KIRILL, on 1/17/2004 04:55:31
// Comments: OnUserAddLogMessage added and ServerLogMessage 
//    modified in order to support multithreading
//
//*********************************************************
// REVISION by ..., on ...
// Comments: ...
//*********************************************************


#include "stdafx.h"
#include "SERVER.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CMainFrame* p_mainFrame = 0; //this is used for logs

/////////////////////////////////////////////////////////////////////////////
// CSERVERApp

BEGIN_MESSAGE_MAP(CSERVERApp, CWinApp)
	//{{AFX_MSG_MAP(CSERVERApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSERVERApp construction

CSERVERApp::CSERVERApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CSERVERApp object

CSERVERApp theApp;


/////////////////////////////////////////////////////////////////////////////
// CSERVERApp initialization

BOOL CSERVERApp::InitInstance()
{
	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("DIRT software"));


	// To create the main window, this code creates a new frame window
	// object and then sets it as the application's main window object.

  CMainFrame* pFrame = new CMainFrame;
	m_pMainWnd = pFrame;
  p_mainFrame = pFrame;

	// create and load the frame with its resources

	pFrame->LoadFrame(IDR_MAINFRAME,
		WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL,
		NULL);




	// The one and only window has been initialized, so show and update it.
	pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CSERVERApp message handlers





/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CSERVERApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CSERVERApp message handlers


void ServerLogMessage( LPCSTR text, MessageType msg_type )
{  
  ASSERT( AfxIsValidString(text) );
  ASSERT( msg_type== LOG_MSG_NORMAL || msg_type == LOG_MSG_ERROR ); 

//  int slen = strlen(text); 
//  char* msg_copy = new char[slen+1]; //this memory must be freed by the message receiver
//  strncpy(msg_copy, text, slen+1); //carefully copy text
//  msg_copy[slen+1]=0;

  CWinApp* theApp = AfxGetApp(); //theApp is the main thread of the application
  
  int ret = ::PostMessage( theApp->m_pMainWnd->m_hWnd, WM_USER_ADD_LOG_MSG
                 , MAKEWPARAM( USER_ADD_LOG_MSG_CODE, msg_type )
                   , (LPARAM) text );
}

void CSERVERApp::WinHelp(DWORD dwData, UINT nCmd) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CWinApp::WinHelp(dwData, nCmd);
}
