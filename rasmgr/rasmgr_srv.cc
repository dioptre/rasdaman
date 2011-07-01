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
 * SOURCE: rasmgr_srv.cc
 *
 * MODULE: rasmgr
 * CLASS:  RasServer, RasServerManager
 *
 * COMMENTS:
 *  		None
*/

#include "rasmgr_srv.hh"
#include "rasmgr_localsrv.hh"
#include "rasmgr_comm.hh"
#include "rasmgr_users.hh"

#include "debug-srv.hh"
#include "raslib/rminit.hh"

// host name returned when we don't have a valid one:
#define NO_VALID_HOST "noValidHost"

// initial countdown value
#define INITIAL_COUNTDOWN 1000
RasServer::RasServer()
{
	serverName[0]=0;
	extraParam[0]=0;
	ptrServerHost=NULL;
	ptrDatabaseHost=NULL;
	//connStr[0]=0;
	available=isup=isstarting=false;
	valid=false;
	downReq=false;
	initialCountDown = INITIAL_COUNTDOWN;
	currentCountDown = INITIAL_COUNTDOWN; // asa doreste management
	
	writeTransaction=false; // no clearTransaction!!
	readTransaction=false;
	connDatabase = NULL;
	
	activityExpected=false;
	crashCount = 0;
	activityCounter = 0;	// will be changed in init()
	
	autorestart = true;
	strcpy(executableName,RASEXECUTABLE);
}

RasServer::~RasServer()
{
}
   
const char* RasServer::getName()
{
	return serverName;
}

const char* RasServer::getHostName()
{ 
	if(ptrServerHost)
	{ 
		return ptrServerHost->getName();
	}	
	return NO_VALID_HOST;
}
   
const char* RasServer::getHostNetwName()
{ 
	if(ptrServerHost)
	{ 
		return ptrServerHost->getNetworkName();
	}	
	return NO_VALID_HOST;
}
   
long  RasServer::getPort()
{
	return listenPort;
}
char  RasServer::getType()
{
	return serverType;
}

void  RasServer::changeName(const char *newName)
{
// FIXME: input parameter validity check!
	strncpy( serverName, newName, sizeof(serverName) );
}

void RasServer::changeType(const char newType) // char not char*!!
// FIXME: input parameter validity check!
{
	serverType=newType;
}

void  RasServer::changePort(long newPort)
{
	listenPort=newPort;
}

void  RasServer::changeExtraParam(const char *xParam)
// FIXME: input parameter validity check!
{
	strncpy(extraParam,xParam,sizeof(extraParam) );
}

void  RasServer::changeCountDown(int newCountDown)
{
	initialCountDown=newCountDown;
	currentCountDown=newCountDown;
}

void RasServer::changeAutoRestart(bool x)
{
	autorestart = x;
}
   
const char* RasServer::getExtraParams()
{
	return extraParam;
}
int RasServer::getCountDown()
{
	return initialCountDown;
}
   
void  RasServer::init(const char *srvName,const char* hostName,char serverType,long listenPort)
{
	strcpy(this->serverName,srvName);
	
	ptrServerHost=&hostmanager[hostName];// should be valid!
	
	isinternal=ptrServerHost->isInternal();
	available=isup=false;
	this->serverType=serverType;
	this->listenPort=listenPort;

	// RNP servers start with first action connectToDBMS() as opposed to the others,
	// therefore set counter appropriately. -- PB 2002-nov-23
	switch (serverType)
	{
		case SERVERTYPE_FLAG_RPC:
		case SERVERTYPE_FLAG_HTTP:
			TALK( "Initializing activity counter to 1 for RPC/HTTP type server." );
			activityCounter = 1;
			break;
		case SERVERTYPE_FLAG_RNP:
			TALK( "Initializing activity counter to 0 for RNP type server." );
			activityCounter = 0;
			break;
		default:
			TALK( "Error: illegal server type specifier " << serverType << ", assuming '" << SERVERTYPE_FLAG_RNP << "'." );
			break;
	}

	valid=true;
}
   
bool RasServer::isValid()
{
	return valid;
}

bool RasServer::isUp()
{
	return isup;
}

bool RasServer::isStarting()
{
	return isstarting;
}

bool RasServer::isAvailable()   
{
	return available;
}

