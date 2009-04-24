#include "mymalloc/mymalloc.h"
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
 * Contains the upper level functions, everything related to communication
 * is in rnpclientcomm2
 *
 ************************************************************/

#include <openssl/evp.h>

#include "rnpclientcomm.hh"
#include "rasdaman.hh"
#include "debug.hh"


// waiting time increment upon subsecuqnet connect tries in RNP [msecs] -- PB 2005-sep-09
const unsigned int RNP_PAUSE_INCREMENT = 100;


RnpClientComm::RnpClientComm( const char* nRasmgrHost, int nRasmgrPort) throw( r_Error )
:RnpBaseClientComm(RnpRasserver::serverID, RnpTransport::crp_Rnp)
{
	ENTER( "RpcClientComm::RnpClientComm( " << nRasmgrHost << "," << nRasmgrPort << " )"  );
	RMDBGENTER( 2, RMDebug::module_clientcomm, "RpcClientComm", "RnpClientComm(" << nRasmgrHost << "," << nRasmgrPort << ")"  );

	clientID = -1;

	clientParams = new r_Parse_Params();
	clientParams->add("compserver", &serverCompresses, r_Parse_Params::param_type_int);
	clientParams->add("exactformat", &exactFormat, r_Parse_Params::param_type_int);

	endianClient = (int)r_Endian::get_endianness();

	rasmgrHost=(char*)nRasmgrHost;
	rasmgrPort=nRasmgrPort;
	serverHost[0]=0;
	capability[0]=0;
	strcpy(identificationString,"rasguest:8e70a429be359b6dace8b5b2500dedb0"); // this is MD5("rasguest");

	transferFormatParams = 0;
	storageFormatParams  = 0;

	// experimentally disabled -- PB 2005-sep-01
	// useTurbo = true;
	useTurbo = true;
	TALK( "useTurbo=" << useTurbo );

	akg::NbJob::setTimeoutInterval(3600);

	RMDBGEXIT( 2, RMDebug::module_clientcomm, "RpcClientComm", "RnpClientComm()" );       
	LEAVE( "RpcClientComm::RnpClientComm()"  );
}

RnpClientComm::~RnpClientComm() throw ()
{
	ENTER( "RpcClientComm::~RnpClientComm()"  );
	RMDBGENTER( 2, RMDebug::module_clientcomm, "RpcClientComm", "~RnpClientComm()"  );
	RMDBGEXIT( 2, RMDebug::module_clientcomm, "RpcClientComm", "~RnpClientComm()" );       
	LEAVE( "RpcClientComm::~RnpClientComm()"  );
}

bool RnpClientComm::effectivTypeIsRNP() throw()
{
	bool retval = true;
	ENTER( "RpcClientComm::effectivTypeIsRNP()"  );
	RMDBGENTER( 2, RMDebug::module_clientcomm, "RpcClientComm", "effectivTypeIsRNP()"  );
	RMDBGEXIT( 2, RMDebug::module_clientcomm, "RpcClientComm", "effectivTypeIsRNP() -> " << retval );       
	LEAVE( "RpcClientComm::effectivTypeIsRNP() -> " << retval );
	return retval;
}

int RnpClientComm::openDB( const char* database )
{
	int retval = 0;

	ENTER( "RpcClientComm::openDB(" << database << ")"  );
	RMDBGENTER( 2, RMDebug::module_clientcomm, "RpcClientComm", "openDB(" << database << ")"  );

	strcpy(databaseName,database);

	getFreeServer(false, true); // read only, openDB

	TALK( "openDB: Connected to server: "<<serverHost<<":"<<serverPort );
	setConnectionParameters(serverHost,serverPort);

/* was commented out, trying it... -- PB 2005-aug-31
*/
	if(useTurbo)
	{
		turboOpenDB(databaseName);
	}
	else
	{ 	
		executeConnect();
		executeOpenDB(database);
		executeCloseDB();
		executeDisconnect();
	}
/* */

	RMDBGEXIT( 2, RMDebug::module_clientcomm, "RpcClientComm", "openDB() -> " << retval );       
	LEAVE( "RpcClientComm::openDB() -> " << retval );
	return retval;
}

int RnpClientComm::closeDB()
{
	int retval = 0;
	ENTER( "RpcClientComm::closeDB()"  );
	RMDBGENTER( 2, RMDebug::module_clientcomm, "RpcClientComm", "closeDB()"  );
	RMDBGEXIT( 2, RMDebug::module_clientcomm, "RpcClientComm", "closeDB() -> " << retval );       
	LEAVE( "RpcClientComm::closeDB() -> " << retval );
	return retval;
}

int RnpClientComm::createDB( const char* name ) throw(r_Error)
{
	int retval = -1;
	ENTER( "RpcClientComm::createDB( " << (name?name:"(null)") << " )"  );
	RMDBGENTER( 2, RMDebug::module_clientcomm, "RpcClientComm", "createDB()"  );
	RMDBGEXIT( 2, RMDebug::module_clientcomm, "RpcClientComm", "createDB() -> " << retval );       
	LEAVE( "RpcClientComm::createDB() -> " << retval );
	return retval;
}

int RnpClientComm::destroyDB( const char* name ) throw(r_Error)
{
	int retval = -1;
	ENTER( "RpcClientComm::destroyDB( " << (name?name:"(null)") << " )"  );
	RMDBGENTER( 2, RMDebug::module_clientcomm, "RpcClientComm", "destroyDB()"  );
	RMDBGEXIT( 2, RMDebug::module_clientcomm, "RpcClientComm", "destroyDB() -> " << retval );       
	LEAVE( "RpcClientComm::destroyDB() -> " << retval );
	return retval;
}

int RnpClientComm::openTA( unsigned short readOnly ) throw(r_Error)
{
	int retval = 1;
	ENTER( "RpcClientComm::openTA(" << readOnly << ")"  );
	RMDBGENTER( 2, RMDebug::module_clientcomm, "RpcClientComm", "openTA(" << readOnly << ")"  );

	bool rw = (readOnly == 0 ? true : false);
	
	getFreeServer(rw, false); // readwrite?, not openDB
	TALK( "openTA: connected to server "<<serverHost<<":"<<serverPort );
	setConnectionParameters(serverHost,serverPort);
	
	if(useTurbo)
	{
		  turboBeginTA(rw);
	}
	else
	{   	
		  executeConnect();
		  executeOpenDB(databaseName);
		  executeBeginTA(rw);
	}
		 
	RMDBGEXIT( 2, RMDebug::module_clientcomm, "RpcClientComm", "openTA() -> " << retval );       
	LEAVE( "RpcClientComm::openTA() -> " << retval );
	return retval;
}

int RnpClientComm::commitTA() throw(r_Error)
{
	int retval = 1;

	ENTER( "RpcClientComm::commitTA()"  );
	RMDBGENTER( 2, RMDebug::module_clientcomm, "RpcClientComm", "commitTA()"  );

	if(useTurbo)
	{
		  turboCommitTA();
	}
	else
	{   	
		  executeCommitTA();
		  executeCloseDB();
		  executeDisconnect();
	}

	RMDBGEXIT( 2, RMDebug::module_clientcomm, "RpcClientComm", "commitTA() -> " << retval );       
	LEAVE( "RpcClientComm::commitTA() -> " << retval );
	return retval;
}

int RnpClientComm::abortTA()
{
	int retval = 1;

	ENTER( "RpcClientComm::abortTA()"  );
	RMDBGENTER( 2, RMDebug::module_clientcomm, "RpcClientComm", "abortTA()"  );

	try
	{
		if(useTurbo)
		{
			turboAbortTA();
		}
		else
		{   	
			executeAbortTA();
			executeCloseDB();
			executeDisconnect();
		}
	}
	// make it nicer, but we are not allowed to throw anything! Later will change the declaration of the function  
	catch(...)
	{
		TALK( "RpcClientComm::abortTA(): caught & ignored exception." );
	}  		

	RMDBGEXIT( 2, RMDebug::module_clientcomm, "RpcClientComm", "abortTA() -> " << retval );       
	LEAVE( "RpcClientComm::abortTA() -> " << retval );
	return retval;
}

