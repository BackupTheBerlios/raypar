// ServerSceneBuilder.cpp: implementation of the CServerSceneBuilder class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "server.h"
#include "ServerSceneBuilder.h"
#include "common/vector.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CSceneBuilder* glb_scene_builder = new CServerSceneBuilder(); //let it live here for a  while

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CServerSceneBuilder::CServerSceneBuilder()
{
}

CServerSceneBuilder::~CServerSceneBuilder()
{
}

void CServerSceneBuilder::AddSphere(const CVector& pos, double radius)
{
  Message("[PARSER]  Sphere pos = ( %.3f, %.3f, %.3f), radius = %.3f", 
        pos.x, pos.y, pos.z, radius );
}

void CServerSceneBuilder::AddLight(const CVector& pos, const CVector& color)
{
  Message("[PARSER]  Light pos = ( %.3f, %.3f, %.3f), color = ( %.3f, %.3f, %.3f)", 
        pos.x, pos.y, pos.z, color.x, color.y, color.z );
}


void CServerSceneBuilder::AddComment(LPCSTR comment)
{
  Message("[PARSER]  Comment =  '%s'", comment );
}

void CServerSceneBuilder::ParserError( LPCSTR error )
{
  ErrorMessage("[PARSER]  ERROR =  '%s'", error );
}
