//****************************************
//** ChildView.h **
// Created By: KIRILL
// On :10/27/2003 01:49:50
// Comments: interface of the CChildView class
//
//***********************************
// REVISION by ..., on ...
// Comments: ...
//***********************************


#if !defined(AFX_CHILDVIEW_H__44C49FC5_EAE3_4DD0_ABD2_92F4BC6D21F6__INCLUDED_)
#define AFX_CHILDVIEW_H__44C49FC5_EAE3_4DD0_ABD2_92F4BC6D21F6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CChildView window

class CChildView : public CWnd
{
// Construction
public:
	CChildView();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChildView)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CChildView();

  //bitmap_bits may be 0
  void SetBitmapParams(int width, int height, void* bitmap_bits);

	// Generated message map functions
protected:
  int m_bitmap_width;
  int m_bitmap_height;
  void* m_bitmap_bits;

	//{{AFX_MSG(CChildView)
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHILDVIEW_H__44C49FC5_EAE3_4DD0_ABD2_92F4BC6D21F6__INCLUDED_)