void RnpClientComm::insertMDD( const char* collName, r_GMarray* mar ) throw( r_Error )
{
	ENTER( "RpcClientComm::insertMDD(" << (collName?collName:"(null)") << "," << (long) mar << ")"  );
	RMDBGENTER( 2, RMDebug::module_clientcomm, "RpcClientComm", "insertMDD(" << collName << "," << (long) mar << ")"  );

	checkForRwTransaction();
	
	r_Minterval     spatdom;
	r_Bytes         marBytes;
	RPCMarray*      rpcMarray;
	r_Bytes         tileSize = 0;

	// get the spatial domain of the r_GMarray
	spatdom = mar->spatial_domain();

	// determine the amount of data to be transferred
	marBytes = mar->get_array_size();

	const r_Base_Type* baseType = mar->get_base_type_schema();

	// if the MDD is too large for being transfered as one block, it has to be
	// divided in tiles
	const r_Tiling* til = mar->get_storage_layout()->get_tiling();
	r_Tiling_Scheme scheme = til->get_tiling_scheme();
	if (scheme == r_NoTiling)
		tileSize = RMInit::RMInit::clientTileSize;
	else
		//allowed because the only subclass of tiling without size is no tiling
		tileSize = ((const r_Size_Tiling*)til)->get_tile_size();
	
	if( RMInit::tiling && marBytes > tileSize )
	{
		// initiate composition of MDD at server side
		int status = executeStartInsertPersMDD(collName, mar);  //rpcStatusPtr = rpcstartinsertpersmdd_1( params, binding_h );

		switch( status )
		{
			case 0:
				break; // OK
			case 2:
				LEAVE( "RpcClientComm::insertMDD() Error: database class undefined." );
				throw r_Error( r_Error::r_Error_DatabaseClassUndefined );
				break;
			case 3:
				LEAVE( "RpcClientComm::insertMDD() Error: collection element type mismatch." );
				throw r_Error( r_Error::r_Error_CollectionElementTypeMismatch );
				break;
			case 4:
				LEAVE( "RpcClientComm::insertMDD() Error: type invalid." );
				throw r_Error( r_Error::r_Error_TypeInvalid );
				break;
			default:
				LEAVE( "RpcClientComm::insertMDD() Error: transfer invalid." );
				throw r_Error( r_Error::r_Error_TransferFailed );
				break;
		}

		r_Set< r_GMarray* >* bagOfTiles;

		bagOfTiles = mar->get_storage_layout()->decomposeMDD( mar );

		RMDBGMIDDLE(2, RMDebug::module_clientcomm, "RpcClientComm", "decomposing into " << bagOfTiles->cardinality() << " tiles")
	 
		r_Iterator< r_GMarray* > iter = bagOfTiles->create_iterator();
		r_GMarray *origTile;

		for(iter.reset(); iter.not_done(); iter.advance() )
	 	{
			origTile = *iter;

  			RMDBGMIDDLE(2, RMDebug::module_clientcomm, "RpcClientComm", "inserting Tile with domain " << origTile->spatial_domain() << ", " << origTile->spatial_domain().cell_count() * origTile->get_type_length() << " bytes")

			getMarRpcRepresentation( origTile, rpcMarray, mar->get_storage_layout()->get_storage_format(), baseType );
		 
			status = executeInsertTile(true, rpcMarray);   

			// free rpcMarray structure (rpcMarray->data.confarray_val is freed somewhere else)
			freeMarRpcRepresentation( origTile, rpcMarray );
		 
			// delete current tile (including data block)
			delete origTile;
		 
			if( status > 0 )
			{
				throw r_Error( r_Error::r_Error_TransferFailed );
			}
		}
	 
		executeEndInsertMDD(true); //rpcendinsertmdd_1( params3, binding_h );

		// delete transient data
		bagOfTiles->remove_all();
		delete bagOfTiles;
	}
	else // begin: MDD is transferred in one piece
	{
		RMDBGMIDDLE(2, RMDebug::module_clientcomm, "RpcClientComm", ", one tile" )

		getMarRpcRepresentation( mar, rpcMarray, mar->get_storage_layout()->get_storage_format(), baseType );

		int status = executeInsertMDD(collName, mar, rpcMarray); //  rpcStatusPtr = rpcinsertmdd_1( params, binding_h );

		freeMarRpcRepresentation( mar, rpcMarray );
	
		RMDBGMIDDLE(2, RMDebug::module_clientcomm, "RpcClientComm", "ok" )

		switch( status )
		{
			case 0:
				break; // OK
			case 2:
				LEAVE( "RpcClientComm::insertMDD(): exception, status = " << status );
				throw r_Error( r_Error::r_Error_DatabaseClassUndefined );
				break;			
			case 3:
				LEAVE( "RpcClientComm::insertMDD(): exception, status = " << status );
				throw r_Error( r_Error::r_Error_CollectionElementTypeMismatch );
				break;	  
			case 4:
				LEAVE( "RpcClientComm::insertMDD(): exception, status = " << status );
				throw r_Error( r_Error::r_Error_TypeInvalid );
				break;	  
			default:
				LEAVE( "RpcClientComm::insertMDD(): exception, status = " << status );
				throw r_Error( r_Error::r_Error_TransferFailed );
				break;
		}
	
	} // end: MDD i transferred in one piece

	RMDBGEXIT( 2, RMDebug::module_clientcomm, "RpcClientComm", "insertMDD()" );       
	LEAVE( "RpcClientComm::insertMDD()"  );
}


//################################################################

r_Ref_Any RnpClientComm::getMDDByOId( const r_OId& oid ) throw( r_Error )
{
	ENTER( "RpcClientComm::getMDDByOId(" << oid << ")"  );
	RMDBGENTER( 2, RMDebug::module_clientcomm, "RpcClientComm", "getMDDByOId(" << oid << ")"  );

	RMInit::logOut << "Internal error: RnpClientComm::getMDDByOId() not implemented, returning empty r_Ref_Any()." << endl; 

	RMDBGEXIT( 2, RMDebug::module_clientcomm, "RpcClientComm", "getMDDByOId()" );       
	LEAVE( "RpcClientComm::getMDDByOId()"  );
	return r_Ref_Any();
}

void RnpClientComm::insertColl( const char* collName, const char* typeName, const r_OId& oid ) throw( r_Error )
{
	ENTER( "RpcClientComm::insertColl(" << collName << "," << typeName << "," << oid << ")"  );
	RMDBGENTER( 2, RMDebug::module_clientcomm, "RpcClientComm", "insertColl(" << collName << "," << typeName << "," << oid << ")"  );

	checkForRwTransaction();

	int status = executeInsertCollection(collName, typeName, oid);  

	switch( status )
	{
		case 0:
			break; //OK
		case 1:
			LEAVE( "RpcClientComm::insertColl(): exception, status = "  << status );
			throw r_Error( r_Error::r_Error_ClientUnknown );  break;		
		case 2:
			LEAVE( "RpcClientComm::insertColl(): exception, status = "  << status );
			throw r_Error( r_Error::r_Error_DatabaseClassUndefined ); break;	  
		case 3:
			LEAVE( "RpcClientComm::insertColl(): exception, status = "  << status );
			throw r_Error( r_Error::r_Error_NameNotUnique );  break;
		default:
			LEAVE( "RpcClientComm::insertColl(): exception, status = "  << status );
			throw r_Error( r_Error::r_Error_General );break;
	}

	RMDBGEXIT( 2, RMDebug::module_clientcomm, "RpcClientComm", "insertColl()" );       
	LEAVE( "RpcClientComm::insertColl()"  );
}

	
void RnpClientComm::deleteCollByName( const char* collName ) throw( r_Error )
{
	ENTER( "RpcClientComm::deleteCollByName(" << (collName?collName:"(null)") << ")"  );
	RMDBGENTER( 2, RMDebug::module_clientcomm, "RpcClientComm", "deleteCollByName(" << collName << ")"  );

	checkForRwTransaction();

	startRequest(RnpRasserver::cmd_delcollbyname);
	encoder.addInt32Parameter(RnpRasserver::pmt_clientid, clientID);
	TALK( "adding clientID 0x" << hex << clientID << dec );
	encoder.addStringParameter(RnpRasserver::pmt_collname, collName);
	
	helper012d("removeObjFromColl");

	RMDBGEXIT( 2, RMDebug::module_clientcomm, "RpcClientComm", "deleteCollByName()" );       
	LEAVE( "RpcClientComm::deleteCollByName()"  );
}

