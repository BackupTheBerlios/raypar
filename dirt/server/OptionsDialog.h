//*********************************************************
//** OptionsDialog.h **
// Created By: Vader
// On: 01/27/2004 
// Comments:  interface for server options dialog 'COptionsDialog'
//*********************************************************
// REVISION by ..., on ...
// Comments: ...
//
//*********************************************************

#if !defined(AFX_OPTIONSDIALOG_H__F5C92077_1FAA_4E79_86CE_84B2F2C72D3B__INCLUDED_)
#define AFX_OPTIONSDIALOG_H__F5C92077_1FAA_4E79_86CE_84B2F2C72D3B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionsDialog.h : header file
//
#include "Options.h"

/////////////////////////////////////////////////////////////////////////////
// COptionsDialog dialog

class COptionsDialog : public CDialog
{
// Construction
public:
	COptionsDialog(CWnd* pParent = NULL);   // standard constructor
  COptionsDialog(CWnd* pParent, COptions* serverOptions);   // standard constructor

// Dialog Data
	//{{AFX_DATA(COptionsDialog)
	enum { IDD = IDD_DIALOG_OPTIONS };
	int		m_width;
	int		m_height;
	int		m_port;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COptionsDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
  COptions *m_serverOptions;
	// Generated message map functions
	//{{AFX_MSG(COptionsDialog)
	virtual void OnOK();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONSDIALOG_H__F5C92077_1FAA_4E79_86CE_84B2F2C72D3B__INCLUDED_)
