//****************************************
//** CLIENT.cpp **
// Created By: KIRILL
// On :10/29/2003 00:18:05
// Comments: Defines the class behaviors for the application.
//
//***********************************
// REVISION by KIRILL, on 11/2/2003 01:11:24
// Comments: I've added implementation of ClientLogMessage()
//
//***********************************
// REVISION by ..., on ...
// Comments: ...
//
//***********************************

#include "stdafx.h"
#include "CLIENT.h"
#include "ClientDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CClientApp

BEGIN_MESSAGE_MAP(CClientApp, CWinApp)
	//{{AFX_MSG_MAP(CClientApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClientApp construction

CClientApp::CClientApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CClientApp object

CClientApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CClientApp initialization

BOOL CClientApp::InitInstance()
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

	CClientDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}


#define MAX_LOG_MESSAGES_COUNT 5 //maximum naumber of log messages
       //if we add more messages the first ones will be deleted

void ClientLogMessage( LPCSTR text, MessageType type ) //we don't use type yet
{
  CWnd* p_wnd = AfxGetMainWnd();
  CClientDlg* p_dlg = (CClientDlg*) p_wnd; //Main window is CClientDlg

  switch ( type ){
    case msgNORMAL: p_dlg->m_log_box.AddMessage( text ); break;
    case msgERROR : p_dlg->m_log_box.AddError( text ); break;
    default: ASSERT( 0 ); //Unknown MessageType!
  }
}
