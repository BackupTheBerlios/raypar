// utils.h: interface for the CCriticalSectionLock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UTILS_H__C4C6DBA8_0EF4_4E1C_BC8C_417402417660__INCLUDED_)
#define AFX_UTILS_H__C4C6DBA8_0EF4_4E1C_BC8C_417402417660__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

///////////////////////////////////////////////////////////
//  CCriticalSectionLock  
///////////////////////////////////////////////////////////

//  You may use this instead of manual creation of CCriticalSection.
//  (look realisation for the details)
//
class CCriticalSectionLock
{
public:
	CCriticalSectionLock();
	virtual ~CCriticalSectionLock();

protected:
  CCriticalSection m_crit_sect;
};

#endif // !defined(AFX_UTILS_H__C4C6DBA8_0EF4_4E1C_BC8C_417402417660__INCLUDED_)
