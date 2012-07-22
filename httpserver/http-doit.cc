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
/*  http-doit.c - functions for handling the communication in its         */
/*                various states.                                         */
/*------------------------------------------------------------------------*/

#include   "servercomm/httpserver.hh"

#include   "defs.h"
#include   "protos.h"
#include   "server.h"
#include   "http-defs.h"
#include   "http.h"

extern struct ServerBase Server;

/****** http/Accept **********************************************************
*
*   NAME
*       Accept -- wrapper function to `accept()' system call.
*
*   SYNOPSIS
*       rc_t Accept( int SockFD, struct ClientBase *Client );
*
*   FUNCTION
*       This function is used as a wrapper for the `accept()' system call so
*       the appropriate variables and structures as defined for this program
*       will be used. In addition, if `accept()' fails it will lead the
*       program into immediate abortion.
*
*   INPUTS
*       SockFD - the file descriptor of the socket, which will be used to
*           accept connections.
*       Client - a pointer to a ClientBase structure, where all necessary
*           informations of the client who made the connection is stored.
*
*   RESULT
*       Returns the "OK" status code on success, never returns if it fails.
*       As a side effect, when `accept()' returns, the socket structure that
*       is part of the ClientBase structure is setup by `accpet()'.
*
*   NOTES
*       "Client" must be a pointer to a valid ClientBase structure.
*       Calls `ErrorMsg()' in case of an failure with option fail, which will
*       cause an immediate abortion of the program.
*
*   BUGS
*       1) Does not check if "Client" is a NULL pointer.
*       2) On some systems the accept call has to be made twice to succeed,
*          because the first attempt fails with an "interrupted system call"
*          error message.
*
******************************************************************************
*
*/

rc_t Accept( int SockFD, struct ClientBase *Client )
{
    short cc = 0;
    int test = -1;

    if( Client == NULL )
        ErrorMsg( E_SYS, FAIL, "FAIL:  accept(): Client is NULL." );
    Client->SockSize = sizeof( Client->Socket );

    while (( cc < 10 ) && ( test < 0 ))
    {
#ifdef DECALPHA
        test  = accept( SockFD, (struct sockaddr *)&Client->Socket,
                        (int*)&Client->SockSize );
#else
        test  = accept( SockFD, (struct sockaddr *)&Client->Socket,
                        (socklen_t*)&Client->SockSize );
#endif
        cc++;
        LogMsg( LG_SERVER, DEBUG, "DEBUG: accept call # %d",cc );
    }
    Client->SockFD = test;
    if( Client->SockFD < 0 )
        ErrorMsg( E_SYS, FAIL, "FAIL:  accept()." );
    return( OK );
}




void GetRequest( struct ClientBase *Client )
{
    int             flags;

    /*-- Try to make SockFD non-blocking;              */
    /*    if it fails, try to continue nevertheless  --*/
    flags = fcntl( Client->SockFD, F_GETFL, 0 );
    if( flags < 0 )
        ErrorMsg( E_SYS, WARN,
                  "WARN:  GetRequest(): Client Socket: fcntl() (get flags) failed." );
    else if( fcntl( Client->SockFD, F_SETFL, flags | O_NONBLOCK ) < 0 )
        ErrorMsg( E_SYS, WARN,
                  "WARN:  GetRequest(): Client Socket: fcntl() (set to non-blocking) failed." );

    /*-- Read Header of Request  */
    Client->Request.State = ReadHeader( Client->SockFD,
                                        &Client->Request.HeadBuff,
                                        &Client->Request.HeadSize );
    //LogMsg( LG_SERVER, DEBUG, "DEBUG: Header is:\n %s",Client->Request.HeadBuff );
}




