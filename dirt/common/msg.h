//****************************************
//** msg.h **
// Created By: KIRILL
// On :11/1/2003 23:33:41
// Comments: This file defines message routines
//   Please use this functions to add messages to log
//
//***********************************
// REVISION by ..., on ...
// Comments: ...
//
//***********************************

#ifndef _MSG_H
#define _MSG_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif


//Message types definitions
enum MessageType {
  msgNORMAL = 1,
  msgERROR,   
    //if you add a new message please add it here - before msgLast
  msgLAST //this message is never used directly. It is used for 
          //the range checking of MessageType:  
          //eg: ASSERT( msg_type >= msgNormal && msg_type < msgLAST )
};


//Adds an error message to (server|client) log depending on
//what you are building
//Allows additional parameters as sprintf() does
void ErrorMessage( LPCSTR text, ... );

//Adds a message to (server|client) log depending on
//what you are building
//Allows additional parameters as sprintf() does
void Message( LPCSTR text, ... );


#endif  _MSG_H