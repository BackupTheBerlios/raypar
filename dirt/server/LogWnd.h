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
//*********************************************************

#if !defined(AFX_LOGWND_H__5E8F16B6_AF1D_4D8F_9171_FD6C2BDC98AE__INCLUDED_)
#define AFX_LOGWND_H__5E8F16B6_AF1D_4D8F_9171_FD6C2BDC98AE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LogWnd.h : header file
//
#include "Options.h" 

#define LOGWND_DEFAULT_LEFT 420
#define LOGWND_DEFAULT_TOP 200
#define LOGWND_DEFAULT_WIDTH 320
#define LOGWND_DEFAULT_HEIGHT 380

//register section for MainFrame
const char logWindowSection[] = "Options\\LogWindow";

/////////////////////////////////////////////////////////////////////////////
// CLogWnd dialog

class CLogWnd : public CDialog
{
// Construction
public:
	CLogWnd(CWnd* pParent = NULL);   // standard constructor

  HWND GetLogBoxHWND(void) const;

// Dialog Data
	//{{AFX_DATA(CLogWnd)
	enum { IDD = IDD_DIALOG_LOG };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLogWnd)
	public:	
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
  CWindowSettings m_settings;
  
	// Generated message map functions
	//{{AFX_MSG(CLogWnd)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg void OnSysCommand( UINT nID, LPARAM lParam );
  afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOGWND_H__5E8F16B6_AF1D_4D8F_9171_FD6C2BDC98AE__INCLUDED_)
