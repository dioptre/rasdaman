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
 * SOURCE: manager.cc
 *
 * MODULE: servercomm
 * CLASS:  -
 *
 * COMMENTS:
 *		No Comments
*/

#include "mymalloc/mymalloc.h"
static const char rcsid[] = "@(#)manager, ServerComm: $Id: manager.cc,v 1.92 2005/09/03 21:05:14 rasdev Exp $";

#include <iostream>
using namespace std;

#include <string.h>    // for strcat()
#include <time.h>      // for time()
#include <malloc.h>

// server option for switching off garbageCollection (Formerly there were problems
// because of illegal functions called in signal handler, see man 5 attributes;
// these are no longer relevant since the introduction of the CallBackManager)
extern int noTimeOut;


// Put it in front of any typedef bool ... because o2 is using bool as a variable. 
// #include "o2template_CC.hxx"

#include <signal.h>    // for signal()
#include <unistd.h>    // for alarm()

#ifndef SOLARIS
  #define PORTMAP        // define to use function declarations for old interfaces
  #include <rpc/rpc.h>
#else  // HPUX
  #include <rpc/rpc.h>
#endif

#ifndef _RPCIF_
  #define _RPCIF_
  #include "clientcomm/rpcif.h"
#endif

#include "raslib/rmdebug.hh"
#include "raslib/minterval.hh"
#include "raslib/oid.hh"
#include "raslib/endian.hh"

#include "servercomm/servercomm.hh"


extern "C" {
void garbageCollection( int );
void garbageCollectionDummy ( int );
}


// 
// Section for global variables pointing to RPC return data.
//
static GetExtendedErrorInfo rpcExtendedErrorInfo  = { 0, 0};
static ServerVersionRes    rpcServerVersionRes    = { 0, 0};
static u_short             rpcDummy               = 0;
static OpenDBRes           rpcOpenDBRes           = { 0, 0};

static ServerStatRes       rpcServerStatRes       = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, { 0, 0 } };
static ExecuteQueryRes     rpcExecuteQueryRes     = { 0, 0, 0, 0, 0, 0, 0 };
static GetMDDRes           rpcGetMDDRes           = { 0, 0, 0, 0, 0 };
static GetElementRes       rpcGetElementRes       = { 0, { 0, 0 } };
static GetTileRes          rpcGetTileRes          = { 0, 0 };
static ExecuteUpdateRes    rpcExecuteUpdateRes    = { 0, 0, 0, 0, 0 };
static GetCollRes          rpcGetCollRes          = { 0, 0, 0, 0, 0 };
static GetCollOIdsRes      rpcGetCollOidsRes      = { 0, 0, 0, 0, 0, { 0, 0 } };
static OIdRes              rpcOidRes              = { 0, 0 };
static GetTypeStructureRes rpcGetTypeStructureRes = { 0, 0 };
static ObjectTypeRes       procResult             = { 0, 0 };

extern char *secureResultBufferForRPC;

void freeDynamicRPCData()
{
  // rpcserverstat
  if( rpcServerStatRes.clientTable.clientTable_len )
  {
    for( int i=0; i<rpcServerStatRes.clientTable.clientTable_len; i++ )
    {
      free( rpcServerStatRes.clientTable.clientTable_val[i].clientIdText );
      free( rpcServerStatRes.clientTable.clientTable_val[i].userName );
      free( rpcServerStatRes.clientTable.clientTable_val[i].baseName );
    }
       
    free( rpcServerStatRes.clientTable.clientTable_val );
      
    rpcServerStatRes.clientTable.clientTable_len = 0;
    rpcServerStatRes.clientTable.clientTable_val = 0;
  }

  // rpcexecutequery
  if( rpcExecuteQueryRes.token )         free( rpcExecuteQueryRes.token );
  if( rpcExecuteQueryRes.typeName )      free( rpcExecuteQueryRes.typeName );
  if( rpcExecuteQueryRes.typeStructure ) free( rpcExecuteQueryRes.typeStructure );
  rpcExecuteQueryRes.token         = 0;
  rpcExecuteQueryRes.status        = 0;
  rpcExecuteQueryRes.errorNo       = 0;
  rpcExecuteQueryRes.lineNo        = 0;
  rpcExecuteQueryRes.columnNo      = 0;
  rpcExecuteQueryRes.typeName      = 0;
  rpcExecuteQueryRes.typeStructure = 0;

  // rpcgetnextmdd, rpcgetmddbyoid
  if( rpcGetMDDRes.domain )        free( rpcGetMDDRes.domain );
  if( rpcGetMDDRes.typeName )      free( rpcGetMDDRes.typeName );
  if( rpcGetMDDRes.typeStructure ) free( rpcGetMDDRes.typeStructure );
  if( rpcGetMDDRes.oid )           free( rpcGetMDDRes.oid );
  rpcGetMDDRes.typeStructure = 0;
  rpcGetMDDRes.typeName      = 0;
  rpcGetMDDRes.domain        = 0;
  rpcGetMDDRes.oid           = 0;

  // rpcgetnextelement
  if( rpcGetElementRes.data.confarray_val ) free( rpcGetElementRes.data.confarray_val );
  rpcGetElementRes.data.confarray_val = 0;

  // rpcgetnexttile
  if( rpcGetTileRes.marray )
  {
    if( rpcGetTileRes.marray->domain ) free( rpcGetTileRes.marray->domain );
    // if( rpcGetTileRes.marray->data   ) free( rpcGetTileRes.marray->data );
    free( rpcGetTileRes.marray );
    rpcGetTileRes.marray = 0;
  }

  // rpcexecuteupdate
  if( rpcExecuteUpdateRes.token ) free( rpcExecuteUpdateRes.token );
  rpcExecuteUpdateRes.token    = 0;
  rpcExecuteUpdateRes.status   = 0;
  rpcExecuteUpdateRes.errorNo  = 0;
  rpcExecuteUpdateRes.lineNo   = 0;
  rpcExecuteUpdateRes.columnNo = 0;

  // rpcgetcollbyname, rpcgetcollbyoid
  if( rpcGetCollRes.typeName )      free( rpcGetCollRes.typeName );
  if( rpcGetCollRes.typeStructure ) free( rpcGetCollRes.typeStructure );
  if( rpcGetCollRes.oid )           free( rpcGetCollRes.oid );
  if( rpcGetCollRes.collName )      free( rpcGetCollRes.collName );
  rpcGetCollRes.oid           = 0;
  rpcGetCollRes.typeStructure = 0;
  rpcGetCollRes.typeName      = 0;
  rpcGetCollRes.collName      = 0;
            
  // rpcgetcolloidsbyname, rpcgetcolloidsbyoid
  if( rpcGetCollOidsRes.typeName )      free( rpcGetCollOidsRes.typeName );
  if( rpcGetCollOidsRes.typeStructure ) free( rpcGetCollOidsRes.typeStructure );
  if( rpcGetCollOidsRes.oid )           free( rpcGetCollOidsRes.oid );
  if( rpcGetCollOidsRes.collName )      free( rpcGetCollOidsRes.collName );
  rpcGetCollOidsRes.oid           = 0;
  rpcGetCollOidsRes.typeStructure = 0;
  rpcGetCollOidsRes.typeName      = 0;
  rpcGetCollOidsRes.collName      = 0;

  if( rpcGetCollOidsRes.oidTable.oidTable_len )
  {
    for( int i=0; i<rpcGetCollOidsRes.oidTable.oidTable_len; i++ )
      free( rpcGetCollOidsRes.oidTable.oidTable_val[i].oid );

    free( rpcGetCollOidsRes.oidTable.oidTable_val );

    rpcGetCollOidsRes.oidTable.oidTable_len = 0;
    rpcGetCollOidsRes.oidTable.oidTable_val = 0;
  }

  // rpcgetnewoid
  if( rpcOidRes.oid ) free( rpcOidRes.oid );
  rpcOidRes.oid = 0;

  // rpcgettypestructure  
  if( rpcGetTypeStructureRes.typeStructure ) free( rpcGetTypeStructureRes.typeStructure );
  rpcGetTypeStructureRes.typeStructure = 0;

  // execute all pending callbacks. Redirect alarm signal first to make sure no
  // reentrancy is possible!
  signal( SIGALRM, garbageCollectionDummy );
  ServerComm::actual_servercomm->callback_mgr.executePending();
  signal( SIGALRM, garbageCollection );
}


