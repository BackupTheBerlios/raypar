//*********************************************************
//** LogWnd.h **
// Created By: KIRILL
// On: 1/10/2004 15:42:01
// Comments: CLogWnd interaface - this is the window in
//   which logs are displayed
//
//*********************************************************
// REVISION by ..., on ...
// Comments: ...
//
//***************************

#include "stdafx.h"
#include "server.h"
#include "LogWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLogWnd dialog


CLogWnd::CLogWnd(CWnd* pParent /*=NULL*/)
: CDialog(CLogWnd::IDD, pParent)
,m_settings (logWindowSection, LOGWND_DEFAULT_LEFT, LOGWND_DEFAULT_TOP
                             , LOGWND_DEFAULT_WIDTH, LOGWND_DEFAULT_HEIGHT)
{
	//{{AFX_DATA_INIT(CLogWnd)
	//}}AFX_DATA_INIT
  
}


void CLogWnd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLogWnd)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLogWnd, CDialog)
  //{{AFX_MSG_MAP(CLogWnd)
  ON_WM_SIZE()
  ON_WM_CREATE()
  ON_WM_DESTROY()
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLogWnd message handlers


void CLogWnd::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);

  CWnd* p_log_list = GetDlgItem( IDC_LOG_LIST );
  if ( ::IsWindow(*p_log_list) )
    p_log_list->MoveWindow(0,0, cx, cy);
}


int CLogWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
  
  //load position from reg
  m_settings.LoadResizablePosition(this);

  return 0;
}

void CLogWnd::OnSysCommand( UINT nID, LPARAM lParam )
{
  if (nID == SC_CLOSE )
    ShowWindow( SW_HIDE );
  else
    CDialog::OnSysCommand(nID, lParam);
}


void CLogWnd::OnDestroy() 
{
  m_settings.SavePosition(this);
	CDialog::OnDestroy();	
}
