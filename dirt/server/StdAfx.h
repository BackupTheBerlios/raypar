//*************************************
//** StdAfx.h **
// Created By: KIRILL
// On :10/27/2003 01:52:10
// Comments: stdafx.h : include file for standard system include files,
//    or project specific include files that are used frequently, but
//    are changed infrequently
//
//***********************************
// REVISION by ..., on ...
// Comments: ...
//***********************************



// 

#if !defined(AFX_STDAFX_H__ECBA9DE3_0DD1_4C5B_9A0F_70DB24CE0089__INCLUDED_)
#define AFX_STDAFX_H__ECBA9DE3_0DD1_4C5B_9A0F_70DB24CE0089__INCLUDED_

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
#define IS_SERVER() 1  //we are building server
#define IS_CLIENT() 0  //we are not building client


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__ECBA9DE3_0DD1_4C5B_9A0F_70DB24CE0089__INCLUDED_)