void RnpClientComm::deleteObjByOId( const r_OId& oid ) throw( r_Error )
{
	ENTER( "RpcClientComm::deleteObjByOId(" << oid << ")"  );
	RMDBGENTER( 2, RMDebug::module_clientcomm, "RpcClientComm", "deleteObjByOId(" << oid << ")"  );

	checkForRwTransaction();

	startRequest(RnpRasserver::cmd_delobjbyoid);
	encoder.addInt32Parameter(RnpRasserver::pmt_clientid, clientID);
	TALK( "adding clientID 0x" << hex << clientID << dec );
	encoder.addStringParameter(RnpRasserver::pmt_oidstring, oid.get_string_representation());
	
	helper012d("deleteObjByOId");

	RMDBGEXIT( 2, RMDebug::module_clientcomm, "RpcClientComm", "deleteObjByOId()" );       
	LEAVE( "RpcClientComm::deleteObjByOId()"  );
}

void RnpClientComm::removeObjFromColl( const char* collName, const r_OId& oid ) throw ( r_Error )
{
	ENTER( "RpcClientComm::removeObjFromColl(" << (collName?collName:"(null)") << "," << oid << ")"  );
	RMDBGENTER( 2, RMDebug::module_clientcomm, "RpcClientComm", "removeObjFromColl(" << collName << "," << oid << ")"  );

	checkForRwTransaction();

	startRequest(RnpRasserver::cmd_remobjfromcoll);
	encoder.addInt32Parameter(RnpRasserver::pmt_clientid, clientID);
	TALK( "adding clientID 0x" << hex << clientID << dec );
	encoder.addStringParameter(RnpRasserver::pmt_collname, collName);
	encoder.addStringParameter(RnpRasserver::pmt_oidstring, oid.get_string_representation());
	
	helper012d("removeObjFromColl");

	RMDBGEXIT( 2, RMDebug::module_clientcomm, "RpcClientComm", "removeObjFromColl()" );       
	LEAVE( "RpcClientComm::removeObjFromColl()"  );
}


r_Ref_Any RnpClientComm::getCollByName( const char* collName ) throw( r_Error )
{
	ENTER( "RpcClientComm::getCollByName(" << (collName?collName:"(null)") << ")"  );
	RMDBGENTER( 2, RMDebug::module_clientcomm, "RpcClientComm", "getCollByName(" << collName << ")"  );

	r_Ref_Any result = executeGetCollByNameOrOId ( collName, r_OId() );

	RMDBGEXIT( 2, RMDebug::module_clientcomm, "RpcClientComm", "getCollByName()" );       
	LEAVE( "RpcClientComm::getCollByName()"  );
	return result;
}

r_Ref_Any RnpClientComm::getCollByOId ( const r_OId& oid ) throw( r_Error )
{
	ENTER( "RpcClientComm::getCollByOId(" << oid << ")"  );
	RMDBGENTER( 2, RMDebug::module_clientcomm, "RpcClientComm", "getCollByOId(" << oid << ")"  );

	r_Ref_Any result = executeGetCollByNameOrOId ( NULL, oid );

	RMDBGEXIT( 2, RMDebug::module_clientcomm, "RpcClientComm", "getCollByOId()" );       
	LEAVE( "RpcClientComm::getCollByOId()"  );
	return result;
}

r_Ref_Any RnpClientComm::getCollOIdsByName( const char* name ) throw( r_Error )
{
	ENTER( "RpcClientComm::getCollOIdsByName(" << (name?name:"(null)") << ")"  );
	RMDBGENTER( 2, RMDebug::module_clientcomm, "RpcClientComm", "getCollOIdsByName(" << name << ")"  );

	r_Ref_Any result = executeGetCollOIdsByNameOrOId ( name, r_OId() );

	RMDBGEXIT( 2, RMDebug::module_clientcomm, "RpcClientComm", "getCollOIdsByName()" );       
	LEAVE( "RpcClientComm::getCollOIdsByName()"  );
	return result;
}

r_Ref_Any RnpClientComm::getCollOIdsByOId ( const r_OId& oid ) throw( r_Error )
{
	ENTER( "RpcClientComm::getCollOIdsByOId(" << oid << ")"  );
	RMDBGENTER( 2, RMDebug::module_clientcomm, "RpcClientComm", "getCollOIdsByOId(" << oid << ")"  );

	r_Ref_Any result = executeGetCollOIdsByNameOrOId ( NULL, oid );

	RMDBGEXIT( 2, RMDebug::module_clientcomm, "RpcClientComm", "getCollOIdsByOId()" );       
	LEAVE( "RpcClientComm::getCollOIdsByOId()"  );
	return result;
}


//------------------------------
void RnpClientComm::executeQuery( const r_OQL_Query& query, r_Set< r_Ref_Any >& result ) throw( r_Error )
{
	ENTER( "RpcClientComm::executeQuery(_,_)"  );
	RMDBGENTER( 2, RMDebug::module_clientcomm, "RpcClientComm", "executeQuery(_,_)"  );

	int status = executeExecuteQuery( query.get_query(), result );
	
	switch(status)
	{
		case 0:
			getMDDCollection( result, 1 );
			break; // 1== isQuery
		case 1:
			getElementCollection( result );
			break;
		//case 2:  nothing
		default:
		      RMInit::logOut << "Internal error: RpcClientComm::executeQuery(): illegal status value " << status << endl;
	}

	RMDBGEXIT( 2, RMDebug::module_clientcomm, "RpcClientComm", "executeQuery()" );       
	LEAVE( "RpcClientComm::executeQuery()"  );
}

void RnpClientComm::getMDDCollection( r_Set< r_Ref_Any >& mddColl, unsigned int isQuery ) throw(r_Error)
{
	ENTER( "RpcClientComm::getMDDCollection(_," << isQuery << ")"  );
	RMDBGENTER( 2, RMDebug::module_clientcomm, "RpcClientComm", "getMDDCollection(_," << isQuery << ")"  );

	unsigned short tileStatus=0;
	unsigned short mddStatus = 0;

	while( mddStatus == 0 ) // repeat until all MDDs are transferred
	{
		r_Ref<r_GMarray> mddResult;
	
		// Get spatial domain of next MDD
		GetMDDRes* thisResult = executeGetNextMDD(); 

		mddStatus = thisResult->status;

		if( mddStatus == 2 )
		{
			RMInit::logOut << "Error: getMDDCollection(...) - no transfer collection or empty transfer collection" << endl;
			LEAVE( "RpcClientComm::getMDDCollection(): exception, status = " << mddStatus );
			throw r_Error( r_Error::r_Error_TransferFailed );
		}
	
		tileStatus = getMDDCore(mddResult, thisResult, isQuery);
	 
		// finally, insert the r_Marray into the set
	
		mddColl.insert_element( mddResult, 1 );

		free(thisResult->domain);
		free(thisResult->typeName);
		free(thisResult->typeStructure);
		free(thisResult->oid);
		delete   thisResult;

		if( tileStatus == 0 ) // if this is true, we're done with this collection
			break;
	
	} // end while( mddStatus == 0 )
	
	executeEndTransfer();
	
	RMDBGEXIT( 2, RMDebug::module_clientcomm, "RpcClientComm", "getMDDCollection()" );       
	LEAVE( "RpcClientComm::getMDDCollection()"  );
}


// small helper for ...
void freeGetTileRes(GetTileRes *ptr)
{
	ENTER( "RpcClientComm::freeGetTileRes(_)"  );
	RMDBGENTER( 2, RMDebug::module_clientcomm, "RpcClientComm", "freeGetTileRes(_)"  );

	if(ptr->marray->domain)
		free(ptr->marray->domain);
	if(ptr->marray->data.confarray_val)
		free(ptr->marray->data.confarray_val);
	delete ptr->marray;
	delete ptr;

	RMDBGEXIT( 2, RMDebug::module_clientcomm, "RpcClientComm", "freeGetTileRes()" );       
	LEAVE( "RpcClientComm::freeGetTileRes(_)"  );
}

