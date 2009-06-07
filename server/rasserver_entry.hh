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
#ifndef RASSERVER_ENTRY_HH
#define RASSERVER_ENTRY_HH

/* 
  This class is the entry point of the rasdaman server. It's the interface of the server to the outside world
  It's functions are called by the communication level.
  
  For now the class inherites HttpServer which inherites ServerComm, but only to have nice acces to the stuff
  found there. Later we will drop both of them and make the life easier.
*/

#include "servercomm/httpserver.hh"


/**
  * \ingroup Servers
  */
class RasServerEntry : public HttpServer
  {
    private:
      RasServerEntry();
      static RasServerEntry* myself;
    public:
      static RasServerEntry& getInstance();

      ~RasServerEntry();
      
      //### inherited stuff - we have to keep them for now
      void startRpcServer() throw( r_Error );
      void stopRpcServer();
      //###
	
	void compat_connectToDBMS() throw( r_Error );
	
	// All "compat_" functions use old ServerComm and HttpServer stuff to do their job
	// Later, the new functions will do the job proper and this old functions will be dropped!
	void compat_connectNewClient(const char *capability);
	    unsigned long currentClientIdx;
	    ClientTblElt* getClientContext( unsigned long ClientId ); // inherited...
	    ClientTblElt* currentClientContext;

	void compat_disconnectClient();	
	
	void compat_openDB(const char* databaseName);
	
	void compat_closeDB();
	
	void compat_beginTA(bool rw);
	
	void compat_commitTA();
	
	void compat_abortTA();
	
	bool compat_isOpenTA();
	
	// provided for temporary compatibility with the encoding of the java interface
	// resultBuffer will be allocated and it's address stored in the given pointer
	// result is the length of the result
	int compat_executeQueryHttp(const char* httpParams, int httpParamsLen, char*& resultBuffer);
	
	r_OId compat_getNewOId(unsigned short objType); // 1 - mddType, 2 -collType
	
	int  compat_executeQueryRpc(const char* query, ExecuteQueryRes &queryResult);

	int  compat_getNextElement(char* &buffer, unsigned int  &bufferSize );
	
	int  compat_endTransfer();
        
	int  compat_getNextMDD(r_Minterval &mddDomain, char* &typeName, char* &typeStructure, r_OId &oid,unsigned short &currentFormat);

        int  compat_getNextTile(RPCMarray** rpcMarray);

	int  compat_ExecuteUpdateQuery(const char *query, ExecuteUpdateRes &returnStructure);

	int  compat_InitUpdate();
        
	int  compat_StartInsertTransMDD(const char *domain, int typeLength, const char *typeName);
	
        int  compat_InsertTile(int persistent, RPCMarray*);
	
        int  compat_EndInsertMDD(int persistent);
        
	int  compat_GetTypeStructure(const char *typeName, int typeType, char* &typeStructure);
	
        int  compat_StartInsertPersMDD(const char *collName, r_Minterval &mddDomain, int typeLength, const char *typeName, r_OId &oid);
        
	int  compat_InsertMDD(const char *collName, RPCMarray *rpcMarray, const char *typeName, r_OId &oid);
	
	int  compat_InsertCollection(const char *collName, const char *typeName, r_OId &oid);

        int  compat_DeleteCollByName(const char *collName);
	
        int  compat_DeleteObjByOId(r_OId &oid);
	
        int  compat_RemoveObjFromColl(const char *collName, r_OId &oid);
	
	int  compat_GetCollectionByName(const char* collName, char* &typeName, char* &typeStructure, r_OId &oid);
	
	int  compat_GetCollectionByName(r_OId oid, char* &typeName, char* &typeStructure, char* &collName);	

	int  compat_GetCollectionOidsByName(const char* collName, char* &typeName, char* &typeStructure, r_OId &oid, RPCOIdEntry* &oidTable, unsigned int &oidTableSize);
	
	int  compat_GetCollectionOidsByOId(r_OId oid, char* &typeName, char* &typeStructure, RPCOIdEntry* &oidTable, unsigned int &oidTableSize, char* &collName);
	
	int  compat_GetObjectType(r_OId &oid, unsigned short &objType);

	int  compat_SetTransferFormat(int format, const char* params);
    
        int  compat_SetStorageFormat(int format, const char* params);
	
	
	r_OId createCollection(const char* collName, const char* collTypeName);
	
	r_OId createMDD(const char* collName, const char* mddTypeName, const char* definitionDomain, const char* tileDomain, bool rcindex);
	
	void  extendMDD(r_OId mddOId, const char *stripeDomain, const char* tileDomain);
	
	vector<r_Minterval> getTileDomains(r_OId mddOId, const char *stripeDomain);

   };
#endif