bool RasServer::forceAvailable()
{ 
	// this function is just for advanced system debug, not for production work
	if(isup)
	{
		available=true; 
		clearPendingTransaction();
		return true;
	}
	return false;
}
   
void RasServer::setNotAvailable()   
{
	available=false;
	regularSignalCounter = MAXREGULARCOUNTER;
}

bool RasServer::isAutoRestart()
{
	return autorestart;
}

char* RasServer::getDescriptionHeader(char *destBuffer)
{
	sprintf(destBuffer,"    %-20s %s   %-20s %-20s %-4s %-2s     %s   %s","Server Name","Type","Host","Db Host","Stat","Av","Acc","Crc");
	return destBuffer;
}

char* RasServer::getDescription(char *destBuffer)
{
	char *sType=(char*)"(none)";
 	if(serverType==SERVERTYPE_FLAG_RPC)
		sType=(char*)"(RPC) ";
 	if(serverType==SERVERTYPE_FLAG_HTTP)
		sType=(char*)"(HTTP)";
 	if(serverType==SERVERTYPE_FLAG_RNP)
		sType=(char*)"(RNP) ";
	
	const char* sUp= isup ? "UP  ":"DOWN";
	
	const char* sAv= available ? "YES":"NO ";
	
	const char* host= ptrServerHost->getName();//"(internal)";
	
	const char* dbHost = getDBHostName();

	//if(isinternal==false)
	//  { host=ptrServerHost->getName();
	//  }
	sprintf(destBuffer,"%-20s %s %-20s %-20s %s %s %6ld    %2d",serverName,sType,host,dbHost,sUp,sAv,activityCounter,crashCount);
	
	return destBuffer;
}

char* RasServer::getDescriptionPortHeader(char *destBuffer)
{
	sprintf(destBuffer,"    %-20s %s   %-20s %-10s  %-3s   %-3s","Server Name","Type","Host","  Port","Ars","  Icd");
	
	return destBuffer;
}

char* RasServer::getDescriptionPort(char *destBuffer)
{
	char *sType=(char*)"(none)";
	if(serverType==SERVERTYPE_FLAG_RPC) sType=(char*)"(RPC) ";
	if(serverType==SERVERTYPE_FLAG_HTTP) sType=(char*)"(HTTP)";
	if(serverType==SERVERTYPE_FLAG_RNP) sType=(char*)"(RNP) ";
	
//    const char* sUp= isup ? "UP  ":"DOWN";
	
//    const char* sAv= available ? "YES":"NO ";
	
	const char* host = ptrServerHost->getName();
	
	const char *ars = autorestart ? "on":"off";   
	
	if(serverType==SERVERTYPE_FLAG_RPC)       
		sprintf(destBuffer,"%-20s %s %-20s %#10lx  %-3s   %3d/%-3d",serverName,sType,host,listenPort,ars,currentCountDown,initialCountDown);
	else 
		sprintf(destBuffer,"%-20s %s %-20s %10ld  %-3s   %3d/%-3d",serverName,sType,host,listenPort,ars,currentCountDown,initialCountDown);
		                                         //countdown=%d InitialCountDown,
	return destBuffer; 
}

char* RasServer::getDescriptionExecHeader(char *destBuffer)
{
	sprintf(destBuffer,"    %-20s  %-10s  Executable      Extraparameters","Server Name","Host");
		                              
	return destBuffer;
}

char* RasServer::getDescriptionExec(char *destBuffer)
{
	
	const char* host = ptrServerHost->getName();
	
	sprintf(destBuffer,"%-20s %-10s   %-15s %s",serverName,host,executableName,extraParam);
	
	return destBuffer; 
}

bool RasServer::connectToDBHost(const char *dbHostName)//, const char *connString)
{
	if(ptrDatabaseHost)
		return false;
	DatabaseHost &dbh=dbHostManager[dbHostName];
	
	if(dbh.isValid()==false)
		return false;
	
	ptrDatabaseHost = &dbh; 
	//removed dbh.incrConnServers();
	//strcpy(connStr,connString);
	
	for(int i=0;i<dbManager.countDatabases();i++)
	{
		if(dbManager[i].isConnectedToDBHost(dbHostName))
		{
			dbManager[i].connectToRasServer(serverName);
		}
	}
	return true;	
}

