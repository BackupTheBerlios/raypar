//*********************************************************
//** Options.h **
// Created By: Vader
// On: 02/01/2004 
// Comments:  interface for the COptions class (stores server
//   options and synchronizes it with system register)
//   Added CWindowSettings class for storing window
//   position in the register
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

//maximum allowed connect_period (minimum is 1)
#define MAX_CONNECT_PERIOD  65535

//maximum allowed server port number (minimum is 1)
#define MAX_SERVER_PORT  65535

class COptions  
{
protected:
  CString m_serverAddress;
  int m_connectPeriod; 
  int m_serverPort;  //server port
  BOOL m_mode;

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

  
  CString GetServerAddress(void) const {return m_serverAddress;}
  int GetConnectPeriod(void) const {return m_connectPeriod;}
  int GetServerPort(void) const {return m_serverPort;}
  BOOL GetMode(void) const {return m_mode;}
  void SetServerAddress(CString serverAddress) {m_serverAddress = serverAddress;}
  void SetConnectPeriod(int connectPeriod) {m_connectPeriod = connectPeriod;}
  void SetServerPort(int serverPort) {m_serverPort = serverPort;}
  void SetMode(BOOL mode) {m_mode = mode;}
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
};

#endif // !defined(AFX_OPTIONS_H__EB3E6444_C4E1_4FA7_B347_CCD8F0D0696A__INCLUDED_)
