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
// REVISION by Vader, on 1/28/2004
// Comments: Added CWindowSettings class for storing window
//   position in the register
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
  BOOL m_b_saveFile;
  CString m_filename;

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
  BOOL DoSaveFile(void) const {return m_b_saveFile;}
  CString GetFileName(void) const {return m_filename;}
  void SetImageHeight(int imageHeight) {m_imageHeight = imageHeight;}
  void SetImageWidth(int imageWidth) {m_imageWidth = imageWidth;}
  void SetServerPort(int serverPort) {m_serverPort = serverPort;}
  void SetSaveFile(BOOL b_saveFile){m_b_saveFile = b_saveFile;}
  void SetFilename(CString filename){m_filename = filename;}
};

class CWindowSettings
{
protected:
  int m_x;  //left
  int m_y;  //top
  int m_cx; //width
  int m_cy; //heigth

  int m_default_x;  //default left
  int m_default_y;  //default top
  int m_default_cx; //default width
  int m_default_cy; //default heigth

  LPCTSTR m_lpszSection; //register section to synchronize to



public:
  CWindowSettings();
  CWindowSettings(LPCTSTR lpszSection, int x, int y, int cx, int cy);  //default values

  void SetSection(LPCTSTR lpszSection) {m_lpszSection = lpszSection;}

  int GetX(void) const {return m_x;}
  int GetY(void) const {return m_y;}
  int GetCx(void) const {return m_cx;}
  int GetCy(void) const {return m_cy;}
  void SetX(int x) {m_x = x;}
  void SetY(int y) {m_y = y;}
  void SetCx(int cx) {m_cx = cx;}
  void SetCy(int cy) {m_cy = cy;}

  void SaveDataToReg(void);
  void GetDataFromReg(void);
  void SavePosition(CWnd *hwnd);
  void LoadResizablePosition(CWnd *hwnd); //for resizable windows
  void LoadNonResizablePosition(CWnd *hwnd);//for non resizable windows
};

#endif // !defined(AFX_OPTIONS_H__EB3E6444_C4E1_4FA7_B347_CCD8F0D0696A__INCLUDED_)