/*************************************************************
 *
 *
 * SOURCE: manager.cc
 *
 * MODULE: servercomm
 *
 *
 * COMMENTS:
 *
 ************************************************************/

#ifdef LINUX
#define RPCFUNCTIONDEF(name, param) name##_svc( param, struct svc_req* )
#define RPCFUNCTIONDEFP(name, param) *name##_svc( param, struct svc_req* )
#else
#define RPCFUNCTIONDEF(name, param) name##_svc( param, struct svc_req* )
#define RPCFUNCTIONDEFP(name, param) *name##_svc( param, struct svc_req*  )
#endif

#ifdef LINUX
extern "C"
#endif

//@ManMemo: Module: {\bf servercomm}

/**
  The global remote procedure \Ref{rpcgetserverversion_1} is called by a client to determine
  version numbers of the server.
*/
   
ServerVersionRes*
RPCFUNCTIONDEF( rpcgetserverversion_1, int* )
{
  freeDynamicRPCData();

  rpcServerVersionRes.serverVersionNo       = RMANVERSION / 1000.0;
  rpcServerVersionRes.rpcInterfaceVersionNo = RPCVERSION / 1000.0;
  
  return &rpcServerVersionRes;
}



#ifdef LINUX
extern "C"
#endif

//@ManMemo: Module: {\bf servercomm}

/**
  The global remote procedure \Ref{rpcshutdown_1} is called by a client to shut
  down the server.
*/
   
unsigned short*
RPCFUNCTIONDEF( rpcshutdown_1, int* )
{
  rpcDummy = 0;
  freeDynamicRPCData();
  // we don't allow the client to shut down the server. This is done thru rasmgr
  // may be is would be better to redesign this server interface and drop several 
  // functions
  return &rpcDummy;
}



#ifdef LINUX
extern "C"
#endif

//@ManMemo: Module: {\bf servercomm}

/**
  The global remote procedure \Ref{rpcServerStat_1} is called by a clientcomm
  object on the client system to display server statistics.
*/
   
ServerStatRes*
RPCFUNCTIONDEF( rpcserverstat_1, int* )
{
  secureResultBufferForRPC = (char*)&rpcServerStatRes;
  
  freeDynamicRPCData();

  // Get a pointer to the actual servercomm object.
  ServerComm* sc = ServerComm::actual_servercomm;
  
  // log server status
#ifdef RMANDEBUG
  sc->printServerStatus( RMInit::dbgOut );  
#endif
  
  // get server status
  sc->getServerStatus( rpcServerStatRes );
  
  return &rpcServerStatRes;
}



#ifdef LINUX
extern "C"
#endif
   
//@ManMemo: Module: {\bf servercomm}

/**
  The global remote procedure \Ref{rpckilltableentry_1} is called by a clientcomm
  object on the client system to kill a specific client table entry on the server.
*/
   
unsigned short*
RPCFUNCTIONDEF( rpckilltableentry_1, unsigned long* killId )
{
  secureResultBufferForRPC = (char*)&rpcDummy;
  rpcDummy = 0;
  unsigned long kId = *killId;

  freeDynamicRPCData();

  accessControl.wantToWrite();
  
  // Get a pointer to the actual servercomm object.
  ServerComm* sc = ServerComm::actual_servercomm;

  RMInit::logOut << " Kill specific table entry request received for client ID " << kId << "." << endl;
  
  if( sc && !sc->clientTbl.empty() )
  {

    list<ServerComm::ClientTblElt*>::iterator iter;
    iter = sc->clientTbl.begin();
      
    while ( *iter != NULL )
    {
      if( (*iter)->clientId == kId )
        {
	  RMInit::logOut << "ID " << (*iter)->clientId << " found, deleting..." << endl;

          sc->deleteClientTblEntry( (*iter)->clientId );

          // if this dead client has locked the transaction semaphor, unlock it
          if( sc->transactionActive == kId )
            sc->transactionActive = 0;
	  
	  // client IDs are unique, so this was the only one...
	  break;
        }
	else
          iter++;
    }   
    // The following is a necessary dummy command for ONC RPC for not 
    // misinterpreting the above break command as a return
    iter = sc->clientTbl.begin();
  }
return &rpcDummy;
}



#ifdef LINUX
extern "C"
#endif

//@ManMemo: Module: {\bf servercomm}

/**
*/   

unsigned short*
RPCFUNCTIONDEF( rpcalive_1, unsigned long* callingClientId )
{
  secureResultBufferForRPC = (char*)&rpcDummy;
  rpcDummy = 0;
  unsigned long cci = *callingClientId;

  freeDynamicRPCData();

  // Get a pointer to the actual servercomm object.
  ServerComm* sc = ServerComm::actual_servercomm;
  
  // Call the corresponding method.
  rpcDummy = sc->aliveSignal( cci );

  // Return the result
  return &rpcDummy;
}



#ifdef LINUX
extern "C"
#endif
   
//@ManMemo: Module: {\bf servercomm}

/*@Doc:
  The global remote procedure {\tt rpcopendb_1()} is called by a clientcomm
  object on the client system to open a database on a RasDaMan server.
*/
   
OpenDBRes*
RPCFUNCTIONDEF( rpcopendb_1, OpenDBParams* params )
{
  secureResultBufferForRPC = (char*)&rpcOpenDBRes;
  
  unsigned long* clientId = new unsigned long;
  
  freeDynamicRPCData();

  char             client[256];
  client[0] = '\0';
  strcat( client, "unknown" );

  const char* dbName   = params->dbName;
  const char* userName = params->userName;
  
  RMInit::logOut << "Client called ... ";
  
  //
  // Create a new entry in the client table (should be moved to ServerComm):
  //
  
  // Get a pointer to the actual servercomm object.
  ServerComm* sc = ServerComm::actual_servercomm;

  // Create a new client table element and initialize it.
  ServerComm::ClientTblElt* contextStore;
  contextStore = new ServerComm::ClientTblElt( client, ++(sc->clientCount) );
  
  // give the client id back to to client via the pointer variable
  *clientId    = sc->clientCount;
  // Put the context information in the static control list
  sc->clientTbl.push_back( contextStore );
  
  // RMInit::logOut << "assigned id " << *clientId << endl;
  
  // check acces permission
  
  rpcOpenDBRes.status=accessControl.crunchCapability(params->capability);
  //
  // Open the database: (only if acces control is OK)
  //
  if(rpcOpenDBRes.status==0) 
      rpcOpenDBRes.status = sc->openDB( *clientId, dbName, userName );

  rpcOpenDBRes.clientID = *clientId;
  delete clientId;
  
  // If database is not successfully opened, the client table entry is deleted again.
  if( rpcOpenDBRes.status != 0 )
    sc->deleteClientTblEntry( rpcOpenDBRes.clientID );
  
  return &rpcOpenDBRes;
}



