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
#include "CLIENT/environment.h"
#include "ServerSceneBuilder.h"

//We use this windows message and user code  in order to send 
//information to the main thread. 
#define WM_USER_ADD_LOG_MSG  ( WM_USER+11 )
#define USER_ADD_LOG_MSG_CODE  0x23fb

//Client threads use this message to inform main thread that 
//the scene rendering was finished (it is sent once per scene)
#define WM_SERVER_FINISHED_SCENE  ( WM_USER+12 )

//Client threads use this message to inform main thread that 
//new line was received from client and send percent of rendered lines
//in WPARAM.
#define WM_SERVER_LINE_RENDERED ( WM_USER+13 )

#define MAINFRAME_DEFAULT_LEFT 100
#define MAINFRAME_DEFAULT_TOP 100
#define MAINFRAME_DEFAULT_WIDTH 640
#define MAINFRAME_DEFAULT_HEIGHT 480

//register section for MainFrame
const char mainFrameSection[] = "Options\\MainFrame";

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
  CBitmap     m_toolbar_bmp_run;
  CBitmap     m_toolbar_bmp_stop;
  bool        m_bServerStarted;
  COptions    m_serverOptions;

  CWindowSettings m_settings;
  CEnvironment m_scene;
  CServerSceneBuilder m_scene_builder;
  int m_last_scene_uid;
  void* m_bitmap_lines;

  int GetNewSceneUID(void) { return ++m_last_scene_uid; }

// Generated message map functions
protected:
  //this is used to handle WM_USER_ADD_LOG_MESSAGE which can be sent 
  //by ServerLogMessage(.)
	LRESULT OnUserAddLogMessage(WPARAM wParam, LPARAM lParam);
  LRESULT OnServerFinishedScene(WPARAM wParam, LPARAM lParam);
  LRESULT OnServerLineRendered(WPARAM wParam, LPARAM lParam);
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	afx_msg void OnViewLogwindow();
	afx_msg void OnViewOptions();
	afx_msg void OnRun();
	afx_msg void OnOpenScene();
	afx_msg void OnDestroy();
	afx_msg void OnStop();
	afx_msg void OnUpdateStop(CCmdUI* pCmdUI);
	afx_msg void OnUpdateRun(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__B8910706_2A05_45E7_A593_113FBD547D75__INCLUDED_)
