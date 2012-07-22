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
 * SOURCE: rasmgr_localsrv.hh
 *
 * MODULE: rasmgr
 * CLASS:  LocalServer, LocalServerManager
 *
 * PURPOSE:
 *   management of rasserver executables
 *
 * COMMENTS:
 *          None
 *
*/

#ifndef RASMGR_LOCALSRV_HH
#define RASMGR_LOCALSRV_HH

#include "rasmgr.hh"


/**
  * \ingroup Rasmgrs
  */
class LocalServer
{
public:
    LocalServer();
    void  init(const char*,pid_t);
    const char* getName();
    pid_t getPID();
    bool  isValid();
private:
    char  serverName[30];
    pid_t serverPid;
    bool  valid;
};

/**
  * \ingroup Rasmgrs
  */
class LocalServerManager
{
public:
    LocalServerManager();
    ~LocalServerManager();
    bool startNewServer(const char* commandline);
    int  countStartedServers();
    bool sendTerminateSignal(const char *serverName);
    bool killServer(const char *serverName);

    LocalServer& operator[](int);
    LocalServer& operator[](const char* srvName);

    void childSignalIn(); //only signal calls this
    void cleanChild();
private:
    void reportDeadServer(LocalServer &);
    bool  wasSignal;
    std::list<LocalServer> srvList;
    LocalServer       protElem;

};

extern LocalServerManager localServerManager;
#endif
