//*********************************************************
//** SceneArray.cpp **
// Created By: KIRILL
// On: 1/25/2004 18:18:44
// Comments:  The interface for CSolidArray and CLightArray 
//   classes which store Solids and Lights. 
//
//*********************************************************
// REVISION by ..., on ...
// Comments: ...
//
//*********************************************************

#include "stdafx.h"
#include "SceneArray.h"
#include "client/environment.h" 

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//Such return means that an error occured in function
#define STORING_ERROR_RETURN 1
#define LOADING_ERROR_RETURN 2

///////////////////////////////////////////////////////////
// CSolidArray     - storage class for all geometrical objects 
///////////////////////////////////////////////////////////

int CSolidArray::write(CArchive& ar) const
{
  ASSERT( ar.IsStoring() );
  ASSERT( IsValid() );

  ar << GetSize();

  for(int i=0; i<GetSize(); i++ ){  
    CSolid* p_solid = GetAt(i);
    ASSERT( p_solid->IsValid() );
    int ret = p_solid->write( ar );

    ASSERT( !ret );
    if( ret )
      return STORING_ERROR_RETURN; //error occured    
  }
  
  return 0;
}

int CSolidArray::read(CArchive& ar)
{
  ASSERT( ar.IsLoading() );
  Empty();

  int num;
  ar >> num;

  ASSERT( num >= 0 );
  if ( num < 0 )
    return LOADING_ERROR_RETURN;

  for(int i=0; i<num; i++){
    CSolid* p_solid = CSolid::readObject( ar );
    
    if( !p_solid || !p_solid->IsValid() ){
      ASSERT( 0 );     
      delete p_solid;
        //note - we don't destroy the objects we've already noramally loaded
      return LOADING_ERROR_RETURN; // error - invalid data arrived
                    
    }
    Add( p_solid );
  }

  if ( !IsValid() ){
    ASSERT( 0 );
    return LOADING_ERROR_RETURN;
  }

  return 0;
}

void CSolidArray::Empty(void)
{
  int solid_count = GetSize();
  for(int i=0; i<solid_count; i++)
    delete GetAt(i); //free objects memory
  RemoveAll(); //free array memery
}

int CSolidArray::IsValid() const
{
  AssertValid(); //internal array debug check

  int light_count = GetSize();
  for(int i=0; i<light_count; i++) {
    CSolid* p_solid = GetAt(i);
    if ( !p_solid || !p_solid->IsValid() ) //check each solid
      return 0;
  }

  return 1;
}

///////////////////////////////////////////////////////////
// CLightArray     - storage class for all geometrical objects 
///////////////////////////////////////////////////////////


//Storing routine. Nonzero if error occured. May throw exceptions.
int CLightArray::write(CArchive& ar) const
{
  ASSERT( ar.IsStoring() );
  ASSERT( IsValid() );

  ar << GetSize();

  for(int i=0; i<GetSize(); i++ ){  
    CLight* p_light = GetAt(i);
    ASSERT( p_light->IsValid() );
    int ret = p_light->write( ar );

    ASSERT( !ret );
    if( ret )
      return STORING_ERROR_RETURN; //error occured    
  }
  
  return 0;
}

//Loading routine. Nonzero if error occured. May throw exceptions.
int CLightArray::read(CArchive& ar)
{
  ASSERT( ar.IsLoading() );
  Empty();

  int num;
  ar >> num;
  ASSERT( num >= 0 );

  for(int i=0; i<num; i++){
    CLight* p_light = new CLight();
    p_light->read( ar );
    if ( ! p_light->IsValid() ){
      ASSERT( 0 );     
      delete p_light;
        //note - we don't destroy the objects we've already noramally loaded
      return LOADING_ERROR_RETURN; // error - invalid data arrived
                    
    }
    Add( p_light );
  }

  if ( !IsValid() ){
    ASSERT( 0 );
    return LOADING_ERROR_RETURN;
  }

  return 0;
}

void CLightArray::Empty(void)
{
  int light_count = GetSize();
  for(int i=0; i<light_count; i++)
    delete GetAt(i); //free objects memory
  RemoveAll(); //free array memery
}

int CLightArray::IsValid() const
{
  AssertValid(); //internal array debug check

  int light_count = GetSize();
  for(int i=0; i<light_count; i++){
    CLight* p_light = GetAt(i);
    if ( !p_light || !p_light->IsValid() ) //check each light
      return 0;
  }

  return 1;
}