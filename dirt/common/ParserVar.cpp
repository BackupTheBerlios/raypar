//*********************************************************
//** ParserVar.h **
// Created By: KIRILL
// On: 1/11/2004 00:35:46
// Comments: implementation of the CVar class.
//  CVar is the type to be used by parser
//
//*********************************************************
// REVISION by ..., on ...
// Comments: ...
//
//*********************************************************

#include "stdafx.h"
#include "ParserVar.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

///////////////////////////////////////////////////////////


void CVar::operator = (const CVar& o)
{
  m_type = o.m_type;
  if ( m_type == Double )
    m_double = o.m_double;
  if ( m_type == Vector )
    m_vector = o.m_vector;
  if ( m_type == String )
    m_string = o.m_string;
}