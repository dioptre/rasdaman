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
 * INCLUDE: akgnet_common.hh
 *
 * MODULE:  akg network
 * CLASS:   
 *
 * COMMENTS:
 * Namespace akg
 * Contains common definitions for the whole package
 * 
*/

#ifndef AKGNET_COMMON_HH
#define AKGNET_COMMON_HH

#if defined(DECALPHA) || defined(LINUX)
 #ifndef _XOPEN_SOURCE_EXTENDED
  #define _XOPEN_SOURCE_EXTENDED // for gethostid
 #endif //_XOPEN_SOURCE_EXTENDED
#endif //DECALPHA || LINUX


#include<stdio.h>
#include<stdlib.h>
#include<errno.h>

#if defined(DECALPHA)
  #include<strings.h>
  #include <arpa/inet.h>
#endif
#include<string.h>



#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include <fcntl.h>
#include <sys/time.h>
#include <time.h>


#include <iostream>

//##### System dependent data types #############

#ifdef X86 
 #define akgSocklen_t socklen_t
#endif

#ifdef AIX
 #define akgSocklen_t socklen_t
#endif

#ifdef SOLARIS
 #define akgSocklen_t socklen_t
 #define INADDR_NONE  ((uint32_t) 0xffffffff)
#endif

#ifdef DECALPHA 
 #define akgSocklen_t size_t
 typedef in_addr_t  uint32_t;
#endif 

#ifndef akgSocklen_t
 #error "What Operating System is this?"
#endif
//##### Debugging stuff ########################

#define DBTALK(a)
//#define DBTALK(a) cout<<a<<endl

using namespace std;
//##############################################

#endif
