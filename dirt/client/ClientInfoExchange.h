//*********************************************************
//** ClientInfoExchange.h **
// Created By: KIRILL
// On: 1/28/2004 17:10:03
// Comments: declaration of ClientInfoExchange and it's return values.
//    ClientInfoExchange() provides all the inforamtuion exchange with server
//   for client.
//
//*********************************************************
// REVISION by ..., on ...
// Comments: ...
//
//*********************************************************


#if !defined(AFX_CLIENTINFOEXCHANGE_H__668D3A50_0F8E_4E9C_B64A_249640D78502__INCLUDED_)
#define AFX_CLIENTINFOEXCHANGE_H__668D3A50_0F8E_4E9C_B64A_249640D78502__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSocket;
class CEnvironment;
class CCamera;

///////////////////////////////////////////////////////////
// return codes of ClientInfoExchange

//this means that scene received and should be rendered
#define CIE_NORMAL_RENDER_RETURN 0

//this means that an error ocured during communication
#define CIE_COMMUNICATION_ERROR 1

//this means that an error ocured during communication
#define CIE_WRONG_PROTOCOL_ERROR 1





// ClientInfoExchange - does all the information exchange with server
// CSocket& socket - socket, connected with server
// CEnvironment*   - the scene, which could be loaded from server if need
// CCamera*        - the camera, which position is received from the server
int ClientInfoExchange(CSocket& socket, CEnvironment& scene, CCamera& camera);

#endif // !defined(AFX_CLIENTINFOEXCHANGE_H__668D3A50_0F8E_4E9C_B64A_249640D78502__INCLUDED_)
