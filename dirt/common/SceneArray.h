//*********************************************************
//** SceneArray.h **
// Created By: KIRILL
// On: 1/25/2004 18:17:28
// Comments: The interface for CSolidArray and CLightArray 
//   classes which store Solids and Lights. 
//
//*********************************************************
// REVISION by ..., on ...
// Comments: ...
//
//*********************************************************


#ifndef _SCENEARRAY_H
#define _SCENEARRAY_H

class CArchive;
class CSolid;
class CLight;

///////////////////////////////////////////////////////////
// CSolidArray     - storage class for all geometrical objects (based on CArray collection)

class CSolidArray : protected CPtrArray{
public:
  CSolid* GetAt( int nIndex ) const    { return (CSolid*) CPtrArray::GetAt(nIndex); }
  CSolid* operator[](int nIndex) const { return GetAt(nIndex); }
  int     Add( CSolid* solid )         { return CPtrArray::Add( solid ); }
  int     GetSize() const              { return CPtrArray::GetSize(); }
  void    Empty(void);
  int     IsValid() const;

  int write(CArchive& ar) const;
  int read (CArchive& ar);
};

///////////////////////////////////////////////////////////
// CLightArray     - storage class for all lights (based on CArray collection)

class CLightArray : protected CPtrArray{
public:
  CLight* GetAt( int nIndex ) const    { return (CLight*) CPtrArray::GetAt(nIndex); }
  CLight* operator[](int nIndex) const { return GetAt(nIndex); }
  int     Add( CLight* light )         { return CPtrArray::Add( light ); }
  int     GetSize() const              { return CPtrArray::GetSize(); }
  void    Empty(void);
  int     IsValid() const;

  int write(CArchive& ar) const;
  int read (CArchive& ar);  
};


#endif //_SCENEARRAY_H