bool RasServer::disconnectFromDBHost()
{ 
	if(isup)
		return false;
	if(ptrDatabaseHost==NULL)
		return false;
	
	const char *dbHostname=ptrDatabaseHost->getName();
	
	for(int i=0;i<dbManager.countDatabases();i++)
	{
		dbManager[i].disconnectFromRasServer(serverName);
	}
		
	// removed ptrDatabaseHost->decrConnServers();  
	ptrDatabaseHost=NULL;
	
	return true;
}
   
const char* RasServer::getDBHostName()
{
	if(ptrDatabaseHost)
		return ptrDatabaseHost->getName();
	return "noDBHost!";
}

bool RasServer::isConnectedToDBHost()
{
	if(ptrDatabaseHost)
		return true;
	return false;
}

void RasServer::changeExecutableName(const char *newExecutable)
{ 
	if(newExecutable == NULL)
		strcpy(executableName,RASEXECUTABLE);
	else
		strcpy(executableName,newExecutable);
}

const char* RasServer::getExecutableName()
{
	return executableName;
}

int RasServer::startServerInDebugger(char *command)
{
	if(ptrDatabaseHost==NULL)
		return RASSERVER_NODATABASEHOST;
	
	if(ptrServerHost->isUp()==false)
		return RASSERVER_SRVNOTUP;
	
	downReq=false;
	const char *sTypeString= serverType==SERVERTYPE_FLAG_HTTP ? "--http":"";
	
	const char *rasmgrHost = ptrServerHost->useLocalHost() ? "localhost" : config.getHostName();    
	sprintf(command,                "%s --rsn %s %s --lport %ld ",executableName,serverName,sTypeString,listenPort);
	sprintf(command+strlen(command),"--mgr %s --mgrport %d --connect %s ",rasmgrHost,config.getListenPort(),ptrDatabaseHost->getConnectionString());
	sprintf(command+strlen(command),"--sync %s %s",authorization.getSyncroString(),extraParam);
	
	currentCountDown=initialCountDown;
	activityExpected=true;
	activityCounter = 0;

	TALK( "RasServer::startServerInDebugger() -> " << command );
	return RASSERVER_OK;
}
	 
