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
/
/**
 * SOURCE: rasmgr_comm.cc
 *
 * MODULE: rasmgr
 * CLASS:  HTTPComm
 *
 * PURPOSE:
 *   Performs reliable, but blocking HTTP communication. used by the slave rasmgr
 *
 * COMMENTS:
 *   Will be removed, the plan is to have only non-blocking communication
 *
*/

#include "rasmgr_comm.hh"

#ifdef X86
#define r_Socklen_t socklen_t
#endif

#ifdef AIX
#define r_Socklen_t socklen_t
#endif

#ifdef SOLARIS
#define r_Socklen_t socklen_t
#endif

#ifdef DECALPHA
#define r_Socklen_t int
#endif

#include "debug-srv.hh"
#include "raslib/rminit.hh"


HTTPComm::HTTPComm()
{
    //parentPID=getpid();
    listen_socket=-1;
    exitRequest=false;;
}
HTTPComm::~HTTPComm()
{
    ENTER("HTTPComm::~HTTPComm: enter." );
    closeListenSocket();
    LEAVE("HTTPComm::~HTTPComm: leave." );
}

void HTTPComm::closeListenSocket()
{
    ENTER("HTTPComm::closeListenSocket: enter." );

    if(listen_socket>=0) close(listen_socket);

    LEAVE("HTTPComm::closeListenSocket: leave." );
}

int HTTPComm::sendAnswer(int socket,int len)
{
    int result = 0;

    ENTER("HTTPComm::sendAnswer: enter." << std::endl );

    int write_count=writeWholeMessage(socket,outBuffer,len);

    // adapted logic to single point of return -- PB 26-may-2003
    if(write_count<0)
    {
        TALK( "HTTPComm::sendAnswer: Error writing answer" );
        result = -1;
    }
    else
        result = 0;

    LEAVE("HTTPComm::sendAnswer: leave. result=" << result );
    return result;
}

int HTTPComm::getMessage()
{
    ENTER("HTTPComm::getMessage: enter." << std::endl );

    int socket=realGetMessage();

    if(socket>0)
    {
        header=inBuffer;
        body=strstr(inBuffer,"\r\n\r\n");
        if(body!=NULL)
        {
            *body=0;
            body+=4;
        }
        else
        {
            close(socket);
            socket = -1;
        }
    }

    LEAVE("HTTPComm::getMessage: leave. socket=" << socket );
    return socket;
}


int HTTPComm::realGetMessage()
{
    struct sockaddr_in clientname;
    r_Socklen_t size=sizeof(clientname);

    ENTER("HTTPComm::getRealMessage: enter." );

    int socket=accept(listen_socket,(struct sockaddr*)&clientname,&size);
    if(socket<0)
    {
        TALK( "HTTPComm::realGetMessage: Error accepting connection.");
        socket = -1;    // normalize error feedback
    }

    if (socket >= 0)    // accept() worked fine, so wa can continue
    {
        int read_count=readWholeMessage(socket,inBuffer,MAXMSG);

        if(read_count<0)
        {
            TALK( "HTTPComm::realGetMessage: Error reading message."<<std::endl );
            close(socket);
            socket = -1;
        }
    }

    LEAVE("HTTPComm::getRealMessage: leave. socket=" << socket );
    return socket;
}

int HTTPComm::initListenSocket(int port)
{
    int queuesize=SOMAXCONN; // the maximim number allowed by SO!!

    ENTER("HTTPComm::initListenSocket: enter. port=" << port );

    FD_ZERO(&active_fd_set);

    listen_socket=makeSocket(port);
    if(listen_socket<0)
    {
        TALK("HTTPComm::initListenSocket: makeSocket failed, errno=" << errno );
        exitbyerror("listen"); // it's OK to exit, we didn't start yet
    }

    if(listen(listen_socket,queuesize)<0)
    {
        TALK("HTTPComm::initListenSocket: listen failed, errno=" << errno );
        exitbyerror("listen"); // it's OK to exit, we didn't start yet
    }

    FD_SET(listen_socket,&active_fd_set);

    LEAVE("HTTPComm::initListenSocket: leave. socket=" << socket );
    return 0; // no error

}

