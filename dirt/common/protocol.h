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


//maximum allowed pixels_count int line (minimum is 1)
//(is used in SendSceneLine)
#define MAX_PIXELS_COUNT  65535




///////////////////////////////////////////////////////////
// Protocol version

//#define PROTOCOL_VERSION 1
//#define PROTOCOL_VERSION 2
#define PROTOCOL_VERSION 3


///////////////////////////////////////////////////////////
// Command IDs

#define CMD_CONNECTION_INIT  1
#define CMD_GET_FRAME_DATA   2
#define CMD_GET_SCENE_DATA   3
#define CMD_SEND_LINE_DATA   4


//Such return means that an error occured in function
#define STORING_ERROR_RETURN 1
#define LOADING_ERROR_RETURN 2




class CArchive;
class CLightArray;
class CSolidArray;
class CEnvironment;


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
//  ConnectionInit   - works with CMD_CONNECTION_INIT

struct CConnectionInit{
  struct Q : public QA  
  {
    int& m_client_protocol_version;
    
    Q(int& client_protocol_version);
    Q(int *p_client_protocol_version);    
    int write(CArchive& ar);
    int read(CArchive& ar);
  };

  struct A : public QA  
  {
    int& m_server_protocol_version;
    int& m_session_id;
    
    A( int& server_protocol_version
       , int& session_id       
     );
    A( int *p_server_protocol_version
       , int* p_session_id       
      );
    int write(CArchive& ar);
    int read(CArchive& ar);
  };

};

///////////////////////////////////////////////////////////
// GetFrameData     - works with CMD_GET_FRAME_DATA

struct CGetFrameData{
  struct Q : public QA  
  {
    int& m_session_id;
    
    Q(int& session_id);
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
    
    A(  int& session_id
       , int& scene_uid
       , CCameraInfo& camera_info
       , CImageLinesInfo& image_info 
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
//  GetSceneData   - works with CMD_GET_SCENE_DATA

struct CGetSceneData {
  struct Q : public QA  
  {
    int& m_session_id;
    int& m_scene_uid;
    
    Q(int& session_id, int& scene_uid);
    Q(int *p_session_id, int *p_sscene_uid);
    int write(CArchive& ar);
    int read(CArchive& ar);    
  };

  struct A : public QA  
  {
    int& m_session_id;
    BOOL& m_scene_changed; //nonzero means that scene with requested UID can't be sent
                           //so the m_scene is not written or read
    CEnvironment& m_scene;
    
    A(  int& session_id
       , BOOL& m_scene_changed
       , CEnvironment& scene       
     );
    A(int *p_session_id
       , BOOL* p_scene_changed
       , CEnvironment* p_scene
     );
    int write(CArchive& ar);
    int read(CArchive& ar);
  };
};


///////////////////////////////////////////////////////////
//  SendLineData   - works with CMD_SEND_LINE_DATA

struct CSendLineData{
  struct Q : public QA  
  {
    int&  m_session_id;
    int&  m_scene_uid;
    int&  m_line_num;
    int&  m_pixels_count;
    void*& m_data;

    Q(int& session_id, int& scene_uid, int& line_num
        , int& pixels_count, void*& data);

    Q(int *p_session_id, int* p_scene_uid, int* p_line_num
      , int *p_pixels_count, void **p_data);
    int write(CArchive& ar);
    int read(CArchive& ar);    
  };

  struct A : public QA  
  {
    int& m_session_id;
    
    A( int& session_id );
    A( int *p_session_id );
    int write(CArchive& ar);
    int read(CArchive& ar);
  };
};



#endif _PROTOCOL_H