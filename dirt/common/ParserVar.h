//*********************************************************
//** ParserVar.h **
// Created By: KIRILL
// On: 1/11/2004 00:35:46
// Comments: interface for the CVar class. 
//  CVar is the type to be used by parser
//
//*********************************************************
// REVISION by ..., on ...
// Comments: ...
//
//*********************************************************


#if !defined(AFX_PARSERVAR_H__BC7FF5EC_730C_4B9C_8075_53EBEF4AD09D__INCLUDED_)
#define AFX_PARSERVAR_H__BC7FF5EC_730C_4B9C_8075_53EBEF4AD09D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "vector.h"

class CVar  
{
public:
  CVar()                 : m_type(None) {};                //the type is set to None
  CVar(double d)         : m_type(Double), m_double(d) {};
  CVar(const CVector& v) : m_type(Vector), m_vector(v) {};
  CVar(const CString& s) : m_type(String), m_string(s) {};

  CVar(const CVar& o) { operator = (o); }

  virtual ~CVar() {};

  enum Type { None, Double, Vector, String };

  bool IsDouble(void) { return m_type == Double; }
  bool IsVector(void) { return m_type == Vector; }
  bool IsString(void) { return m_type == String; }

  //be carefull using this
  double   GetDouble() const { ASSERT(m_type==Double); return m_double; };
  const CVector& GetVector() const { ASSERT(m_type==Vector); return m_vector; };
  const CString& GetString() const { ASSERT(m_type==String); return m_string; };

  void SetDouble(double dbl)          { m_type = Double; m_double = dbl; }
  void SetString(LPCSTR str)          { m_type = String; m_string = str; }
  void SetVector(const CVector& vect) { m_type = Vector; m_vector = vect; }

  void operator = (const CVar& o);
  void operator = (double dbl)          { SetDouble( dbl ); }
  void operator = (LPCSTR str)          { SetString( str ); }
  void operator = (const CVector& vect) { SetVector( vect ); }

  operator double() const { return GetDouble(); }
  operator const CVector& () const { return GetVector(); }

protected:
  Type m_type;

  //data
  double  m_double;
  CVector m_vector;
  CString m_string;

  int operator == (const CVar& o); //to avoid using of the default realization
};

#endif // !defined(AFX_PARSERVAR_H__BC7FF5EC_730C_4B9C_8075_53EBEF4AD09D__INCLUDED_)