void InterpreteRequest( struct ClientBase *Client, struct ToDoArgs *ToDo )
{
    struct CacheData *Data = NULL;

    switch( Client->Request.State )
    {
    case RI_PARSE_ERROR:
        // The request couldn't be parsed (comm. error or something similar)
        // --> Shutdown the connection.
        Client->Comm.Protocol   = HTTP_1_0;
        Client->Comm.ConnStatus = CONN_ERROR;
        ToDo->What       = DO_SHUTDOWN;
        ToDo->Which.Code = CLOSE_CLIENT_ONLY;
        break;
    case RI_PARSE_OK:
        // Check for Protocol Version of Client
        //   May be a HTTP/0.9 request?
        if( Client->Request.Line.Version.Major <= 0 )
        {
            // Yes -- we don't support it, would be too much trouble ahead...
            ToDo->What       = DO_SEND_ERROR;
            ToDo->Which.Code = STATUS_HTTP_Version_Not_Supported;
            return;
        }
        else if( Client->Request.Line.Version.Major == 1 )
        {
            Client->Comm.Protocol   = HTTP_1_0;
        }
        else
        {
            ToDo->What       = DO_SEND_ERROR;
            ToDo->Which.Code = STATUS_HTTP_Version_Not_Supported;
            return;
        }

        // Check for Content-Length and if available, then ReadBody()
        Client->Request.BodySize = GetContentLength( Client->Request.First );
        if( Client->Request.BodySize > 0 )
        {
            //LogMsg( LG_SERVER, DEBUG, "DEBUG: InterpreteRequest() -> ReadBody()." );
            Client->Request.BodyBuff = ReadBody( Client->SockFD, Client->Request.BodySize );
        }
        // Get Connection Status //### currently not really...
        if( TRUE )
            Client->Comm.ConnStatus = CONN_CLOSE;

        // Check for supported Methods
        if( Client->Request.Line.Method != MKEY_POST )
        {
            // No supported Method found...
            //LogMsg( LG_SERVER, DEBUG, "DEBUG: No post request!" );
            ToDo->What       = DO_SEND_ERROR;
            ToDo->Which.Code = STATUS_Method_Not_Allowed;
        }
        // Send response
        else
        {
            ToDo->What       = DO_SEND_RESPONSE;
            ToDo->Which.Code = REQU_OK;
        }
        break;
    default:
        // Strange State...
        ErrorMsg( E_PRIV, ERROR,
                  "ERROR: InterpreteRequest(): Request can't be handled!" );
        Client->Comm.ConnStatus = CONN_ERROR;
        ToDo->What       = DO_SHUTDOWN;
        ToDo->Which.Code = CLOSE_CLIENT_ONLY;
        break;
    }
}



char *ComposeErrorResponse( int Errno, int ClientType )
{
    char   *Msg;
    char   *Buff;
    size_t  BuffSize = BUFFSIZE;

    if( ( Msg = (char*)mymalloc( BuffSize ) ) == NULL )
    {
        ErrorMsg( E_SYS, ERROR,
                  "ERROR: ComposeErrorMsg() - malloc() error for Msg Buffer buffer." );
    }
    BuffSize -= 1;
    Buff = Msg;

    switch( ClientType )
    {
    case BROWSER:
        /* Fehlermeldungen fuer HTML-Browser */
        SNPrintf( Buff, &BuffSize, "<BR><H1>Error %d: ", Errno );
        Buff = Msg + strlen( Msg );
        switch( Errno )
        {
        case REQU_UNKNOWN_PARAMETER:
            SNPrintf( Buff, &BuffSize, "Unknown protocol parameter." );
            break;
        default:
            SNPrintf( Buff, &BuffSize, "Undefined protocol error." );
            break;
        }
        break;

    case RASCLIENT:
    default:
        /* Fehlermeldungen fuer RasClients */
        SNPrintf( Buff, &BuffSize, "%d+%d+%d", RESULT_ERROR, Errno, 0 );
        break;
    }

    //LogMsg( LG_SERVER, DEBUG, "DEBUG:  ComposeErrorMsg() returns %s", Msg );
    return Msg;
}



void SendResponse( struct ClientBase *Client )
{

    /*
    LogMsg( LG_SERVER, DEBUG, "DEBUG:   SendResponse() : %s",
      Client->Response.Body );
    */

    SendHTTPMsg( Client->SockFD, &Client->Response );
}


/*********************************************************************
 *
 * This function interpretes the body of an incoming request
 * and fills the RequestInfo struct accordingly.
 *
 *********************************************************************/
