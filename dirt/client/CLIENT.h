//****************************************
//** CLIENT.h **
// Created By: KIRILL
// On :10/29/2003 00:19:30
// Comments: main header file for the CLIENT application
//
//***********************************
// REVISION by ..., on ...
// Comments: ...
//***********************************


#if !defined(AFX_CLIENT_H__ABFA5D5F_C1EE_480B_8718_4F5ADFDE4D39__INCLUDED_)
#define AFX_CLIENT_H__ABFA5D5F_C1EE_480B_8718_4F5ADFDE4D39__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "../COMMON/msg.h"


/////////////////////////////////////////////////////////////////////////////
// CClientApp:
// See CLIENT.cpp for the implementation of this class
//

class CClientApp : public CWinApp
{
public:
	CClientApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CClientApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CClientApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//This function adds a line to the client log
//Don't use it directly!
//Use Message() and ErrorMessage() instead.
void ClienLogMessage( LPCSTR text, MessageType type );

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLIENT_H__ABFA5D5F_C1EE_480B_8718_4F5ADFDE4D39__INCLUDED_)
