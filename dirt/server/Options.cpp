//*********************************************************
//** Options.cpp **
// Created By: Vader
// On: 01/27/2004 
// Comments:  implementation of COptions class  (stores server
//  options and synchronizes it with system register)
//*********************************************************
// REVISION by ..., on ...
// Comments: ...
//
//*********************************************************

#include "stdafx.h"
#include "Options.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COptions::COptions()
{
  m_imageWidth = 0;
  m_imageHeight = 0;
  m_serverPort = 0;
  int status = GetDataFromReg();
  ASSERT(status);
}

COptions::~COptions()
{
  int status = SaveDataToReg();
  ASSERT(status);
}

int COptions::SaveDataToReg()
{
  CRegKey key;
  char setting[255];
  
  _ltoa(m_imageHeight,setting,10);
  LONG status = key.SetValue(HKEY_CURRENT_USER, REGISTRY_KEY, setting,"image height");
  if(status != ERROR_SUCCESS) return 0;

  _ltoa(m_imageWidth,setting,10);
  status = key.SetValue(HKEY_CURRENT_USER, REGISTRY_KEY, setting,"image width");
  if(status != ERROR_SUCCESS) return 0;

    _ltoa(m_serverPort,setting,10);
  status = key.SetValue(HKEY_CURRENT_USER, REGISTRY_KEY, setting,"server port");
  if(status != ERROR_SUCCESS) return 0;

  return 1;
}

int COptions::GetDataFromReg()
{
  CRegKey key;

  LONG status = key.Open(HKEY_CURRENT_USER,REGISTRY_KEY,KEY_READ);
  if(status != ERROR_SUCCESS) // error opening specified key
  {
    //try to create (may be the key does not exist)
    return SaveDataToReg();
  }
  else
  {
    //get necessary data
    char setting[255];
	  DWORD settingSize=sizeof(setting);
	  
    status = key.QueryValue(setting, "image width", &settingSize);
    if(status != ERROR_SUCCESS) return 0;
    m_imageWidth = atoi(setting);

    settingSize=sizeof(setting);
    status = key.QueryValue(setting, "image height", &settingSize);
    if(status != ERROR_SUCCESS) return 0;
    m_imageHeight = atoi(setting);


    settingSize=sizeof(setting);
    status = key.QueryValue(setting, "server port", &settingSize);
    if(status != ERROR_SUCCESS) return 0;
    m_serverPort = atoi(setting);
  }
  
  return 1;
}