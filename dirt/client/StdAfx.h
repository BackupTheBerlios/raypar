//****************************************
//** StdAfx.h **
// Created By: KIRILL
// On :10/29/2003 00:19:46
// Comments: include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//
//***********************************
// REVISION by ..., on ...
// Comments: ...
//***********************************


#if !defined(AFX_STDAFX_H__439958D5_C8F0_4368_934E_66B480186303__INCLUDED_)
#define AFX_STDAFX_H__439958D5_C8F0_4368_934E_66B480186303__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxsock.h>		// MFC socket extensions
#include <afxmt.h>      // MFC synchronization support


//We sometimes need to know whether we are building server or client
#define IS_CLIENT() 1 //we are building clent
#define IS_SERVER() 0 //we are not building server



//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__439958D5_C8F0_4368_934E_66B480186303__INCLUDED_)