#ifdef LINUX
extern "C"
#endif
   
//@ManMemo: Module: {\bf servercomm}

/*@Doc:
  The global remote procedure {\tt rpcclosedb_1()} is called by a clientcomm
  object on the client system to close a database on a RasDaMan server.
*/
   
unsigned short*
RPCFUNCTIONDEF( rpcclosedb_1, unsigned long* callingClientId )
{
  secureResultBufferForRPC = (char*)&rpcDummy;
  
  unsigned long cci;
  cci = *callingClientId;

  freeDynamicRPCData();

  // Get a pointer to the actual servercomm object.
  ServerComm* sc = ServerComm::actual_servercomm;
  
  // Call the corresponding method.
  rpcDummy = sc->closeDB( cci );

  if( rpcDummy == 0 )
    sc->deleteClientTblEntry( cci );

  return &rpcDummy;
}



#ifdef LINUX
extern "C"
#endif
   
//@ManMemo: Module: {\bf servercomm}

/*@Doc:
  The global remote procedure {\tt rpccreatedb_1()} is called by a clientcomm
  object on the client system to create a database on a RasDaMan server.
*/
unsigned short*
RPCFUNCTIONDEF( rpccreatedb_1, char** name )
{
  secureResultBufferForRPC = (char*)&rpcDummy;
  
  char* dbname = *name;

  freeDynamicRPCData();
  
  accessControl.wantToWrite();
  
  // Get a pointer to the actual servercomm object.
  ServerComm* sc = ServerComm::actual_servercomm;
  
  // Call the corresponding method. (createDB doesn't actually return something
  // other than 0, so this can be used in later extensions)
  rpcDummy = sc->createDB( dbname );  

  // Return the result
  return &rpcDummy;
}


#ifdef LINUX
extern "C"
#endif
   
//@ManMemo: Module: {\bf servercomm}

/*@Doc:
  The global remote procedure {\tt rpcdestroydb_1()} is called by a clientcomm
  object on the client system to destroy a database on a RasDaMan server.
*/
unsigned short*
RPCFUNCTIONDEF( rpcdestroydb_1, char** name )
{
  secureResultBufferForRPC = (char*)&rpcDummy;
  
  char* dbname = *name;

  freeDynamicRPCData();

  accessControl.wantToWrite();
  
  // Get a pointer to the actual servercomm object.
  ServerComm* sc = ServerComm::actual_servercomm;
  
  // Call the corresponding method. (createDB doesn't actually return something
  // other than 0, so this can be used in later extensions)
  rpcDummy = sc->destroyDB( dbname );  

  // Return the result
  return &rpcDummy;
}



#ifdef LINUX
extern "C"
#endif
   
//@ManMemo: Module: {\bf servercomm}

/*@Doc:
  The global remote procedure {\tt rpcBeginTA()} is called by a clientcomm
  object on the client system to begin a new transaction in a session with
  a RasDaMan server.
*/
   
unsigned short*
RPCFUNCTIONDEF( rpcbeginta_1, BeginTAParams* params )
{
  secureResultBufferForRPC = (char*)&rpcDummy;
  
  freeDynamicRPCData();

  // Get a pointer to the actual servercomm object.
  ServerComm* sc = ServerComm::actual_servercomm;

  // check acces permission
  rpcDummy = accessControl.crunchCapability(params->capability);
  
  // Call the corresponding method. (only if acces control is OK)
  if(rpcDummy==0)
    rpcDummy = sc->beginTA( params->clientID, params->readOnly );

  // Return the result
  return &rpcDummy;
}



#ifdef LINUX
extern "C"
#endif
   
//@ManMemo: Module: {\bf servercomm}

/*@Doc:
  The global remote procedure {\tt rpcCommitTA()} is called by a clientcomm
  object on the client system to commit the current transaction in a session with
  a RasDaMan server.
*/
   
unsigned short*
RPCFUNCTIONDEF( rpccommitta_1, unsigned long* callingClientId )
{
  secureResultBufferForRPC = (char*)&rpcDummy;
  
  unsigned long cci;
  cci = *callingClientId;

  freeDynamicRPCData();

  // Get a pointer to the actual servercomm object.
  ServerComm* sc = ServerComm::actual_servercomm;
  
  // Call the corresponding method.
  rpcDummy = sc->commitTA( cci );

  // Return the result
  return &rpcDummy;
}



#ifdef LINUX
extern "C"
#endif
   
//@ManMemo: Module: {\bf servercomm}

/*@Doc:
  The global remote procedure {\tt rpcAbortTA()} is called by a clientcomm
  object on the client system to abort the current transaction in a session with
  a RasDaMan server.
*/
   
unsigned short*
RPCFUNCTIONDEF( rpcabortta_1, unsigned long* callingClientId )
{
  secureResultBufferForRPC = (char*)&rpcDummy;
  
  unsigned long cci;
  cci = *callingClientId;

  freeDynamicRPCData();

  // Get a pointer to the actual servercomm object.
  ServerComm* sc = ServerComm::actual_servercomm;
  
  // Call the corresponding method.
  rpcDummy = sc->abortTA( cci );

  // Return the result
  return &rpcDummy;
}



#ifdef LINUX
extern "C"
#endif

//@ManMemo: Module: {\bf servercomm}

/*@Doc:
  The global remote procedure {\tt rpcexecutequery_1()} is called by a clientcomm
  object on the client system to initiate a retrieval query execution on a RasDaMan
  server.
*/
   
ExecuteQueryRes*
RPCFUNCTIONDEF( rpcexecutequery_1, ExecuteQueryParams* params )
{
  secureResultBufferForRPC = (char*)&rpcExecuteQueryRes;
  
  unsigned short         returnValue;
  unsigned long          callingClientId = params->clientID;
  const char*            query = params->query;

  freeDynamicRPCData();
  
  // prevent RPC from NULL pointers in case of exceptions
  char *prev1 = strdup("");  
  char *prev2 = strdup("");
  char *prev3 = strdup("");
  rpcExecuteQueryRes.token         = prev1;         
  rpcExecuteQueryRes.typeName      = prev2;
  rpcExecuteQueryRes.typeStructure = prev3;

  // Get a pointer to the actual servercomm object.
  ServerComm* sc = ServerComm::actual_servercomm;
  
  // Call the corresponding method.
  returnValue = sc->executeQuery( callingClientId, (const char*) query, rpcExecuteQueryRes );

  // if no exception was thrown we are here, first check if the pointers have changed
  if( rpcExecuteQueryRes.token         != prev1 ) free(prev1);
  if( rpcExecuteQueryRes.typeName      != prev2 ) free(prev2);
  if( rpcExecuteQueryRes.typeStructure != prev3 ) free(prev3);
  
  // than check if the pointers are NULL
  if( !rpcExecuteQueryRes.token )         rpcExecuteQueryRes.token         = strdup("");
  if( !rpcExecuteQueryRes.typeName )      rpcExecuteQueryRes.typeName      = strdup("");
  if( !rpcExecuteQueryRes.typeStructure ) rpcExecuteQueryRes.typeStructure = strdup("");
  
  // set missing parts of return structure
  rpcExecuteQueryRes.status     = returnValue;


  return &rpcExecuteQueryRes;
} 


