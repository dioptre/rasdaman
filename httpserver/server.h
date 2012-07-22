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
/*  server.h - server structures.                                         */
/*------------------------------------------------------------------------*/


#ifndef _SERVER_H
#define _SERVER_H

#include    <sys/types.h>
#include    <unistd.h>
#include    <limits.h>
#include    <errno.h>

#include    <stdio.h>
#include    <stdlib.h>
#include    <stdarg.h>

#include    <signal.h>
#include    <sys/stat.h>
#include    <sys/wait.h>

#ifndef AIX
#include    <sys/fcntl.h>
#endif

#include    <fcntl.h>
#include    <string.h>
#include    <strings.h>
#include    <sys/time.h>
#include    <time.h>

#include    <sys/socket.h>
#include    <netinet/in.h>
#include    <arpa/inet.h>
#include    <netdb.h>

#include    "types.h"
#include    "http.h"

struct HTTPRequest
{
    char *Database;
    int   Command;
    char *QueryString;
    int   ClientType;
    char *ClientID;
    int   Endianess;
    int   NumberOfQueryParams;
    char *BinData;
    int   BinDataSize;
    char *Capability;
};

union WhichArg
{
    int          Code;
};

struct ToDoArgs
{
    int            What;
    union WhichArg Which;
};

struct FDsets
{
    int       MaxFD;
    fd_set    Read;
    fd_set    Write;
};

struct LogFile
{
    char     *Filename;
    int       FD;
    int       State;
};


struct Logging
{
    int                Mode;
    struct LogFile     Server;
    struct LogFile     Access;
    struct LogFile     Comm;
};

struct ChildBase
{
    struct ChildBase *next;
    struct ChildBase *prev;
    pid_t             PId;
    int               PD[2];
    int               PipeStatus;
};


struct Host
{
    char             *Name;
    char              IPAddrString[ 16 ];
#ifdef NO_in_addr_t
    unsigned long     IPAddress;
#else
    in_addr_t         IPAddress;
#endif
};


struct HTTPMode
{
    int Protocol;
    int ConnStatus;
};


struct ClientBase
{
    /*   Client Host Infos    */
    struct Host        Host;
    int                ClientType;
    /*        Socket          */
    int                SockFD;
    struct sockaddr_in Socket;
    int                SockSize;
    /*    select() timeout    */
    struct timeval     TimeOut;

    struct HTTPMode    Comm;
    /*        Request         */
    struct ReqInfo     Request;
    /*        Response        */
    struct HTTPMsg     Response;
    int                RespStatus;
    /* Pipe to Parent process */
    int                Pipe;
    char               PipeBuffer[PIPE_BUFFSIZE];
};


struct ServerBase
{
    pid_t              PId;
    /* -- Config.Information  */
    struct Host        Host;
    int                Port;
    char              *AdminMailAddress;
    char              *Directory;
    char              *ConfigFile;
    char              *PidFile;
    //char              *CacheFile;
    size_t             MaxURLLength;
    /* -- Status Information  */
    int                Status;
    struct ChildBase  *ChildList;
    struct FDsets      PipeSets;
    /* -- Global Data         */
    struct Logging     Log;
    struct CacheNode  *Cache;
    /* -- Server Socket       */
    int                SockFD;
    struct sockaddr_in Socket;
    /* -- Client Information  */
    struct ClientBase  Client;
};


#endif  /*  _SERVER_H not defined  */
