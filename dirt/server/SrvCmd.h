//*********************************************************
//** SrvCmd.h **
// Created By: KIRILL
// On: 1/26/2004 00:50:55
// Comments: Server protocol commands declaration
//
//*********************************************************
// REVISION by ..., on ...
// Comments: ...
//
//*********************************************************

#ifndef _SRV_CMD_H
#define _SRV_CMD_H

//this return means that an error occured and the
//connection must be terminated
#define ERROR_MUST_TERMINATE 1


// Supports CMD_CONNECTION_INIT command.
// Checks the required server parameters
int CmdConnectionInit(CArchive& arIn, CArchive& arOut, int& last_session_id );


#endif//_SRV_CMD_H