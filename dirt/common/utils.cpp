// utils.cpp: implementation of the CCriticalSectionLock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "utils.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// CCriticalSectionLock
//////////////////////////////////////////////////////////////////////

CCriticalSectionLock::CCriticalSectionLock()
{
  m_crit_sect.Lock(); //lock on creation
}

CCriticalSectionLock::~CCriticalSectionLock()
{
  m_crit_sect.Unlock(); //unlock on destruction
}
