//*********************************************************
//** Options.h **
// Created By: Vader
// On: 01/27/2004 
// Comments:  interface for the COptions class (stores server
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

#if !defined(AFX_OPTIONS_H__EB3E6444_C4E1_4FA7_B347_CCD8F0D0696A__INCLUDED_)
#define AFX_OPTIONS_H__EB3E6444_C4E1_4FA7_B347_CCD8F0D0696A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//maximum allowed height (minimum is 1)
#define MAX_IMAGE_HEIGHT 65535

//maximum allowed width (minimum is 1)
#define MAX_IMAGE_WIDTH  65535

//maximum allowed server port number (minimum is 1)
#define MAX_SERVER_PORT  65535

class COptions  
{
protected:
  int m_imageHeight; //rendered image height
  int m_imageWidth; //rendered image width
  int m_serverPort;  //server port

protected:
  //Checks all values and modifies invalid members to its valid default combinations
  void ModifyInvalid(void);

public:
	COptions();
	virtual ~COptions();

  // SaveDataToReg saves data to HKEY_CURRENT_USER\<Application registry key>
  // section. 
  void SaveDataToReg(void);
  
  // GetDataFromReg loads data from HKEY_CURRENT_USER\<Application registry key>
  // section which is created and initialized with default values if absent.
  void GetDataFromReg(void);

  
  int GetImageHeight(void) const {return m_imageHeight;}
  int GetImageWidth(void) const {return m_imageWidth;}
  int GetServerPort(void) const {return m_serverPort;}
  void SetImageHeight(int imageHeight) {m_imageHeight = imageHeight;}
  void SetImageWidth(int imageWidth) {m_imageWidth = imageWidth;}
  void SetServerPort(int serverPort) {m_serverPort = serverPort;}
};

#endif // !defined(AFX_OPTIONS_H__EB3E6444_C4E1_4FA7_B347_CCD8F0D0696A__INCLUDED_)