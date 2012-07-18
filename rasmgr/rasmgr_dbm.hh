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
 * SOURCE: rasmgr_dbm.hh
 *
 * MODULE: rasmgr
 * CLASS:  DatabaseHost, DatabaseHostManager, Database, DatabaseManager
 *
 * COMMENTS:
 *   none
 *
*/

#ifndef RASMGR_DBM_HH
#define RASMGR_DBM_HH

#include "rasmgr.hh"
#include "rasmgr_config.hh"
//#include "rasmgr_srv.hh"

/**
  * \ingroup Rasmgrs
  */
class DatabaseHost 
  {
    public:
      DatabaseHost();
      ~DatabaseHost();
      
      void  init(const char* hostName,const char *connectString,const char* userString,const char* passwdString);      
      
      const char* getName();
      const char* getConnectionString();
      const char* getUser();
      const char* getPasswd();
      void  changeConnectionString(const char *connectString);
      void  changeName(const char *newName);
      void  regStartServer();
      void  regDownServer();
      //void  incrConnServers();
      //void  decrConnServers();
      //void  incrConnDatabases();
      //void  decrConnDatabases();
      bool  prepareToBeRemoved();
      bool  isBusy();
      bool  isValid();
    private:
      char hostName[100];
      char connectString[100];
      char userString[100];
      char passwdString[100];
      int  activServers;
      //int  connServers;
      //int  connDatabases;
      bool valid;
   };

/**
  * \ingroup Rasmgrs
  */
class DatabaseHostManager
  { 
    public:
      DatabaseHostManager();
      ~DatabaseHostManager();
      bool insertNewHost(const char* hostName,const char* connectString,const char* userString,const char* passwdString);
      bool removeHost(const char* hostName);
      int  countHosts();
      DatabaseHost& operator[](int);
      DatabaseHost& operator[](const char* hostName);
      bool reset();
      bool acceptChangeName(const char *oldName,const char *newName);      
    private:
      bool testUniqueness(const char* hostName);
      list<DatabaseHost> hostList;
      DatabaseHost       protElem;      
   };
   
extern DatabaseHostManager dbHostManager;


//*****************************************************

class RasServer;

/**
  * \ingroup Rasmgrs
  */
class Database 
  {
    public:
      Database();
      ~Database();
      
      void  init(const char* databaseName);      
      const char* getName();
      void  changeName(const char* databaseName);
      
      static const char* getDescriptionHeader(char *destBuffer);
             const char* getDescription(char *destBuffer);

      bool  connectToDBHost(const char* hostName);
      bool  disconnectFromDBHost(const char* hostName);
      bool  isConnectedToDBHost(const char* hostName);
      int   countConnectionsToDBHosts();
      const char* getDBHostName(int);
      
      bool  connectToRasServer(const char *serverName);
      bool  disconnectFromRasServer(const char *serverName);
      bool  isConnectedToRasServer(const char *serverName);
      int   countConnectionsToRasServers();
      const char* getRasServerName(int);
      
      void  disconnectForRemove();
      
      void startWriteTransaction();
      void endWriteTransaction();
      void startReadTransaction();
      void endReadTransaction();
      
      int  getWriteTransactionCount();
      int  getReadTransactionCount();
      bool isBusy();
      
      void setTraceWriteTrans(bool);
      
      bool isValid();
    private:
      bool checkConnection(DatabaseHost &);
      bool checkConnection(RasServer &);
      char databaseName[100];
      list<DatabaseHost*> hostPtrList;
      list<RasServer*>    rasPtrList;
      
      bool traceWT;
      int countWriteTransactions;
      int countReadTransactions;
      
      bool valid;  
   };

/**
  * \ingroup Rasmgrs
  */
class DatabaseManager
  { 
    public:
      DatabaseManager();
      ~DatabaseManager();
      bool insertNewDatabase(const char* databaseName);
      bool removeDatabase(const char* databaseName);
      int  countDatabases();
      Database& operator[](int);
      Database& operator[](const char*);
      void disconnectAllDatabasesFromDBH(const char* dbhName);
      bool reset();
      bool acceptChangeName(const char *oldName,const char *newName);      
    private:
      bool testUniqueness(const char* dbName);
      list<Database> dtbList;      
      Database      protElem; 
   };
   
extern DatabaseManager dbManager;


#endif
