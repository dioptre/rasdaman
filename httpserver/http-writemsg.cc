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
/*  http-writemsg.c - generic HTTP-Message functions:                     */
/*         Creating and sending of HTTP-Messages.                         */
/*------------------------------------------------------------------------*/
/*  Comments:                                                             */
/*      - Status:                                                         */
/*          - To check: All Buffers free()ed? Also in error cases?        */
/*------------------------------------------------------------------------*/
/*
 * RCS:
 *   $RCSfile: http-writemsg.c,v $ $Revision: 1.3 $ $State: Exp $
 *   $Locker:  $
 */


#include   "defs.h"
#include   "protos.h"
#include   "types.h"
#include   "server.h"
#include   "http-defs.h"
#include   "http.h"

extern struct HTTPError HTTPErrorTable[];


/****** http-writemsg/AddField ***********************************************
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

rc_t AddField( struct MsgHeader *Ptr, int Field, char *Content )
{
    struct MsgHeader *NewHeader;

    if( Ptr != NULL )
        while( Ptr->Next != NULL )
            Ptr = Ptr->Next;

    NewHeader = AppendMsgHeader( Ptr, Field, Content );
    if( NewHeader != NULL )
        return( OK );
    else
        return( WARN );
}


/****** http-writemsg/CreateStatusLine ***************************************
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

rc_t CreateStatusLine( char *Buffer, size_t *BuffSize, int Code, int Protocol )
{
    int Entry;

    Entry = GetHTTPErrorTableEntry( Code );
    switch( Protocol )
    {
    case HTTP_1_0:
    {
        if( Entry != 0 )
            SNPrintf( Buffer, BuffSize, "HTTP/1.0 %d %s\r\n", Code, HTTPErrorTable[Entry].Reason );
        else
            SNPrintf( Buffer, BuffSize, "HTTP/1.0 %d Statuscode %d\r\n", Code, Code );
        return( OK );
    }
    break;
    case HTTP_1_1:
    {
        if( Entry != 0 )
            SNPrintf( Buffer, BuffSize, "HTTP/1.1 %d %s\r\n", Code, HTTPErrorTable[Entry].Reason );
        else
            SNPrintf( Buffer, BuffSize, "HTTP/1.1 %d Statuscode %d\r\n", Code, Code );
        return( OK );
    }
    break;
    default:
    {
        return( ERROR );
    }
    }
}


/****** http-writemsg/CreateHTTPMsg ******************************************
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

struct HTTPMsg *CreateHTTPMsg( char *Header, char *Body, size_t BodySize )
{
    struct HTTPMsg *Msg;

    /*  printf( "###  CreateHTTPMsg(+)\n" );  */
    Msg = (struct HTTPMsg*)mymalloc( sizeof( struct HTTPMsg ) );
    if( Msg != NULL )
    {
        /*      printf( "###      Header[%d]:\n%s", strlen( Header ), Header );  */
        /*      printf( "###      Body[%d]:\n", BodySize );  */

        Msg->Body        = Body;
        Msg->BodySize    = BodySize;

        if( ( Msg->Head  = (char*)mymalloc( strlen( Header ) + 1 ) ) != NULL )
            strcpy( Msg->Head, Header );
        else
        {
            free( Msg );
            return( NULL );
        }
        /*      printf( "###    Done!\n" );  */
    }
    /*  printf( "###  CreateHTTPMsg(-)\n" ); */
    return( Msg );
}


/****** http-writemsg/SendHTTPMsg ********************************************
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

rc_t SendHTTPMsg( int SockFD, struct HTTPMsg *Msg )
{
    size_t Check;

    if( ( Msg != NULL ) && ( Msg->Head != NULL ) )
    {
        Check = WriteN( SockFD, Msg->Head, strlen(Msg->Head) );
        if( Check < 0 )
            ErrorMsg( E_SYS, ERROR, "ERROR: WriteN(): write() failed." );
        else if( Check != strlen(Msg->Head) )
            LogMsg( LG_SERVER, WARN,
                    "WARN:  SendHTTPMsg(): %d of %d Bytes of Header written!",
                    Check, strlen(Msg->Head) );

        if( ( Msg->Body != NULL ) && ( Msg->BodySize > 0 ) )
        {
            Check = WriteN( SockFD, Msg->Body, Msg->BodySize );
            if( Check < 0 )
                ErrorMsg( E_SYS, ERROR, "ERROR: WriteN(): write() failed." );
            else if( Check != Msg->BodySize )
                LogMsg( LG_SERVER, WARN,
                        "WARN:  SendHTTPMsg(): %d of %d Bytes of Body written!",
                        Check, Msg->BodySize );
        }
        return( OK );
    }
    else
    {
        return( ERROR );
    }
}


/****** http-writemsg/FreeHTTPMsg ********************************************
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

rc_t FreeHTTPMsg( struct HTTPMsg *Ptr )
{
    free( Ptr->Head );
    free( Ptr );
    return( OK );
}
