//*********************************************************
//** Options.cpp **
// Created By: Vader
// On: 01/27/2004 
// Comments:  implementation of COptions class  (stores server
//  options and synchronizes it with system register)
//*********************************************************
// REVISION by KIRILL, on 1/28/2004 14:40:09
// Comments: Slight modificatins done. ATL calls removed.
//   Destructor / constructor calls removed.
//
//*********************************************************
// REVISION by Vader, on 1/28/2004
// Comments: Added CWindowSettings class for storing window
//   position in the register
//
//*********************************************************
// REVISION by ..., on ...
// Comments: ...
//
//*********************************************************
#include "stdafx.h"
#include "Options.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// COptions

COptions::COptions()
: m_imageWidth( 0 )
, m_imageHeight( 0 )
, m_serverPort( 0 )  
{
}

COptions::~COptions()
{
}

//
// Registry entry names
//

const char s_options_section[] = "Options";

const char s_image_width[]  = "ImageWidth";
const char s_image_height[] = "ImageHeight";
const char s_server_port[]  = "ServerPort";
const char s_do_save[] = "DoSave";
const char s_filename[] = "FileName";

const char s_window_left[] = "WindowLeft";
const char s_window_top[] = "WindowTop";
const char s_window_width[] = "WindowWidth";
const char s_window_height[] = "WindowHeight";

//
// Default registry entry values
//

const int def_image_width  = 400;
const int def_image_height = 300;
const int def_server_port  = 8700;
const BOOL def_do_save = TRUE;
const char def_filename[] = "Result.bmp";

const int def_window_top = 100;
const int def_window_left = 100;
const int def_window_width = 400;
const int def_window_height = 300;


void COptions::SaveDataToReg()
{
  CWinApp* p_app = AfxGetApp();
  ASSERT( p_app );

  ModifyInvalid(); //we check data and modify invalid ones before writing it to registry

  //store data to the registry
  p_app->WriteProfileInt(s_options_section, s_image_width, m_imageWidth );
  p_app->WriteProfileInt(s_options_section, s_image_height, m_imageHeight );
  p_app->WriteProfileInt(s_options_section, s_server_port, m_serverPort );
  p_app->WriteProfileInt(s_options_section, s_do_save, m_b_saveFile );
  p_app->WriteProfileString(s_options_section, s_filename, m_filename);
}

void COptions::GetDataFromReg()
{
  CWinApp* p_app = AfxGetApp();
  ASSERT( p_app );
  
  //get data from the registry
  m_imageWidth  = p_app->GetProfileInt(s_options_section, s_image_width,  def_image_width  );
  m_imageHeight = p_app->GetProfileInt(s_options_section, s_image_height, def_image_height );
  m_serverPort  = p_app->GetProfileInt(s_options_section, s_server_port,  def_server_port  );
  m_b_saveFile = p_app->GetProfileInt(s_options_section, s_do_save,  def_do_save  );
  m_filename = p_app->GetProfileString(s_options_section, s_filename,  def_filename );  
  ModifyInvalid(); //we check data and modify invalid ones which might be read from registry
}

void COptions::ModifyInvalid()
{
  if ( m_imageWidth < 1 || m_imageWidth > MAX_IMAGE_WIDTH )
    m_imageWidth = def_image_width;

  if ( m_imageHeight < 1 || m_imageHeight > MAX_IMAGE_WIDTH )
    m_imageHeight= def_image_height;

  if ( m_serverPort < 1 || m_serverPort > 65535 ) //65535 - max port number
    m_serverPort = def_server_port;

  //!!! here check m_filename !!!
}


CWindowSettings::CWindowSettings()
: m_x (0)
, m_y (0)
, m_cx (0)
, m_cy (0)
, m_lpszSection ("")
{
  m_default_x = def_window_left;  
  m_default_y = def_window_top;  
  m_default_cx = def_window_width; 
  m_default_cy = def_window_height;
}

CWindowSettings::CWindowSettings(LPCTSTR lpszSection, int x, int y, int cx, int cy)
: m_x (0)
, m_y (0)
, m_cx (0)
, m_cy (0)
{
  m_lpszSection = lpszSection;
  m_default_x = x;  
  m_default_y = y;  
  m_default_cx = cx; 
  m_default_cy = cy; 
}


void CWindowSettings::GetDataFromReg()
{
  CWinApp* p_app = AfxGetApp();
  ASSERT( p_app );
  
  //get data from the registry
  m_x  = p_app->GetProfileInt(m_lpszSection, s_window_left,  m_default_x  );
  m_y  = p_app->GetProfileInt(m_lpszSection, s_window_top,  m_default_y  );
  m_cx = p_app->GetProfileInt(m_lpszSection, s_window_width,  m_default_cx  );
  m_cy = p_app->GetProfileInt(m_lpszSection, s_window_height,  m_default_cy  ); 
}


void CWindowSettings::SaveDataToReg()
{
  CWinApp* p_app = AfxGetApp();
  ASSERT( p_app );

  //store data to the registry
  p_app->WriteProfileInt(m_lpszSection, s_window_left, m_x );
  p_app->WriteProfileInt(m_lpszSection, s_window_top, m_y );
  p_app->WriteProfileInt(m_lpszSection, s_window_width, m_cx );
  p_app->WriteProfileInt(m_lpszSection, s_window_height, m_cy );
}

void CWindowSettings::SavePosition(CWnd *hwnd)
{
  WINDOWPLACEMENT wplc;
  hwnd->GetWindowPlacement(&wplc);
  m_x = wplc.rcNormalPosition.left;
  m_y = wplc.rcNormalPosition.top;
  m_cx = wplc.rcNormalPosition.right - wplc.rcNormalPosition.left;
  m_cy = wplc.rcNormalPosition.bottom - wplc.rcNormalPosition.top;
  SaveDataToReg();
}

void CWindowSettings::LoadNonResizablePosition(CWnd *hwnd)
{
  CRect rect;
  hwnd->GetWindowRect(rect);
  
  GetDataFromReg();
  int x,y,cx,cy;
  x = m_x;
  y = m_y;
  cx = rect.right - rect.left;
  cy = rect.bottom - rect.top;

  hwnd->SetWindowPos(&CWnd::wndTop, x, y, cx, cy, 0);
}

void CWindowSettings::LoadResizablePosition(CWnd *hwnd)
{
  CRect rect;
  hwnd->GetWindowRect(rect);
  
  GetDataFromReg();
  hwnd->SetWindowPos(&CWnd::wndTop, m_x, m_y, m_cx, m_cy, 0);
}