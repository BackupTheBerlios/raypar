//****************************************
//** msg.cpp **
// Created By: KIRILL
// On :11/1/2003 23:33:03
// Comments: Implementation of message functions
//
//***********************************
// REVISION by ..., on ...
// Comments: ...
//
//***********************************


#include "stdafx.h"
#include "msg.h"
#include "utils.h"

#include <stdio.h>
#include <stdarg.h>

#define MAX_MESSAGE_LEN 1024 

#if IS_SERVER()
//this function adds the message to the server log
//must be implemented somewhere in server
extern void ServerLogMessage( LPCSTR msg, MessageType type ); 
#endif//IS_SERVER

#if IS_CLIENT()
//this function adds the message to the client log
//must be implemented somewhere in client
extern void ClientLogMessage( LPCSTR msg, MessageType type ); 
#endif//IS_SERVER

//First this builds message text using the format string and 
//parameters as sprintf does. Afer that:
//If you build the server this will send 'msg' to server log
//If you build the client this will send 'msg' to client log
static void _LogMessage( MessageType type, LPCSTR format, va_list args )
{
  CCriticalSectionLock cs_lock; // buffer is static!

  ASSERT( type >= msgNORMAL && type < msgLAST );

  static char msg_buf[MAX_MESSAGE_LEN];

  int ret = _vsnprintf(msg_buf, MAX_MESSAGE_LEN, format, args );

  if ( ret<0 ) 
    msg_buf[ MAX_MESSAGE_LEN-1 ] = 0; // ret<0 means that
       //the length of the formated message is more than MAX_MESSAGE_LEN
       //We must set LAST char of msg_buf to 0 manually 
       //to define the end of the string
  
  #if IS_SERVER()
  ServerLogMessage( msg_buf, type );
  #endif//IS_SERVER

  #if IS_CLIENT()
  ClientLogMessage( msg_buf, type );
  #endif//IS_CLIENT
}

//Adds an error message to log
//
//LPCSTR text: text of the message including formating
// ... : Additional information arguments 
//
//eg: ErrorMessage( "TCP/IP error - %d", 134);
void ErrorMessage( LPCSTR text, ... )
{
  CCriticalSectionLock cs_lock;

  va_list args;
  va_start( args, text );  // initialize variable arguments
  
  _LogMessage( msgERROR, text, args );

  va_end( args );  //we have to clean up 'args'
}


//Adds a normal message to log
//
//LPCSTR text: text of the message including formating
// ... : Additional information arguments 
//
//eg: Message( "Frame number - %d, name = '%s'", 27, "my_frame");
void Message( LPCSTR text, ... )
{
  va_list args;
  va_start( args, text );  // initialize variable arguments
  
  _LogMessage( msgNORMAL, text, args );

  va_end( args );  //we have to clean up 'args'
}


//  Extracts the error information from Exception class
//  and adds the error message to log
//  If bShowMsgBox = TRUE displays message box with error message
//
void ErrorMessageFromException( CException* ex, BOOL bShowMsgBox /* = FALSE */ )
{
  CCriticalSectionLock cs_lock; // buffer is static!
  
  ASSERT( ex );
  static char buf[MAX_MESSAGE_LEN];
  ex->GetErrorMessage(buf, MAX_MESSAGE_LEN);
  ErrorMessage( "%s", buf );  //not simply ErrorMessage( buf ) to prevent formating problems 
  if (bShowMsgBox)
    AfxMessageBox(buf, MB_OK | MB_ICONSTOP);
}


//  Adds error_text to the log
//  and displays message box with the error text
void ErrorMessageWithBox(LPCSTR error_text)
{
  ErrorMessage("%s", error_text);  //not simply ErrorMessage( error_text ) to prevent formating problems 
  AfxMessageBox(error_text, MB_OK | MB_ICONSTOP);
}



//  Adds error messagge to log provided error code.
//  Describes the error for some "most popular" error codes
//
CString GetErrorMessageByErrorCode(DWORD errorCode)
{
  CString err_msg;
  LPSTR buf = err_msg.GetBufferSetLength(MAX_MESSAGE_LEN);
   
  FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM |  FORMAT_MESSAGE_IGNORE_INSERTS,
    0, errorCode, 0,  buf, MAX_MESSAGE_LEN, 0 );

  err_msg.ReleaseBuffer();
  
  err_msg.TrimRight("\n\r\t "); //trim new line chars, which FormatMessage adds

  return err_msg;
}
