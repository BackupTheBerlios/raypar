//*********************************************************
//** ClientDlg.h **
// Created By: KIRILL
// On :10/29/2003 00:18:43
// Comments: header file of ClientDlg
//
//*********************************************************
// REVISION by ..., on ...
// Comments: ...
//*********************************************************

#if !defined(AFX_CLIENTDLG_H__9920DA7B_9F33_4DDF_B847_56E7A2AA7E7A__INCLUDED_)
#define AFX_CLIENTDLG_H__9920DA7B_9F33_4DDF_B847_56E7A2AA7E7A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "COMMON/LogBox.h"
#include "simpletracer.h"
#include "environment.h"
#include "geometry.h"
#include "ClientThread.h"
#include "Options.h"

//Client thread use this message to inform main thread that 
//line was rendered and the client thread finished its work
#define WM_CLIENT_LINE_RENDERED ( WM_USER+23 )

//dialog is not resizable
#define DIALOG_DEFAULT_LEFT 100
#define DIALOG_DEFAULT_TOP 100


//register section for MainFrame
const char mainFrameSection[] = "Options\\DialogWindow";


/////////////////////////////////////////////////////////////////////////////
// CClientDlg dialog

class CEnvironment;
class CCamera;

class CClientDlg : public CDialog
{
// Construction
public:
	CClientDlg(CWnd* pParent = NULL);	// standard constructor

  bool m_bWannaClose;

// Dialog Data
	//{{AFX_DATA(CClientDlg)
	enum { IDD = IDD_CLIENT_DIALOG };
	CButton	m_standalone_check;
	CString	m_edit_addr;
	int		m_edit_port;
	int		m_connect_period;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CClientDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

public:
	void UpdateOnCheckStandAlone();
  CLogBox m_log_box;

// Implementation
protected:
	HICON m_hIcon;
  
  COptions m_options;   //client options
  CWindowSettings m_settings;

  CEnvironment m_scene; //current scene
  CCamera   m_camera;   //carrent camera informarion
  int m_line_to_render; //number of the line which is currently rendered
                        //negative number means thatr no line is rendered
  COLORREF * m_line_data; //image line is stroed here
  bool m_bWorking; //this meeans that we are computing a line or 
                   //waiting for timer in order to reconnect

  CWinThread* m_client_thread; //pointer to the client 
                            //thread which renders image line

  SClientThreadParam m_thread_params;

  BOOL m_b_standalone; //stores value of m_standalone_check.GetCheck()

  //is called when new line was rendered and client thread terminates 
  LRESULT OnLineRendered(WPARAM wParam, LPARAM lParam);

  //does all information exchange with server
  void DoCommunications(void);

  //starts client rendering process which renders line of image
  void StartRenderImageThread(void);

  //Reset all scene data, used when connection was suddenly broken and so on
  void ResetAllSceneData(void);

	void _DoRepeatedCommunications();//does commincations, starts computing process 
                    //or setups timer for reconnection if requiered

	void SetupTimer();   //setups timer to send an event for reconnection

	void SwitchToRelaxedMode(); //switches to relaxed mode when now computing 
                        //and no connections are done. user may change client properties
             
	void SwitchToWorkingMode(); //switches to working mode in which all computing 
                              //and connections are done.

	// Generated message map functions
	//{{AFX_MSG(CClientDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnDestroy();
  afx_msg void OnClose();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonStart();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnCheckStandalone();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLIENTDLG_H__9920DA7B_9F33_4DDF_B847_56E7A2AA7E7A__INCLUDED_)