#ifdef LINUX
extern "C"
#endif
   
//@ManMemo: Module: {\bf servercomm}

/*@Doc:
  The global remote procedure {\tt rpcgetnextmdd_1()} is called by a clientcomm
  object on the client system to transmit the next element of a MDD collection
  from a RasDaMan server.
*/
   
GetMDDRes*
RPCFUNCTIONDEF( rpcgetnextmdd_1, unsigned long* callingClientId )
{
  RMDBGENTER(2, RMDebug::module_servercomm, "Manager", "rpcgetnextmdd_1" )
  
  secureResultBufferForRPC = (char*)&rpcGetMDDRes;

  r_Minterval mddDomain;
  r_OId       oid;
     
  freeDynamicRPCData();
  
  // prevent RPC from NULL pointers
  char *prev1 = strdup("");  
  char *prev2 = strdup("");
  char *prev3 = strdup("");
  char *prev4 = strdup("");
  
  rpcGetMDDRes.typeName      = prev1;
  rpcGetMDDRes.typeStructure = prev2;
  rpcGetMDDRes.domain        = prev3;
  rpcGetMDDRes.oid           = prev4;
  
  accessControl.wantToRead();
  
  // Get a pointer to the actual servercomm object.
  ServerComm* sc = ServerComm::actual_servercomm;

  // Call the corresponding method and return the result
  rpcGetMDDRes.status = sc->getNextMDD( *callingClientId, mddDomain,
                                        rpcGetMDDRes.typeName,
                                        rpcGetMDDRes.typeStructure, oid,
                                        rpcGetMDDRes.currentFormat );
  rpcGetMDDRes.domain = mddDomain.get_string_representation();
  rpcGetMDDRes.oid    = oid.get_string_representation() ? strdup( oid.get_string_representation() ) : strdup("");

  // if no exceptions...
  if( rpcGetMDDRes.typeName      != prev1 ) free(prev1);
  if( rpcGetMDDRes.typeStructure != prev2 ) free(prev2);
  if( rpcGetMDDRes.domain        != prev3 ) free(prev3);
  if( rpcGetMDDRes.oid           != prev4 ) free(prev4);
  
  // prevent RPC from NULL pointers
  if( !rpcGetMDDRes.typeName      ) rpcGetMDDRes.typeName      = strdup("");
  if( !rpcGetMDDRes.typeStructure ) rpcGetMDDRes.typeStructure = strdup("");
  // the other 2 are not null
  
  RMDBGEXIT(2, RMDebug::module_servercomm, "Manager", "rpcgetnextmdd_1" )
  return &rpcGetMDDRes;
}



#ifdef LINUX
extern "C"
#endif
   
//@ManMemo: Module: {\bf servercomm}

/*@Doc:
  The global remote procedure {\tt rpcgetnextelement_1()} is called by a clientcomm
  object on the client system to transmit the next element of a non-MDD collection
  from a RasDaMan server.
*/
   
GetElementRes*
RPCFUNCTIONDEF( rpcgetnextelement_1, unsigned long* callingClientId )
{
  RMDBGENTER(2, RMDebug::module_servercomm, "Manager", "rpcgetnextelement_1" )
  
  secureResultBufferForRPC = (char*)&rpcGetElementRes;

  freeDynamicRPCData();
  
  accessControl.wantToRead();
  
  // Get a pointer to the actual servercomm object.
  ServerComm* sc = ServerComm::actual_servercomm;

  // Call the corresponding method and return the result
  rpcGetElementRes.status = sc->getNextElement( *callingClientId, rpcGetElementRes.data.confarray_val,
                                                                  rpcGetElementRes.data.confarray_len );

  RMDBGEXIT(2, RMDebug::module_servercomm, "Manager", "rpcgetnextelement_1" )
  return &rpcGetElementRes;
}



#ifdef LINUX
extern "C"
#endif
   
//@ManMemo: Module: {\bf servercomm}

/*@Doc:
  The global remote procedure \Ref{rpcgetmddbyoid_1} is called by a clientcomm
  object on the client system to receive an MDD by its oid.
*/
   
GetMDDRes*
RPCFUNCTIONDEF( rpcgetmddbyoid_1, OIdSpecParams* params )
{
  RMDBGENTER(2, RMDebug::module_servercomm, "Manager", "rpcgetmddbyoid_1" )
  
  secureResultBufferForRPC = (char*)&rpcGetMDDRes;

  r_Minterval mddDomain;
  r_OId       oid( params->oid );
     
  freeDynamicRPCData();
  // prevent RPC from NULL pointers if exception occur
  char *prev1 = strdup("");
  char *prev2 = strdup("");
  char *prev3 = strdup("");
  char *prev4 = strdup("");
  
  rpcGetMDDRes.typeName      = prev1;
  rpcGetMDDRes.typeStructure = prev2;
  rpcGetMDDRes.domain        = prev3;
  rpcGetMDDRes.oid           = prev4;
  
  accessControl.wantToRead();
  
  // Get a pointer to the actual servercomm object.
  ServerComm* sc = ServerComm::actual_servercomm;

  // Call the corresponding method and return the result
  rpcGetMDDRes.status = sc->getMDDByOId( params->clientID, oid, mddDomain, 
                                         rpcGetMDDRes.typeName, 
                                         rpcGetMDDRes.typeStructure,
                                         rpcGetMDDRes.currentFormat  );
  rpcGetMDDRes.domain = mddDomain.get_string_representation();
  rpcGetMDDRes.oid    = oid.get_string_representation() ? strdup( oid.get_string_representation() ) : strdup("");

  // prevent RPC from NULL pointers
  if( rpcGetMDDRes.typeName      != prev1) free(prev1);
  if( rpcGetMDDRes.typeStructure != prev2) free(prev2);
  if( rpcGetMDDRes.domain        != prev3 ) free(prev3);
  if( rpcGetMDDRes.oid           != prev4 ) free(prev4);
  
  if( !rpcGetMDDRes.typeName      ) rpcGetMDDRes.typeName      = strdup("");
  if( !rpcGetMDDRes.typeStructure ) rpcGetMDDRes.typeStructure = strdup("");
  // the other 2 are not null
  
  RMDBGEXIT(2, RMDebug::module_servercomm, "Manager", "rpcgetmddbyoid_1" )
  return &rpcGetMDDRes;
}



#ifdef LINUX
extern "C"
#endif
   
//@ManMemo: Module: {\bf servercomm}

/*@Doc:
  The global remote procedure {\tt rpcgetnexttile_1()} is called by a clientcomm
  object on the client system to transmit the next element of a MDD collection
  from a RasDaMan server.
*/
   
