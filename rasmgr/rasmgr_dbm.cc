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
 * SOURCE: rasmgr_dbm.cc
 *
 * MODULE: rasmgr
 * CLASS:  DatabaseHost, DatabaseHostManager, Database, DatabaseManager
 *
 * COMMENTS:
 *   none
 *
\*/

#include "rasmgr_dbm.hh"
#include "rasmgr_srv.hh"
#include "rasmgr_users.hh"

#include "debug-srv.hh"
#include "raslib/rminit.hh"

extern bool hostCmp( const char *h1, const char *h2);


DatabaseHost::DatabaseHost()
{
    hostName[0]=0;
    connectString[0]=0;
    valid=false;
    activServers=0;
}
DatabaseHost::~DatabaseHost()
{
}

const char* DatabaseHost::getName()
{
    return hostName;
}

const char* DatabaseHost::getConnectionString()
{
    return connectString;
}

const char* DatabaseHost::getUser()
{
    return userString;
}

const char* DatabaseHost::getPasswd()
{
    return passwdString;
}

void  DatabaseHost::changeConnectionString(const char *connectString)
{
    strcpy(this->connectString,connectString);
}
void DatabaseHost::changeName(const char *newName)
{
    strcpy(hostName,newName);
}

void  DatabaseHost::init(const char* hostName,const char* connectString,const char* userString,const char* passwdString)
{
    strcpy(this->hostName,hostName);
    strcpy(this->connectString,connectString);
    if (userString != NULL && strlen(userString) > 0)
        strcpy(this->userString,userString);
    else
        this->userString[0] = '\0';
    if (passwdString != NULL && strlen(passwdString) > 0)
        strcpy(this->passwdString,passwdString);
    else
        this->passwdString[0] = '\0';
    valid=true;
}

void  DatabaseHost::regStartServer()
{
    activServers++;
}

void  DatabaseHost::regDownServer()
{
    activServers--;
}

//void  DatabaseHost::incrConnServers()   { connServers++;}

//void  DatabaseHost::decrConnServers()   { connServers--;}

//void  DatabaseHost::incrConnDatabases() { connDatabases++;}

//void  DatabaseHost::decrConnDatabases() { connDatabases--;}

bool  DatabaseHost::isBusy()
{
    //RMInit::logOut<<"DBH="<<hostName<<"s="<<connServers<<" d="<<connDatabases<<std::endl;
    return activServers ? true:false; //(connServers + connDatabases) ? true:false;
}


bool DatabaseHost::isValid()
{
    return valid;
}

bool DatabaseHost::prepareToBeRemoved()
{
    if(isBusy()) return false;

    //disconnect all servers from me
    rasManager.disconnectAllServersFromDBH(hostName);

    //disconnect all databases from me
    dbManager.disconnectAllDatabasesFromDBH(hostName);

    return true;
}

//**********************************************************************
DatabaseHostManager::DatabaseHostManager()
{
}
DatabaseHostManager::~DatabaseHostManager()
{

}

bool DatabaseHostManager::insertNewHost(const char* hostName,const char* connectString,const char* userString,const char* passwdString)
{
    char tempHostName[200];
    strcpy(tempHostName,hostName);
    // why?? strtolwr(tempHostName);

    if(testUniqueness(tempHostName)==false) return false;

    DatabaseHost tempDatabaseHost;
    hostList.push_back(tempDatabaseHost);
    DatabaseHost &refDatabaseHost=hostList.back();

    refDatabaseHost.init(tempHostName,connectString,userString,passwdString);

    return true;
}

bool DatabaseHostManager::removeHost(const char* hostName)
{
    list<DatabaseHost>::iterator iter=hostList.begin();
    for(int i=0; i<hostList.size(); i++)
    {
        if(hostCmp(iter->getName(),hostName))
        {
            if(iter->isBusy())
                return false;
            iter->prepareToBeRemoved();
            hostList.erase(iter);
            return true;
        }

        iter++;
    }
    return false;
}

bool DatabaseHostManager::testUniqueness(const char* hostName)
{
    list<DatabaseHost>::iterator iter=hostList.begin();
    for(int i=0; i<hostList.size(); i++)
    {
        if(hostCmp(iter->getName(),hostName))
            return false;
        iter++;
    }
    return true;
}

DatabaseHost& DatabaseHostManager::operator[](int x)
{
    list<DatabaseHost>::iterator iter=hostList.begin();
    for(int i=0; i<x; i++)
        iter++;
    return *iter;
}
DatabaseHost& DatabaseHostManager::operator[](const char* hostName)
{

    list<DatabaseHost>::iterator iter=hostList.begin();
    for(int i=0; i<hostList.size(); i++)
    {
        if(hostCmp(iter->getName(),hostName))
            return *iter;
        iter++;
    }
    return protElem;
}


