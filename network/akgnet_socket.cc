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
 * SOURCE: akgnet_socket.cc
 *
 * MODULE: akg network
 * CLASS:  Socket, ListenSocket, ServerSocket, ClientSocket
 *
 * COMMENTS:
 *
 */

#include <assert.h>
#include <akgnet_socket.hh>

akg::Socket::Socket() throw()
{
}

bool akg::Socket::createTcpSocket() throw()
{
    struct protoent *getprotoptr = getprotobyname("tcp");
    fileDescriptor= socket(PF_INET,SOCK_STREAM,getprotoptr->p_proto);
    if(fileDescriptor<0)
    {
        saveErrno();
        return false;
    }
    return true;
}

akg::SocketAddress akg::Socket::getAddress() throw()
{
    akgSocklen_t size = sizeof(sockaddr_in);
    sockaddr_in buffer;

    getsockname(fileDescriptor, (sockaddr*)&buffer,&size);

    return SocketAddress(buffer);
}

akg::SocketAddress akg::Socket::getPeerAddress() throw()
{
    akgSocklen_t size = sizeof(sockaddr_in);
    sockaddr_in buffer;

    int rasp=getpeername(fileDescriptor, (sockaddr*)&buffer,&size);
    saveErrno();
    return rasp != -1 ? SocketAddress(buffer) : SocketAddress();
}

//###########################################################

akg::ListenSocket::ListenSocket() throw()
{
    queuesize = SOMAXCONN;
}

akg::ListenSocket::~ListenSocket() throw()
{
}

bool akg::ListenSocket::open(int port) throw()
{
    close();
    if(createTcpSocket() == false) return false;

    struct sockaddr_in internetAddress;
    internetAddress.sin_family     = AF_INET;
    internetAddress.sin_port       = htons(port);
    internetAddress.sin_addr.s_addr= htonl(INADDR_ANY);

#ifdef SO_REUSEADDR
    int val = 1;
    int len = sizeof( val );
    if(setsockopt( fileDescriptor, SOL_SOCKET, SO_REUSEADDR, (char*)&val, len ))
    {
        DBTALK("Can't set address reusable: "<<strerror(errno));
    }
#endif

    if(bind(fileDescriptor,(sockaddr*)&internetAddress,sizeof(sockaddr_in)) <0)
    {
        saveErrno();
        return false;
    }

    if(listen(fileDescriptor,queuesize) < 0)
    {
        saveErrno();
        return false;
    }

    return true;
}

void akg::ListenSocket::setQueueSize(int newSize) throw()
{
    assert(newSize > 0);
    queuesize = newSize < SOMAXCONN ? newSize : SOMAXCONN;
}

int  akg::ListenSocket::getQueueSize() throw()
{
    return queuesize;
}


//###########################################################
akg::ServerSocket::ServerSocket() throw()
{
}

akg::ServerSocket::~ServerSocket() throw()
{
}

bool akg::ServerSocket::acceptFrom(ListenSocket& listenSocket) throw()
{
    close();
    struct sockaddr_in internetAddress;
    akgSocklen_t size=sizeof(sockaddr_in);

    savedErrno = 0;
    fileDescriptor=accept(listenSocket(),(struct sockaddr*)&internetAddress,&size);
    if(fileDescriptor < 0)
    {
        saveErrno();
        return false;
    }

    return true;
}

//###########################################################

akg::ClientSocket::ClientSocket() throw()
{
}
akg::ClientSocket::~ClientSocket() throw()
{
}

bool akg::ClientSocket::open(const char *serverHost,int serverPort) throw()
{

    close();
    savedErrno=0;

    if(createTcpSocket() == false) return false;

    struct hostent *hostinfo=gethostbyname(serverHost);
    if(hostinfo==NULL)
    {
        saveErrno();
        return false;
    }

    sockaddr_in internetAddress;

    internetAddress.sin_family = AF_INET;
    internetAddress.sin_port   = htons(serverPort);
    internetAddress.sin_addr   = *(struct in_addr*)hostinfo->h_addr;

    if(connect(fileDescriptor,(struct sockaddr*)&internetAddress,sizeof(sockaddr_in)) < 0)
    {
        saveErrno();
        return false;
    }
    return true;
}