unsigned short
RnpClientComm::getMDDCore( r_Ref< r_GMarray > &mdd, GetMDDRes *thisResult, unsigned int isQuery ) throw( r_Error )
{
	ENTER( "RpcClientComm::getMDDCore(_,_," << isQuery << ")"  );
	RMDBGENTER( 2, RMDebug::module_clientcomm, "RpcClientComm", "getMDDCore(_,_," << isQuery << ")"  );

	//  create r_Minterval and oid
	r_Minterval mddDomain( thisResult->domain );
	r_OId       rOId     ( thisResult->oid );
	r_GMarray  *marray;

	if( isQuery )
		marray = new( r_Database::actual_database, r_Object::transient, rOId ) r_GMarray();
	else
		marray = new( r_Database::actual_database, r_Object::read     , rOId ) r_GMarray();

	marray->set_spatial_domain( mddDomain );
	marray->set_type_by_name  ( thisResult->typeName );
	marray->set_type_structure( thisResult->typeStructure );

	r_Data_Format currentFormat = (r_Data_Format)(thisResult->currentFormat);
	if (r_Tile_Compression::check_data_format(currentFormat) == 1)
		currentFormat = r_Array;
	marray->set_current_format( currentFormat );

	r_Data_Format decompFormat;

	const r_Base_Type *baseType = marray->get_base_type_schema();
	
	// Variables needed for tile transfer
	GetTileRes* tileRes=0;
	unsigned short  mddDim = mddDomain.dimension();  // we assume that each tile has the same dimensionality as the MDD
	r_Minterval     tileDomain; 
	r_GMarray*      tile;  // for temporary tile
	char*           memCopy;
	unsigned long   memCopyLen;
	int             tileCntr = 0;
	unsigned short  tileStatus   = 0;

	tileStatus = 2; // call rpcgetnexttile_1 at least once

	while( tileStatus == 2 || tileStatus == 3 )  // while( for all tiles of the current MDD )
	{
		tileRes = executeGetNextTile();
	
		tileStatus = tileRes->status;
		
		if( tileStatus == 4 )
		{
			freeGetTileRes(tileRes);
			RMInit::logOut << "Error: rpcGetNextTile(...) - no tile to transfer or empty transfer collection" << endl;
			LEAVE( "RpcClientComm::getMDDCore(): exception, status = " << tileStatus );
			throw r_Error( r_Error::r_Error_TransferFailed );	 
		}

		// take cellTypeLength for current MDD of the first tile
		if( tileCntr == 0 )
			marray->set_type_length( tileRes->marray->cellTypeLength );

		tileDomain = r_Minterval( tileRes->marray->domain );
		memCopyLen = tileDomain.cell_count() * marray->get_type_length(); // cell type length of the tile must be the same
		if (memCopyLen < tileRes->marray->data.confarray_len)
			memCopyLen = tileRes->marray->data.confarray_len;	// may happen when compression expands
		memCopy    = new char[ memCopyLen ];

		// create temporary tile
		tile = new r_GMarray();
		tile->set_spatial_domain( tileDomain );
		tile->set_array( memCopy );
		tile->set_array_size( memCopyLen );
		tile->set_type_length( tileRes->marray->cellTypeLength );
		tileCntr++;
		
		// Variables needed for block transfer of a tile      
		unsigned long  blockOffset = 0;
		unsigned short subStatus  = 3;
		currentFormat = (r_Data_Format)(tileRes->marray->currentFormat);

		switch( tileStatus )
		{
			case 3: // at least one block of the tile is left
			
		  		// Tile arrives in several blocks -> put them together
				concatArrayData(tileRes->marray->data.confarray_val, tileRes->marray->data.confarray_len, memCopy, memCopyLen, blockOffset);
		  		freeGetTileRes(tileRes);

				tileRes = executeGetNextTile();//rpcgetnexttile_1( &clientID, binding_h );
		  
				subStatus = tileRes->status;
		  
		  		if( subStatus == 4 ) 
				{
		      			freeGetTileRes(tileRes);
		      			LEAVE( "RpcClientComm::getMDDCore(): exception, status = " << tileStatus << ", subStatus = " << subStatus );
		      			throw r_Error( r_Error::r_Error_TransferFailed );
				}
	
				concatArrayData(tileRes->marray->data.confarray_val, tileRes->marray->data.confarray_len, memCopy, memCopyLen, blockOffset);
		  		freeGetTileRes(tileRes);
		
		  		tileStatus = subStatus;	
		  		break;

			default: // tileStatus = 0,3 last block of the current tile
			
		  		// Tile arrives as one block.
				concatArrayData(tileRes->marray->data.confarray_val, tileRes->marray->data.confarray_len, memCopy, memCopyLen, blockOffset);
		  		freeGetTileRes(tileRes);

		  		break;
		}

		// uncompress the tile if necessary
		decompFormat = doTransferDecompression( tile, baseType, currentFormat, blockOffset );

		char* marrayData = NULL;
		// Now the tile is transferred completely, insert it into current MDD
		if( tileStatus < 2 && tileCntr == 1 && (tile->spatial_domain() == marray->spatial_domain()))
		{
			// MDD consists of just one tile that is the same size of the mdd

			// simply take the data memory of the tile
			marray->set_array( tile->get_array() );
			marray->set_array_size( tile->get_array_size() );
			tile->set_array( 0 );
		}
		else
		{
			// MDD consists of more than one tile or the tile does not cover the whole domain

			r_Bytes size = mddDomain.cell_count() * marray->get_type_length();
	 
			if( tileCntr == 1 )
			{
		  		// allocate memory for the MDD
		  		marrayData = new char[ size ];
				memset(marrayData, 0, size);

		  		marray->set_array( marrayData );
			}
			else
		  		marrayData = marray->get_array();
		  

			// copy tile data into MDD data space (optimized, relying on the internal representation of an MDD )
			char*         mddBlockPtr;
			char*         tileBlockPtr = tile->get_array();
			unsigned long blockCells   = tileDomain[tileDomain.dimension()-1].high()-tileDomain[tileDomain.dimension()-1].low()+1;
			unsigned long blockSize    = blockCells * marray->get_type_length();
			unsigned long blockNo      = tileDomain.cell_count() / blockCells;

			for( unsigned long blockCtr = 0; blockCtr < blockNo; blockCtr++ )
			{	   
		  		mddBlockPtr = marrayData + marray->get_type_length()*mddDomain.cell_offset( tileDomain.cell_point( blockCtr * blockCells ) );
		  		memcpy( (void*)mddBlockPtr, (void*)tileBlockPtr, (size_t)blockSize ); 
		  		tileBlockPtr += blockSize;
			}

			// former non-optimized version
			// for( i=0; i<tileDomain->cell_count(); i++ )
			//   (*marray)[tileDomain->cell_point( i )] = (*tile)[tileDomain->cell_point( i )];
		
			marray->set_array_size( size );
		}

		// delete temporary tile
		delete tile;

	}  // end while( MDD is not transferred completely )


	mdd = r_Ref<r_GMarray>( marray->get_oid(), marray );
  
	RMDBGEXIT( 2, RMDebug::module_clientcomm, "RpcClientComm", "getMDDCore() -> " << tileStatus );       
	LEAVE( "RpcClientComm::getMDDCore() -> " << tileStatus );
	return tileStatus;
}


int RnpClientComm::concatArrayData( const char *source, unsigned long srcSize, char *&dest, unsigned long &destSize, unsigned long &destLevel )
{
	ENTER( "RpcClientComm::concatArrayData( 0x" << hex << (unsigned long) source << dec << "," << srcSize << ",_,_,_ )" );
	RMDBGENTER( 2, RMDebug::module_clientcomm, "RpcClientComm", "concatArrayData(" << source << "," << srcSize << ",_,_,_)" );

	if (destLevel + srcSize > destSize)
	{
		// need to extend dest
		unsigned long newSize = destLevel + srcSize;
		char *newArray;

		// allocate a little extra if we have to extend
		newSize = newSize + newSize / 16;

		//    RMDBGOUT( 1, "RpcClientComm::concatArrayData(): need to extend from " << destSize << " to " << newSize );

		if ((newArray = new char[newSize]) == NULL)
		{
			RMDBGEXIT( 2, RMDebug::module_clientcomm, "RpcClientComm", "concatArrayData() -> " << -1 );       
			LEAVE( "RpcClientComm::concatArrayData() -> -1" );
			return -1;
		}

		memcpy(newArray, dest, destLevel);
		delete [] dest;
		dest = newArray;
		destSize = newSize;
	}

	memcpy(dest + destLevel, source, srcSize);
	destLevel += srcSize;

	RMDBGEXIT( 2, RMDebug::module_clientcomm, "RpcClientComm", "concatArrayData() -> " << 0 );       
	LEAVE( "RpcClientComm::concatArrayData() -> 0" );
	return 0;
}