int RasServer::startServer()
{
	if(ptrDatabaseHost==NULL)
		return RASSERVER_NODATABASEHOST;
	
	if(ptrServerHost->isUp()==false)
		return RASSERVER_SRVNOTUP;
	
	downReq=false;
	const char *sTypeString= serverType==SERVERTYPE_FLAG_HTTP ? "--http" : "";
		    sTypeString= serverType==SERVERTYPE_FLAG_RNP ? "--rnp" : sTypeString;
	
	const char *rasmgrHost = ptrServerHost->useLocalHost() ? "localhost" : config.getHostName();    
	char command[ARG_MAX+1];
	const char *SPRINTF_FORMAT = "%s %s %s --rsn %s %s --lport %ld --mgr %s --mgrport %ld --connect %s %s";
	// check for buffer oflo
	unsigned int commandLen =
	     strlen(SPRINTF_FORMAT)
	   + strlen(serverName) + strlen(executableName) + strlen(executableName) + strlen(serverName) + strlen(sTypeString)
	   + 4 // aka strlen(listenPort)
	   + strlen(rasmgrHost)
	   + 4 // aka strlen(config.getListenPort())
	   + strlen(ptrDatabaseHost->getConnectionString())
	   + strlen(extraParam);
	if (commandLen > ARG_MAX)
	{
		TALK( "RasServer::startServer(): fatal error: command line can host " << ARG_MAX << " bytes, but needs " << commandLen << "." );
		RMInit::logOut<<"Error: rasserver command line too long, cannot launch. Disappointedly aborting server start." <<std::endl;
		return RASSERVER_CMDLINEOFLO;
	}
	sprintf(command, SPRINTF_FORMAT, serverName,executableName,executableName,serverName,sTypeString,listenPort, rasmgrHost,config.getListenPort(),ptrDatabaseHost->getConnectionString(), extraParam);

	if(isinternal)
	{
		TALK( "launching local server, command=" << command );
		localServerManager.startNewServer(command);
	}
	else    
	{
		TALK( "connecting to remote rasmgr" );
		int socket=ptrServerHost->getConnectionSocket();
		if(socket<0)
			return RASSERVER_NOREMOTERASMGR;
	
		char message[MAXMSG]="POST exec HTTP/1.1\r\nAccept: text/plain\r\nUserAgent: RasMGR/1.0\r\nContent-length: ";
		
		sprintf(message+strlen(message),"%d\r\n\r\n%s",strlen(command)+1,command);
		
		int nbytes=write(socket,message,strlen(message)+1);
	
		close(socket);
		if(nbytes<0)
			return RASSERVER_INCOMPLETESEND; 
	}
	
	currentCountDown=initialCountDown;
	activityExpected=true;
	isstarting=true;
	activityCounter = 0;
	return RASSERVER_OK;       
}
int RasServer::downServer(bool forced)
{        
	if(available==false && (forced == false || isstarting==true) )
	{
		downReq=true;
		//RMInit::logOut<<"Down request, but working"<<std::endl;
		return RASSERVER_OK;
	}
	return downNow();
}    
int RasServer::downNow()
{           
	//RMInit::logOut<<"Down server"<<std::endl;   
	if(isinternal)
	{
		localServerManager.sendTerminateSignal(serverName);
	}
	else    
	{
		int socket=ptrServerHost->getConnectionSocket();
		if(socket<0)
			return RASSERVER_CANNOTSTARTSRV;
	
		char message[MAXMSG]="POST downserver HTTP/1.1\r\nAccept: text/plain\r\nUserAgent: RasMGR/1.0\r\nContent-length: ";
	
		sprintf(message+strlen(message),"%d\r\n\r\n%s",strlen(serverName)+1,serverName);
		
		int nbytes=write(socket,message,strlen(message)+1);
	
		close(socket);
		if(nbytes<0)
			return RASSERVER_NOREMOTERASMGR; // FIXME: should be RASSERVER_INCOMPLETESEND?
	}

	//ptrServerHost->regDownServer();
	
	return RASSERVER_OK;       
}
int RasServer::killServer()
{    
	if(isup)
	{
		ptrServerHost->regDownServer();
		if(ptrDatabaseHost)
			ptrDatabaseHost->regDownServer();    
		isup=available=0; 
		activityExpected=false;
	}  	
	if(isinternal)
	{
		localServerManager.killServer(serverName);
	}
	else    
	{
		int socket=ptrServerHost->getConnectionSocket();
		if(socket<0)
			return RASSERVER_CANNOTSTARTSRV;

		char message[MAXMSG]="POST killserver HTTP/1.1\r\nAccept: text/plain\r\nUserAgent: RasMGR/1.0\r\nContent-length: ";
	
		sprintf(message+strlen(message),"%d\r\n\r\n%s",strlen(serverName)+1,serverName);
		
		int nbytes=write(socket,message,strlen(message)+1);
	
		close(socket);
		if(nbytes<0)
			return RASSERVER_NOREMOTERASMGR; // FIXME: should be RASSERVER_INCOMPLETESEND?
	}

	return RASSERVER_OK;       
}
	
