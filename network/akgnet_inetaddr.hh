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
 * INCLUDE: akgnet_inetaddr.hh
 *
 * MODULE:  akg network
 * CLASS:   HostAddress, SocketAddress
 *
 * COMMENTS:
 * Namespace akg
 *
*/
#ifndef AKGNET_INETADDR_HH
#define AKGNET_INETADDR_HH

#include "akgnet_common.hh"


namespace akg
{

/** This class represents the internet address of a computer and envelops
    the OS data structure 'in_addr'
       Important: new is supposed to throw
*/


/**
  * \ingroup Networks
  */

class HostAddress
{
public:
    static const uint32_t addrLocalhost = INADDR_LOOPBACK;
    static const uint32_t addrBroadcast = INADDR_BROADCAST;
    static const uint32_t addrAny       = INADDR_ANY;
    static const uint32_t addrNone      = INADDR_NONE;

    /// Default constructor, creating an 'invalid' object
    HostAddress() throw();

    /// Copy constructor
    HostAddress(const HostAddress&);

    /// Constructor taking a 32-bit internet address
    HostAddress(uint32_t);

    /** Constructor taking a string representation of the address
        It can be the the name or the internet address
    Assert: theHostName != 0
    */
    HostAddress(const char* theHostName);

    /// Destructor
    ~HostAddress() throw();

    /// Returns true if the object was initialized correctly
    bool        isValid() const throw();

    /// Returns the full host name, meaning hostname.domainname
    const char* getFullHostName() const throw();

    /// Returns the short form of the host name
    const char* getShortHostName() const throw();

    /// Returns the IP-Address as a 32-bit value
    uint32_t    getAddress() const throw();

    /// Returns the string representation of the IP-Address
    const char* getStringAddress() const throw();

private:
    /// Initializes the object with default values. Used by the constructors
    void initDefault() throw();

    /// Initializes the object from a OS 'hostent' object. Used by the constructors
    bool init(hostent *);

    char *fullHostName;
    char *shortHostName;
    char *strAddress;
    in_addr address;

    ///unimplemented
    HostAddress operator=(const HostAddress&);

};


/** This class represents the IP address of a OS socket and envelops
    the OS data structure 'sockaddr_in'
*/

/**
  * \ingroup Networks
  */

class SocketAddress
{
public:
    /// Default constructor
    SocketAddress() throw();

    /// Constructor taking a 'sockaddr_in'
    SocketAddress(sockaddr_in&) throw();

    /// Initialization from a 'sockaddr_in'
    void        init(sockaddr_in&) throw();

    /// Returns true if the object is initialized
    bool        isValid() const throw();

    /// Returns the HostAddress of this socket
    HostAddress getHostAddress() const throw();

    /// Returns the IP Address
    uint32_t    getAddress() const throw();
    int         getPort() const throw();
private:
    bool valid;
    void clear() throw();

private:
    sockaddr_in address;
    /*
       sa_family_t sin_family
    struct inaddr
        { uint32_t s_addr;
         }
                    sin_addr
     ushort         sin_port
         */
};

} // namespace
#endif