r_Data_Format
RnpClientComm::doTransferDecompression( r_GMarray* tile, const r_Base_Type *type, r_Data_Format fmt, unsigned long size )
{
	ENTER( "RpcClientComm::doTransferDecompression(...) tile dom:" << tile->spatial_domain() << " array size:" << tile->get_array_size() << " type size:" << tile->get_type_length());
	RMDBGENTER( 2, RMDebug::module_clientcomm, "RpcClientComm", "doTransferDecompression(...) tile dom:" 
  	      << tile->spatial_domain() << " array size:" << tile->get_array_size() 
  	      << " type size:" << tile->get_type_length());

	if (fmt != r_Array)
	{
		r_Tile_Compression *engine = NULL;
		char *newTileData = NULL;
		r_Data_Format newFormat;

		RMDBGMIDDLE( 2, RMDebug::module_clientcomm, "RpcClientComm", "doTransferDecompression(...) decompressing from " 
		         << fmt << ", " << size << "bytes");

		try
		{
			r_Storage_Man_CPP sman;
			engine = r_Tile_Compression::create( fmt, tile->spatial_domain(), type );
			engine->set_storage_handler(sman);
			newTileData = (char*)(engine->decompress(tile->get_array(), size));
			RMDBGMIDDLE( 2, RMDebug::module_clientcomm, "RpcClientComm", 
			         "doTransferDecompression(...) decompression to " << engine->get_decomp_format() << " OK");
		}
		catch (r_Error &err)
		{
			RMDBGMIDDLE( 2, RMDebug::module_clientcomm, "RpcClientComm", 
			         "doTransferDecompression(...) decompression to " << engine->get_decomp_format() << " FAILED");
			RMInit::logOut << "RpcClientComm::doTransferDecompression() Error decompressing data, assuming raw" << endl;
		}

		newFormat = engine->get_decomp_format();

		if (newTileData != NULL)
		{
			delete [] tile->get_array();
			tile->set_array(newTileData);
			tile->set_array_size(tile->spatial_domain().cell_count()*tile->get_type_length());      
		}
		else
			newFormat = fmt;

		RMDBGMIDDLE( 2, RMDebug::module_clientcomm, "RpcClientComm",  "doTransferDecompression(...) tile newFmt:" 
		         << newFormat << " dom:" << tile->spatial_domain() 
		         << " array size:" << tile->get_array_size() 
		         << " type size:" << tile->get_type_length());

		// ... also make sure the decoded format is really raw array data (r_Array)
		if ((endianClient != endianServer) && (newFormat == r_Array))
		{
			// if compression engine already handles endianness we mustn't change again
			if (!engine->converts_endianness()) {
			RMDBGMIDDLE( 2, RMDebug::module_clientcomm, "RpcClientComm",  "doTransferDecompression(...) for " 
			     << fmt  << " endianness changed from "
			     << (r_Endian::r_Endianness)endianServer << " to " << (r_Endian::r_Endianness) endianClient);
			changeEndianness(tile, type);
			}
		}

		if (engine != NULL)
			delete engine;
		
		RMDBGEXIT( 2, RMDebug::module_clientcomm, "RpcClientComm", "doTransferDecompression(...) tile fmt:" << newFormat);       
		LEAVE( "RpcClientComm::doTransferDecompression() -> " << newFormat );
		return newFormat;
	}

	if (endianClient != endianServer)
	{
		RMDBGMIDDLE( 2, RMDebug::module_clientcomm, "RpcClientComm",  "doTransferDecompression(...) for " 
			<< fmt << " endianness changed from "
			<< (r_Endian::r_Endianness)endianServer << " to " << (r_Endian::r_Endianness) endianClient);
		changeEndianness(tile, type);
	}

	RMDBGEXIT( 2, RMDebug::module_clientcomm, "RpcClientComm", "doTransferDecompression(...) tile fmt:" << r_Array);       
	LEAVE( "RpcClientComm::doTransferDecompression() -> " << r_Array );
	return r_Array;
}


void RnpClientComm::getElementCollection( r_Set< r_Ref_Any >& resultColl ) throw(r_Error)
{
	ENTER( "RpcClientComm::getElementCollection()" );
	RMDBGENTER( 2, RMDebug::module_clientcomm, "RpcClientComm", "getElementCollection(_)" );
  
	unsigned short rpcStatus = 0;
	
	TALK( "got set of type " << resultColl.get_type_structure() );

	while( rpcStatus == 0 ) // repeat until all elements are transferred
	{
		GetElementRes* thisResult = executeGetNextElement();
	
		rpcStatus = thisResult->status;

		if( rpcStatus == 2 )
		{
			RMInit::logOut << "Error: getElementCollection(...) - no transfer collection or empty transfer collection" << endl;
			LEAVE( "RpcClientComm::getElementCollection(): exception: rpcStatus = " << rpcStatus );
			throw r_Error( r_Error::r_Error_TransferFailed );
		}
		// create new collection element, use type of collection resultColl
		r_Ref_Any     element;
		const r_Type* elementType = resultColl.get_element_type_schema();

		// convert the endianness before creating the new element!
		if (endianClient != endianServer)
		{
	
			if (endianClient == 0)
				elementType->convertToBigEndian(thisResult->data.confarray_val, 1);
			else
				elementType->convertToLittleEndian(thisResult->data.confarray_val, 1);
		}

		switch( elementType->type_id() )
		{
			case r_Type::BOOL:
			case r_Type::CHAR:
			case r_Type::OCTET:
			case r_Type::SHORT:
			case r_Type::USHORT:
			case r_Type::LONG:
			case r_Type::ULONG:
			case r_Type::FLOAT:
			case r_Type::DOUBLE:
				element = new r_Primitive( thisResult->data.confarray_val, (r_Primitive_Type*) elementType );
				r_Transaction::actual_transaction->add_object_list( r_Transaction::SCALAR, (void*) element );       
				break;

			case r_Type::COMPLEXTYPE1:
			case r_Type::COMPLEXTYPE2:
		    		element = new r_Complex(thisResult->data.confarray_val, (r_Complex_Type *)elementType);
		    		r_Transaction::actual_transaction->add_object_list(r_Transaction::SCALAR, (void *)element);
		    		break;       

			case r_Type::STRUCTURETYPE:
		    		element = new r_Structure( thisResult->data.confarray_val, (r_Structure_Type*) elementType );
		    		r_Transaction::actual_transaction->add_object_list( r_Transaction::SCALAR, (void*) element );       
		  		break;

			case r_Type::POINTTYPE:
				{
					char* stringRep = new char[thisResult->data.confarray_len+1];
					strncpy( stringRep, thisResult->data.confarray_val, thisResult->data.confarray_len );
					stringRep[thisResult->data.confarray_len] = '\0';

					r_Point* typedElement = new r_Point( stringRep );
					element               = typedElement;
					r_Transaction::actual_transaction->add_object_list( r_Transaction::POINT, (void*) typedElement );       
					delete [] stringRep;
				}
		  		break;

			case r_Type::SINTERVALTYPE:
				{
					char* stringRep = new char[thisResult->data.confarray_len+1];
					strncpy( stringRep, thisResult->data.confarray_val, thisResult->data.confarray_len );
					stringRep[thisResult->data.confarray_len] = '\0';

					r_Sinterval* typedElement = new r_Sinterval( stringRep );
					element                   = typedElement;
					r_Transaction::actual_transaction->add_object_list( r_Transaction::SINTERVAL, (void*) typedElement );       
					delete [] stringRep;
				}
		  		break;

			case r_Type::MINTERVALTYPE:
				{
					char* stringRep = new char[thisResult->data.confarray_len+1];
					strncpy( stringRep, thisResult->data.confarray_val, thisResult->data.confarray_len );
					stringRep[thisResult->data.confarray_len] = '\0';

					r_Minterval* typedElement = new r_Minterval( stringRep );
					element                   = typedElement;
					r_Transaction::actual_transaction->add_object_list( r_Transaction::MINTERVAL, (void*) typedElement );
					delete [] stringRep;
				}
				break;

			case r_Type::OIDTYPE:
				{
					char* stringRep = new char[thisResult->data.confarray_len+1];
					strncpy( stringRep, thisResult->data.confarray_val, thisResult->data.confarray_len );
					stringRep[thisResult->data.confarray_len] = '\0';

					r_OId* typedElement = new r_OId( stringRep );
					element             = typedElement;
					r_Transaction::actual_transaction->add_object_list( r_Transaction::OID, (void*) typedElement );       
					delete [] stringRep;
				}
				break;
			default:
				RMDBGENTER(2, RMDebug::module_clientcomm, "RpcClientComm", "getElementCollection(...) bad element typeId" << elementType->type_id())      
				break;
		}

		TALK( "got an element" );
	
		// insert element into result set
		resultColl.insert_element( element, 1 );
	
		delete[] thisResult->data.confarray_val;
		delete   thisResult;
	}

  	executeEndTransfer();

	RMDBGEXIT( 2, RMDebug::module_clientcomm, "RpcClientComm", "getElementCollection()" );       
	LEAVE( "RpcClientComm::getElementCollection()" );
}
	
