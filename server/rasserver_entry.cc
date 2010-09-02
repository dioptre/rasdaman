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
*/
/*************************************************************
 *
 *
 * PURPOSE:
 *
 *
 * COMMENTS:
 *
 ************************************************************/

using namespace std;

#include "mymalloc/mymalloc.h"

#include "rasserver_entry.hh"

#include "debug-srv.hh"

// console output describing successful/unsuccessful actions (cf. servercomm/servercomm.cc)
#define MSG_OK     "ok"
#define MSG_FAILED "failed"


struct HTTPRequest
{
	char *Database;
	int   Command;
	char *QueryString;
	int   ClientType;
	char *ClientID;
	int   Endianess;
	int   NumberOfQueryParams;
	char *BinData;
	int   BinDataSize;
	char *Capability;
};


RasServerEntry* RasServerEntry::myself = 0;

RasServerEntry& RasServerEntry::getInstance()
{
	if(myself == 0)
		myself = new RasServerEntry;
	return *myself;
}

RasServerEntry::RasServerEntry()
{
}

RasServerEntry::~RasServerEntry()
{
}

void RasServerEntry::compat_connectToDBMS() throw( r_Error )
{
	// here no log output, this is a server startup function!

	admin = AdminIf::instance();
	if( !admin )
		throw r_Error( r_Error::r_Error_BaseDBMSFailed );
}
	
void RasServerEntry::compat_connectNewClient(const char *capability)
{
	// we need to add the log information which otherwise is provided in ServerComm (servercomm/servercomm2.cc)
	RMInit::logOut << "Request: connectNewClient..." << flush;

	char client[256]; strcpy( client, "unknown" );

#if 0 // client table is a relict from 1-process, multi-user rasdaman;
      // now it conflicts with rasmgr dispatcher mimics, so we disable -- PB 2005-sep-01
	currentClientContext = new ClientTblElt( client, ++(clientCount) );
	currentClientIdx    = clientCount;
#else
// this id must be !=0 because otherwise in ServerComm::getClientContext() it will not be recognized as valid id, and then the "last action" time will not be updated (timeout!)
#define SINGLETON_CLIENTID ULONG_MAX

	// disable client list by using only 1 element (fixed id), initialize only once
	currentClientIdx = SINGLETON_CLIENTID;
	// if (currentClientContext==NULL)
		currentClientContext = new ClientTblElt( client, currentClientIdx );
	TALK( "using constant Client id " << currentClientIdx );

	// make sure any old element is deleted; currently inhibited, crashes :( -- PB 2005-sep-02
	// RMInit::logOut << "client table has " << clientTbl.size() << " elements before cleanup, ";
	// ServerComm::deleteClientTblEntry( currentClientIdx );
	// RMInit::logOut << " and " << clientTbl.size() << " after.";
#endif // 0

	// Put the context information in the static control list
	ServerComm::addClientTblEntry( currentClientContext );

	TALK( "assigned Client id " << currentClientIdx );

	if(accessControl.crunchCapability(capability) == CAPABILITY_REFUSED)
		throw r_Ecapability_refused();

	RMInit::logOut << MSG_OK << endl;
}
	
void RasServerEntry::compat_disconnectClient()
{
	// we need to add the log information which otherwise is provided in ServerComm (servercomm/servercomm2.cc)
	RMInit::logOut << "Request: disconnect..." << flush;

#if 0 // 
	deleteClientTblEntry( currentClientIdx );
	currentClientIdx = -1;
#else
	// disable client list, use 1 constant element (see above) -- PB 2005-sep-01
	currentClientIdx = SINGLETON_CLIENTID;
	ServerComm::deleteClientTblEntry( currentClientIdx );

	// delete currentClientContext;
	// currentClientContext = NULL;

	// clientTbl.resize( 0 );

	RMInit::logOut << MSG_OK << endl;
#endif // 0

// in a disconnect following an abortta, the client can't be found any more in the table.
// the delete op below then doesn't decrease table size -> endless loop.
// Therefore let us take the original approach: delete only the requested client. -- PB 2003-nov-24
#if 0
	// THIS IS PARANOIA, ONCE WE HAD SOME PROBLEMS, SO WE KILL ALL
	while(!clientTbl.empty())
		{
		  ClientTblElt *temp = clientTbl.front();
	
	temp->currentUsers = 0; // forced!

	deleteClientTblEntry(temp->clientId);
		 }
#endif // 0
}

