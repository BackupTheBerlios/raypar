//****************************************
//** SERVER.h **
// Created By: KIRILL
// On :10/27/2003 01:50:56
// Comments: main header file for the SERVER application
//
//***********************************
// REVISION by ..., on ...
// Comments: ...
//***********************************


#if !defined(AFX_SERVER_H__35A7AA5E_CD58_4360_A848_A5F8F5197C60__INCLUDED_)
#define AFX_SERVER_H__35A7AA5E_CD58_4360_A848_A5F8F5197C60__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "../COMMON/msg.h"

/////////////////////////////////////////////////////////////////////////////
// CSERVERApp:
// See SERVER.cpp for the implementation of this class
//

class CSERVERApp : public CWinApp
{
public:
	CSERVERApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSERVERApp)
	public:
	virtual BOOL InitInstance();
	virtual void WinHelp(DWORD dwData, UINT nCmd = HELP_CONTEXT);
	//}}AFX_VIRTUAL

// Implementation

public:
	//{{AFX_MSG(CSERVERApp)
	afx_msg void OnAppAbout();	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//This function adds a line to the server log
//Don't use it directly!
//Use Message(), ErrorMessage() & so on instead. Check msg.h for details.
void ServerLogMessage( LPCSTR text, MessageType type );

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERVER_H__35A7AA5E_CD58_4360_A848_A5F8F5197C60__INCLUDED_)
