//****************************************
//** SceneList.cpp **
// Created By: Tonic
// On: 2/2/2004
// Comments: CSceneList class implementation
//*********************************************************
// REVISION by ..., on ...
// Comments: ...
//*********************************************************

#include "stdafx.h"
#include "scenelist.h"

void CSceneList::Add(CEnvironment *scene, CCamera *camera)
{
  ASSERT( camera != NULL );
  ASSERT( (m_counter > 0) || (scene != NULL) );
  
  CEnvironment *sceneToAdd;
  
  if( scene == NULL )
    sceneToAdd = (CEnvironment *) m_scenes.GetAt( m_counter - 1 );
  else
    sceneToAdd = scene;
  
  m_scenes.Add(sceneToAdd);
  m_cameras.Add(camera);
  m_counter++;
};

void CSceneList::GetScene(int index, CEnvironment** p_scene, CCamera** p_camera) const
{
  ASSERT( (index < m_counter) && (index >= 0));

  *p_scene = (CEnvironment *) m_scenes.GetAt(index);
  *p_camera = (CCamera *) m_cameras.GetAt(index);
};

int CSceneList::GetSceneCount( void ) const
{
  return m_counter;
};

void CSceneList::Empty(void)
{
  EmptySceneArray();
  
  for( int i = 0; i < m_counter; i++){
    delete (CCamera*)m_cameras[i];
  }
  m_cameras.RemoveAll();

  m_counter = 0;
};

void CSceneList::EmptySceneArray()
{
  for( int i = 0; i < m_counter; i++)
  {
    CEnvironment *current, *temp;
    bool encountered = false;

    current = (CEnvironment*)m_scenes.GetAt(i);
    //check the pointers with indexes less than ours
    //maybe we have encountered the same pointer before
    for( int j = 0; j < i; j++)
    {
      temp = (CEnvironment*)m_scenes.GetAt(j);
      if( temp == current )
      {
        encountered = true;
        break;
      };
    };

    if( !encountered ){
      current->Empty();
      delete current;
    }      
  }
  m_scenes.RemoveAll();
}