void RasServer::changeStatus(int newStatus,long infCount)
{
	ENTER( "RasServer::changeStatus: enter. servername="<<serverName<<", newStatus="<<newStatus<<", c="<<infCount );

	if(activityExpected==false && newStatus==SERVER_AVAILABLE)
	{
		RMInit::logOut<<"Error: Server intruder detected in server '"<<serverName<< "' (trying to manually start rasserver?)"<<std::endl;
		return; 
	}
	
	if(newStatus == SERVER_REGULARSIG)
	{ 
		TALK( "RasServer::changeStatus: SERVER_REGULARSIG from "<<serverName<<", newStatus=" << newStatus );
		
		if(available == false)
		{
			TALK( "RasServer::changeStatus: "<<serverName<<" not available, SERVER_REGULARSIG ok, regularSignalCounter="<<regularSignalCounter);
			regularSignalCounter--;
			if(regularSignalCounter > 0)
			{
				LEAVE( "RasServer::changeStatus: leave. regularSignalCounter nonzero: " << regularSignalCounter );
				return;
			}
			newStatus = SERVER_AVAILABLE;
			TALK( "rasmgr: Dead client detected, server "<<serverName<<" is set free again." );
		}
		else
		{
			LEAVE( "RasServer::changeStatus: leave. srv not available." );
			return;	 
		}
	}
	bool wasup=isup;
	
	bool crashed=   (newStatus == SERVER_CRASHED) ? true:false;	
	
	isup=available= (newStatus == SERVER_AVAILABLE) ? true:false;
	
	TALK( "RasServer::changeStatus: wasup=" << wasup << ", isup=" << isup );

	if(wasup==false && isup==true) 
	{
		ptrServerHost->regStartServer();
		if(ptrDatabaseHost)
			ptrDatabaseHost->regStartServer();
		isstarting=false;
	} 
	if(wasup==true  && isup==false) 
	{ 
		//then, ok, I'm down
		ptrServerHost->regDownServer();
		if(ptrDatabaseHost)
			ptrDatabaseHost->regDownServer();
		activityExpected=false;
	}
	
	clearPendingTransaction(); // when the server talks to RasMgr, is always clear, without client

	if(available) 
	{
		activityCounter++; // just a counter
	}

	if(downReq && available)
	{
		downReq=false;
		available=false; //until it's really down it shouldn't get any clients
		TALK( "RasServer::changeStatus: srv down request, available - setting to unavailable and shutting down. ");
		downNow(); 
	}
	
	if(crashed)
	{
		crashCount++;
		TALK( "server has crashed, current crash count is " << crashCount << ", activity count is " << activityCounter );
		// restart if "work has started already" (see init() comment on different counting wrt. server types)
		// changed by PB 2003-nov-23
		// if(activityCounter && autorestart)
		if (activityCounter>1 && autorestart)
		{ // a crashed server doesn't autorestart if it crashes before starting work. 
			TALK( "auto restart activated, restarting." );
			startServer();
		}	
	}

	// commented out due to some error	
	if(initialCountDown)
	{
		TALK( "rasmgr: initialCountDown is " << initialCountDown );
		if(available) 
		{
			TALK( "rasmgr: " << serverName << " is available." );
			currentCountDown--;
			if(currentCountDown==0)
		  	{
				available=false;
				TALK( "rasmgr: Countdown reached for "<<serverName<< ", shutting down." );
				downNow();
			}
		}	
		if(wasup==true && isup==false && currentCountDown==0)
		{
			TALK( "rasmgr: wasup==true && isup==false && currentCountDown==0" );
			TALK( "rasmgr: Restart after countdown for server "<<serverName<< "." );
			currentCountDown=initialCountDown;
			startServer();
		}
	}
			
	LEAVE( "RasServer::changeStatus: leave. ns="<<newStatus<<"  av="<<available );
} // changeStatus()

void RasServer::startWriteTransaction(Database& dataBase)
{
	ENTER( "RasServer::startWriteTransaction: enter." );
	dataBase.startWriteTransaction();
	writeTransaction=true;
	connDatabase=&dataBase;
	LEAVE( "RasServer::startWriteTransaction: leave. servername=" << serverName << ", rwTrans-in on db " << dataBase.getName() );
}

void RasServer::startReadTransaction(Database& dataBase)
{
	ENTER( "RasServer::startReadTransaction: enter." );
	dataBase.startReadTransaction();
	readTransaction=true;
	connDatabase=&dataBase;
	LEAVE( "RasServer::startReadTransaction: leave. servername=" << serverName << " roTrans-in on db " << dataBase.getName() );
}

void RasServer::clearPendingTransaction()
{   
	if(connDatabase) 
	if(writeTransaction)
		connDatabase->endWriteTransaction();
	if(readTransaction )
		connDatabase->endReadTransaction();
	writeTransaction=false;
	readTransaction =false;
	connDatabase = NULL;
}
//**********************************************************************
RasServerManager::RasServerManager()
{ 
}
RasServerManager::~RasServerManager()
{
}
   
bool RasServerManager::insertNewServer(const char *srvName,const char* hostName,char serverType,long listenPort)
{
	bool result = true;

	if(testUniqueness(srvName)==false)
		result = false;
	
	if( result == true && hostmanager[hostName].isValid()==false)
		result = false;
	
	if(result == true && serverType!=SERVERTYPE_FLAG_RPC && serverType!=SERVERTYPE_FLAG_HTTP && serverType!=SERVERTYPE_FLAG_RNP)
	{
		TALK( "RasServerManager::insertNewServer: server " << srvName << " has illegal type " << serverType );
		result = false;
	}

	if (result == true)
	{
		RasServer tempRasServer;
		srvList.push_back(tempRasServer);
		RasServer &refRasServer=srvList.back();
		refRasServer.init(srvName,hostName,serverType,listenPort); 
	}

	return result;
}

