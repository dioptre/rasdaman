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
 * SOURCE: rasmgr.hh
 *
 * MODULE: rasmgr
 * CLASS:  
 *
 * PURPOSE:
 *   Used for including system headers and general defines
 *    
 * COMMENTS:
 * - Beware using VLOG in: 'if (...) VLOG...;' !
 *   do it like this:      'if (...) { VLOG...; }'
 *
*/

#ifndef RASMGR_HH
#define RASMGR_HH

#include<iostream>
#include<stdio.h>
#include<errno.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<signal.h>
#include<sys/wait.h>

#include<string.h>

#ifdef AIX
#include<strings.h>
#include<time.h>
#endif
 
#include<stdlib.h>
#include<fstream>
#include <fcntl.h>
#include<sys/time.h>

#include <vector>
#include <list>
#include <iterator>

#include "rasmgr_protocol.hh"			// protocol keyword definitions

// clear this def for production release!
// #define TALK(a) cout<<a<<endl << flush;
//#define TALK(a) { /* TALK (a) */ }

void exitbyerror(const char* text);

char *strtolwr(char*); // should be somewhere in the C-library, but can't find it

#ifdef RASMGR_IHC
  #define INCLUDE_HIDDEN_COMMANDS
#endif

// global defs -- PB 2003-jun-05
//------------

/// name of the machine where we lock into portmapper to be a Highlander:
#define HOSTNAME "localhost"
/// RPC program number used by rasmgr:
#define RPCIF 0x29990000
/// RPC version number used by rasmgr:
#define RPCVERS 1

// rasmgr return codes
// note: these were from -3 to +2 for errors, and none for OK from W.Schatz. I ordered them to 0 for ok, <0 for errors.
#define RASMGR_RESULT_OK		0
#define RASMGR_RESULT_NO_MD5		1
#define RASMGR_RESULT_ILL_ARGS		2
#define RASMGR_RESULT_LICENSE_FAIL	3
#define RASMGR_RESULT_NOT_ALONE		4
#define RASMGR_RESULT_AUTH_CORRUPT	5
#define RASMGR_RESULT_AUTH_OTHERHOST	6
#define RASMGR_RESULT_AUTH_INCOMPAT	7
#define RASMGR_RESULT_NO_SLAVE_IN_TEST	8
#define RASMGR_EXIT_FAILURE		9 // was: EXIT_FAILURE from stdlib.h
#define RASMGR_RESULT_INTERNAL		10

// these should be still more global, they are part of the c/s protocol
// indicator for server type
#define SERVERTYPE_FLAG_RPC	'r'
#define SERVERTYPE_FLAG_HTTP	'h'
#define SERVERTYPE_FLAG_RNP	'n'

// output depending on verbose cmd line flag
#define VLOG if (config.isVerbose()) cout

#endif // RASMGR_HH
