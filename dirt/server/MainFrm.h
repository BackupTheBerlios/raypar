//*********************************************************
//** MainFrm.h **
// Created By: KIRILL
// On :10/27/2003 01:48:23
// Comments: interface of the CMainFrame class
//
//*********************************************************
// REVISION by KIRILL, on 1/17/2004 05:20:28
// Comments: OnUserAddLogMessage was added in order to ssupport 
// multithreaded logging.
// Be carefull: we use (WM_USER+1) message
//
//*********************************************************
// REVISION by ..., on ...
// Comments: ...
//*********************************************************


#if !defined(AFX_MAINFRM_H__B8910706_2A05_45E7_A593_113FBD547D75__INCLUDED_)
#define AFX_MAINFRM_H__B8910706_2A05_45E7_A593_113FBD547D75__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ChildView.h"
#include "LogWnd.h"
#include "COMMON/LogBox.h"
#include "ServerThread.h"
#include "Options.h"
#include "OptionsDialog.h"

//We use this windows message and user code  in order to send 
//information to the main thread. 
#define WM_USER_ADD_LOG_MSG  ( WM_USER+1 )
#define USER_ADD_LOG_MSG_CODE  0x23fb


class CMainFrame : public CFrameWnd
{
	
public:
	CMainFrame();
protected: 
	DECLARE_DYNAMIC(CMainFrame)

// Attributes
public:
  CLogBox     m_log_box;
  CServerControl m_srv_ctrl;
  COptions m_serverOptions;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;
	CChildView  m_wndView;
  CLogWnd     m_log_wnd;

// Generated message map functions
protected:
  //this is used to handle WM_USER_ADD_LOG_MESSAGE which can be sent 
  //by ServerLogMessage(.)
	LRESULT OnUserAddLogMessage(WPARAM wParam, LPARAM lParam);
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	afx_msg void OnViewLogwindow();
	afx_msg void OnViewOptions();
	afx_msg void OnRun();
	afx_msg void OnOpenScene();
	afx_msg void OnOpenCamera();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__B8910706_2A05_45E7_A593_113FBD547D75__INCLUDED_)
