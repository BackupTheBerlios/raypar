//*********************************************************
//** ClientThread.h **
// Created By: KIRILL
// On: 1/31/2004 08:08:56
// Comments: client worker thread function declaration.
//    client worker thread reners the image line
//
//*********************************************************
// REVISION by ..., on ...
// Comments: ...
//
//*********************************************************

#ifndef _CLIENT_THREAD_H
#define _CLIENT_THREAD_H

#pragma once

#include "simpletracer.h"
#include "environment.h"
#include "geometry.h"

struct SClientThreadParam{  
  CEvent process_params_event; //this an event handle. Client waits for it 
                      //and when the event is signaled the client may read 
                    //enter folowing critical section, read params and do rendering
  CCriticalSection params_cs; //you should enter this critical section before modifing 
              //folowing these params in main thread, and before reading they in client thread

  bool bShouldExit; //if true the client thread terminates, this member can be set to true
                    //by main thread any time
  
  bool bShouldStop; //if true the client thread stops rendering, this member can be 
                    //set to true by main thread any time

  //folowing members may be modified only in 
  //p_params_cs critical section

  COLORREF* data;  //momory to store rendered image line data
  CEnvironment* scene; //current scene
  CCamera* camera; //current camera information
  int line_number; //number of the line to render       
  HWND hwnd_main;  //main window hwnd, used for sending notification messages 
                    //to the main thread
};

CWinThread* StartClientThread( SClientThreadParam * param );


#endif// _CLIENT_THREAD_H