// we want the ServerComm version, we'll drop this client management anyway
ServerComm::ClientTblElt* RasServerEntry::getClientContext( unsigned long ClientId )
{
#if 0 // see above -- we just have 1 single context -- PB 2005-sep-01
	ClientId = SINGLETON_CLIENTID;
#endif // 0

	return ServerComm::getClientContext( ClientId );
}

		
void RasServerEntry::compat_openDB(const char* databaseName)
{
	// we use "ServercComm::" just to show that it's that function
	ServerComm::openDB( currentClientIdx, databaseName, "" );
}

void RasServerEntry::compat_closeDB()
{
	ServerComm::closeDB( currentClientIdx);
}

void RasServerEntry::compat_beginTA(bool rw)
{
	ServerComm::beginTA( currentClientIdx, rw ? 0 : 1);
}
	
void RasServerEntry::compat_commitTA()
{
	ServerComm::commitTA( currentClientIdx);
}

void RasServerEntry::compat_abortTA()
{
	ServerComm::abortTA( currentClientIdx);
}

bool RasServerEntry::compat_isOpenTA()
{
	return ServerComm::isTAOpen( currentClientIdx );
}

int GetHTTPRequestTemp( char *Source, int SourceLen, struct HTTPRequest *RequestInfo);

int RasServerEntry::compat_executeQueryHttp(const char* httpParams, int httpParamsLen, char*& resultBuffer) 
{
	strcpy(currentClientContext->clientIdText, ServerComm::HTTPCLIENT);
	
	HTTPRequest   RequestInfo;
	/* Initialize RequestInfo */
	RequestInfo.Database = NULL;
	RequestInfo.QueryString = NULL;
	RequestInfo.ClientType = 0;
	RequestInfo.ClientID = NULL;
	RequestInfo.Command = 0;
	RequestInfo.Endianess = 0;
	RequestInfo.NumberOfQueryParams = 0;
	RequestInfo.BinDataSize = 0;
	RequestInfo.BinData = NULL;
	RequestInfo.Capability = NULL;
	
	int resultLen = 0;
	if(GetHTTPRequestTemp( (char*)httpParams, httpParamsLen, &RequestInfo) == 0)
	{
		// we need to add the log information which otherwise is provided in ServerComm (servercomm/servercomm2.cc)
		// logged now in rnprotocol modules -- PB 2005-sep-05
		// RMInit::logOut << "Request: http query '" << RequestInfo.QueryString << "'..." << flush;
		char* queryResult;
		  
		resultLen = HttpServer::processRequest(currentClientIdx, RequestInfo.Database, RequestInfo.Command,
			RequestInfo.QueryString, RequestInfo.BinDataSize,
			RequestInfo.BinData, RequestInfo.Endianess,
			queryResult,RequestInfo.Capability);
		  	
		resultBuffer = queryResult;
		RMInit::logOut << MSG_OK << endl;
	}
	else
		RMInit::logOut << "Error: Internal HTTP protocol mismatch." << endl;

	// free RequestInfo
	free(RequestInfo.Database);
	free(RequestInfo.QueryString);
	free(RequestInfo.Capability);
	free(RequestInfo.BinData);
	free(RequestInfo.ClientID);
	
	return resultLen;
}

r_OId RasServerEntry::compat_getNewOId(unsigned short objType)
{
	r_OId result;
	
	ServerComm::getNewOId(currentClientIdx, objType, result);
	
	return result;
}


int RasServerEntry::compat_executeQueryRpc(const char* query, ExecuteQueryRes &queryResult)
{
	queryResult.token=NULL;
	queryResult.typeName=NULL;
	queryResult.typeStructure=NULL;
		
	return ServerComm::executeQuery(currentClientIdx, query, queryResult);
}
   
int RasServerEntry::compat_getNextElement(char* &buffer, unsigned int  &bufferSize )
{
	 return ServerComm::getNextElement(currentClientIdx, buffer, bufferSize);
}
	
int RasServerEntry::compat_endTransfer()
{
	return ServerComm::endTransfer(currentClientIdx);
}
   
int RasServerEntry::compat_getNextMDD(r_Minterval &mddDomain, char* &typeName, char* &typeStructure, r_OId &oid,unsigned short &currentFormat)
{
	 return ServerComm::getNextMDD( currentClientIdx, mddDomain, typeName, typeStructure, oid, currentFormat);
}
int RasServerEntry::compat_getNextTile(RPCMarray** rpcMarray)
{
	return ServerComm::getNextTile( currentClientIdx, rpcMarray);
}

int RasServerEntry::compat_ExecuteUpdateQuery(const char *query, ExecuteUpdateRes& returnStructure)
{
	return ServerComm::executeUpdate(currentClientIdx, query, returnStructure);
}

int RasServerEntry::compat_InitUpdate()
{
	return ServerComm::initExecuteUpdate(currentClientIdx);
}
		  
