//*********************************************************
//** LogBox.h **
// Created By: KIRILL
// On: 11/2/2003 01:23:48
// Comments: CLogBox class definition. CLogBox is a special kind 
//       of CListBox which allows you to draw beautiful logs
//
//*********************************************************
// REVISION by ..., on ...
// Comments: ...
//
//*********************************************************

#if !defined(AFX_LOGBOX_H__0C5B1003_F4DA_4C72_A3C6_36472B76BD15__INCLUDED_)
#define AFX_LOGBOX_H__0C5B1003_F4DA_4C72_A3C6_36472B76BD15__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CLogBox window


///////////////////////////////////
//Types of messages (values must be > 0! ) 
#define LOG_MSG_NORMAL 1
#define LOG_MSG_ERROR  2




class CLogBox : protected CListBox
{
// Construction
public:
	CLogBox();

// Operations
public:
  //Attaches 'hwnd' to a CLogBox object
  int Attach( HWND hwnd );

  //Detaches CLogBox object from a window
  HWND Detach();


  //This functions are now thread safe, i.e. you cann call
  //them from any thread
  void AddError( LPCSTR text ); //adds error message text
  void AddMessage( LPCSTR text ); //adds normal message text


  int GetCount() { return CListBox::GetCount(); }

  void SetErrBkgColor ( COLORREF color ) { m_err_bkg_color = color; }
  void SetErrTextColor( COLORREF color ) { m_err_text_color = color; }
  void SetMsgBkgColor ( COLORREF color ) { m_msg_bkg_color = color; }
  void SetMsgTextColor( COLORREF color ) { m_msg_text_color = color; }
  void SetFocusFrameColor( COLORREF color ) { m_focus_frame_color = color; }
  void SetMaxMessageCount( int count )   { m_max_message_count = count; }
         
  COLORREF GetErrBkgColor () const { return m_err_bkg_color; }
  COLORREF GetErrTextColor() const { return m_err_text_color; }
  COLORREF GetMsgBkgColor () const { return m_msg_bkg_color; }
  COLORREF GetMsgTextColor() const { return m_msg_text_color; }
  COLORREF GetFocusFrameColor() const { return m_focus_frame_color; }
  int      GetMaxMessageCount() const { return m_max_message_count; }

protected:
  COLORREF m_err_bkg_color;
  COLORREF m_err_text_color;
  COLORREF m_msg_bkg_color;
  COLORREF m_msg_text_color;
  COLORREF m_focus_frame_color;
  
  int m_max_message_count;

  void _AddLine( LPCSTR text, int msg_type );//Adds line 'text' and sets up msg_type.
                             //Removes excess lines in the beginnig of the log list.


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLogBox)
	public:
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
  afx_msg LRESULT OnUserAddLogMessage(WPARAM wParam, LPARAM lParam);    
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CLogBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CLogBox)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOGBOX_H__0C5B1003_F4DA_4C72_A3C6_36472B76BD15__INCLUDED_)