GetTileRes*
RPCFUNCTIONDEF( rpcgetnexttile_1, unsigned long* callingClientId )
{
  RMDBGENTER(2, RMDebug::module_servercomm, "Manager", "rpcgetnexttile_1" )
  
  secureResultBufferForRPC = (char*)&rpcGetTileRes;

  freeDynamicRPCData();
  
  // fake data for security
  RPCMarray *secureRpcMarray = (RPCMarray*)mymalloc(sizeof(RPCMarray));
  secureRpcMarray->domain = strdup("");
  secureRpcMarray->cellTypeLength = 1;
  secureRpcMarray->currentFormat = 1;
  secureRpcMarray->storageFormat = 1;
  secureRpcMarray->data.confarray_len = 1;
  secureRpcMarray->data.confarray_val = strdup("");
  
  rpcGetTileRes.marray = secureRpcMarray;
  
  RPCMarray *tempRpcMarray;
  
  accessControl.wantToRead();
  
  // Get a pointer to the actual servercomm object.
  ServerComm* sc = ServerComm::actual_servercomm;

  // Call the corresponding method and return the result
  rpcGetTileRes.status = sc->getNextTile( *callingClientId, &tempRpcMarray); //&rpcGetTileRes.marray );
  // if this throws, secure... is nice initialized
  
  rpcGetTileRes.marray = tempRpcMarray;
  
  free(secureRpcMarray->data.confarray_val);
  free(secureRpcMarray->domain);
  free(secureRpcMarray);
  
  RMDBGEXIT(2, RMDebug::module_servercomm, "Manager", "rpcgetnexttile_1" )
  return &rpcGetTileRes;
}



#ifdef LINUX
extern "C"
#endif   
//@ManMemo: Module: {\bf servercomm}

/**
*/   
unsigned short*
RPCFUNCTIONDEF( rpcendtransfer_1, unsigned long* callingClientId )
{
  unsigned long cci = *callingClientId;
  
  secureResultBufferForRPC = (char*)&rpcDummy;

  freeDynamicRPCData();

  // Get a pointer to the actual servercomm object.
  ServerComm* sc = ServerComm::actual_servercomm;
  
  // Call the corresponding method.
  rpcDummy = sc->endTransfer( cci );

  // Return the result
  return &rpcDummy;
}



#ifdef LINUX
extern "C"
#endif   
//@ManMemo: Module: {\bf servercomm}

/**
*/   
unsigned short*
RPCFUNCTIONDEF( rpcinitexecuteupdate_1, unsigned long* callingClientId )
{
  secureResultBufferForRPC = (char*)&rpcDummy;
  
  unsigned long cci;
  cci = *callingClientId;

  freeDynamicRPCData();

  accessControl.wantToWrite();

  // Get a pointer to the actual servercomm object.
  ServerComm* sc = ServerComm::actual_servercomm;

  // Call the corresponding method.
  rpcDummy = sc->initExecuteUpdate( cci );

  // Return the result
  return &rpcDummy;
}



#ifdef LINUX
extern "C"
#endif

//@ManMemo: Module: {\bf servercomm}

/*@Doc:
  The global remote procedure {\tt rpcexecuteupdate_1()} is called by a clientcomm
  object on the client system to initiate an update query execution on a RasDaMan
  server.
*/
   
ExecuteUpdateRes*
RPCFUNCTIONDEF( rpcexecuteupdate_1, ExecuteQueryParams* params )
{
  secureResultBufferForRPC = (char*)&rpcExecuteUpdateRes;
  
  unsigned short          returnValue;
  unsigned long           callingClientId = params->clientID;
  const char*             query = params->query;

  freeDynamicRPCData();
  
  // prevent RPC from NULL pointers
  char *prev1 = strdup("");
  rpcExecuteUpdateRes.token = prev1;

  accessControl.wantToWrite();
  
  // Get a pointer to the actual servercomm object.
  ServerComm* sc = ServerComm::actual_servercomm;
  
  // Call the corresponding method.
  returnValue = sc->executeUpdate( callingClientId, (const char*) query, rpcExecuteUpdateRes );

  // set missing parts of return structure
  rpcExecuteUpdateRes.status     = returnValue;
     
  // prevent RPC from NULL pointers
  if( rpcExecuteUpdateRes.token != prev1 ) free(prev1);
  
  if( !rpcExecuteUpdateRes.token )         rpcExecuteUpdateRes.token         = strdup("");

  return &rpcExecuteUpdateRes;
} 



#ifdef LINUX
extern "C"
#endif
//@ManMemo: Module: {\bf servercomm}

/**
*/   
unsigned short*
RPCFUNCTIONDEF( rpcstartinserttransmdd_1, InsertTransMDDParams* params )
{
  secureResultBufferForRPC = (char*)&rpcDummy;
  
  unsigned long callingClientId = params->clientID;
  //  const char* collName          = params->collName; // not used
  const char* typeName          = params->typeName;
  unsigned long typeLength      = params->typeLength;
  r_Minterval mddDomain( params->domain );

  freeDynamicRPCData();
  
  accessControl.wantToWrite();

  // Get a pointer to the actual servercomm object.
  ServerComm* sc = ServerComm::actual_servercomm;
  
  // Call the corresponding method.
  rpcDummy = sc->startInsertTransMDD( callingClientId, mddDomain, typeLength, typeName );

  // Return the result
  return &rpcDummy;
}



#ifdef LINUX
extern "C"
#endif
//@ManMemo: Module: {\bf servercomm}

/**
*/   
unsigned short*
RPCFUNCTIONDEF( rpcstartinsertpersmdd_1, InsertPersMDDParams* params )
{
  secureResultBufferForRPC = (char*)&rpcDummy;
  
  unsigned long callingClientId = params->clientID;
  const char* collName          = params->collName;
  const char* typeName          = params->typeName;
  unsigned long typeLength      = params->typeLength;
  r_Minterval mddDomain( params->domain );
  r_OId       oid( params->oid );

  freeDynamicRPCData();
  
  accessControl.wantToWrite();

  // Get a pointer to the actual servercomm object.
  ServerComm* sc = ServerComm::actual_servercomm;
  
  // Call the corresponding method.
  rpcDummy = sc->startInsertPersMDD( callingClientId, (const char*) collName, mddDomain, typeLength, typeName, oid );

  // Return the result
  return &rpcDummy;
}



#ifdef LINUX
extern "C"
#endif
//@ManMemo: Module: {\bf servercomm}

/**
*/   
unsigned short*
RPCFUNCTIONDEF( rpcinserttile_1, InsertTileParams* params )
{
  secureResultBufferForRPC = (char*)&rpcDummy;

  freeDynamicRPCData();

  accessControl.wantToWrite();
  
  // Get a pointer to the actual servercomm object.
  ServerComm* sc = ServerComm::actual_servercomm;
  
  // Call the corresponding method.
  rpcDummy = sc->insertTile( params->clientID, params->isPersistent, params->marray );

  // Return the result
  return &rpcDummy;
}



#ifdef LINUX
extern "C"
#endif
   
//@ManMemo: Module: {\bf servercomm}

/**
*/   
unsigned short*
RPCFUNCTIONDEF( rpcendinsertmdd_1, EndInsertMDDParams* params )
{
  secureResultBufferForRPC = (char*)&rpcDummy;

  freeDynamicRPCData();
  
  accessControl.wantToWrite();

  // Get a pointer to the actual servercomm object.
  ServerComm* sc = ServerComm::actual_servercomm;

  // Call the corresponding method.
  rpcDummy = sc->endInsertMDD( params->clientID, params->isPersistent );

  // Return the result
  return &rpcDummy;
}



#ifdef LINUX
extern "C"
#endif
   
//@ManMemo: Module: {\bf servercomm}

/*@Doc:
  The global remote procedure {\tt rpcInsertMDD()} is called by a clientcomm
  object on the client system to insert a MDD object in an existing MDD 
  collection on a RasDaMan server.
*/
   
