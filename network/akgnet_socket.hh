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
 * INCLUDE: akgnet_socket.hh
 *
 * MODULE:  akg network
 * CLASS:   Socket, ListenSocket, ServerSocket, ClientSocket
 *
 * COMMENTS:
 * Namespace akg
 *
 */

#ifndef AKGNET_SOCKET_HH
#define AKGNET_SOCKET_HH

#include "akgnet_fdescr.hh"
#include "akgnet_inetaddr.hh"

namespace akg
{

/** This class represents the sockets in the file descriptor hierarchie.
    Socket is the base class of the socket hierarchie, which contains also
    ListenSocket, ServerSocket and ClientSocket
*/

/**
  * \ingroup Networks
  */

class Socket : public FileDescriptor
{
public:
    /// Default constructor
    Socket() throw();

    /// Returns the SocketAddress of this socket
    SocketAddress getAddress() throw();

    /** Returns the SocketAddress of the peer. If the Socket is not connected
        returns the SocketAddress of this socket
    */
    SocketAddress getPeerAddress() throw();
protected:
    /// helper function to initialize this Socket as a TCP/IP socket
    bool createTcpSocket() throw();

private:
    /// unimplemented, objects of this type can't be copied
    Socket(const Socket&);
    /// unimplemented, objects of this type can't be copied
    Socket& operator=(const Socket&);
};

/** ListenSocket - socket for servers, to listen for clients
*/

/**
  * \ingroup Networks
  */

class ListenSocket : public Socket
{
public:
    /// Default constructor
    ListenSocket() throw();

    /// Destructor, closes, indirectly, the socket
    ~ListenSocket() throw();

    /// Opens the listen socket. Returns true if succes
    bool open(int port) throw();

    /** Sets the OS queue size for this socket. Maximal size is SOMAXCONN
        Assert: newSize > 0
    */
    void setQueueSize(int newSize) throw();

    /// Returns the OS queue size for this socket
    int  getQueueSize() throw();

private:
    int  queuesize;

    /// unimplemented, objects of this type can't be copied
    ListenSocket(const ListenSocket&);
    /// unimplemented, objects of this type can't be copied
    ListenSocket& operator=(const ListenSocket&);
};

/** ServerSocket - socket for servers, to communicate with clients
    It opens by accepting a pending connection from a ListenSocket
*/

/**
  * \ingroup Networks
  */

class ServerSocket : public Socket
{
public:
    /// Default constructor
    ServerSocket() throw();

    /// Destructor
    ~ServerSocket() throw();

    /// Accepts a pending connection from a ListenSocket. Returns true on succes
    bool acceptFrom(ListenSocket&) throw();
private:
    /// unimplemented, objects of this type can't be copied
    ServerSocket(const ServerSocket&);
    /// unimplemented, objects of this type can't be copied
    ServerSocket& operator=(const ServerSocket&);
};

/** ClientSocket - socket for clients, to communicate with servers
*/

/**
  * \ingroup Networks
  */

class ClientSocket : public Socket
{
public:
    /// Default constructor
    ClientSocket() throw();

    /// Destructor
    ~ClientSocket() throw();

    /// Opens the connection with the given server. Returns true on succes
    bool open(const char *serverHost,int serverPort) throw();
private:
    /// unimplemented, objects of this type can't be copied
    ClientSocket(const ClientSocket&);
    /// unimplemented, objects of this type can't be copied
    ClientSocket& operator=(const ClientSocket&);
};


} // namespace
#endif