//##################################################################################
void RnpClientComm::executeQuery( const r_OQL_Query& query ) throw( r_Error )
{
	ENTER( "RnpClientComm::executeQuery(_)" );

	checkForRwTransaction();
  
	unsigned short status;

	// 
	// Send MDD constants to the server.
	//
	if( query.get_constants() )
	{
		r_Set< r_GMarray* >* mddConstants = (r_Set< r_GMarray* >*)query.get_constants();

		if(executeInitUpdate() < 0)	// error would be nicer
		{
		    LEAVE( "Error: RnpClientComm::executeQuery(): update query initialization failed." );
		    return;
		}
	
		r_Iterator<r_GMarray*> iter = mddConstants->create_iterator();

		for( iter.reset(); iter.not_done(); iter++ )
		{
			r_GMarray* mdd = *iter;
		   
	 		const r_Base_Type* baseType = mdd->get_base_type_schema();

			if( mdd )
			{
				status = executeStartInsertTransMDD(mdd);
				switch( status )
				{
					case 0:
						break; // OK
					case 2:
						LEAVE( "RnpClientComm::executeQuery(): exception, status = " << status );
						throw r_Error( r_Error::r_Error_DatabaseClassUndefined );  
						break;			
					case 3:
						LEAVE( "RnpClientComm::executeQuery(): exception, status = " << status );
						throw r_Error( r_Error::r_Error_TypeInvalid );             
						break;	  
					default:
						LEAVE( "RnpClientComm::executeQuery(): exception, status = " << status );
						throw r_Error( r_Error::r_Error_TransferFailed );          
						break;
				}

				r_Set< r_GMarray* >* bagOfTiles;

				bagOfTiles = mdd->get_storage_layout()->decomposeMDD( mdd );
		            
				r_Iterator< r_GMarray* > iter2 = bagOfTiles->create_iterator();

				for(iter2.reset(); iter2.not_done();iter2.advance())
				{
					RPCMarray* rpcMarray;

					r_GMarray *origTile = *iter2;

					getMarRpcRepresentation( origTile, rpcMarray, mdd->get_storage_layout()->get_storage_format(), baseType );
		    
					status = executeInsertTile(false, rpcMarray);

					// free rpcMarray structure (rpcMarray->data.confarray_val is freed somewhere else)
					freeMarRpcRepresentation( origTile, rpcMarray );
		    
					// delete current tile (including data block)
					delete origTile;
		 
					if( status > 0 )
		          		{
		              			LEAVE( "RnpClientComm::executeQuery(): exception, status = " << status );
		              			throw r_Error( r_Error::r_Error_TransferFailed );
		          		}
		    		} 

		      		bagOfTiles->remove_all();
		      		delete bagOfTiles;
	
		  		executeEndInsertMDD(false);
		    	}
		}    
	}

	executeExecuteUpdateQuery(query.get_query());
	LEAVE( "RnpClientComm::executeQuery(_)" );
}


// helper functions
void
RnpClientComm::getMarRpcRepresentation( const r_GMarray* mar, RPCMarray*& rpcMarray,
		                               r_Data_Format initStorageFormat,
				     const r_Base_Type *baseType)
{
	ENTER( "RpcClientComm::getMarRpcRepresentation(...)");
	RMDBGENTER(2, RMDebug::module_clientcomm, "RpcClientComm", "getMarRpcRepresentation(...)");
  
	// allocate memory for the RPCMarray data structure and assign its fields 
	rpcMarray                 = (RPCMarray*)mymalloc( sizeof(RPCMarray) );
	rpcMarray->domain         = mar->spatial_domain().get_string_representation();
	rpcMarray->cellTypeLength = mar->get_type_length();

	void* arrayData = NULL;
	r_ULong arraySize=0;

	if (initStorageFormat == r_Array)
	{
		if (transferFormat != r_Array)
		{
			r_Tile_Compression *engine = NULL;

			try
			{
				r_Storage_Man_CPP sman;
				engine = r_Tile_Compression::create(transferFormat, mar->spatial_domain(), baseType);
				engine->set_storage_handler(sman);
  				RMDBGMIDDLE(2, RMDebug::module_clientcomm, "RpcClientComm", "getMarRpcRepresentation(...) compress with " << engine->get_name())
				if ((endianClient != endianServer) && (!engine->converts_endianness()))
				{
		   			RMDBGMIDDLE( 2, RMDebug::module_clientcomm, "RpcClientComm",  "getMarRpcRepresentation(...) for " 
			     			<<  transferFormat << " endianness changed before compression from " << (r_Endian::r_Endianness)endianClient 
						<< " to " << (r_Endian::r_Endianness) endianServer);
					char *endianData = new char[mar->get_array_size()];
					changeEndianness(mar, endianData, baseType);
					arrayData = engine->compress(endianData, arraySize, transferFormatParams);
					delete [] endianData;
					endianData=NULL;
				}
				else
				{
					arrayData = engine->compress(mar->get_array(), arraySize, transferFormatParams);
				}
  				RMDBGMIDDLE(2, RMDebug::module_clientcomm, "RpcClientComm", "compression returned " << arrayData << " (" << arraySize << " bytes)")
/*				void *testData = engine->decompress(arrayData, arraySize);
				cout << "Decompression worked " << ((memcmp(mar->get_array(), testData, (mar->get_type_length()) * (mar->spatial_domain().cell_count())) == 0) ? "OK" : "!NOT!") << endl;
				delete [] testData;
*/

				// ForWiss: revert to uncompressed data if the compressed data is larger
				// coman: and introduced a bug of endianess ...
				if (arraySize > mar->get_array_size())
				{
					RMInit::logOut << "RpcClientComm::getMarRpcRepresentation(...) Warning: overriding compression setting(" 
			               		<< transferFormat << ") to " << r_Array 
			               		<< " because compressed size( " << arraySize
			               		<< " bytes) > uncompressed size( " << mar->get_array_size() << " bytes)" << endl;
					delete [] arrayData;
					arrayData = NULL;
				}
			}
			catch (r_Error &err)
			{
				RMInit::logOut << "RpcClientComm::getMarRpcRepresentation() Error: Unsupported data format " << transferFormat << endl;
			}
			if (engine != NULL)
				delete engine;
		}
		else
		{
			if (endianClient != endianServer)
			{
				RMDBGMIDDLE( 2, RMDebug::module_clientcomm, "RpcClientComm",  "getMarRpcRepresentation(...) for " 
				     <<  transferFormat << " endianness changed from "
				     << (r_Endian::r_Endianness)endianClient << " to " << (r_Endian::r_Endianness) endianServer);
				arraySize = mar->get_array_size();
				arrayData = new char[arraySize];
				changeEndianness(mar, arrayData, baseType);
			}
		}
	}

	if (arrayData == NULL)
	{
		//error in compression or compression inefficient
		rpcMarray->currentFormat = initStorageFormat;
		rpcMarray->data.confarray_len = mar->get_array_size();
		if (endianClient != endianServer)
		{
			RMDBGMIDDLE( 2, RMDebug::module_clientcomm, "RpcClientComm",  "getMarRpcRepresentation(...) for " 
				<<  transferFormat << "endianness changed from "
				<< (r_Endian::r_Endianness)endianClient << " to " << (r_Endian::r_Endianness) endianServer
		 		<< " because compression " << transferFormat << " failed" ); 
		 	arrayData = new char[arraySize];
		 	changeEndianness(mar, arrayData, baseType);
		 	rpcMarray->data.confarray_val = (char*)(arrayData);
		}
		else
		{
			rpcMarray->data.confarray_val = (char*)(mar->get_array());
		}
	}
	else
	{
		if (arraySize != mar->get_array_size())
		{
  			RMDBGMIDDLE(2, RMDebug::module_clientcomm, "RpcClientComm", "compressed to " << (100.0*arraySize) / mar->get_array_size() << "%")
		}
		rpcMarray->currentFormat = transferFormat;
		rpcMarray->data.confarray_len = arraySize;
		rpcMarray->data.confarray_val = (char*)arrayData;
	}
	rpcMarray->storageFormat = storageFormat;
	
	RMDBGEXIT(2, RMDebug::module_clientcomm, "RpcClientComm", "getMarRpcRepresentation(...)");
	LEAVE( "RpcClientComm::getMarRpcRepresentation()");
}


