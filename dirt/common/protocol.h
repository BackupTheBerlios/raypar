//*********************************************************
//** protocol.h **
// Created By: KIRILL
// On :1/8/2004 23:37:55
// Comments: protocol definitions and structures
//
//*********************************************************
// REVISION by ..., on ...
// Comments: ...
//
//*********************************************************

#ifndef _PROTOCOL_H
#define _PROTOCOL_H

#pragma once
#include "vector.h"


///////////////////////////////////////////////////////////
// Protocol version

//#define PROTOCOL_VERSION 1
#define PROTOCOL_VERSION 2


///////////////////////////////////////////////////////////
// Command IDs

#define CMD_CONNECTION_INIT  1
#define CMD_GET_FRAME_DATA   2
#define CMD_GET_SCENE_DATA   3


//Such return means that an error occured in function
#define STORING_ERROR_RETURN 1
#define LOADING_ERROR_RETURN 2




class CArchive;
class CLightArray;
class CSolidArray;
class Environment;


///////////////////////////////////////////////////////////
//  QA    - base network I/O structure

struct QA
{
  virtual ~QA() {};
  int virtual write(CArchive& ar) = 0; //returns 0 if successful, nonzero otherwise
  int virtual read(CArchive& ar) = 0;  //returns 0 if successful, nonzero otherwise
};


///////////////////////////////////////////////////////////
// CCameraInfo   - we hold camera postion here

struct CCameraInfo
{
  CVector m_camera_pos;
  CVector m_camera_z_axis;
  CVector m_camera_y_axis;

  //Storing/loading routines. Zero if successful
  int write(CArchive& ar) const; 
  int read (CArchive& ar); 
};


///////////////////////////////////////////////////////////
// ImageLineInfo   - we hold image properties here

struct CImageLinesInfo
{
  int m_line_number;
  int m_image_width;
  int m_image_height;

  //Storing/loading routines. Zero if successful
  int write(CArchive& ar) const; 
  int read (CArchive& ar); 
};


///////////////////////////////////////////////////////////
//  ConnectionInit

struct CConnectionInit{
  struct Q : public QA  
  {
    int& m_client_protocol_version;
    
    Q(const int& client_protocol_version);
    Q(int *p_client_protocol_version);    
    int write(CArchive& ar);
    int read(CArchive& ar);
  };

  struct A : public QA  
  {
    int& m_server_protocol_version;
    int& m_session_id;
    
    A( const int& server_protocol_version
       , const int& session_id       
     );
    A( int *p_server_protocol_version
       , int* p_session_id       
      );
    int write(CArchive& ar);
    int read(CArchive& ar);
  };

};

///////////////////////////////////////////////////////////
// GetFrameData

struct CGetFrameData{
  struct Q : public QA  
  {
    int& m_session_id;
    
    Q(const int& session_id);
    Q(int *p_session_id);
    int write(CArchive& ar);
    int read(CArchive& ar);    
  };

  struct A : public QA  
  {
    int& m_session_id;
    int& m_scene_uid;    
    CCameraInfo& m_camera_info;
    CImageLinesInfo& m_image_info;
    
    A(  const int& session_id
       , const int& scene_uid
       , const CCameraInfo& camera_info
       , const CImageLinesInfo& image_info 
     );
    A(int *p_session_id
       , int* p_scene_uid
       , CCameraInfo* p_camera_info
       , CImageLinesInfo* p_image_info
     );
    int write(CArchive& ar);
    int read(CArchive& ar);
  };
};


///////////////////////////////////////////////////////////
//  GetSceneData

struct CGetSceneData {
  struct Q : public QA  
  {
    int& m_session_id;
    
    Q(const int& session_id);
    Q(int *p_session_id);
    int write(CArchive& ar);
    int read(CArchive& ar);    
  };

  struct A : public QA  
  {
    int& m_session_id;
    int& m_scene_uid;
    Environment& m_scene;
    
    A(  const int& session_id
       , const int& scene_uid
       , const Environment& scene       
     );
    A(int *p_session_id
       , int* p_scene_uid
       , Environment* p_scene
     );
    int write(CArchive& ar);
    int read(CArchive& ar);
  };
};




#endif _PROTOCOL_H