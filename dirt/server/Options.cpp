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
// REVISION by ..., on ...
// Comments: ...
//
//*********************************************************

#include "stdafx.h"
#include "Options.h"

//////////////////////////////////////////////////////////////////////
// COptions

COptions::COptions()
: m_imageWidth( 0 )
, m_imageHeight( 0 )
, m_serverPort( 0 )  
{}

COptions::~COptions()
{}

//
// Registry entry names
//

const char s_options_section[] = "Options";

const char s_image_width[]  = "ImageWidth";
const char s_image_height[] = "ImageHeight";
const char s_server_port[]  = "ServerPort";

//
// Default registry entry values
//

const int def_image_width  = 400;
const int def_image_height = 300;
const int def_server_port  = 8700;


void COptions::SaveDataToReg()
{
  CWinApp* p_app = AfxGetApp();
  ASSERT( p_app );

  ModifyInvalid(); //we check data and modify invalid ones before writing it to registry

  //store data to the registry
  p_app->WriteProfileInt(s_options_section, s_image_width, m_imageWidth );
  p_app->WriteProfileInt(s_options_section, s_image_height, m_imageHeight );
  p_app->WriteProfileInt(s_options_section, s_server_port, m_serverPort );
}

void COptions::GetDataFromReg()
{
  CWinApp* p_app = AfxGetApp();
  ASSERT( p_app );
  
  //store data to the registry
  m_imageWidth  = p_app->GetProfileInt(s_options_section, s_image_width,  def_image_width  );
  m_imageHeight = p_app->GetProfileInt(s_options_section, s_image_height, def_image_height );
  m_serverPort  = p_app->GetProfileInt(s_options_section, s_server_port,  def_server_port  );
  
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
}