unsigned short*
RPCFUNCTIONDEF( rpcinsertmdd_1, InsertMDDParams* params )
{
  secureResultBufferForRPC = (char*)&rpcDummy;
  
  unsigned long callingClientId = params->clientID;
  const char*   collName        = params->collName;
  const char*   typeName        = params->typeName;
  RPCMarray*    rpcMarray       = params->marray;

  r_OId oid( params->oid );

  freeDynamicRPCData();
  
  accessControl.wantToWrite();

  // Get a pointer to the actual servercomm object.
  ServerComm* sc = ServerComm::actual_servercomm;
  
  // Call the corresponding method.
  rpcDummy = sc->insertMDD( callingClientId, collName, rpcMarray, typeName, oid );

  return &rpcDummy;
}



#ifdef LINUX
extern "C"
#endif
   
//@ManMemo: Module: {\bf servercomm}

/*@Doc:
  The global remote procedure {\tt rpcgetcollbyname_1()} is called by a clientcomm
  object on the client system to initiate the lookup of a MDD collection by its name 
  on a RasDaMan server.
*/
   
GetCollRes*
RPCFUNCTIONDEF( rpcgetcollbyname_1, NameSpecParams* params )
{
  RMDBGENTER(2, RMDebug::module_servercomm, "Manager", "rpcgetcollbyname_1" )
  
  secureResultBufferForRPC = (char*)&rpcGetCollRes;

  r_OId       oid;

  freeDynamicRPCData();
  
  char *prev1 = strdup("");
  char *prev2 = strdup("");
  char *prev3 = strdup("");
  char *prev4 = strdup("");
  rpcGetCollRes.typeName      = prev1;
  rpcGetCollRes.typeStructure = prev2;
  rpcGetCollRes.oid           = prev3;
  rpcGetCollRes.collName      = prev4;
  
  accessControl.wantToRead();

  unsigned long callingClientId = params->clientID;
  const char*   collName        = params->name;
  
  // Get a pointer to the actual servercomm object.
  ServerComm* sc = ServerComm::actual_servercomm;
  
  // Call the corresponding method.
  rpcGetCollRes.status        = sc->getCollByName( callingClientId, collName, 
                                                   rpcGetCollRes.typeName,
                                                   rpcGetCollRes.typeStructure, oid );

  rpcGetCollRes.oid      = oid.get_string_representation() ? strdup( oid.get_string_representation() ) : strdup("");
  rpcGetCollRes.collName = params->name                    ? strdup( params->name )                    : strdup("");

  // prevent RPC from NULL pointers
  if( rpcGetCollRes.typeName      != prev1) free(prev1);
  if( rpcGetCollRes.typeStructure != prev2) free(prev2);
  if( rpcGetCollRes.oid           != prev3) free(prev3);
  if( rpcGetCollRes.collName      != prev4) free(prev4);

  if( !rpcGetCollRes.typeName      ) rpcGetCollRes.typeName      = strdup("");
  if( !rpcGetCollRes.typeStructure ) rpcGetCollRes.typeStructure = strdup("");
  
  // Return the result
  RMDBGEXIT(2, RMDebug::module_servercomm, "Manager", "rpcgetcollbyname_1" )
  return &rpcGetCollRes;
} 



#ifdef LINUX
extern "C"
#endif
   
//@ManMemo: Module: {\bf servercomm}

/*@Doc:
  The global remote procedure {\tt rpcgetcollbyoid_1()} is called by a clientcomm
  object on the client system to initiate the lookup of a MDD collection by its oid
  on a RasDaMan server.
*/
   
GetCollRes*
RPCFUNCTIONDEF( rpcgetcollbyoid_1, OIdSpecParams* params )
{
  RMDBGENTER(2, RMDebug::module_servercomm, "Manager", "rpcgetcollbyoid_1" )
  
  secureResultBufferForRPC = (char*)&rpcGetCollRes;
  
  r_OId       oid( params->oid );

  freeDynamicRPCData();
  // prevent RPC from NULL pointers
  char *prev1 = strdup("");
  char *prev2 = strdup("");
  char *prev3 = strdup("");
  char *prev4 = strdup("");
  rpcGetCollRes.typeName      = prev1;
  rpcGetCollRes.typeStructure = prev2;
  rpcGetCollRes.collName      = prev3;
  rpcGetCollRes.oid           = prev4;
  
  accessControl.wantToRead();

  unsigned long callingClientId = params->clientID;
  
  // Get a pointer to the actual servercomm object.
  ServerComm* sc = ServerComm::actual_servercomm;
  
  // Call the corresponding method.
  rpcGetCollRes.status        = sc->getCollByOId( callingClientId, oid, rpcGetCollRes.typeName,
                                               rpcGetCollRes.typeStructure, rpcGetCollRes.collName );
  
  rpcGetCollRes.oid = oid.get_string_representation() ? strdup( oid.get_string_representation() ) : strdup("");

  // prevent RPC from NULL pointers
  if( rpcGetCollRes.typeName      != prev1) free(prev1);
  if( rpcGetCollRes.typeStructure != prev2) free(prev2);
  if( rpcGetCollRes.collName      != prev3) free(prev3);
  if( rpcGetCollRes.oid           != prev4) free(prev4);
  
  if( !rpcGetCollRes.typeName      ) rpcGetCollRes.typeName      = strdup("");
  if( !rpcGetCollRes.typeStructure ) rpcGetCollRes.typeStructure = strdup("");
  if( !rpcGetCollRes.collName      ) rpcGetCollRes.collName      = strdup("");
  // oid is not null, 
  
  // Return the result
  RMDBGEXIT(2, RMDebug::module_servercomm, "Manager", "rpcgetcollbyoid_1" )
  return &rpcGetCollRes;
} 



#ifdef LINUX
extern "C"
#endif
   
//@ManMemo: Module: {\bf servercomm}

/*@Doc:
  The global remote procedure {\tt rpcgetcolloidsbyname_1()} is called by a clientcomm
  object on the client system to retrieve the collection of oids by the collection's name.
*/
   
GetCollOIdsRes*
RPCFUNCTIONDEF( rpcgetcolloidsbyname_1, NameSpecParams* params )
{
  RMDBGENTER(2, RMDebug::module_servercomm, "Manager", "rpcgetcolloidsbyname_1" )

  secureResultBufferForRPC = (char*)&rpcGetCollOidsRes;
  
  r_OId       oid;

  freeDynamicRPCData();
  
  char *prev1 = strdup("");
  char *prev2 = strdup("");
  char *prev3 = strdup("");
  char *prev4 = strdup("");
  rpcGetCollOidsRes.typeName      = prev1;
  rpcGetCollOidsRes.typeStructure = prev2;
  rpcGetCollOidsRes.collName      = prev3;
  rpcGetCollRes.oid               = prev4;
  
  accessControl.wantToRead();

  unsigned long callingClientId = params->clientID;
  const char*   collName        = params->name;
  
  // Get a pointer to the actual servercomm object.
  ServerComm* sc = ServerComm::actual_servercomm;
  
  // Call the corresponding method.
  rpcGetCollOidsRes.status        = sc->getCollOIdsByName( callingClientId, collName, rpcGetCollOidsRes.typeName,
                                                    rpcGetCollOidsRes.typeStructure, oid,
                                                    rpcGetCollOidsRes.oidTable.oidTable_val, rpcGetCollOidsRes.oidTable.oidTable_len );

  rpcGetCollOidsRes.oid      = oid.get_string_representation() ? strdup( oid.get_string_representation() ) : strdup("");
  rpcGetCollOidsRes.collName = params->name                    ? strdup( params->name )                    : strdup("");

  // prevent RPC from NULL pointers
  if( rpcGetCollOidsRes.typeName      != prev1 ) free(prev1);
  if( rpcGetCollOidsRes.typeStructure != prev2 ) free(prev2);
  if( rpcGetCollOidsRes.collName      != prev3 ) free(prev3);
  if( rpcGetCollRes.oid               != prev4) free(prev4);
  
  if( !rpcGetCollOidsRes.typeName      ) rpcGetCollOidsRes.typeName      = strdup("");
  if( !rpcGetCollOidsRes.typeStructure ) rpcGetCollOidsRes.typeStructure = strdup("");
  if( !rpcGetCollOidsRes.collName      ) rpcGetCollOidsRes.collName      = strdup("");
  // oid is not null
  
  // Return the result
  RMDBGEXIT(2, RMDebug::module_servercomm, "Manager", "rpcgetcolloidsbyname_1" )
  return &rpcGetCollOidsRes;
} 




