// LogWnd.cpp : implementation file
//

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
{
	//{{AFX_DATA_INIT(CLogWnd)
	//}}AFX_DATA_INIT
  m_settings = new CWindowSettings(logWindowSection
                             , LOGWND_DEFAULT_LEFT, LOGWND_DEFAULT_TOP
                             , LOGWND_DEFAULT_WIDTH, LOGWND_DEFAULT_HEIGHT);
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
   
  m_settings->GetDataFromReg();
  int x,y,cx,cy;
  x = m_settings->GetX();
  y = m_settings->GetY();
  cx = m_settings->GetCx();
  cy = m_settings->GetCy();

  SetWindowPos(&CWnd::wndTop, x, y, cx, cy, 0);
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
  CRect rect;
  GetWindowRect(rect);
  
  m_settings->SetX(rect.left);
  m_settings->SetY(rect.top);
  m_settings->SetCx(rect.right - rect.left);
  m_settings->SetCy(rect.bottom - rect.top);
  
  m_settings->SaveDataToReg();
	CDialog::OnDestroy();	
}