int RasServerEntry::compat_StartInsertTransMDD(const char *domain, int typeLength, const char *typeName)
{
	r_Minterval mddDomain(domain);
	return ServerComm::startInsertTransMDD(currentClientIdx, mddDomain, typeLength, typeName);
}
	
int RasServerEntry::compat_InsertTile(int persistent, RPCMarray *rpcMarray)
{
	return ServerComm::insertTile( currentClientIdx, persistent, rpcMarray );
}
	
int RasServerEntry::compat_EndInsertMDD(int persistent)
{
	return ServerComm::endInsertMDD( currentClientIdx, persistent );
}

int RasServerEntry::compat_GetTypeStructure(const char *typeName, int typeType, char* &typeStructure)
{
	return ServerComm::getTypeStructure( currentClientIdx, typeName, typeType, typeStructure );
}

int RasServerEntry::compat_StartInsertPersMDD(const char *collName, r_Minterval &mddDomain, int typeLength, const char *typeName, r_OId &oid)
{
	return ServerComm::startInsertPersMDD( currentClientIdx, collName, mddDomain, typeLength, typeName, oid );
}
   
int RasServerEntry::compat_InsertMDD(const char *collName, RPCMarray *rpcMarray, const char *typeName, r_OId &oid)
{
	return ServerComm::insertMDD( currentClientIdx, collName, rpcMarray, typeName, oid );
} 

int RasServerEntry::compat_InsertCollection(const char *collName, const char *typeName, r_OId &oid)
{
	return ServerComm::insertColl( currentClientIdx, collName, typeName, oid );
}

int RasServerEntry::compat_DeleteCollByName(const char *collName)
{
	return ServerComm::deleteCollByName( currentClientIdx, collName );
}
	
int RasServerEntry::compat_DeleteObjByOId(r_OId &oid)
{
	return ServerComm::deleteObjByOId( currentClientIdx, oid );
}
	
int RasServerEntry::compat_RemoveObjFromColl(const char *collName, r_OId &oid)
{
	return ServerComm::removeObjFromColl( currentClientIdx, collName, oid );
}

int RasServerEntry::compat_GetCollectionByName(const char* collName, char* &typeName, char* &typeStructure, r_OId &oid)
{
	return ServerComm::getCollByName( currentClientIdx, collName, typeName, typeStructure, oid );
}
	
int RasServerEntry::compat_GetCollectionByName(r_OId oid, char* &typeName, char* &typeStructure, char* &collName)
{
	return ServerComm::getCollByOId( currentClientIdx, oid, typeName,typeStructure, collName );
}

int RasServerEntry::compat_GetCollectionOidsByName(const char* collName, char* &typeName, char* &typeStructure, r_OId &oid, RPCOIdEntry* &oidTable, unsigned int &oidTableSize)
{
	return ServerComm::getCollOIdsByName( currentClientIdx, collName, typeName, typeStructure, oid, oidTable, oidTableSize);
}	

int RasServerEntry::compat_GetCollectionOidsByOId(r_OId oid, char* &typeName, char* &typeStructure, RPCOIdEntry* &oidTable, unsigned int &oidTableSize, char* &collName)
{
	return ServerComm::getCollOIdsByOId( currentClientIdx, oid, typeName, typeStructure, oidTable, oidTableSize, collName);
}	

int RasServerEntry::compat_GetObjectType(r_OId &oid, unsigned short &objType)
{
	return ServerComm::getObjectType( currentClientIdx, oid, objType );
}

int RasServerEntry::compat_SetTransferFormat(int format, const char* params)
{
	return ServerComm::setTransferMode( currentClientIdx, format, params ); 
}
	
int RasServerEntry::compat_SetStorageFormat(int format, const char* params)
{
	return ServerComm::setStorageMode( currentClientIdx, format, params );
}

//### stupid inherited stuff, we'll lose them later #####
void RasServerEntry::startRpcServer() throw( r_Error ){}
void RasServerEntry::stopRpcServer(){}
//#######################################################

