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
#include "mymalloc/mymalloc.h"
/*------------------------------------------------------------------------*/
/*  http-support.c - Support functions for HTTP-Message data.             */
/*------------------------------------------------------------------------*/
/*  Comments:                                                             */
/*      - Status:                                                         */
/*          - To check: All Buffers free()ed? Also in error cases?        */
/*------------------------------------------------------------------------*/
/*
 * RCS:
 *   $RCSfile: http-support.c,v $ $Revision: 1.5 $ $State: Exp $
 *   $Locker:  $
 */


#include   "defs.h"
#include   "protos.h"
#include   "server.h"
#include   "http-defs.h"
#include   "http.h"



/****** http-support/InitClientBase ******************************************
*
*   NAME
*
*
*   SYNOPSIS
*
*
*   FUNCTION
*
*
*   INPUTS
*
*
*   RESULT
*
*
*   NOTES
*
*
*   BUGS
*
*
*   SEE ALSO
*
*
******************************************************************************
*
*/

rc_t InitClientBase( struct ClientBase *Client )
{
    return( ERROR );
}


/****** http-support/InitReqInfo *********************************************
*
*   NAME
*
*
*   SYNOPSIS
*
*
*   FUNCTION
*
*
*   INPUTS
*
*
*   RESULT
*
*
*   NOTES
*
*
*   BUGS
*
*
*   SEE ALSO
*
*
******************************************************************************
*
*/

rc_t InitReqInfo( struct ReqInfo *Request )
{
    Request->HeadBuff             = NULL;
    Request->HeadSize             = 0;
    Request->BodyBuff             = NULL;
    Request->BodySize             = 0;
    Request->Line.Method          = 0;
    Request->Line.URL.Protocol    = NULL;
    Request->Line.URL.Servername  = NULL;
    Request->Line.URL.Path        = NULL;
    Request->Line.URL.Extra       = NULL;
    Request->Line.Version.Major   = 0;
    Request->Line.Version.Minor   = 0;
    Request->First                = NULL;
    Request->Last                 = NULL;
    Request->Body                 = NULL;
    return( OK );
}


/****** http-support/InitRespInfo ********************************************
*
*   NAME
*
*
*   SYNOPSIS
*
*
*   FUNCTION
*
*
*   INPUTS
*
*
*   RESULT
*
*
*   NOTES
*
*
*   BUGS
*
*
*   SEE ALSO
*
*
******************************************************************************
*
*/

rc_t InitRespInfo( struct RespInfo *Response )
{
    Response->HeadBuff            = NULL;
    Response->HeadSize            = 0;
    Response->BodyBuff            = NULL;
    Response->BodySize            = 0;
    Response->Line.Version.Major  = 0;
    Response->Line.Version.Minor  = 0;
    Response->Line.Status         = 0;
    Response->Line.Reason         = NULL;
    Response->First               = NULL;
    Response->Last                = NULL;
    Response->Body                = NULL;
    return( OK );
}


/****** http-support/InitHTTPMsg *********************************************
*
*   NAME
*
*
*   SYNOPSIS
*
*
*   FUNCTION
*
*
*   INPUTS
*
*
*   RESULT
*
*
*   NOTES
*
*
*   BUGS
*
*
*   SEE ALSO
*
*
******************************************************************************
*
*/

void InitHTTPMsg( struct HTTPMsg *Msg )
{
    Msg->Head        = NULL;
    Msg->Body        = NULL;
    Msg->BodySize    = 0;
}


/****** http-support/NewMsgHeader ********************************************
*
*   NAME
*
*
*   SYNOPSIS
*
*
*   FUNCTION
*
*
*   INPUTS
*
*
*   RESULT
*
*
*   NOTES
*
*
*   BUGS
*
*
*   SEE ALSO
*
*
******************************************************************************
*
*/

struct MsgHeader *NewMsgHeader( int Key, char *String )
{
    struct MsgHeader *NewHeader;

    NewHeader = (struct MsgHeader*)mymalloc( sizeof( struct MsgHeader ) );
    if( NewHeader == NULL )
        return( NULL );
    NewHeader->Next    = NULL;
    NewHeader->Field   = Key;
    NewHeader->Content = String;
    return( NewHeader );
}


