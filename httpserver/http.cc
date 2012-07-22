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
/*  http.c - reworked HTTP communication main loop.                       */
/*------------------------------------------------------------------------*/


#include   "defs.h"
#include   "protos.h"
#include   "server.h"
#include   "http-defs.h"
#include   "http.h"

/*
 * Release ClientBase
 */
void releaseClientBase(struct ClientBase *cb)
{
    // free Host
    free(cb->Host.Name);

    // free Socket
    //  struct sockaddr_in Socket;

    // free TimeOut
    //  struct timeval     TimeOut;

    // free Request
    free(cb->Request.HeadBuff);
    free(cb->Request.BodyBuff);
    struct MsgHeader *dummy = NULL;
    while(cb->Request.First != NULL)
    {
        dummy = cb->Request.First->Next;
        //free(cb->Request.First->Content);
        free(cb->Request.First);
        cb->Request.First = dummy;
    }
    free(cb->Request.Body);
    free(cb->Request.Line.Vanilla);

    // free Response
    free(cb->Response.Head);
    free(cb->Response.Body);

}


/****** http/HandleRequest ***************************************************
*
*   NAME
*       HandleRequest -- main function for communication processing.
*
*   SYNOPSIS
*       void HandleRequest( struct ClientBase *Client,
*                         SubServerPtr SubServerList );
*
*   FUNCTION
*       This is the main function which handles the HTTP communication.
*       It calls successivley the functions for handling the request from
*       the client, opening the sockets to the sub-servers and sending
*       the rewritten requests to them, handling the responses and finally
*       sending a response to the client.
*
*   INPUTS
*       Client - Data structure which holds various informations about
*           the client.
*       SubServerList - The head to a list of all sub-server data structures.
*
*   NOTES
*       "Client" and "SubServerList" must be valid - they will not be
*       checked. Since "Client" have to be valid in the previous call to
*       "Accept()" this shouldn't be much of a problem...
*
*       This is "rewritten from scratch" version of "HandleRequest()"
*       since the old version seemed not suitable enough for expansion
*       to handle more error conditions in the communication, and for
*       integration of more features.
*
*   BUGS
*       Various left. Especially the authorization mechanism doesn't work
*       (yet). Also, some aspects of the HTTP aren't handled gracefully
*       (HTTP/0.9 messages, handling of other methods than GET, HEAD and
*       POST, etc.).
*
******************************************************************************
*
*/
void HandleRequest( struct ClientBase *Client )
{
    struct ToDoArgs ToDo;
    int             RespState;

    ToDo.What       = DO_NOTHING;
    ToDo.Which.Code = REALLY_NOTHING;
    Client->Comm.ConnStatus = CONN_OPEN;

    while( Client->Comm.ConnStatus == CONN_OPEN )
    {
        //LogMsg( LG_SERVER, DEBUG, "DEBUG: HandleRequest(): Reading Request.");
        GetRequest( Client );
        //LogMsg( LG_SERVER, DEBUG, "DEBUG: HandleRequest(): Parsing Request.");
        ParseReqHeader( &Client->Request );

        //LogMsg( LG_SERVER, DEBUG, "DEBUG: HandleRequest(): Interpreting Request.");
        InterpreteRequest( Client, &ToDo );
        switch( ToDo.What )
        {
        case DO_SEND_RESPONSE:
            //LogMsg( LG_SERVER, DEBUG, "DEBUG: HandleRequest(): doing InterpreteRequest.");
            InterpretePOSTRequest( Client );

            //LogMsg( LG_SERVER, DEBUG, "DEBUG: HandleRequest(): doing CreateRasResponse.");
            CreateRasResponse( &Client->Comm, Client );
            /*
                LogMsg( LG_SERVER, DEBUG, "DEBUG:   SendResponse() for Request: \"%s\".",
                    Client->Request.Line.Vanilla );
            */

            SendHTTPMsg( Client->SockFD, &Client->Response );
            WriteAccessLog( Client );
            LogMsg( LG_SERVER, DEBUG, "DEBUG: HandleRequest(): SEND_RESPONSE done.");
            break;

        case DO_SEND_ERROR:
            LogMsg( LG_SERVER, DEBUG, "DEBUG: HandleRequest(): doing SEND_ERROR." );
            Client->RespStatus = ToDo.Which.Code;
            LogMsg( LG_SERVER, DEBUG, "DEBUG:   CreateHTTPError( %d, -, %p )...",
                    Client->RespStatus, &Client->Response );
            CreateHTTPError( Client->RespStatus, &Client->Comm, &Client->Response );
            LogMsg( LG_SERVER, DEBUG, "DEBUG:   CreateHTTPError() done." );
            LogMsg( LG_SERVER, DEBUG, "DEBUG:   SendResponse()..." );
            SendResponse( Client );
            LogMsg( LG_SERVER, DEBUG, "DEBUG:   SendResponse() done." );
            LogMsg( LG_SERVER, DEBUG, "DEBUG:   Write*Log()..." );
            WriteAccessLog( Client );
            LogMsg( LG_SERVER, DEBUG, "DEBUG:   WriteLog() done." );
            LogMsg( LG_SERVER, DEBUG, "DEBUG: HandleRequest(): SEND_ERROR done.");
            break;

        case DO_SHUTDOWN:
            LogMsg( LG_SERVER, DEBUG, "DEBUG: HandleRequest(): doing SHUTDOWN.");
            close( Client->SockFD );
            LogMsg( LG_SERVER, DEBUG, "DEBUG: HandleRequest(): SHUTDOWN done.");
            break;

        default:
            LogMsg( LG_SERVER, DEBUG, "DEBUG: HandleRequest(): doing UNDEFINED ACTION.");
            ErrorMsg( E_PRIV, ERROR, "ERROR: HandleRequest(): Undefined action!" );
            Client->RespStatus = STATUS_Internal_Server_Error;
            CreateHTTPError( Client->RespStatus, &Client->Comm, &Client->Response );
            SendResponse( Client );
            WriteAccessLog( Client );
            LogMsg( LG_SERVER, DEBUG, "DEBUG: HandleRequest(): UNDEFINED ACTION done.");
            break;
        }
        releaseClientBase(Client);

    }
}

