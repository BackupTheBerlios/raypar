#error NOT USED       //KIRILL 

//*********************************************************
//** SceneBuilder.h **
// Created By: KIRILL
// On: 1/18/2004 17:30:04
// Comments: interface for the CSceneBuilder class.
//  This is an abstract class which supports scene loading
//  You have to use its server or client realization
//
//*********************************************************
// REVISION by KIRILL, on 1/29/2004 18:40:31
// Comments: NOT USED
//
//*********************************************************
// REVISION by ..., on ...
// Comments: ...
//
//*********************************************************


#ifndef _SCENEBUILDER_H
#define _SCENEBUILDER_H

#pragma once

class CVector;

class CSceneBuilder  
{
public:
	virtual void AddSphere(const CVector& pos, double radius) = 0;
  virtual void AddLight(const CVector& pos, const CVector& color) = 0;

  virtual void AddComment(LPCSTR comment) = 0; //for debug purpose

  virtual void ParserError( LPCSTR error ) = 0;
};

#endif //_SCENEBUILDER_H
