/*
 * This file is part of rasdaman community.
 *
 * Rasdaman community is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Rasdaman community is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with rasdaman community.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright 2003 - 2010 Peter Baumann / rasdaman GmbH.
 *
 * For more information please see <http://www.rasdaman.org>
 * or contact Peter Baumann via <baumann@rasdaman.com>.
*/

/**
 * INCLUDE: rminit.hh
 *
 * PURPOSE:
 * Contains the RMInit class.
 *
 * COMMENTS:
 *      - RASMGRPORT should be centrally defined
 */

#ifndef _RMINIT_
#define _RMINIT_

#include <iostream>
using std::ios;
using std::endl;

#include <fstream>
#include <raslib/mddtypes.hh>
#include <raslib/rm.hh>

//@ManMemo: Module: {\bf raslib}

/*@Doc:

  RMANVERSION is the version of the RasDaMan server times 1000 (int).
  RPCVERSION is the version of the RPC interface times 1000 (int);

*/

//@Man: Version numbers
// RMANVERSION now comes in via -DRMANVERSION -- PB 2003-sep-03
// const int  RMANVERSION = 5100;
const int  RPCVERSION  = 1003;

// default rasmgr port
const int  RASMGRPORT  = 7001;

// RPC timeout [secs]; used in clientcomm/rpcclientcomm.cc -- PB 2005-sep-09
const unsigned int RPC_TIMEOUT = 3;

// timeout in RNP communication
const unsigned int RNP_COMM_TIMEOUT = 60;

// timeout in nerver.cc
const unsigned int RNP_TIMEOUT_LISTEN = 30;

// maximum number of retries in rnprotocol/rnpclientcomm2.cc
const unsigned int RNP_MAX_RETRY = 10;

//@ManMemo: Module: {\bf raslib}

/*@Doc:

  Class {\tt RMInit} is supposed to be instantiated just once. The scope
  of this instance is equal to the program scope (global variable)
  and therefore lifetime of the {\tt RMInit} instance and the application
  are the same. The constructor is used to allocate ressources needed
  during the whole application and the destructor cleans up memory at
  the end of the application.

 */

/**
  * \ingroup RMs
  */

class RMInit : public RM_Class
{
public:
    /// constructor
    RMInit( char applicationType );

    /// destructor
    ~RMInit();

    /// output stream for log information
    static std::ostream logOut;

    /// output stream for debug information
    static std::ostream dbgOut;

    /// output stream for benchmark information
    static std::ostream bmOut;

    /// file output for log information
    static std::ofstream logFileOut;

    /// file output for debug information
    static std::ofstream dbgFileOut;

    /// file output for benchmark information
    static std::ofstream bmFileOut;

    /// name of the user
    static char* userName;

    /// use inlinetiles and inlinetile container index
    static bool useTileContainer;

    /// switch for turning on/off tiling
    static bool tiling;

    /// specifies time out in seconds
    static unsigned long timeOut;

    /// flag, if time out checking is enabled (causes sometimes problems because of signal handler)
    static bool noTimeOut;

    static r_Bytes clientTileSize;

    static unsigned int clientcommMaxRetry;

    static unsigned int clientcommSleep;

    static unsigned int rpcMaxRetry;

private:
    /// check the option string for the occurance of an option and return its value if available
    int checkOptionString( const char* optString, const char* option, char* &value );
    /**
      The method searches for an option of type {\tt -option value } in the option string specified.
      It returns 1 if the option is in the string, otherwise 0. If a value is given for the option
      to check, it is returned by the reference pointer {\tt value}. The value has to freed using
      {\tt delete[] value} after it is not needed anymore.
    */

    /// type of application can either be 'S' for server or 'C' for client
    char applicationType;
};

// Macro for initialization of static members and creation of global RMInit instance
// ---------------------------------------------------------------------------------
// The macro has to be invoked exactly once in each executable using raslib.
//
// rasserver -> in servercomm.o
// rasdaman clients -> in clientcomm.o
// test programms without clientcomm.o and servercomm.o -> in main.o
//
// The reason is that initialisation of globals can be initialized in any
// order if they are in different files. Only in one file the order is defined.
// As it is implemented now, the streams have to be defined somewhere. This
// could easily be done in rminit.cc. But the call to the constructor of RMInit
// has to get a flag for client ('C') or server ('S') as a parameter. This has
// be done somewhere else. In that case the constructor may be called before
// initialization of the streams, the program crashes! If all initialisations
// are in the same file, the order is defined. That is what this macro is for.
//
// Note: At some point it may be useful to reimplement this mess.

#define RMINITGLOBALS( appMode )  std::ostream RMInit::logOut(std::cout.rdbuf()); \
                                  std::ostream RMInit::dbgOut(std::cout.rdbuf()); \
                                  std::ostream RMInit::bmOut(std::cout.rdbuf()); \
                                  std::ofstream RMInit::logFileOut; \
                                  std::ofstream RMInit::dbgFileOut; \
                                  std::ofstream RMInit::bmFileOut; \
                                  RMInit rmInit( appMode );

#endif
