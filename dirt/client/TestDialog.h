#if !defined(AFX_TESTDIALOG_H__E2A16E66_F271_4851_AECC_C3DAA0AE09A3__INCLUDED_)
#define AFX_TESTDIALOG_H__E2A16E66_F271_4851_AECC_C3DAA0AE09A3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TestDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTestDialog dialog

class CTestDialog : public CDialog
{
// Construction
public:
	CTestDialog(int width, int height, COLORREF* img);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTestDialog)
	enum { IDD = IDD_TEST_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

  int m_width;
  int m_height;
  COLORREF * m_img;


	// Generated message map functions
	//{{AFX_MSG(CTestDialog)
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTDIALOG_H__E2A16E66_F271_4851_AECC_C3DAA0AE09A3__INCLUDED_)
