//****************************************
//** Vector.cpp **
// Created By: KIRILL
// On :11/4/2003 20:10:01
// Comments: Describes some debug routines. Let this file live.
//
//***********************************
// REVISION by KIRILL, on 11/8/2003 20:46:01
// Comments: unnecessary include of "CLIENT.H" removed
//
//***********************************
// REVISION by ..., on ...
// Comments: ...
//
//***********************************


#include "stdafx.h"
#include "Vector.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


#ifdef _DEBUG

CDumpContext& operator << (CDumpContext& dc, const CVector& v)
{
  dc << "(" << v.x << ", " << v.y <<", " << v.z <<")";
  return dc;
}

void CVector::Dump(CDumpContext& dc /*= afxDump*/)
{
  operator << (dc, *this);
}
#endif//_DEBUG