int HTTPComm::makeSocket(int port)
{
    int sock;
    struct sockaddr_in name;
    struct protoent *getprotoptr;

    ENTER("HTTPComm::makeSocket: enter. port=" << port );

    getprotoptr=getprotobyname("tcp");
    sock=socket(PF_INET,SOCK_STREAM,getprotoptr->p_proto);
    if(sock<0)
    {
        TALK("HTTPComm::makeSocket: socket failed. errno=" << errno );
        exitbyerror("make socket");
    }

    name.sin_family=AF_INET;
    name.sin_port=htons(port);
    name.sin_addr.s_addr=htonl(INADDR_ANY);

#ifdef SO_REUSEADDR
    int val = 1;
    int len = sizeof( val );
    if(setsockopt( sock, SOL_SOCKET, SO_REUSEADDR, (char*)&val, len ))
    {
        TALK( "HTTPComm::makeSocket: Can't set address reusable: "<< strerror(errno) );
    }
#endif

    int sockResult = bind(sock,(sockaddr*)&name,sizeof(name));
    TALK( "HTTPComm::makeSocket: bind() with socket=" << sock << ", name.sin_port="<< name.sin_port << " returned " << sockResult );
    if(sockResult < 0)
    {
        TALK( "HTTPComm::makeSocket: bind failed: "<< strerror(errno) );
        exitbyerror("bind");
        // This is OK to exit, program just starts and we can't have an address
    }

    ENTER("HTTPComm::makeSocket: leave. socket=" << sock << std::endl );
    return sock;
}

void HTTPComm::shouldExit()
{
    exitRequest=true;
}

bool HTTPComm::isMessage(const char *messageStart)
{
    ENTER("HTTPComm::isMessage: enter. messageStarte=" << messageStart );

    bool rasp= (strncasecmp(header,messageStart,strlen(messageStart))==0) ? true:false;
    if(rasp)
    {
        TALK("HTTPComm::isMessage: (b) Message=" << messageStart );
    }

    LEAVE("HTTPComm::isMessage: leave. result=" << rasp );
    return rasp;
}

//
int readWholeMessage(int socket,char *destBuffer,int buffSize)
{
    ENTER("HTTPComm::readWholeMessage: enter. socket=" << socket << ", destBuffer=" << destBuffer << ", buffSize=" << buffSize );

    // we read what is comming in until we encounter a '\0'
    // this is our end-sign.
    int totalLength=0;
    int redNow;
    while(1)
    {
        redNow = read(socket,destBuffer+totalLength,buffSize-totalLength);
        if(redNow == -1)
        {
            if(errno == EINTR) continue; // read was interrupted by signal

            TALK("HTTPComm::readWholeMessage: read error. errno=" << errno );
            return -1; // another error
        }
        totalLength+=redNow;

        if(destBuffer[totalLength-1]==0) break; // THE END
    }

    LEAVE("HTTPComm::readWholeMessage: leave. totalLength=" << totalLength );
    return totalLength;
}

int writeWholeMessage(int socket,char *destBuffer,int buffSize)
{
    ENTER("HTTPComm::writeWholeMessage: enter. socket=" << socket << ", destBuffer=" << destBuffer << ", buffSize=" << buffSize );

    // we write the whole message, including the ending '\0', which is already in
    // the buffSize provided by the caller
    int totalLength=0;
    int writeNow;
    while(1)
    {
        writeNow = write(socket,destBuffer+totalLength,buffSize-totalLength);
        if(writeNow == -1)
        {
            if(errno == EINTR) continue; // read was interrupted by signal

            TALK("HTTPComm::writeWholeMessage: read error. errno=" << errno );
            return -1; // another error
        }
        totalLength+=writeNow;

        if( totalLength==buffSize ) break; // THE END
    }

    LEAVE("HTTPComm::writeWholeMessage: leave. totalLength=" << totalLength );
    return totalLength;
}

