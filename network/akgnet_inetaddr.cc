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
 * SOURCE: akgnet_inetaddr.cc
 *
 * MODULE: akg network
 * CLASS:  HostAddress, SocketAddress
 *
 * COMMENTS:
 *
*/

#include <akgnet_inetaddr.hh>
#include <arpa/inet.h>
#include <string.h>
#include <assert.h>

akg::HostAddress::HostAddress() throw()
  {
    initDefault();
   }

akg::HostAddress::HostAddress(uint32_t x)
  {
    initDefault();
    
    address.s_addr = htonl(x);
    struct hostent *host = gethostbyaddr((const char*)&address, sizeof(in_addr), AF_INET);
    init(host);
   }
   
akg::HostAddress::HostAddress(const char *theHostName)
  {
    assert(theHostName != 0);
    
    initDefault();
    
    struct hostent *host = gethostbyname (theHostName);
    init(host);
   }
   
akg::HostAddress::HostAddress(const akg::HostAddress &ha)
  {
    fullHostName   = new char[strlen(ha.fullHostName) + 1];
    strcpy(fullHostName, ha.fullHostName);
    
    shortHostName  = new char[strlen(ha.shortHostName) + 1];
    strcpy(shortHostName, ha.shortHostName);
    
    strAddress     = new char[strlen(ha.strAddress) + 1];
    strcpy(strAddress, ha.strAddress);
   }
akg::HostAddress::~HostAddress() throw()
  {
    if(fullHostName)  delete[] fullHostName;
    if(shortHostName) delete[] shortHostName;
    if(strAddress)    delete[] strAddress;
   }

bool akg::HostAddress::isValid() const throw()
  {
    return address.s_addr == addrNone ? false:true;
   }

void akg::HostAddress::initDefault() throw()
  {
    fullHostName   = NULL;
    shortHostName  = NULL;
    strAddress     = NULL;
    address.s_addr = addrNone;
   }

// New is supposed to throw
bool akg::HostAddress::init(hostent *host)
  {
    if(host == NULL) return false;
    
    in_addr *ptr = (in_addr*)host->h_addr_list[0];
    if(host->h_name == NULL || ptr == NULL) return false;
        
    fullHostName = new char[ strlen(host->h_name) +1];
    strcpy(fullHostName,host->h_name);
   
    char *dotPos = strchr(fullHostName,'.');
    int copyLen = dotPos ? dotPos-fullHostName : strlen(fullHostName);
    
    shortHostName = new char[copyLen+1];
    strncpy(shortHostName,fullHostName,copyLen);
    shortHostName[copyLen] = 0;
    
    char *nta = inet_ntoa(*ptr);
    strAddress = new char[strlen(nta) + 1];
    strcpy(strAddress,nta);
    address = *ptr;
    
    return true;    
   }
      
const char* akg::HostAddress::getFullHostName() const throw()
  {
    return fullHostName;
   }

const char* akg::HostAddress::getShortHostName() const throw()
  {
    return shortHostName;
   }
   
uint32_t akg::HostAddress::getAddress() const throw()
  {
    return ntohl(address.s_addr);
   }
   
const char* akg::HostAddress::getStringAddress() const throw()
  {
    return strAddress;
   }

//############################################################
            
akg::SocketAddress::SocketAddress() throw()
  { 
    clear();
   }
  
akg::SocketAddress::SocketAddress(sockaddr_in &x) throw()
  {
    init(x);
   }
   
void akg::SocketAddress::init(sockaddr_in &x) throw()
  {
    valid = true;
    address = x;
   }
      
bool akg::SocketAddress::isValid() const throw()
  {
    return valid;
   }
void akg::SocketAddress::clear() throw()
  {
    valid = false;
    address.sin_family = AF_INET;
   }
      
akg::HostAddress akg::SocketAddress::getHostAddress() const throw()
  {
    return valid ? HostAddress(getAddress()) : HostAddress();
   }
   
uint32_t akg::SocketAddress::getAddress() const throw()
  {
    return ntohl(address.sin_addr.s_addr);
   }
   
int akg::SocketAddress::getPort() const throw()
  {
    return ntohs(address.sin_port);
   }
    