// local version of this function, with small adaptations to compile here.
int GetHTTPRequestTemp( char *Source, int SourceLen, struct HTTPRequest *RequestInfo)
{
	ENTER( "GetHTTPRequestTemp, source=" << Source << ", SourceLen=" << SourceLen );

	int result = 0;		// function return value
	char *Buffer = NULL;	// ptr to current analysis point in Input
	char *Input = NULL;	// local copy of Source

	Input = new char[ SourceLen + 1 ];
	memcpy( Input, Source, SourceLen );
	Input[SourceLen] = '\0';
	// Read the message body and check for the post parameters
	Buffer = strtok( Input, "=" );
	while( Buffer != NULL )
	{
		if( strcmp(Buffer,"Database") == 0 )
		{
			RequestInfo->Database = strdup(strtok( NULL, "&" ));
			TALK( "Parameter Database is " << RequestInfo->Database );
			Buffer = strtok( NULL, "=" );
		}
		else if( strcmp(Buffer,"QueryString") == 0 )
		{
			RequestInfo->QueryString = strdup(strtok( NULL, "&" ));
			TALK( "Parameter QueryString is " << RequestInfo->QueryString );
			Buffer = strtok( NULL, "=" );
		}
		else if( strcmp(Buffer,"Capability") == 0 )
		{
			RequestInfo->Capability = strdup(strtok( NULL, "&\0" ));
			TALK( "Parameter Capability is " << RequestInfo->Capability );
			Buffer = strtok( NULL, "=" );
		}
		else if( strcmp(Buffer,"ClientID") == 0 )
		{
			RequestInfo->ClientID = strdup(strtok( NULL, "&" ));
			TALK( "Parameter ClientID is " << RequestInfo->ClientID );
			Buffer = strtok( NULL, "=" );
		}
		else if( strcmp(Buffer,"Command") == 0 )
		{
			RequestInfo->Command = atoi( strtok( NULL, "&" ) );
			TALK( "Parameter Command is " << RequestInfo->Command );
			Buffer = strtok( NULL, "=" );
		}
		else if( strcmp(Buffer,"Endianess") == 0 )
		{
			RequestInfo->Endianess = atoi( strtok( NULL, "&" ) );
			TALK( "Parameter Endianess is " << RequestInfo->Endianess );
			Buffer = strtok( NULL, "=" );
		}
		else if( strcmp(Buffer,"NumberOfQueryParameters") == 0 )
		{
			RequestInfo->NumberOfQueryParams = atoi( strtok( NULL, "&" ) );
			TALK( "Parameter NumberOfQueryParams is " << RequestInfo->NumberOfQueryParams );
			Buffer = strtok( NULL, "=" );
		}
		else if( strcmp(Buffer,"BinDataSize") == 0 )
		{
			RequestInfo->BinDataSize = atoi( strtok( NULL, "&" ) );
			TALK( "Parameter BinDataSize is " << RequestInfo->BinDataSize );
			Buffer = strtok( NULL, "=" );
		}
		else if( strcmp(Buffer,"BinData") == 0 )
		{
			// This parameter has to be the last one!
			RequestInfo->BinData = new char[RequestInfo->BinDataSize ];
			memcpy(RequestInfo->BinData, Source + (SourceLen-RequestInfo->BinDataSize), RequestInfo->BinDataSize );
			//set Buffer to NULL => exit this while block
			Buffer = NULL;
		}
		else if( strcmp(Buffer,"ClientType") == 0 )
		{
			Buffer = strtok( NULL, "&" );
			TALK( "Parameter Type is " << Buffer );
			/* BROWSER? */
			if( strcmp(Buffer,"BROWSER") == 0 )
		   	RequestInfo->ClientType = 1;
			/* Rasclient? */
			else if( strcmp(Buffer,"RASCLIENT") == 0 )
		   	RequestInfo->ClientType = 2;
			/* Sonstiges */
			else
			{
				TALK( "Error: Unknown Parameter: " << Buffer );
				result = 2;
			}
			Buffer = strtok( NULL, "=" );
		}
		else
			result = 1;
	}

	if (result == 0)
		delete[] Input;

	LEAVE( "GetHTTPRequestTemp, result=" << result );
	return result;
}

//#####################################################################################################
#include "server/createinitmdd.hh"

r_OId RasServerEntry::createCollection(const char* collName, const char* collTypeName)
{
	FastCollectionCreator fcc(collName, collTypeName);
	
	return fcc.createCollection();
}

r_OId RasServerEntry::createMDD(const char* collName, const char* mddTypeName, const char* definitionDomain, const char* tileDomain, bool rcindex)
{
	FastMDDCreator fc;
	
	fc.setCollectionName(collName);
	fc.setMDDTypeName(mddTypeName);
	
	if(rcindex)
		return fc.createRCxMDD(definitionDomain, tileDomain);
	
	return fc.createMDD(definitionDomain);
}
	
void  RasServerEntry::extendMDD(r_OId mddOId, const char *stripeDomain, const char* tileDomain)
{
	FastMDDCreator fc;
	
	fc.addStripe(mddOId, stripeDomain, tileDomain);
}

vector<r_Minterval> RasServerEntry::getTileDomains(r_OId mddOId, const char *stripeDomain)
{
	FastMDDCreator fc;
	
	return fc.getTileDomains(mddOId, stripeDomain);
}

