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

#include "common/SceneBuilder.h"

class CServerSceneBuilder : public CSceneBuilder
{
public:
	CServerSceneBuilder();
  virtual ~CServerSceneBuilder();
  virtual void AddSphere(const CVector& pos, double radius);
  virtual void AddLight(const CVector& pos, const CVector& color);
  virtual void AddComment(LPCSTR comment); //for debug purpose

  virtual void ParserError( LPCSTR error );
};

#endif //_SERVERSCENEBUILDER_H