void
RnpClientComm::freeMarRpcRepresentation( const r_GMarray* mar, RPCMarray* rpcMarray )
{
	ENTER( "RnpClientComm::freeMarRpcRepresentation(_,_)" );

	if (rpcMarray->data.confarray_val != ((r_GMarray*)mar)->get_array())
	{
		delete[] rpcMarray->data.confarray_val;
	}
	free( rpcMarray->domain );
	free( rpcMarray );

	LEAVE( "RnpClientComm::freeMarRpcRepresentation()" );
}


//#########################################################################
r_OId RnpClientComm::getNewOId( unsigned short objType ) throw(r_Error)
{
	return executeGetNewOId(objType);
}
	
unsigned short RnpClientComm::getObjectType( const r_OId& oid ) throw(r_Error)
{
	return executeGetObjectType(oid);
}

char* RnpClientComm::getTypeStructure( const char* typeName, r_Type_Type typeType ) throw(r_Error)
{
	return executeGetTypeStructure( typeName, typeType );
}

int RnpClientComm::setStorageFormat( r_Data_Format format, const char *formatParams)
{
	ENTER( "RnpClientComm::setStorageFormat( format=" << format << ", formatParams=" << (formatParams?formatParams:"(null)") << " )" );

	storageFormat = format;

	if (storageFormatParams != NULL)
	{
		free(storageFormatParams);
		storageFormatParams = NULL;
	}
	if (formatParams != NULL)
	{
		storageFormatParams = (char*)mymalloc(strlen(formatParams) + 1);
		strcpy(storageFormatParams, formatParams);
		// extract ``compserver'' if present
		clientParams->process(storageFormatParams);
	}

	int result = executeSetFormat( false, format, formatParams);

	LEAVE( "RnpClientComm::setStorageFormat() -> " << result );
	return result;
}
	
int RnpClientComm::setTransferFormat( r_Data_Format format, const char* formatParams)
{
	ENTER( "RnpClientComm::setTransferFormat( format=" << format << ", formatParams=" << (formatParams?formatParams:"(null)") << " )" );

	transferFormat = format;

	if (transferFormatParams != NULL)
	{
		  free(transferFormatParams);
		  transferFormatParams = NULL;
	}
	if (formatParams != NULL)
	{
		  transferFormatParams = (char*)mymalloc(strlen(formatParams)+1);
		  strcpy(transferFormatParams, formatParams);
		  // extract ``exactformat'' if present
		  clientParams->process(transferFormatParams);
	}
		 
	int result = executeSetFormat( true, format, formatParams);
	LEAVE( "RnpClientComm::setTransferFormat() -> " << result );
	return result;
}
	
const char* RnpClientComm::getExtendedErrorInfo() throw(r_Error)
{
	 // This function shouldn't be called for RNP protocol
	 static char *errorInfo = new char[30];
	 strcpy(errorInfo,"No info");
	 
	 return errorInfo;
}
	
const char* RnpClientComm::getServerName()
{
	return serverHost;
}
	
void RnpClientComm::setUserIdentification(const char *userName, const char *plainTextPassword)
{
	ENTER( "RnpClientComm::setUserIdentification( userName=" << (userName?userName:"(null)") << ", plainTextPassword=" << (plainTextPassword?plainTextPassword:"(null)") << " )" );

	char digest[33]="";
	messageDigest(plainTextPassword,digest,"MD5");
	sprintf(identificationString,"%s:%s",userName,digest);  

	LEAVE( "RnpClientComm::setUserIdentification()" );
}

unsigned long RnpClientComm::getClientID() const
{
	return clientID;
}
			
void RnpClientComm::triggerAliveSignal(){}

void RnpClientComm::sendAliveSignal(){}
	
//############# helper functions ###################   
int RnpClientComm::messageDigest(const char *input,char *output,const char *mdName)
{ 
		   
	EVP_MD_CTX mdctx;
	const EVP_MD *md;
	unsigned int md_len, i;
	unsigned char md_value[100];

	OpenSSL_add_all_digests();

	md = EVP_get_digestbyname(mdName);
	
	if(!md)
		return 0;
		         
	EVP_DigestInit(&mdctx, md);
	EVP_DigestUpdate(&mdctx,input, strlen(input));
	EVP_DigestFinal(&mdctx, md_value, &md_len);

	for(i = 0; i < md_len; i++)
		sprintf(output+i+i,"%02x", md_value[i]);
	
	return strlen(output);
}


void RnpClientComm::setMaxRetry(unsigned int newMaxRetry)
{
	RMInit::clientcommMaxRetry = newMaxRetry;
}
	
unsigned int  RnpClientComm::getMaxRetry()
{
	return RMInit::clientcommMaxRetry;
}

// aux function: sleep incrementally, with subsecond precision
static void pause(int retryCount)
{
	// changed PB 2005-sep-09
	// was: unsigned int millisec = 50 + retryCount * 50;
	unsigned int millisec = RNP_PAUSE_INCREMENT * (retryCount + 1);
	// if(millisec > 1000)
	// 	millisec = 1000;
	
	timeval tv;
	tv.tv_sec  = millisec / 1000;
	tv.tv_usec = millisec * 1000;
	select(0,NULL,NULL,NULL,&tv);
}

int RnpClientComm::getFreeServer(bool readwrite, bool openDB)
{
	ENTER( "RnpClientComm::getFreeServer( readwrite=" << readwrite << ", openDB=" << openDB << " )" );

	// to avoid nested retries, only this loop is kept,
	// the one in executeGetFreeServer() has been removed -- PB 2005-aug-31
	for ( int retryCount=0; ; retryCount++ )
	{
		try
		{
			executeGetFreeServer(readwrite, openDB);

			// if no error, we have the server, so break
			break;
		}
		catch(r_Error &e)
		{ 
			int errorno = e.get_errorno();
			RMInit::logOut << "Error: Connection to rasdaman server failed with " << errorno << ": " << e.what() << endl;
			// for these errors a retry makes sense, as long as we haven't reached the retry limit:
			if ( ( errorno==801 || errorno==805 || errorno==806 ) && retryCount < RMInit::clientcommMaxRetry )
		  	{
				TALK( "  retry="<<retryCount );
				pause(retryCount);	// waiting with incremental delays
			}
			else
			{
				LEAVE( "RnpClientComm::getFreeServer(): exception " << errorno << ": " << e.what() );
 				throw;
			}
		}
	}

	LEAVE( "RnpClientComm::getFreeServer() -> 1" );
	return 1;
} 