rc_t GetHTTPRequest( char *Source, int SourceLen, struct HTTPRequest *RequestInfo)
{
    char *Buffer = NULL;
    char *Input;

    Input = (char*)mymalloc( SourceLen + 1 );
    memcpy( Input, Source, SourceLen );
    Input[SourceLen] = '\0';
    // Read the message body and check for the post parameters
    Buffer = strtok( Input, "=" );
    while( Buffer != NULL )
    {
        if( strcmp(Buffer,"Database") == 0 )
        {
            RequestInfo->Database = strdup(strtok( NULL, "&" ));
            //LogMsg( LG_SERVER, DEBUG, "DEBUG: Parameter Database is %s", RequestInfo->Database );
            Buffer = strtok( NULL, "=" );
        }
        else if( strcmp(Buffer,"QueryString") == 0 )
        {
            RequestInfo->QueryString = strdup(strtok( NULL, "&" ));
            //LogMsg( LG_SERVER, DEBUG, "DEBUG: Parameter QueryString is %s", RequestInfo->QueryString );
            Buffer = strtok( NULL, "=" );
        }
        else if( strcmp(Buffer,"Capability") == 0 )
        {
            RequestInfo->Capability = strdup(strtok( NULL, "&\0" ));
            //LogMsg( LG_SERVER, DEBUG, "DEBUG: Parameter Capability is %s", RequestInfo->Capability );
            Buffer = strtok( NULL, "=" );
        }
        else if( strcmp(Buffer,"ClientID") == 0 )
        {
            RequestInfo->ClientID = strdup(strtok( NULL, "&" ));
            //LogMsg( LG_SERVER, DEBUG, "DEBUG: Parameter ClientID is %s", RequestInfo->ClientID );
            Buffer = strtok( NULL, "=" );
        }
        else if( strcmp(Buffer,"Command") == 0 )
        {
            RequestInfo->Command = atoi( strtok( NULL, "&" ) );
            //LogMsg( LG_SERVER, DEBUG, "DEBUG: Parameter Command is %i", RequestInfo->Command );
            Buffer = strtok( NULL, "=" );
        }
        else if( strcmp(Buffer,"Endianess") == 0 )
        {
            RequestInfo->Endianess = atoi( strtok( NULL, "&" ) );
            //LogMsg( LG_SERVER, DEBUG, "DEBUG: Parameter Endianess is %i", RequestInfo->Endianess );
            Buffer = strtok( NULL, "=" );
        }
        else if( strcmp(Buffer,"NumberOfQueryParameters") == 0 )
        {
            RequestInfo->NumberOfQueryParams = atoi( strtok( NULL, "&" ) );
            //LogMsg( LG_SERVER, DEBUG, "DEBUG: Parameter NumberOfQueryParams is %i", RequestInfo->NumberOfQueryParams );
            Buffer = strtok( NULL, "=" );
        }
        else if( strcmp(Buffer,"BinDataSize") == 0 )
        {
            RequestInfo->BinDataSize = atoi( strtok( NULL, "&" ) );
            //LogMsg( LG_SERVER, DEBUG, "DEBUG: Parameter BinDataSize is %i", RequestInfo->BinDataSize );
            Buffer = strtok( NULL, "=" );
        }
        else if( strcmp(Buffer,"BinData") == 0 )
        {
            // This parameter has to be the last one!
            RequestInfo->BinData = (char*)mymalloc( RequestInfo->BinDataSize );
            memcpy(RequestInfo->BinData, Source + (SourceLen-RequestInfo->BinDataSize), RequestInfo->BinDataSize );
            //set Buffer to NULL => exit this while block
            Buffer = NULL;
        }
        else if( strcmp(Buffer,"ClientType") == 0 )
        {
            Buffer = strtok( NULL, "&" );
            //LogMsg( LG_SERVER, DEBUG, "DEBUG: Parameter Type is %s", Buffer );
            /* BROWSER? */
            if( strcmp(Buffer,"BROWSER") == 0 )
                RequestInfo->ClientType = 1;
            /* Rasclient? */
            else if( strcmp(Buffer,"RASCLIENT") == 0 )
                RequestInfo->ClientType = 2;
            /* Sonstiges */
            else
            {
                //LogMsg( LG_SERVER, DEBUG, "DEBUG: Unknown Parameter %s", Buffer );
                return REQU_UNKNOWN_CLIENT;
            }
            Buffer = strtok( NULL, "=" );
        }
        else
            return REQU_UNKNOWN_PARAMETER;
    }

    free(Input);
    return REQU_OK;
}


/********************************************************
 *
 * Analyses the client request (using the "GetHTTPRequest" function)
 * and executes the query (function "processQuery").
 * The result is written into the client response structure.
 *
 **********************************************************/
void InterpretePOSTRequest ( struct ClientBase *Client )
{
    int                  result = 0;
    struct HTTPRequest   RequestInfo;
    char                *tmp;

    /* Initialize RequestInfo */
    RequestInfo.Database = NULL;
    RequestInfo.QueryString = NULL;
    RequestInfo.ClientType = 0;
    RequestInfo.ClientID = NULL;
    RequestInfo.Command = 0;
    RequestInfo.Endianess = 0;
    RequestInfo.NumberOfQueryParams = 0;
    RequestInfo.BinDataSize = 0;
    RequestInfo.BinData = NULL;
    RequestInfo.Capability = NULL;

    //LogMsg( LG_SERVER, DEBUG, "DEBUG: InterpretePostRequest() ..." );
    //LogMsg( LG_SERVER, DEBUG, "DEBUG: Request.BodySize is %d",Client->Request.BodySize );

    /* get the necessary parameters */
    result = GetHTTPRequest( Client->Request.BodyBuff, Client->Request.BodySize, &RequestInfo );

    if( result == REQU_OK )
    {
        // here the query is actually executed
        char* queryResult;
        long resultLen;
        ServerComm* scObject = ServerComm::actual_servercomm;

        // the cast should be save, function only called on HTTP requests
        //LogMsg( LG_SERVER, DEBUG, "DEBUG: Vor processRequest ..." );
        resultLen = ((HttpServer*)scObject)->processRequest(1, RequestInfo.Database, RequestInfo.Command,
                    RequestInfo.QueryString, RequestInfo.BinDataSize,
                    RequestInfo.BinData, RequestInfo.Endianess,
                    queryResult,RequestInfo.Capability);

        //LogMsg( LG_SERVER, DEBUG, "DEBUG: ... nach processRequest." );

        // Note that this has to be freed!!!!!
        Client->Response.Body = queryResult;
        Client->Response.BodySize = resultLen;


        Client->ClientType = RequestInfo.ClientType;
        //LogMsg( LG_SERVER, DEBUG, "DEBUG: ResponseBody is %s", Client->Response.Body );
    }
    else
    {
        //LogMsg( LG_SERVER, DEBUG, "DEBUG: InterpretePostRequest(): send error response. " );
        Client->Response.Body        = ComposeErrorResponse( result, RequestInfo.ClientType );
        Client->Response.BodySize    = strlen( Client->Response.Body );
        Client->ClientType           = RequestInfo.ClientType;
        //LogMsg( LG_SERVER, DEBUG, "DEBUG: ResponseBody is %s", Client->Response.Body );
    }

    // free RequestInfo
    free(RequestInfo.Database);
    free(RequestInfo.QueryString);
    free(RequestInfo.Capability);
    free(RequestInfo.BinData);
    free(RequestInfo.ClientID);

}



