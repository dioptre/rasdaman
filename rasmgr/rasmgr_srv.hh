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
 * SOURCE: rasmgr_srv.hh
 *
 * MODULE: rasmgr
 * CLASS:  RasServer, RasServerManager
 *
 * COMMENTS:
 *          None
 *
*/

#ifndef RASMGR_SRV_HH
#define RASMGR_SRV_HH

#include "rasmgr.hh"
#include "rasmgr_config.hh"
#include "rasmgr_host.hh"
#include "rasmgr_dbm.hh"

//#define RASEXECUTABLE "rst"
#define RASEXECUTABLE BINDIR"rasserver"
#define MAXREGULARCOUNTER 3

/// return codes
const unsigned int RASSERVER_OK             =  0;
const unsigned int RASSERVER_NODATABASEHOST = -1;
const unsigned int RASSERVER_CANNOTSTARTSRV = -2;
const unsigned int RASSERVER_NOREMOTERASMGR = -3;
const unsigned int RASSERVER_INCOMPLETESEND = -4;
const unsigned int RASSERVER_SRVNOTUP       = -5;
const unsigned int RASSERVER_CMDLINEOFLO    = -6;


/**
  * \ingroup Rasmgrs
  */
class RasServer
{
public:
    RasServer();
    ~RasServer();
    void  init(const char *srvName,const char* hostName,char serverType,long listenPort);
    const char *getName();
    const char *getHostName();
    const char *getHostNetwName();
    long  getPort();
    char  getType();
    void  changeName(const char *newName);
    void  changeType(const char newType); // char not char*!!

    void  changePort(long newPort);
    void  changeExtraParam(const char *extraParam);
    void  changeCountDown(int);
    void  changeAutoRestart(bool);
    const char *getExtraParams();
    int   getCountDown();

    bool connectToDBHost(const char *dbHostName);//,const char *connString);
    bool disconnectFromDBHost();
    const char *getDBHostName();

    static char* getDescriptionHeader(char *destBuffer);
    char* getDescription(char *destBuffer);
    static char* getDescriptionExecHeader(char *destBuffer);
    char* getDescriptionExec(char *destBuffer);
    static char* getDescriptionPortHeader(char *destBuffer);
    char* getDescriptionPort(char *destBuffer);

    int  startServer();
    int  startServerInDebugger(char *command); // test modus only

    int  downServer(bool forced);
    int  killServer();

    void changeStatus(int,long);
    bool isUp();
    bool isStarting();
    bool isValid();
    bool isAvailable();
    bool forceAvailable();

    bool isConnectedToDBHost();
    bool isAutoRestart();

    void setNotAvailable();
    void startWriteTransaction(Database& dataBase);
    void startReadTransaction(Database& dataBase);
    void changeExecutableName(const char*);
    const char* getExecutableName();
private:
    int          downNow();
    void         clearPendingTransaction();

    char         serverName[ARG_MAX];
    ServerHost   *ptrServerHost;
    bool         isinternal;
    char         serverType;     //'r','h'
    long         listenPort; // 'r' ->rpc prognum; 'h' ->TCP/IP port
    char         extraParam[ARG_MAX];

    char         executableName[ARG_MAX];

    DatabaseHost *ptrDatabaseHost;
    //char         connStr[100];

    bool downReq;
    bool available;
    bool isup;
    bool isstarting;
    bool activityExpected; // to avoid the possibility of starting rasserver by hand

    int  regularSignalCounter; // how namy times should a nonavailable server signal before we put it available again
    unsigned long activityCounter; // counts "activities" per server, i.e., actions that are noticeable by the server

    int  initialCountDown;
    int  currentCountDown;
    int  crashCount;
    bool autorestart;

    bool     writeTransaction;
    bool     readTransaction;
    Database *connDatabase;

    bool valid;
};

/**
  * \ingroup Rasmgrs
  */
class RasServerManager
{
public:
    RasServerManager();
    ~RasServerManager();
    bool insertNewServer(const char *srvName,const char* hostName,char serverType,long listenPort);
    bool removeServer(const char *srvName);
    int  countServers();
    RasServer& operator[](int);
    RasServer& operator[](const char*srvName);
    RasServer& last();

    int changeServerStatus(char *reqMessage);
    void disconnectAllServersFromDBH(const char *dbhName);

    int  countUpServers();
    void printStatus();
    bool reset(); // test modus only
    bool acceptChangeName(const char *oldName,const char *newName);
private:
    bool testUniqueness(const char* srvName);
    list<RasServer> srvList;
    RasServer       protElem;
};

extern RasServerManager rasManager;

#endif


