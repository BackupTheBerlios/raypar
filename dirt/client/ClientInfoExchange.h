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

//this means that server uses incompatible protocol
#define CIE_WRONG_PROTOCOL_ERROR 2

//this means that server has no lines for 
//us to render and we should wait fo a while
#define CIE_SERVER_WAIT 3

// rendereded_line_number - number of rendered line, negative means no line were rendered
//                            so we don't send line_data to server
// COLORREF* line_data - redered line data
// socket  - socket, connected with server
// scene  - the scene, which could be loaded from server if need
// camera  - the camera, which position is received from the server
// line_number  - number of the line to render
int ClientInfoExchange(int rendereded_line_number, COLORREF* line_data
                       , CSocket& socket, CEnvironment& scene
                        , CCamera& camera, int& line_number );


#endif // !defined(AFX_CLIENTINFOEXCHANGE_H__668D3A50_0F8E_4E9C_B64A_249640D78502__INCLUDED_)