#ifdef LINUX
extern "C"
#endif
   
//@ManMemo: Module: {\bf servercomm}

/*@Doc:
  The global remote procedure {\tt rpcgetcolloidsbyoid_1()} is called by a clientcomm
  object on the client system to retrieve the collection of oids by the collection's oid.
*/
   
GetCollOIdsRes*
RPCFUNCTIONDEF( rpcgetcolloidsbyoid_1, OIdSpecParams* params )
{
  RMDBGENTER(2, RMDebug::module_servercomm, "Manager", "rpcgetcolloidsbyname_1" )
  
  secureResultBufferForRPC = (char*)&rpcGetCollOidsRes;
  
  r_OId       oid( params->oid );

  freeDynamicRPCData();
  
  char *prev1 = strdup("");
  char *prev2 = strdup("");
  char *prev3 = strdup("");
  char *prev4 = strdup("");
  rpcGetCollOidsRes.typeName      = prev1;
  rpcGetCollOidsRes.typeStructure = prev2;
  rpcGetCollOidsRes.collName      = prev3;
  rpcGetCollRes.oid               = prev4;
  
  accessControl.wantToRead();

  unsigned long callingClientId = params->clientID;
  
  // Get a pointer to the actual servercomm object.
  ServerComm* sc = ServerComm::actual_servercomm;
  
  // Call the corresponding method.
  rpcGetCollOidsRes.status = sc->getCollOIdsByOId(  callingClientId, oid, 
                                                    rpcGetCollOidsRes.typeName,
                                                    rpcGetCollOidsRes.typeStructure,
                                                    rpcGetCollOidsRes.oidTable.oidTable_val,
                                                    rpcGetCollOidsRes.oidTable.oidTable_len,
                                                    rpcGetCollOidsRes.collName );

  rpcGetCollOidsRes.oid = oid.get_string_representation() ? strdup( oid.get_string_representation() ) : strdup("");

  // prevent RPC from NULL pointers
  if( rpcGetCollOidsRes.typeName      != prev1 ) free(prev1);
  if( rpcGetCollOidsRes.typeStructure != prev2 ) free(prev2);
  if( rpcGetCollOidsRes.collName      != prev3 ) free(prev3);
  if( rpcGetCollRes.oid               != prev4)  free(prev4);

  if( !rpcGetCollOidsRes.typeName      ) rpcGetCollOidsRes.typeName      = strdup("");
  if( !rpcGetCollOidsRes.typeStructure ) rpcGetCollOidsRes.typeStructure = strdup("");
  if( !rpcGetCollOidsRes.collName      ) rpcGetCollOidsRes.collName      = strdup("");
  
  // Return the result
  RMDBGEXIT(2, RMDebug::module_servercomm, "Manager", "rpcgetcolloidsbyname_1" )
  return &rpcGetCollOidsRes;
} 



#ifdef LINUX
extern "C"
#endif
   
//@ManMemo: Module: {\bf servercomm}

/*@Doc:
  The global remote procedure {\tt rpcinsertcoll_1()} is called by a clientcomm
  object on the client system to create a MDD collection for further use on
  a RasDaMan server.
*/
   
unsigned short*
RPCFUNCTIONDEF( rpcinsertcoll_1, InsertCollParams* params )
{
  secureResultBufferForRPC = (char*)&rpcDummy;
  
  unsigned long callingClientId = params->clientID;
  const char* collName          = params->collName;
  const char* typeName          = params->typeName;

  freeDynamicRPCData();
  
  accessControl.wantToWrite();

  r_OId oid( params->oid );
  
  // Get a pointer to the actual servercomm object.
  ServerComm* sc = ServerComm::actual_servercomm;
  
  // Call the corresponding method.
  rpcDummy = sc->insertColl( callingClientId, collName, typeName, oid );
  
  // Return the result
  return &rpcDummy;
}



#ifdef LINUX
extern "C"
#endif
   
//@ManMemo: Module: {\bf servercomm}

/*@Doc:
  The global remote procedure {\tt rpcdeletecollbyname_1()} is called by a clientcomm
  object on the client system to delete a existing MDD collection.
*/
   
unsigned short*
RPCFUNCTIONDEF( rpcdeletecollbyname_1, NameSpecParams* params )
{
  secureResultBufferForRPC = (char*)&rpcDummy;
  
  unsigned long callingClientId = params->clientID;
  const char*   collName        = params->name;

  freeDynamicRPCData();

  accessControl.wantToWrite();
  
  // Get a pointer to the actual servercomm object.
  ServerComm* sc = ServerComm::actual_servercomm;
  
  // Call the corresponding method.
  rpcDummy = sc->deleteCollByName( callingClientId, collName );
  
  // Return the result
  return &rpcDummy;
}



#ifdef LINUX
extern "C"
#endif
   
//@ManMemo: Module: {\bf servercomm}

/*@Doc:
  The global remote procedure {\tt rpcdeleteobjbyoid_1()} is called by a clientcomm
  object on the client system to delete an object specified by oid.
*/
   
unsigned short*
RPCFUNCTIONDEF( rpcdeleteobjbyoid_1, OIdSpecParams* params )
{
  secureResultBufferForRPC = (char*)&rpcDummy;
  
  unsigned long callingClientId = params->clientID;
  r_OId         oid( params->oid );

  freeDynamicRPCData();
  
  accessControl.wantToWrite();

  // Get a pointer to the actual servercomm object.
  ServerComm* sc = ServerComm::actual_servercomm;
  
  // Call the corresponding method.
  rpcDummy = sc->deleteObjByOId( callingClientId, oid );
  
  // Return the result
  return &rpcDummy;
}



#ifdef LINUX
extern "C"
#endif
   
//@ManMemo: Module: {\bf servercomm}

/*@Doc:
  The global remote procedure {\tt rpcremoveobjfromcoll_1()} is called by a clientcomm
  object on the client system to delete an object from a collection.
*/
   
unsigned short*
RPCFUNCTIONDEF( rpcremoveobjfromcoll_1, RemoveObjFromCollParams* params )
{
  secureResultBufferForRPC = (char*)&rpcDummy;
  
  unsigned long callingClientId = params->clientID;
  const char*   collName        = params->collName;  
  r_OId         oid( params->oid );

  freeDynamicRPCData();
  
  accessControl.wantToWrite();

  // Get a pointer to the actual servercomm object.
  ServerComm* sc = ServerComm::actual_servercomm;
  
  // Call the corresponding method.
  rpcDummy = sc->removeObjFromColl( callingClientId, collName, oid );
  
  // Return the result
  return &rpcDummy;
}