int  DatabaseHostManager::countHosts()
{
    return hostList.size();
}

bool DatabaseHostManager::reset()
{
    if(config.isTestModus()==false) return false;

    list<DatabaseHost>::iterator iter=hostList.begin();
    for(int i=0; i<hostList.size(); i++,iter++)
    {
        iter->prepareToBeRemoved();
    }

    while(hostList.size())
    {
        hostList.pop_front();
    }

    return true;
}

bool DatabaseHostManager::acceptChangeName(const char *oldName,const char *newName)
{
    if(hostCmp(oldName,newName))
        return true; // if someone really wants to change a name with the same,
    return testUniqueness(newName);
}

//*************************************************************************

Database::Database()
{
    databaseName[0]=0;
    valid=false;

    traceWT = false;
    countWriteTransactions =0;
    countReadTransactions =0;
}
Database::~Database()
{
}

const char* Database::getName()
{
    return databaseName;
}
void Database::changeName(const char* databaseName)
{
    strcpy(this->databaseName,databaseName);
}

void  Database::init(const char* databaseName)
{
    strcpy(this->databaseName,databaseName);
    valid=true;
}
bool Database::isValid()
{
    return valid;
}

const char* Database::getDescriptionHeader(char *destBuffer)
{
    sprintf(destBuffer,"    Database Name         Open Trans.");
    return destBuffer;
}
const char* Database::getDescription(char *destBuffer)
{
    sprintf(destBuffer,"%-20s  (%dw + %dr)",databaseName,countWriteTransactions,countReadTransactions);
    return destBuffer;
}

bool Database::connectToDBHost(const char* hostName)
{
    DatabaseHost &TempDBH=dbHostManager[hostName];

    if(TempDBH.isValid()==false) return false; // no such hostName

    if(checkConnection(TempDBH)) return false; // is already connected

    hostPtrList.push_back(&TempDBH);
    //removed TempDBH.incrConnDatabases();

    // alse connecting to servers connected to this database host
    for(int i=0; i<rasManager.countServers(); i++)
    {
        RasServer &r = rasManager[i];
        if(hostCmp(r.getDBHostName(),hostName))
        {
            connectToRasServer(r.getName());
        }
    }

    return true;
}

bool Database::disconnectFromDBHost(const char* hostName)
{
    list<DatabaseHost*>::iterator iter=hostPtrList.begin();
    for(int i=0; i<hostPtrList.size(); i++)
    {
        DatabaseHost *ptrDBH=*iter;

        if(hostCmp(ptrDBH->getName(),hostName))
        {
            for(int j=0; j<rasManager.countServers(); j++)
            {
                // disconnectig from the RasServers connected to the same database host
                if(hostCmp(hostName,rasManager[j].getDBHostName()))
                {
                    disconnectFromRasServer(rasManager[j].getName());
                }
            }

            //removed  ptrDBH->decrConnDatabases();
            hostPtrList.erase(iter);
            return true;
        }
        iter++;
    }
    return false;;
}
void Database::disconnectForRemove()
{
    // this means disconnect from all database hosts
    list<DatabaseHost*>::iterator iter=hostPtrList.begin();
    int listsize=hostPtrList.size();
    for(int i=0; i<listsize; i++)
    {
        //removed (*iter)->decrConnDatabases();
        iter++;
    }
    for(int i=0; i<listsize; i++)
    {
        hostPtrList.pop_front();
    }
    //and revoke all trustees for it
    userManager.removeDatabaseRights(databaseName);
}

bool Database::isConnectedToDBHost(const char* hostName)
{
    DatabaseHost &r = dbHostManager[hostName];

    if(r.isValid()==false) return false;

    return checkConnection(r);
}
bool Database::checkConnection(DatabaseHost &databaseHost)
{
    list<DatabaseHost*>::iterator iter=hostPtrList.begin();
    for(int i=0; i<hostPtrList.size(); i++)
    {
        if(*iter== &databaseHost) return true;

        iter++;
    }
    return false;
}

int Database::countConnectionsToDBHosts()
{
    return hostPtrList.size();
}
const char* Database::getDBHostName(int x)
{
    if( x < hostPtrList.size() )
    {
        list<DatabaseHost*>::iterator iter=hostPtrList.begin();
        for(int i=0; i<x; i++,iter++);
        return (*iter)->getName();
    }
    return "noHost!";
}

bool Database::connectToRasServer(const char *serverName)
{
    RasServer &rasServer=rasManager[serverName];

    if(rasServer.isValid()==false) return false; // no such serverName

    if(checkConnection(rasServer)) return false; // is already connected

    rasPtrList.push_back(&rasServer);

    return true;

}
bool Database::disconnectFromRasServer(const char *serverName)
{
    list<RasServer*>::iterator iter=rasPtrList.begin();
    for(int i=0; i<rasPtrList.size(); i++)
    {
        RasServer *ptrRas=*iter;

        if(hostCmp(ptrRas->getName(),serverName))
        {
            rasPtrList.erase(iter);
            return true;
        }
        iter++;
    }
    return false;
}

