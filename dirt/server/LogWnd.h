#if !defined(AFX_LOGWND_H__5E8F16B6_AF1D_4D8F_9171_FD6C2BDC98AE__INCLUDED_)
#define AFX_LOGWND_H__5E8F16B6_AF1D_4D8F_9171_FD6C2BDC98AE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LogWnd.h : header file
//

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

	// Generated message map functions
	//{{AFX_MSG(CLogWnd)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg void OnSysCommand( UINT nID, LPARAM lParam );
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOGWND_H__5E8F16B6_AF1D_4D8F_9171_FD6C2BDC98AE__INCLUDED_)
