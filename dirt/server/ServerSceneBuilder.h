//*********************************************************
//** ServerSceneBuilder.h **
// Created By: KIRILL
// On: 1/18/2004 17:28:30
// Comments: interface for the CServerSceneBuilder class.
//  This class supports scene loading on the server side
//
//*********************************************************
// REVISION by ..., on ...
// Comments: ...
//
//*********************************************************

#ifndef _SERVERSCENEBUILDER_H
#define _SERVERSCENEBUILDER_H

#pragma once

//this return code means that the object has invalid parameters (eg. - negative color)
#define ERROR_INVALID_PARAMS 1 

class CEnvironment;
class CVector;
class CSolid;

class CServerSceneBuilder
{
public:
	CServerSceneBuilder(CEnvironment& scene);
  virtual ~CServerSceneBuilder();

  void Init(void);

  //zero if successfull
  int AddSphere(const CVector& pos, double radius, const CVector& color);

  //zero if successfull
  int  AddLight(const CVector& pos, const CVector& color);
  
  //zero if successfull
  int SetAmbientColor( const CVector& color );
  
  void AddComment(LPCSTR comment); //for debug purpose

  virtual void ParserError( LPCSTR error );

  void SetErrorFlag(void)   { m_bErrorOccured = true;  }  
  void ResetErrorFlag(void) { m_bErrorOccured = false; }
  bool GetErrorFlag(void)   { return m_bErrorOccured;  }
  

  void IncreaseLineNumber(void)  { m_line_number++; }
  int  GetCurrentLineNumber(void) const { return m_line_number; }
  void ResetLineNumber() { m_line_number = 1; }

protected:
  //Checks p_solid->IsValid and adds valid object to the scene and returns zero
  //or destroys invalid object and returns nonzero
  int _AddSolid(CSolid* p_solid);

  CEnvironment& m_scene;
  bool m_bErrorOccured;
  int m_line_number;
};

#endif //_SERVERSCENEBUILDER_H
