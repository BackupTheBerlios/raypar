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

//#define PROTOCOL_VERSION 1
#define PROTOCOL_VERSION 2

class CVector;
class CArchive;

struct QA
{
  virtual ~QA() {};
  int virtual read(CArchive& ar) = 0;  //returns 0 if successful, nonzero otherwise
  int virtual write(CArchive& ar) = 0; //returns 0 if successful, nonzero otherwise
};

struct CameraInfo
{
  CVector& m_camera_pos;
  CVector& m_camera_z_axis;
  CVector& m_camera_y_axis;    

  CArchive& operator << (CArchive& ar);
  CArchive& operator >> (CArchive& ar);
};

struct ImageLinesInfo
{
  int& m_line_number;
  int& m_image_width;
  int& m_image_height;

  CArchive& operator << (CArchive& ar);
  CArchive& operator >> (CArchive& ar);
};

///////////////////////////////////////////////////////////

struct ConnectionInit{
  struct Q : public QA  
  {
    int& m_client_protocol_version;
    
    Q(const int& client_protocol_version);
    Q(int *p_client_protocol_version);
    int read(CArchive& ar);
    int write(CArchive& ar);
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
    int read(CArchive& ar);
    int write(CArchive& ar);
  };

};

///////////////////////////////////////////////////////////

struct GetSceneData{
  struct Q : public QA  
  {
    int& m_session_id;
    
    Q(const int& session_id);
    Q(int *p_session_id);
    int read(CArchive& ar);
    int write(CArchive& ar);
  };

  struct A : public QA  
  {
    int& m_session_id;
    int& m_scene_uid;    
    CameraInfo& m_camera_info;
    ImageLinesInfo& m_image_info;
    
    
    A(  const int& session_id
       , const int& m_scene_uid
       , const CameraInfo& camera_info
       , const ImageLinesInfo& image_info 
     );
    A(int *p_session_id
       , int* p_scene_uid
       , CameraInfo* p_camera_info
       , ImageLinesInfo* p_image_info
     );
    int read(CArchive& ar);
    int write(CArchive& ar);
  };

};


#endif _PROTOCOL_H