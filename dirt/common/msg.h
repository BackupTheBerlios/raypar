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

//Message types definitions
enum MessageType {
  msgNORMAL = 1,
  msgERROR,   
    //if you add a new message please add it here - before msgLast
  msgLAST //this message is never used directly. It is used for 
          //the range checking of MessageType:  
          //eg: ASSERT( msg_type >= msgNormal && msg_type < msgLAST )
};


//Adds an error message to log  (server or client - the program you build & execute )
//Allows additional parameters as sprintf() does
void ErrorMessage( LPCSTR text, ... );


//Adds a message to log  (server or client - the program you build & execute )
//Allows additional parameters as sprintf() does
void Message( LPCSTR text, ... );


//  Extracts the error information from Exception class
//  and adds the error message to log.
//  If bShowMsgBox = TRUE displays message box with error message
void ErrorMessageFromException( CException* ex, BOOL bShowMsgBox  = FALSE );


//  Adds error_text to the log
//  and displays message box with the error text
void ErrorMessageWithBox(LPCSTR error_text);


//  Builds error message for the provided error code.
CString GetErrorMessageByErrorCode(DWORD errorCode);



#endif  _MSG_H