#define MAXMSG 512
int RnpClientComm::executeGetFreeServer(bool readwrite, bool openDB)
{ 
	ENTER( "RnpClientComm::executeGetFreeServer( readwrite=" << readwrite << ", openDB=" << openDB << " )" );

	static char myRasmgrID[100]="";
	if(myRasmgrID[0]==0) 
	{
		unsigned int hostid = gethostid();
		unsigned int pid    = getpid();
		sprintf(myRasmgrID,"%u:%u",hostid,pid);
	}
	char message[MAXMSG];
	char header[MAXMSG];
	char body[MAXMSG];
	
	if (openDB)
		  sprintf(header,"POST getfreeserver2 HTTP/1.1\r\nAccept: text/plain\r\nUserAgent: RasClient/1.0\r\nAuthorization: ras %s\r\nContent-length:",identificationString);
	else
		  sprintf(header,"POST getfreeserver HTTP/1.1\r\nAccept: text/plain\r\nUserAgent: RasClient/1.0\r\nAuthorization: ras %s\r\nContent-length:",identificationString);
	sprintf(body,"%s RNP %s %s",databaseName,(readwrite ? "rw":"ro"), myRasmgrID);
	sprintf(message,"%s %d\r\n\r\n%s",header,strlen(body)+1,body);
	
	struct protoent* getprotoptr = getprotobyname("tcp");

	struct hostent *hostinfo = gethostbyname(rasmgrHost);
	if(hostinfo==NULL)
	{
		RMInit::logOut << "Error locating rasmgr on host " << rasmgrHost <<" ("<<strerror(errno)<<')'<<endl; 
		LEAVE( "RnpClientComm::executeGetFreeServer(): exception, unable to contact rasmgr." );
		throw r_Error( r_Error::r_Error_ServerInvalid );
	} 

	sockaddr_in internetSocketAddress;
	
	internetSocketAddress.sin_family=AF_INET;
	internetSocketAddress.sin_port=htons(rasmgrPort);
	internetSocketAddress.sin_addr=*(struct in_addr*)hostinfo->h_addr;    

	int sock;
	bool ok = false;
	// old comment by Walter Schatz: "this has to be 5000 or so, now that counter is 120 default (later we'll make this better)"
	// new comment by PB 2005-aug-31:
	// - these retries + the ones in getFreeServer lead to max*max*40 = 400,000 retries! one loop should be enough.
	// - there are unmotivated factors at every step (cf pause()), let's do just one wait loop;
	//   all loop code is commented out with prefix "NOLOOP"
	// X: int retry;
	// X: for(retry=0;retry<RMInit::clientcommMaxRetry * 40;retry++)
	// X: {
		sock=socket(PF_INET,SOCK_STREAM,getprotoptr->p_proto);
		if(sock<0)
		{
	// X:		if(retry==0)
				RMInit::logOut << "Error: cannot open socket to rasmgr (" << strerror(errno) << ")." << endl; 
	// X:	        sleep(RMInit::clientcommSleep);
	// X:		continue;
		}
		 
		else if(connect(sock,(struct sockaddr*)&internetSocketAddress,sizeof(internetSocketAddress)) < 0)
		{
	// X:		if(retry==0)
	// X:	      		RMInit::logOut <<"getFreeServer: Connection to rasmgr failed: "<<strerror(errno)<<endl;
			close(sock);
	// X:		sleep(RMInit::clientcommSleep);
	// X:		continue;
		} 
		 
		TALK( "Socket="<<sock<<" protocol(tcp)="<<getprotoptr->p_proto );
		ok = true;
	// X:	break;
	// X:}     
	// X:if(retry)
	// X:	RMInit::logOut << "Warning: tried connecting " << retry+1 << " times " <<endl; 
	 	     
	if( !ok )
	{
	// X:	RMInit::logOut << "Error: Giving up on connecting, sorry, after this number of tries: " << retry+1 <<endl; 
		close(sock);
		LEAVE( "RnpClientComm::executeGetFreeServer(): exception, giving up on getting a free server." );
		throw r_Error( r_Error::r_Error_ServerInvalid );
	}    

	// --- from here on we have a connection ---

	//write_to_server
	// TALK( "want to write this message to rasmgr: " << message );	// message is said to be 0-terminated
	int nbytes=writeWholeMessage(sock,message,strlen(message)+1);
	
	if(nbytes<0) 
	{
		RMInit::logOut << "Error: cannot send message to rasmgr on host " << rasmgrHost << " ("<<strerror(errno)<<')' << endl;
		LEAVE( "RnpClientComm::executeGetFreeServer(): exception, error writing message to rasmgr." );
		close(sock);
		throw r_Error( r_Error::r_Error_ServerInvalid );
	} 
	
	//wait and read answer 
	nbytes=readWholeMessage(sock,message,MAXMSG);
	close(sock);

	if(nbytes<0)
	{
		RMInit::logOut << "Error reading answer from rasmgr on host " << rasmgrHost <<" ("<<strerror(errno)<<')'<<endl;
		LEAVE( "RnpClientComm::executeGetFreeServer(): exception, error reading answer from rasmgr." );
		throw r_Error( r_Error::r_Error_ServerInvalid );
	} 
	// TALK( "received this message: " << message );	// quite verbose!

	// and now, analyze answer
	// first line is: HTTP/1.1 code answertext(CRLF)
	char *p=strstr(message," "); //looks for the first white space to locate status-code
	
	int statusCode=strtoul( p, (char **)NULL, 10);
	 
	char *pEOL=strstr(p,"\r\n"); // locate CRLF
	if(!pEOL)  
	{
		RMInit::logOut << "Error: Invalid answer from rasmgr." << endl;
		LEAVE( "RnpClientComm::executeGetFreeServer(): exception, invalid answer from rasmgr." );
		throw r_Error( r_Error::r_Error_ServerInvalid );
	}   
	
	if(statusCode==200)
	{	// It's OK
		char *addr = strstr(message,"\r\n\r\n")+4; //looks for the address of server
		  
		addr = strtok(addr," \r\n\t");       //isolates the RasMGR host name
	
		char *portString = strtok(NULL," \r\n\t"); //looks for the port, sended as string
	
		char *capab      = strtok(NULL," \r\n\t");
		    
		if(portString && addr && capab)
		{ 
			strcpy(serverHost,addr);
			serverPort = strtoul( portString, (char **)NULL, 0);
			strcpy(capability,capab);
			TALK( "RnpClientComm::executeGetFreeServer(): got server " << serverHost << ":" << serverPort << ", capability: " << capability );
		}
		else  
		{
			RMInit::logOut << "Error: Invalid answer from rasmgr." << endl;
			LEAVE( "RnpClientComm::executeGetFreeServer(): exception, server invalid." );
			throw r_Error( r_Error::r_Error_ServerInvalid );
		}

	}
	else 
	{       
		 char *errText = strstr(message,"\r\n\r\n")+4;
		 RMInit::logOut << "Communication error: "<<errText<< endl;
		 
		 int errorCode = strtoul(errText, (char **)NULL, 0);

		 switch(errorCode)
		 {
			case 802:
		 	case 803:
		 	case 804:
				LEAVE( "RnpClientComm::executeGetFreeServer(): exception, errorCode = " << errorCode );
				throw r_Error( r_Error::r_Error_AccesDenied,errorCode);
		      		break;
		 	case 801:
		 	case 805: 
		 	case 806:
				LEAVE( "RnpClientComm::executeGetFreeServer(): exception, errorCode = " << errorCode );
				throw r_Error( r_Error::r_Error_SystemOverloaded,errorCode);
				break;
		 	case 807:
				LEAVE( "RnpClientComm::executeGetFreeServer(): exception, errorCode = " << errorCode );
				throw r_Error( r_Error::r_Error_DatabaseUnknown,errorCode);
				break;
		 	default : 
				LEAVE( "RnpClientComm::executeGetFreeServer(): exception, errorCode = " << errorCode );
				throw r_Error( r_Error::r_Error_General,808 );
				break;            
		}		     
	} 

	LEAVE( "RnpClientComm::executeGetFreeServer() -> 1" );
	return 1;
}

int RnpClientComm::readWholeMessage(int socket,char *destBuffer,int buffSize)
{
	// we read what is comming in until we encounter a '\0'
	// this is our end-sign. 
	int totalLength=0;
	int redNow;
	while(1)
	{
		redNow = read(socket,destBuffer+totalLength,buffSize-totalLength);
		if(redNow == -1)
		{
			if(errno == EINTR)
				continue; // read was interrupted by signal
			return -1; // another error
		}
		totalLength+=redNow;
	
		if(destBuffer[totalLength-1]==0)
			break; // THE END	    
	}

	TALK( "RnpClientComm::readWholeMessage(): read " << totalLength << " bytes, buffer is: " << destBuffer );
	return totalLength;
}

int RnpClientComm::writeWholeMessage(int socket,char *destBuffer,int buffSize)
{
	// we write the whole message, including the ending '\0', which is already in
	// the buffSize provided by the caller
	int totalLength=0;
	int writeNow;
	while(1)
	{
		  writeNow = write(socket,destBuffer+totalLength,buffSize-totalLength);
		if(writeNow == -1)
		{
			if(errno == EINTR)
				continue; // read was interrupted by signal
			return -1; // another error
		}
		totalLength+=writeNow;
	
		if( totalLength==buffSize )
			break; // THE END	    
	}

	TALK( "RnpClientComm::writeWholeMessage(): wrote " << totalLength << " bytes, buffer is: " << destBuffer );
	return totalLength;
}

	
void RnpClientComm::checkForRwTransaction() throw (r_Error)
{
	r_Transaction *trans = r_Transaction::actual_transaction;
	if(  trans == 0 || trans->get_mode() == r_Transaction::read_only )
	{
		TALK( "RnpClientComm::checkForRwTransaction(): throwing exception from failed TA rw check." );
		throw r_Error( r_Error::r_Error_TransactionReadOnly );
	}
}
