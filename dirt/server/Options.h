//*********************************************************
//** Options.h **
// Created By: Vader
// On: 01/27/2004 
// Comments:  interface for the COptions class (stores server
//  options and synchronizes it with system register)
//*********************************************************
// REVISION by ..., on ...
// Comments: ...
//
//*********************************************************

#if !defined(AFX_OPTIONS_H__EB3E6444_C4E1_4FA7_B347_CCD8F0D0696A__INCLUDED_)
#define AFX_OPTIONS_H__EB3E6444_C4E1_4FA7_B347_CCD8F0D0696A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "atlbase.h"
#define REGISTRY_KEY "Software\\DIRT server"


class COptions  
{
protected:
  int m_imageHeight, m_imageWidth; //rendered image params
  int m_serverPort;  //server port

  // SaveDataToReg saves data to HKEY_CURRENT_USER\REGISTRY_KEY section
  // which is created if absent. Runs only once during destruction
  int SaveDataToReg(); // 0 - error, 1 - success
  
  // GetDataFromReg loads data from HKEY_CURRENT_USER\REGISTRY_KEY section
  // which is created and initialized with zeros if absent.
  // Runs only once during construction.  
  int GetDataFromReg();// 0 - error, 1 - success

public:
	COptions();
	virtual ~COptions();
  int GetImageHeight(void) {return m_imageHeight;}
  int GetImageWidth(void) {return m_imageWidth;}
  int GetServerPort(void) {return m_serverPort;}
  void SetImageHeight(int imageHeight) {m_imageHeight = imageHeight;}
  void SetImageWidth(int imageWidth) {m_imageWidth = imageWidth;}
  void SetServerPort(int serverPort) {m_serverPort = serverPort;}
};

#endif // !defined(AFX_OPTIONS_H__EB3E6444_C4E1_4FA7_B347_CCD8F0D0696A__INCLUDED_)
