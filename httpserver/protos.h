/*
* This file is part of rasdaman community.
*
* Rasdaman community is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* Rasdaman community is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with rasdaman community.  If not, see <http://www.gnu.org/licenses/>.
*
* Copyright 2003, 2004, 2005, 2006, 2007, 2008, 2009 Peter Baumann /
rasdaman GmbH.
*
* For more information please see <http://www.rasdaman.org>
* or contact Peter Baumann via <baumann@rasdaman.com>.
*/
/*------------------------------------------------------------------------*/
/*  protos.h - Prototypes of all functions                                */
/*------------------------------------------------------------------------*/
/*
 * RCS:
 *   $RCSfile: protos.h,v $ $Revision: 1.10 $ $State: Exp $
 *   $Locker:  $ 
 */

#ifndef _PROTOS_H
#define _PROTOS_H

#include "server.h"
 
/*------------------------------------------------------------------------*/
/*  Protoypes from main.c                                                 */
/*------------------------------------------------------------------------*/

int    Exit( int );

/*  init.c    */
rc_t   Initialize( int, char **, struct ServerBase * );
rc_t   InitDaemon( int );
rc_t   InitSocket( int *, struct sockaddr_in *, int );
rc_t   InitClientSocket( int *, struct sockaddr_in *, char *, int );
void   SavePId( char * );

/*  childs.c    */
rc_t   InitChild( struct ClientBase * );
pid_t  NewChild( struct ChildBase *List, struct FDsets *PDSets, struct ClientBase *Client );
void   CleanupChild( struct ChildBase *List, struct FDsets *PDSets, pid_t PId );
void   AddChild( struct ChildBase *List, struct ChildBase *Child );
void   RemChild( struct ChildBase *List, struct ChildBase *Child );
struct ChildBase *GetChild( struct ChildBase *List, pid_t PId );

/*------------------------------------------------------------------------*/
/*  Protoypes from config.c                                               */
/*------------------------------------------------------------------------*/

rc_t   ReadArgs( struct ServerBase *, int, char ** );
rc_t   ReadConfig( struct ServerBase * );
rc_t   CheckAndSet( struct ServerBase *, char *, int );
rc_t   SetServDir( struct ServerBase *, char * );
rc_t   SetFilename( struct ServerBase *, int, char * );
rc_t   SetString( struct ServerBase *, int, char * );
rc_t   ConfigureServer( struct ServerBase * );
int    GetConfigKey( char * );

/*------------------------------------------------------------------------*/
/*  Protoypes from signals.c                                               */
/*------------------------------------------------------------------------*/

typedef void(*sighandler)(int);

sighandler Signal( int, sighandler );
void   SigHandler( int );
rc_t   InitSigHandler( void );

/*------------------------------------------------------------------------*/
/*  Protoypes from logging.c                                              */
/*------------------------------------------------------------------------*/

rc_t   OpenLog( struct Logging *, char *, char *, char * );
void   CloseLog( struct Logging *Log );
void   LogMsg( int, int, const char *, ... );
void   ErrorMsg( int, int, const char *, ... );
rc_t   LogDate( char *, int );
struct tm *Get_GMToffset( int * );

/*------------------------------------------------------------------------*/
/*  Protoypes for communication handling.                                 */
/*------------------------------------------------------------------------*/

/*  http.c   */
void   HandleRequest( struct ClientBase *Client );

/*  http-doit.c   */
rc_t   Accept( int, struct ClientBase * );
void   GetRequest( struct ClientBase *Client );
void   InterpreteRequest( struct ClientBase *Client, struct ToDoArgs *ToDo );
int    SendRequest( int SockFD, struct HTTPMsg *Request, struct FDsets *RW_Sets );
int    ReadResponseHead( int SockFD, struct RespInfo *Response, struct FDsets *RW_Sets );
int    ReadResponseBody( int SockFD, struct RespInfo *Response, struct FDsets *RW_Sets );
void   SendResponse( struct ClientBase *Client );
void   CreateRasResponse( struct HTTPMode *Mode, struct ClientBase *Client );
void   DoMessageBody( struct ClientBase *Client );
void   InterpretePOSTRequest ( struct ClientBase *Client );
void   WriteAccessLog( struct ClientBase *Client );


/*  http-readmsg.c    */
int    ReadHeader( int SockFD, char **Buffer, size_t *BuffSize );
char  *ReadBody( int SockFD, size_t BuffSize );
rc_t   ParseReqHeader( struct ReqInfo *Request );
rc_t   ParseRespHeader( struct RespInfo *Response );
char  *ParseReqLine( char *Buffer, struct ReqInfo *Request );
char  *ParseRespLine( char *Buffer, struct RespInfo *Response );
char  *ParseMsgLine( char *Buffer, int *Key, char **Param );
void   SplitURL( char *Buffer, struct URLComps *URL );
size_t GetContentLength( struct MsgHeader *Ptr );
char  *GetFieldContent( struct MsgHeader *Ptr, int Field, struct MsgHeader **Next );
int    GetRealm( char *String );

/*  http-writemsg.g    */
rc_t   AddField( struct MsgHeader *Ptr, int Field, char *Content );
rc_t   CreateStatusLine( char *Buffer, size_t *BuffSize, int Code, int Protocol );
struct HTTPMsg *CreateHTTPMsg( char *, char *, size_t );  // ???
rc_t   SendHTTPMsg( int SockFD, struct HTTPMsg *Msg );
rc_t   FreeHTTPMsg( struct HTTPMsg *Ptr );

/*  http-error.c    */
int    GetHTTPErrorTableEntry( int Code );
rc_t   CreateHTTPError( int Code, struct HTTPMode *Mode, struct HTTPMsg *Msg );

/*  http-methods.c    */
int    HTTP_GetMKey( char * );
char  *HTTP_GetMethodName( int );

/*  http-fields.c    */
int    HTTP_GetHKey( char * );
char  *HTTP_GetFieldName( int );

/*  http-date.c    */
rc_t   HTTP_Date( char *, size_t );

/*  http-support.c    */
rc_t   InitClientBase( struct ClientBase * );
rc_t   InitReqInfo( struct ReqInfo * );
rc_t   InitRespInfo( struct RespInfo * );
void   InitHTTPMsg( struct HTTPMsg *Msg );
struct MsgHeader *NewMsgHeader( int, char * );
struct MsgHeader *AppendMsgHeader( struct MsgHeader *, int, char * );
void   DeleteMsgHeader( struct MsgHeader * );
void   PrintReqInfo( struct ReqInfo * );
void   PrintRespInfo( struct RespInfo * );
rc_t   CheckSockError( int, int, int );
char  *CharToBits( char c );



/*------------------------------------------------------------------------*/
/*  Protoypes from support.c                                              */
/*------------------------------------------------------------------------*/

int   Get_OpenMax( void );
char *PathAlloc( size_t * );
int   ReadN( register int, register char *, register int );
int   WriteN( register int, register char *, register int );
int   ReadLine( register int, register char *, register int );
rc_t  ParseString( char *, char *, ... );
int   SNPrintf( char *, size_t *, const char *, ... );
int   VSNPrintf( char *, size_t, const char *, va_list );
char *StrError( int );
char *StrToLower( char * );

#endif  /*  _PROTOS_H not defined  */
