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

#define PROTOCOL_VERSION 1

struct QA
{
  virtual ~QA() {};
  int virtual read(CArchive& ar) = 0;  //returns 0 if successful, nonzero otherwise
  int virtual write(CArchive& ar) = 0; //returns 0 if successful, nonzero otherwise
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
    
    A(const int& server_protocol_version, const int& session_id);
    A(int *p_server_protocol_version, int *p_session_id);
    int read(CArchive& ar);
    int write(CArchive& ar);
  };

};

///////////////////////////////////////////////////////////



#endif _PROTOCOL_H