/****** http-support/AppendMsgHeader *****************************************
*
*   NAME
*
*
*   SYNOPSIS
*
*
*   FUNCTION
*
*
*   INPUTS
*
*
*   RESULT
*
*
*   NOTES
*
*
*   BUGS
*
*
*   SEE ALSO
*
*
******************************************************************************
*
*/

struct MsgHeader *AppendMsgHeader( struct MsgHeader *Last, int Key, char *String )
{
    struct MsgHeader *NewHeader;

    NewHeader = NewMsgHeader( Key, String );
    if( NewHeader == NULL )
    {
        ErrorMsg( E_SYS, ERROR, "ERROR: malloc error for MsgHeader structure." );
        return( NULL );
    }
    if( Last != NULL )
    {
        Last->Next = NewHeader;
    }
    return( NewHeader );
}


/****** http-support/DeleteMsgHeader *****************************************
*
*   NAME
*
*
*   SYNOPSIS
*
*
*   FUNCTION
*
*
*   INPUTS
*
*
*   RESULT
*
*
*   NOTES
*
*
*   BUGS
*
*
*   SEE ALSO
*
*
******************************************************************************
*
*/

void DeleteMsgHeader( struct MsgHeader *First )
{
    struct MsgHeader *Tmp;

    if( First != NULL )
    {
        Tmp = First->Next;
        free( First );
        First = Tmp;
    }
    return;
}


/****** http-support/PrintReqInfo ********************************************
*
*   NAME
*
*
*   SYNOPSIS
*
*
*   FUNCTION
*
*
*   INPUTS
*
*
*   RESULT
*
*
*   NOTES
*
*
*   BUGS
*
*
*   SEE ALSO
*
*
******************************************************************************
*
*/

void PrintReqInfo( struct ReqInfo *Request )
{
    struct MsgHeader *Ptr;

    printf( ">> %s %s HTTP/%d.%d\n",
            HTTP_GetMethodName( Request->Line.Method ),
            Request->Line.URL.Protocol ? Request->Line.URL.Protocol : "",
            Request->Line.URL.Servername ? Request->Line.URL.Servername : "",
            Request->Line.URL.Path ? Request->Line.URL.Path : "",
            Request->Line.URL.Extra ? Request->Line.URL.Extra : "",
            Request->Line.Version.Major, Request->Line.Version.Minor );

    Ptr = Request->First;
    while( Ptr != NULL )
    {
        printf( ">> %s: %s\n", HTTP_GetFieldName( Ptr->Field ), Ptr->Content );
        Ptr = Ptr->Next;
    }
    return;
}


/****** http-support/PrintRespInfo *******************************************
*
*   NAME
*
*
*   SYNOPSIS
*
*
*   FUNCTION
*
*
*   INPUTS
*
*
*   RESULT
*
*
*   NOTES
*
*
*   BUGS
*
*
*   SEE ALSO
*
*
******************************************************************************
*
*/

void PrintRespInfo( struct RespInfo *Response )
{
    struct MsgHeader *Ptr;

    printf( ">> HTTP/%d.%d %d %s\n",
            Response->Line.Version.Major, Response->Line.Version.Minor ,
            Response->Line.Status,
            Response->Line.Reason );

    Ptr = Response->First;
    while( Ptr != NULL )
    {
        printf( ">> %s: %s\n", HTTP_GetFieldName( Ptr->Field ), Ptr->Content );
        Ptr = Ptr->Next;
    }
    return;
}


/****** http-support/CheckSockError ******************************************
*
*   NAME
*
*
*   SYNOPSIS
*
*
*   FUNCTION
*
*
*   INPUTS
*
*
*   RESULT
*
*
*   NOTES
*
*
*   BUGS
*
*
*   SEE ALSO
*
*
******************************************************************************
*
*/

rc_t CheckSockError( int SockFD, int Level, int OptName )
{
    int    error;
    size_t len = sizeof( error );

#ifdef DECALPHA
    if( getsockopt( SockFD, Level, OptName, (char *)&error, (int*)&len ) < 0 )
#else
    if( getsockopt( SockFD, Level, OptName, (char *)&error, (socklen_t*)&len ) < 0 )
#endif
        return( FAIL );
    if( error )
    {
        errno = error;
        return( ERROR );
    }
    return( OK );
}