#ifdef LINUX
extern "C"
#endif
   
//@ManMemo: Module: {\bf servercomm}

/*@Doc:
*/
   
ObjectTypeRes*
RPCFUNCTIONDEF( rpcgetobjecttype_1, OIdSpecParams* params )
{
  secureResultBufferForRPC = (char*)&procResult;
  
  freeDynamicRPCData();
  
  accessControl.wantToRead();


  // Get a pointer to the actual servercomm object.
  ServerComm* sc = ServerComm::actual_servercomm;
  
  // Call the corresponding method.
  r_OId oid( params->oid );

  procResult.status = sc->getObjectType( params->clientID, oid, procResult.objType );
  
  // Return the result
  return &procResult;
}



#ifdef LINUX
extern "C"
#endif
   
//@ManMemo: Module: {\bf servercomm}

/*@Doc:
*/
   
OIdRes*
RPCFUNCTIONDEF( rpcgetnewoid_1, NewOIdParams* params )
{
  secureResultBufferForRPC = (char*)&rpcOidRes;
  
  freeDynamicRPCData();
  
  char *prev1   = strdup("");
  rpcOidRes.oid = prev1;
  
  accessControl.wantToWrite();

  // Get a pointer to the actual servercomm object.
  ServerComm* sc = ServerComm::actual_servercomm;
  
  // Call the corresponding method.
  r_OId oid;

  rpcOidRes.status = sc->getNewOId( params->clientID, params->objType, oid );
  rpcOidRes.oid    = oid.get_string_representation() ? strdup( oid.get_string_representation() ) : strdup("");
    free(prev1);
    
  // Return the result
  return &rpcOidRes;
}


// Callback for callback manager, performs actual garbage collection.
// The context points to the ServerComm object (see registering call in
// garbageCollection() )
static void callback_garbage_collection(void *context)
{
  ServerComm *sc = (ServerComm*)context;

  if( sc && !sc->clientTbl.empty() )
  {
    RMInit::dbgOut << "Garbage Collection ... " << flush;

#ifdef RMANDEBUG
    sc->printServerStatus( RMInit::dbgOut );
#endif

    if (!noTimeOut)
    {
      list<ServerComm::ClientTblElt*>::iterator iter;
      iter = sc->clientTbl.begin();
      unsigned long now = time( NULL );
      
      RMDBGONCE(2, RMDebug::module_servercomm, "Manager", "checking " << sc->clientTbl.size() << " clients..." );
      while ( iter != sc->clientTbl.end() )
      {
	if( now - (*iter)->lastActionTime >= sc->clientTimeout )
	{
	  RMInit::logOut << "Message: Found timed-out client with id " << (*iter)->clientId
			 << " (" << (*iter)->clientIdText << ", "
			 << now - (*iter)->lastActionTime << "s)..." << flush;

	  if( sc->deleteClientTblEntry( (*iter)->clientId ) == 0 )
	  {
	    RMInit::logOut << "deleted." << endl;
	
	    sc->informRasMGR(SERVER_AVAILABLE);
	    // reset the iterator (otherwise, it would skip one object
	    // because one was deleted)
	    iter = sc->clientTbl.begin();
	  }
	  else
	  {
	    RMInit::logOut << "deletion postponed." << endl;
	    iter++;
	  }
	}
	else
	  iter++;
      }
    }
    RMInit::dbgOut << "garbage collection done." << endl;
  }
}


#ifdef LINUX
extern "C"
#endif
void
garbageCollection( int )
{
  // Get a pointer to the actual servercomm object.
  ServerComm* sc = ServerComm::actual_servercomm;

  /*
   *  Just register the callbacks because we're not allowed to do any
   *  mallocs or prints in the alarm handler environment. The pending
   *  callbacks will be executed whenever there's RPC activity.
   */
  sc->callback_mgr.registerUniqueCallback(callback_garbage_collection, (void*)sc);

  // Re-initialize the signal handler to point to this function
  signal( SIGALRM, garbageCollection);
  
  // Reset the alarm
  alarm( (unsigned int)sc->garbageCollectionInterval );
}


#ifdef LINUX
extern "C"
#endif
void
garbageCollectionDummy( int )
{
  /* Dummy garbage collection function for avoiding reentrance of the callback manager.
     Does nothing but reinstall the signal. */
  signal( SIGALRM, garbageCollection);
  alarm( (unsigned int)(ServerComm::actual_servercomm->garbageCollectionInterval) );
}


#ifdef LINUX
extern "C"
#endif
GetTypeStructureRes*
RPCFUNCTIONDEF( rpcgettypestructure_1, GetTypeStructureParams* params )
{
  secureResultBufferForRPC = (char*)&rpcGetTypeStructureRes;
  
  freeDynamicRPCData();
  
  char *prev1 = strdup("");
  rpcGetTypeStructureRes.typeStructure = prev1;
  
  accessControl.wantToRead();

  // Get a pointer to the actual servercomm object.
  ServerComm* sc = ServerComm::actual_servercomm;
  
  // Call the corresponding method.
  rpcGetTypeStructureRes.status = sc->getTypeStructure( params->clientID, params->typeName, 
                                            params->typeType, rpcGetTypeStructureRes.typeStructure );
  
  // prevent RPC from NULL pointers
  if( rpcGetTypeStructureRes.typeStructure != prev1 ) free(prev1);
  
  if( !rpcGetTypeStructureRes.typeStructure ) rpcGetTypeStructureRes.typeStructure = strdup("");

  // Return the result
  return &rpcGetTypeStructureRes;
}



#ifdef LINUX
extern "C"
#endif
int*
RPCFUNCTIONDEF( rpcgetserverendian_1, int * )
{
  freeDynamicRPCData();

  static int result;

#ifdef LITTLE_ENDIAN
  result = 1;
#else
  result = 0;
#endif

  return &result;
}


#ifdef LINUX
extern "C"
#endif
unsigned short*
RPCFUNCTIONDEF( rpcsetservertransfer_1, SetServerTransferParams* params )
{
  freeDynamicRPCData();
  secureResultBufferForRPC = (char*)&rpcDummy;

  ServerComm* sc = ServerComm::actual_servercomm;
  
  rpcDummy = (unsigned short)(sc->setTransferMode( params->clientID, params->format, params->formatParams )); 
  
  return &rpcDummy;
}


#ifdef LINUX
extern "C"
#endif
GetExtendedErrorInfo *
RPCFUNCTIONDEF( rpcgeterrorinfo_1, void * params )
{
  freeDynamicRPCData();

  ServerComm* sc = ServerComm::actual_servercomm;

  rpcExtendedErrorInfo.errorText = (char*) sc->getExtendedErrorInfo();
  
  return &rpcExtendedErrorInfo;
}

#ifdef LINUX
extern "C"
#endif
unsigned short*
RPCFUNCTIONDEF( rpcsetserverstorage_1, SetServerTransferParams* params )
{
  secureResultBufferForRPC = (char*)&rpcDummy;
  
  freeDynamicRPCData();

  ServerComm* sc = ServerComm::actual_servercomm;

  rpcDummy = (unsigned short)(sc->setStorageMode(params->clientID, params->format, params->formatParams ));

  return &rpcDummy;
}