void CreateRasResponse( struct HTTPMode *Mode, struct ClientBase *Client )
{
    char   *Head;
    char   *Buff;
    size_t  BuffSize = BUFFSIZE;
    char    MDate[30];



    //LogMsg( LG_SERVER, DEBUG, "DEBUG: Response Header begin, malloc BUFFSIZE is %d", BUFFSIZE );

    HTTP_Date( MDate, 30 );

    if( ( Head = (char*)mymalloc( BuffSize ) ) == NULL )
    {
        ErrorMsg( E_SYS, ERROR,
                  "ERROR: CreateRasResponse() - malloc() error for head buffer." );
    }

    BuffSize -= 1;
    Buff = Head;
    bzero( Head, BuffSize );
    CreateStatusLine( Buff, &BuffSize, STATUS_OK, Mode->Protocol );

    /*  General Message Header  */
    Buff = Head + strlen( Head );
    SNPrintf( Buff, &BuffSize, "Date: %s\r\n", MDate );
    Buff = Head + strlen( Head );
    SNPrintf( Buff, &BuffSize, "Connection: close\r\n" );

    /*  Response Message Header  */
    Buff = Head + strlen( Head );
    SNPrintf( Buff, &BuffSize, SERVERFIELD );

    /*  Entity Message Header    */
    Buff = Head + strlen( Head );
    switch( Client->ClientType )
    {
    case BROWSER:
        SNPrintf( Buff, &BuffSize, "Content-Type: text/html\r\n" );
        break;
    case RASCLIENT:
        SNPrintf( Buff, &BuffSize, "Content-Type: application/octet-stream\r\n" );
        break;
    default:
        SNPrintf( Buff, &BuffSize, "Content-Type: application/octet-stream\r\n" );
        break;
    }
    Buff = Head + strlen( Head );

    //LogMsg( LG_SERVER, DEBUG, "DEBUG: ClientResponse BodySize is %d", Client->Response.BodySize );

    SNPrintf( Buff, &BuffSize, "Content-Length: %d\r\n", Client->Response.BodySize );
    Buff = Head + strlen( Head );
    SNPrintf( Buff, &BuffSize, "Last-Modified: %s\r\n", MDate );
    Buff = Head + strlen( Head );
    SNPrintf( Buff, &BuffSize, "\r\n" );

    Client->Response.Head = Head;
    //LogMsg( LG_SERVER, DEBUG, "DEBUG: Response Header is %s", Client->Response.Head );

}



void WriteAccessLog( struct ClientBase *Client )
{
    char TimeString[ DATE_BUFFSIZE ];
    struct hostent *Host;
    char  *Hostname = NULL;

    bzero( TimeString, DATE_BUFFSIZE );

    if( (int)Client->Host.IPAddress != -1 )
    {
        Host = gethostbyaddr( (const char *)&Client->Host.IPAddress,
                              sizeof( Client->Host.IPAddress ), AF_INET );
        if( Host != NULL )
            Hostname = Host->h_name;
        else
            Hostname = Client->Host.IPAddrString;
    }
    else
        Hostname = Client->Host.IPAddrString;

    LogDate( TimeString, DATE_BUFFSIZE );
    LogMsg( LG_ACCESS, INFO, "%s - - %s \"%s\" %d %d\n",
            Hostname,
            TimeString,
            Client->Request.Line.Vanilla,
            Client->RespStatus,
            Client->Response.BodySize );
}




