#ifndef _CLIENT_THREAD_H
#define _CLIENT_THREAD_H

#pragma once

#include "simpletracer.h"
#include "environment.h"
#include "geometry.h"

struct SClientThreadParam{  
  COLORREF* data;
  CEnvironment* scene;
  CCamera* camera;
  int line_number;
  HWND hwnd_main;
};

CWinThread* StartClientThread( SClientThreadParam * param );


#endif// _CLIENT_THREAD_H