bool RasServerManager::removeServer(const char *srvName)
{
	list<RasServer>::iterator iter=srvList.begin();
	for(int i=0;i<srvList.size();i++) 
	{
		if(strcmp(iter->getName(),srvName)==0) 
		{
			if(iter->isUp())
		      		return false;

			iter->disconnectFromDBHost(); //it's not up, so it fails only if it is not connected at all
		
			srvList.erase(iter);
			return true; 
	 	}
		iter++;
	}
	return false;
}
   
bool RasServerManager::testUniqueness(const char* srvName)
{
	list<RasServer>::iterator iter=srvList.begin();
	for(int i=0;i<srvList.size();i++)
	{
		if(strcmp(iter->getName(),srvName)==0)
			return false;
		iter++;
	}	 
	return true;
}

RasServer& RasServerManager::operator[](int x)	// FIXME: check against upper limit
{ 
	list<RasServer>::iterator iter=srvList.begin();
	for(int i=0;i<x;i++)
		iter++;
	return *iter;
}

RasServer& RasServerManager::operator[](const char* srvName)	// FIXME: check against upper limit
{   
	list<RasServer>::iterator iter=srvList.begin();
	for(int i=0;i<srvList.size();i++) 
	{
		if(strcmp(iter->getName(),srvName)==0)
		  return *iter; 
	
		iter++;
	}
	return protElem; 
} 
   
RasServer& RasServerManager::last()
{
	return srvList.back();
}
	

int  RasServerManager::countServers() 
{
	return srvList.size();
}

// return values of changeServerStatus:
#define SERVERSTATUS_OK 0
#define SERVERSTATUS_ERR -1
int RasServerManager::changeServerStatus(char *reqMessage)
{ 
	char serverName[50];
	int  newstatus;
	long infCount;

	sscanf(reqMessage,"%s %d %ld",serverName,&newstatus,&infCount);
	TALK( "RasServerManager::changeServerStatus: Trying to change status of "<<serverName<<" to "<<newstatus );
	RasServer &r=operator[](serverName);
	
	if(r.isValid()==false)
	{
		RMInit::logOut<<"Error: Unexpected message from rasserver '"<<serverName<<"'; new status is "<<newstatus<<std::endl;
		return SERVERSTATUS_ERR;
	}
	
	r.changeStatus(newstatus,infCount);
	return SERVERSTATUS_OK;
}
   
void RasServerManager::disconnectAllServersFromDBH(const char *dbhName)
{
	list<RasServer>::iterator iter=srvList.begin();
	for(int i=0;i<srvList.size();i++,iter++) 
	{
		const char *cDbhName=iter->getDBHostName();
		
		if(cDbhName==NULL)
			continue;
		
		if(strcmp(cDbhName,dbhName)==0) iter->disconnectFromDBHost();
	}
  
}
   
int RasServerManager::countUpServers()
{
	int count=0;
	list<RasServer>::iterator iter=srvList.begin();
	for(int i=0;i<srvList.size();i++) 
	{
		if(iter->isUp())
			count++; 
		iter++;
	}

	TALK( "RasServerManager::countUpServers() -> " << count );
	return count; 
	
}
		  
void RasServerManager::printStatus()
{
	char buff[100];
	list<RasServer>::iterator iter=srvList.begin();
   
	TALK( "RasServerManager::printStatus. current status is:" );
	for(int i=0;i<srvList.size();i++) 
	{
		iter->getDescription(buff);
		TALK( "\t" << i << ": " << buff );
		iter++;
	}

}
	
bool RasServerManager::reset()
{  // test modus only
	if(config.isTestModus()==false)
		return false;
	
	list<RasServer>::iterator iter=srvList.begin();
	for(int i=0;i<srvList.size();i++, iter++) 
	{
		if(iter->isUp())
			return false;
	}
	
	iter=srvList.begin();
	for(int i=0;i<srvList.size();i++, iter++) 
	{
		iter->disconnectFromDBHost(); //it's not up, so it fails only if it is not connected at all
	}   	
	
	while(srvList.size())
	{    
		srvList.pop_front();
	} 	
	return true; 
  
}
   
bool RasServerManager::acceptChangeName(const char *oldName,const char *newName)
{ 
	if(strcmp(oldName,newName)==0)
		return true; // if someone really wants to change a name with the same,
	return testUniqueness(newName);
}

   
