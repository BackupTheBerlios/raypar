//****************************************
//** ClientDlg.h **
// Created By: KIRILL
// On :10/29/2003 00:18:43
// Comments: header file of ClientDlg
//
//***********************************
// REVISION by ..., on ...
// Comments: ...
//***********************************

#if !defined(AFX_CLIENTDLG_H__9920DA7B_9F33_4DDF_B847_56E7A2AA7E7A__INCLUDED_)
#define AFX_CLIENTDLG_H__9920DA7B_9F33_4DDF_B847_56E7A2AA7E7A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../COMMON/LogBox.h"

/////////////////////////////////////////////////////////////////////////////
// CClientDlg dialog

class CClientDlg : public CDialog
{
// Construction
public:
	CClientDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CClientDlg)
	enum { IDD = IDD_CLIENT_DIALOG };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CClientDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

public:
  CLogBox m_log_box;

// Implementation
protected:
	HICON m_hIcon;  

	// Generated message map functions
	//{{AFX_MSG(CClientDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonTest();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLIENTDLG_H__9920DA7B_9F33_4DDF_B847_56E7A2AA7E7A__INCLUDED_)
