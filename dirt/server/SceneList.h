//****************************************
//** SceneList.h **
// Created By: Tonic
// On: 2/2/2004
// Comments: CSceneList class declaration
//*********************************************************
// REVISION by ..., on ...
// Comments: ...
//*********************************************************

#if !defined(SERVER_SCENELIST_H_INCLUDED)
#define SERVER_SCENELIST_H_INCLUDED

#include "client/environment.h"

class CSceneList
{
protected:
  CPtrArray m_scenes;
  CPtrArray m_cameras;
  int m_counter;

public:
  CSceneList() 
   : m_counter(0) 
  {}

  void Add(CEnvironment *scene, CCamera *camera);
  void GetScene(int index, CEnvironment** p_scene, CCamera** p_camera) const;
  int  GetSceneCount( void ) const;
  void Empty(void);

protected:
  void EmptySceneArray();
};

#endif // SERVER_SCENELIST_H_INCLUDED