bool Database::isConnectedToRasServer(const char *serverName)
{
    RasServer &r = rasManager[serverName];

    if(r.isValid()==false) return false;

    return checkConnection(r);
}

int  Database::countConnectionsToRasServers()
{
    return rasPtrList.size();
}

const char* Database::getRasServerName(int x)
{
    if( x < rasPtrList.size() )
    {
        list<RasServer*>::iterator iter=rasPtrList.begin();
        for(int i=0; i<x; i++,iter++);
        return (*iter)->getName();
    }
    return "noRasServer!";
}

bool Database::checkConnection(RasServer &rasServer)
{
    list<RasServer*>::iterator iter=rasPtrList.begin();
    for(int i=0; i<rasPtrList.size(); i++)
    {
        if(*iter== &rasServer) return true;

        iter++;
    }
    return false;
}

void Database::setTraceWriteTrans(bool how)
{
    traceWT=how;
}

void Database::startWriteTransaction()
{
    if(traceWT) RMInit::logOut<<"  DbName="<<databaseName<<" rwTrans-in"<<std::endl;
    countWriteTransactions++;
}

void Database::endWriteTransaction()
{
    if(traceWT) RMInit::logOut<<"  DbName="<<databaseName<<" rwTrans-out"<<std::endl;
    countWriteTransactions--;
}
int Database::getWriteTransactionCount()
{
    if(traceWT) RMInit::logOut<<"  DbName="<<databaseName<<" ask rwTrans? ("<<countWriteTransactions<<")"<<std::endl;
    return countWriteTransactions;
}

void Database::startReadTransaction()
{
    countReadTransactions++;
}

void Database::endReadTransaction()
{
    countReadTransactions--;
}
int Database::getReadTransactionCount()
{
    return countReadTransactions;
}
bool Database::isBusy()
{
    return countReadTransactions+countWriteTransactions ? true:false;
}
//**********************************************************************
DatabaseManager::DatabaseManager()
{
}
DatabaseManager::~DatabaseManager()
{

}

bool DatabaseManager::insertNewDatabase(const char* databaseName)
{
    if(testUniqueness(databaseName)==false) return false;

    Database tempDatabase;
    dtbList.push_back(tempDatabase);
    Database &refDatabase=dtbList.back();
    refDatabase.init(databaseName);
    return true;
}

bool DatabaseManager::removeDatabase(const char* databaseName)
{
    list<Database>::iterator iter=dtbList.begin();
    for(int i=0; i<dtbList.size(); i++)
    {
        if(hostCmp(iter->getName(),databaseName))
        {
            iter->disconnectForRemove();
            dtbList.erase(iter);
            return true;
        }

        iter++;
    }
    return false;
}

bool DatabaseManager::testUniqueness(const char* databaseName)
{
    list<Database>::iterator iter=dtbList.begin();
    for(int i=0; i<dtbList.size(); i++)
    {
        if(hostCmp(iter->getName(),databaseName))
            return false;
        iter++;
    }
    return true;
}

Database& DatabaseManager::operator[](int x)
{
    list<Database>::iterator iter=dtbList.begin();
    for(int i=0; i<x; i++) iter++;
    return *iter;
}
Database& DatabaseManager::operator[](const char* dbName)
{
    list<Database>::iterator iter=dtbList.begin();
    for(int i=0; i<dtbList.size(); i++)
    {
        if(hostCmp(iter->getName(),dbName))
            return *iter;

        iter++;
    }
    return protElem;    // FIXME: is this correct? PB 2010-10-16
}

void DatabaseManager::disconnectAllDatabasesFromDBH(const char* dbhName)
{
    list<Database>::iterator iter=dtbList.begin();
    for(int i=0; i<dtbList.size(); i++,iter++)
    {
        iter->disconnectFromDBHost(dbhName);
    }
}
int  DatabaseManager::countDatabases()
{
    return dtbList.size();
}

bool DatabaseManager::reset()
{
    if(config.isTestModus()==false) return false;

    list<Database>::iterator iter=dtbList.begin();
    for(int i=0; i<dtbList.size(); i++,iter++)
    {
        iter->disconnectForRemove();
    }

    while(dtbList.size())
    {
        dtbList.pop_front();
    }
    return true;
}

bool DatabaseManager::acceptChangeName(const char *oldName,const char *newName)
{
    if(hostCmp(oldName,newName))
        return true; // if someone really wants to change a name with the same,

    return testUniqueness(newName);
}

