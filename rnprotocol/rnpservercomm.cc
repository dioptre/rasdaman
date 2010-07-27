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
 * -  CLIENTID: | 0|.counter.|....timestamp ......|
 *    	 	|31,30.....24|23...16|15...8|7...0|
 * - requests count from 1..n between connect and disconnect
 * - fragments count from 1..n between ???
 *
 ************************************************************/

#ifndef RMANVERSION
#error "Please specify RMANVERSION variable!"
#endif

#ifndef COMPDATE
#error "Please specify the COMPDATE variable!"
/*
COMPDATE=`date +"%d.%m.%Y %H:%M:%S"`

and -DCOMPDATE="\"$(COMPDATE)\"" when compiling
*/
#endif

#include "mymalloc/mymalloc.h"
#include <time.h>
#include "rnprasserver.hh"
#include "rnpservercomm.hh"
#include "srvrasmgrcomm.hh"

#include "server/rasserver_entry.hh"

#include "debug-srv.hh"



// aux function to avoid a compiler warning (see 'man strftime')
size_t my_strftime(char *s, size_t max, const char *fmt, const struct tm *tm)
{
        return strftime(s, max, fmt, tm);
}

// now(): aux function returning, as a static string, the current time
// keep in sync with same function in rasmgr/rasmgr_localsrv.cc!
const char* now()
{
	size_t strfResult = 0;		// return value of strftime()
        static char timestring[50];	// must hold 20+1 chars

        time_t t = time(NULL);		// get time
        struct tm* tm = localtime(&t);	// break down time
        strfResult = my_strftime( timestring, sizeof(timestring), "[%F %T]", tm );	// format time
	if (strfResult == 0)		// bad luck? then take fallback message
		(void) strncpy( timestring, "[-no time available-]", sizeof(timestring) );
	return( timestring );
}

const int RnpRasDaManComm::NoClient = -1;

RnpRasDaManComm::RnpRasDaManComm() throw()
{
	ENTER( "RnpRasDaManComm::RnpRasDaManComm" );
	requestCounter  =  0;
	fragmentCounter =  0;
	clientID        =  NoClient;
	LEAVE( "RnpRasDaManComm::RnpRasDaManComm" );
}
      
RnpRasDaManComm::~RnpRasDaManComm() throw()
{
	TALK( "RnpRasDaManComm::~RnpRasDaManComm" );
}

// we need our implementation because of r_Error, but we will go for the default when r_Error is AkgException
void RnpRasDaManComm::processRequest(CommBuffer *receiverBuffer, CommBuffer *transmiterBuffer, RnpTransport::CarrierProtocol protocol, RnpServerJob *callingJob) throw()
{
	RMTimer requestTime("RnpRasDaManComm","request");

	ENTER( "RnpRasDaManComm::processRequest, at " << now() << ", client=" << callingJob->getClientHostAddress().getStringAddress() );
	RMInit::logOut << endl << now() << " request from " << callingJob->getClientHostAddress().getStringAddress() << endl;
	
	decoder.decode(receiverBuffer);
	RnpQuark destServerType       = decoder.getDestinationServerType();
	Rnp::Endianness desEndianness = decoder.getDesiredEndianness();

	// test if servertype matches!
	
	transmiterBuffer->allocate(getTransmitterBufferSize());
	transmiterBuffer->clearToWrite();

	encoder.setBuffer(transmiterBuffer);
	encoder.setFinalEndianness(desEndianness);
	encoder.startAnswer(destServerType, protocol);
	
	requestCounter++;

	decoder.getFirstFragment();
	bool wasError = false;
	for(int fragment=0; fragment < decoder.countFragments(); fragment++)
	{
		if(wasError == false)
		{
	   		try
			{
				decodeFragment(); 
			}
			// DBMS connection lost? then need to disconnect client to allow to resync
			catch(r_Ebase_dbms &edb)
			{
				RMInit::logOut << "Error: base DBMS reports: " << edb.what() << endl;
				wasError = true;
				answerr_Error(edb);
#if 0 // seems too hard -- PB 2005-jul-25
				try
				{
					RMInit::logOut << "detaching client..." ;
        				executeDisconnect();
					RMInit::logOut << "ok" << endl;
				}
				catch (...)		// ignore any further error, just log it
				{
					RMInit::logOut << "failed" << endl;
				}
#endif // 0
			}
			catch(r_Error &ex)
			{
				RMInit::logOut << "Error: request terminated: " << ex.what() << endl;
				TALK( "rasdaman exception kind=" << ex.get_kind() << ", errorno=" << ex.get_errorno() );
				wasError = true;
				answerr_Error(ex);

#if 0 // seems too hard -- PB 2005-jul-25
				// a base DBMS error we treat just like above
				// -- PB 2003-nov-24
				if (ex.get_kind() == r_Error::r_Error_BaseDBMSFailed
				 || ex.get_errorno() == 206)	// serializable error, see errtxts
				{
					try
					{
						RMInit::logOut << "detaching client...";
        					executeDisconnect();
						RMInit::logOut << "ok" << endl;
					}
					catch (...)		// ignore any further error, just log it
					{
						RMInit::logOut << "failed" << endl;
					}
				}
#endif // 0
			}
			catch(exception &ex)
			{
				RMInit::logOut << "Error: request terminated with general exception: " << ex.what() << endl;
				wasError = true;
				answerSTLException(ex);
			}  
			catch(...)
			{
				RMInit::logOut << "Error: request terminated with generic exception." << endl;
				wasError = true; 
				answerUnknownError();
			}
		}
		else
		{
			discardFragment();
		}   
		decoder.getNextFragment();   
	}
	encoder.endMessage();

	RMInit::logOut << now() << " request completed in " << requestTime.getTime() << " usecs." << endl;
	LEAVE( "RnpRasDaManComm::processRequest" );
}

RnpServerJob* RnpRasDaManComm::createJobs(int howMany)
{
	TALK( "RNP: creating "<<howMany<<" RnpRasserverJob's" );
	return new RnpRasserverJob[howMany];
}

void RnpRasDaManComm::setTimeoutInterval(int seconds)
{
	clientTimer.setTimeoutInterval(seconds);
}
   
void RnpRasDaManComm::checkForTimeout()
{
	ENTER( "RnpRasDaManComm::checkForTimeout" );
	if(clientID != NoClient) 
	{
		if(clientTimer.checkForTimeout()) 
		{
			TALK( "Client 0x" << hex << clientID << dec << " has timed out." );
			RMInit::logOut << "Client has timed out, connection being freed." << endl;
			disconnectClient();
		}
	}	  
	else
		rasmgrComm.informRasmgrServerStillAvailable();
	LEAVE( "RnpRasDaManComm::checkForTimeout" );
}

   
void RnpRasDaManComm::decodeFragment() throw( r_Error )
{
	ENTER( "RnpRasDaManComm::decodeFragment" );

	try	// somewhere during cmd execution there can be exceptions; we pass them thru
	{
		fragmentCounter++;

		RnpQuark command = decoder.getCommand();

		RnpRasserver *hook = new RnpRasserver;
		TALK( "fragmentCounter=" << fragmentCounter << ", command is " << hook->getCommandName( command ) );

		// first parameter has to be the clientID
		verifyClientID( command );

		switch(command)
		{
			case RnpRasserver::cmd_connect:     	executeConnect();     		break;
			case RnpRasserver::cmd_disconnect:  	executeDisconnect();  		break;
			case RnpRasserver::cmd_opendb:      	executeOpenDB();      		break;
			case RnpRasserver::cmd_closedb:     	executeCloseDB();     		break;
			case RnpRasserver::cmd_beginta:	    	executeBeginTA();     		break;
        		case RnpRasserver::cmd_committa:    	executeCommitTA();    		break;
        		case RnpRasserver::cmd_abortta:     	executeAbortTA();     		break;
        		case RnpRasserver::cmd_istaopen:    	executeIsTAOpen();    		break;
        		case RnpRasserver::cmd_queryhttp:   	executeQueryHttp();   		break;
        		case RnpRasserver::cmd_getnewoid:   	executeGetNewOId();   		break;
			case RnpRasserver::cmd_queryrpc:    	executeQueryRpc();    		break;
			case RnpRasserver::cmd_getnextelem: 	executeGetNextElement(); 	break;
			case RnpRasserver::cmd_endtransfer: 	executeEndTransfer(); 		break;
			case RnpRasserver::cmd_getnextmdd:  	executeGetNextMDD();  		break;
			case RnpRasserver::cmd_getnexttile: 	executeGetNextTile(); 		break;
			case RnpRasserver::cmd_updaterpc  : 	executeUpdateQuery(); 		break;
			case RnpRasserver::cmd_startinsTmdd: 	executeStartInsertTransMDD();	break;
			case RnpRasserver::cmd_inserttile : 	executeInsertTile();  		break;
			case RnpRasserver::cmd_endinsmdd  : 	executeEndInsertMDD();		break;
			case RnpRasserver::cmd_initupdate : 	executeInitUpdate();  		break;
        		case RnpRasserver::cmd_gettypestruct: 	executeGetTypeStructure();  	break;
			case RnpRasserver::cmd_startinsPmdd: 	executeStartInsertPersMDD(); 	break;
			case RnpRasserver::cmd_insertmdd:   	executeInsertMDD();           	break;
			case RnpRasserver::cmd_insertcoll:  	executeInsertCollection();    	break;
        		case RnpRasserver::cmd_remobjfromcoll: 	executeRemoveObjFromColl();	break;
        		case RnpRasserver::cmd_delobjbyoid:    	executeDeleteObjByOId();   	break;
        		case RnpRasserver::cmd_delcollbyname:  	executeDeleteCollByName(); 	break;
			case RnpRasserver::cmd_getcoll:     	executeGetCollection();       	break;
			case RnpRasserver::cmd_getcolloids: 	executeGetCollectionOIds();   	break;
			case RnpRasserver::cmd_getobjecttype: 	executeGetObjectType();     	break;
        		case RnpRasserver::cmd_setformat:     	executeSetFormat();		break;
			// --- until here the compatible ones ---
	
			// -- the secret, unofficial ones --
			case RnpRasserver::cmd_createcollection:   executeCreateCollection();  break;
			case RnpRasserver::cmd_createmdd:          executeCreateMDD();         break;
			case RnpRasserver::cmd_extendmdd:          executeExtendMDD();         break;
			case RnpRasserver::cmd_gettiledomains:     executeGetTileDomains();    break;
	
			default:
				RMInit::logOut << "Protocol error: Unknown command: " << command << endl;
				LEAVE( "RnpRasDaManComm::decodeFragment: Unknown command: "<<command );
				throw r_Error(822);  
				break;
		}

		clientTimer.markAction();
	}
	catch (r_Error &e)			// any rasdaman error is passed through
	{
		LEAVE( "RnpRasDaManComm::decodeFragment, rasdaman exception caught: " << e.what() );
		throw;				// pass on
	}

	catch (...)				// any other error is "unexpected", by def
	{
		LEAVE( "RnpRasDaManComm::decodeFragment, general exception caught." );
		throw( r_Error( 10000 ) );	// unexpected internal error - FIXME: can we be more precise?
	}

	LEAVE( "RnpRasDaManComm::decodeFragment" );
}

//######## here the executing functions ################
void RnpRasDaManComm::executeConnect()
{
	ENTER( "RnpRasDaManComm::executeConnect" );

	RasServerEntry& rasserver = RasServerEntry::getInstance();

	decoder.getNextParameter();
	const char *capability = decoder.getDataAsString();
	
	TALK("capability: "<<capability);

	// a new connect requires to drop any eventually preexisting connection first -- PB 2005-sep-02
	// if (clientID != NoClient)		// any previous un-disconnected activity?
	if (fragmentCounter > 1 || requestCounter > 1)		// any previous un-disconnected activity?
	{
		RMInit::logOut << "Preparing request for new connect by resetting old connection; ";
		RnpRasDaManComm::disconnectInternally();
		// FIXME: the entry in CltTable still remains (see compat_*())
		// - although this doesn't harm in any way it should be removed
	}

	rasserver.compat_connectNewClient(capability);
	connectClient();
    
	encoder.startFragment(Rnp::fgt_OkAnswer, decoder.getCommand()); 
	encoder.addInt32Parameter(RnpRasserver::pmt_clientid, clientID);
	encoder.endFragment();
	TALK( "adding clientID 0x" << hex << clientID << dec );

	LEAVE( "RnpRasDaManComm::executeConnect, assigned clientID=0x"<<hex<<clientID<<dec);      
}
   
void RnpRasDaManComm::executeDisconnect()
{
	ENTER("RnpRasDaManComm::executeDisconnect, clientID=0x"<<hex<<clientID<<dec);
	RasServerEntry& rasserver = RasServerEntry::getInstance(); 
    
	rasserver.compat_disconnectClient();
	TALK( "rasserver.compat_disconnectClient() done, now disconnectClient()." );
	disconnectClient();

	encoder.startFragment(Rnp::fgt_OkAnswer, decoder.getCommand()); 
	encoder.endFragment();     

	LEAVE("RnpRasDaManComm::executeDisconnect");
}

void RnpRasDaManComm::executeOpenDB()
{
	ENTER("RnpRasDaManComm::executeOpenDB");
  
	RasServerEntry& rasserver = RasServerEntry::getInstance(); 
    
	decoder.getNextParameter();
	const char* databaseName = decoder.getDataAsString();
	
	TALK( "Execute open DB, database="<< databaseName );
      
	rasserver.compat_openDB(databaseName);
    
	encoder.startFragment(Rnp::fgt_OkAnswer, decoder.getCommand()); 
	encoder.endFragment();
      
	LEAVE("RnpRasDaManComm::executeOpenDB");
}
   
void RnpRasDaManComm::executeCloseDB()
{
	ENTER("RnpRasDaManComm::executeCloseDB");  

	RasServerEntry& rasserver = RasServerEntry::getInstance(); 
    
	rasserver.compat_closeDB();
    
	encoder.startFragment(Rnp::fgt_OkAnswer, decoder.getCommand()); 
	encoder.endFragment();     

	LEAVE("RnpRasDaManComm::executeCloseDB");  
}

void RnpRasDaManComm::executeBeginTA()
{
	ENTER( "RnpRasDaManComm::executeBeginTA" );

	RasServerEntry& rasserver = RasServerEntry::getInstance();
    
	decoder.getNextParameter();
	bool rw = decoder.getDataAsInteger() ? true:false;
       
	TALK( "executeBeginTA  transaction: "<<(rw ? "rw":"ro") );
    
	rasserver.compat_beginTA(rw);
    
	encoder.startFragment(Rnp::fgt_OkAnswer, decoder.getCommand()); 
	encoder.endFragment();     

	LEAVE( "RnpRasDaManComm::executeBeginTA" );
}
   
void RnpRasDaManComm::executeCommitTA()
{
    ENTER("executeCommitTA - in");  
    RasServerEntry& rasserver = RasServerEntry::getInstance(); 
    
    rasserver.compat_commitTA();
    
    encoder.startFragment(Rnp::fgt_OkAnswer, decoder.getCommand()); 
    encoder.endFragment();     
    LEAVE("executeCommitTA - out");  
}
   
void RnpRasDaManComm::executeAbortTA()
  {
    ENTER("executeAbortTA - in");
    RasServerEntry& rasserver = RasServerEntry::getInstance(); 
    
    rasserver.compat_abortTA();
    
    encoder.startFragment(Rnp::fgt_OkAnswer, decoder.getCommand()); 
    encoder.endFragment();     
    LEAVE("executeAbortTA - out");
   }
   
void RnpRasDaManComm::executeIsTAOpen()
  {
    ENTER("executeIsTAOpen - in");
    RasServerEntry& rasserver = RasServerEntry::getInstance(); 
    
    bool isOpen = rasserver.compat_isOpenTA();
 
    encoder.startFragment(Rnp::fgt_OkAnswer, decoder.getCommand());
    encoder.addInt32Parameter(RnpRasserver::pmt_transstatus, isOpen);
    encoder.endFragment();     
    LEAVE("executeIsTAOpen - out; isOpen=" << isOpen);
   }
   
void RnpRasDaManComm::executeQueryHttp()
{
	ENTER("executeQueryHttp - in");

	RasServerEntry& rasserver = RasServerEntry::getInstance();
	// TALK("have inst" );    
	decoder.getNextParameter();
        
	const void* httpParams = decoder.getData();
	int      httpParamsLen = decoder.getDataLength();
	// TALK( "httpParamsLen=" << httpParamsLen );
	char *resultBuffer = 0;
	int resultLen = rasserver.compat_executeQueryHttp((const char*)httpParams, httpParamsLen, resultBuffer);
	// TALK( "resultLen=" << resultLen );
 
	encoder.startFragment(Rnp::fgt_OkAnswer, decoder.getCommand());

        if(resultLen && resultBuffer) 
	{ 
		encoder.adjustBufferSize(resultLen);
		encoder.addOpaqueParameter(RnpRasserver::pmt_httpqanswer, resultBuffer, resultLen);
		delete[] resultBuffer; 
		resultBuffer = 0;
	} 
	encoder.endFragment();
      
	LEAVE("executeQueryHttp - out; resultLen=" << resultLen );
}
   
void RnpRasDaManComm::executeGetNewOId()
  {
    ENTER("executeGetNewOId - in");
    RasServerEntry& rasserver = RasServerEntry::getInstance();
    
    decoder.getNextParameter();
    
    int objType = decoder.getDataAsInteger();
    
    r_OId oid = rasserver.compat_getNewOId( (unsigned short)objType );
    const char* cOId = oid.get_string_representation();

    TALK("executeGetNewOId objType = "<<objType<<"  oid="<<cOId);
    
    encoder.startFragment(Rnp::fgt_OkAnswer, decoder.getCommand());
    encoder.addStringParameter(RnpRasserver::pmt_oidstring, cOId);
    encoder.endFragment();
 
   LEAVE("executeGetNewOId - out");
   }

#define INITPTR(a) a = 0
#define SECUREPTR(a) if(a == 0) a = strdup("")
#define FREEPTR(a) free(a)

void RnpRasDaManComm::executeQueryRpc()
  {
    ENTER("executeQueryRpc() - in");
    
    RasServerEntry& rasserver = RasServerEntry::getInstance();
    
    decoder.getNextParameter();
    
    const char *query = decoder.getDataAsString();
    TALK("query="<<query);
     
    ExecuteQueryRes queryResult;
    	INITPTR(queryResult.token);
    	INITPTR(queryResult.typeName);
    	INITPTR(queryResult.typeStructure);
    
    int status = rasserver.compat_executeQueryRpc(query, queryResult);
    	SECUREPTR(queryResult.token);
    	SECUREPTR(queryResult.typeName);
    	SECUREPTR(queryResult.typeStructure);
    
    encoder.startFragment(Rnp::fgt_OkAnswer, decoder.getCommand());
    encoder.addInt32Parameter( RnpRasserver::pmt_returnstatus,status); 
    TALK( "adding return status " << status );
    encoder.addInt32Parameter( RnpRasserver::pmt_errorno,   queryResult.errorNo);
    encoder.addInt32Parameter( RnpRasserver::pmt_lineno,    queryResult.lineNo);
    encoder.addInt32Parameter( RnpRasserver::pmt_columnno,  queryResult.columnNo);
    encoder.addStringParameter(RnpRasserver::pmt_errortoken,queryResult.token);
    encoder.addStringParameter(RnpRasserver::pmt_typename,  queryResult.typeName);
    encoder.addStringParameter(RnpRasserver::pmt_typestructure,queryResult.typeStructure);
    encoder.endFragment();	

    FREEPTR(queryResult.token);
    FREEPTR(queryResult.typeName);
    FREEPTR(queryResult.typeStructure);
    
   LEAVE("executeQueryRpc - out");
   }
   
void RnpRasDaManComm::executeGetNextElement()
  {
    ENTER("executeGetNextElement - in");
    RasServerEntry& rasserver = RasServerEntry::getInstance();
    
    char *buffer = NULL;
    unsigned int  bufferSize;
    
    int status = rasserver.compat_getNextElement(buffer,bufferSize);
    
    encoder.startFragment(Rnp::fgt_OkAnswer, decoder.getCommand());
    encoder.addInt32Parameter(RnpRasserver::pmt_returnstatus,status);
    TALK( "adding return status " << status );
    if(buffer != NULL) 
      encoder.addOpaqueParameter(RnpRasserver::pmt_skalarobject, buffer, bufferSize);
      
    encoder.endFragment();	
    
    free(buffer);

    LEAVE("executeGetNextElement - out");
   }

void RnpRasDaManComm::executeEndTransfer()
  {
    ENTER("executeEndTransfer - in");
    RasServerEntry& rasserver = RasServerEntry::getInstance();
    
    int status = rasserver.compat_endTransfer();
    
    encoder.startFragment(Rnp::fgt_OkAnswer, decoder.getCommand());
    encoder.addInt32Parameter(RnpRasserver::pmt_returnstatus,status);
    TALK( "adding return status " << status );
    encoder.endFragment();	

    LEAVE("executeEndTransfer - out");
   }
   

void RnpRasDaManComm::executeGetNextMDD()
  {
    ENTER("executeGetNextMDD - in");
    RasServerEntry& rasserver = RasServerEntry::getInstance();
    
    
    r_Minterval  mddDomain;
    char*        typeName;
    char*        typeStructure;
    r_OId        oid;
    unsigned short  currentFormat;

    
    int status = rasserver.compat_getNextMDD(mddDomain, typeName, typeStructure, oid, currentFormat);
    
    encoder.startFragment(Rnp::fgt_OkAnswer, decoder.getCommand());
    encoder.addInt32Parameter(RnpRasserver::pmt_returnstatus,status);
    TALK( "adding return status " << status );
    encoder.addStringParameter(RnpRasserver::pmt_oidstring,     mddDomain.get_string_representation());
    encoder.addStringParameter(RnpRasserver::pmt_typename,      typeName);
    encoder.addStringParameter(RnpRasserver::pmt_typestructure, typeStructure);
    encoder.addStringParameter(RnpRasserver::pmt_oidstring, oid.get_string_representation() ? oid.get_string_representation() : "");
    encoder.addInt32Parameter(RnpRasserver::pmt_currentformat,  currentFormat);
    encoder.endFragment();	
    
    free(typeName);
    free(typeStructure);

   LEAVE("executeGetNextMDD - out");
   }

void RnpRasDaManComm::executeGetNextTile()
  {
    ENTER("executeGetNextTile - in");
    RasServerEntry& rasserver = RasServerEntry::getInstance();
    
    RPCMarray *tempRpcMarray;
    
    int status = rasserver.compat_getNextTile(&tempRpcMarray);
    
    encoder.startFragment(Rnp::fgt_OkAnswer, decoder.getCommand());
    encoder.addInt32Parameter(RnpRasserver::pmt_returnstatus,status);
    TALK( "adding return status " << status );
    
    if(tempRpcMarray != 0)
      { 
        encoder.addStringParameter(RnpRasserver::pmt_domain,        tempRpcMarray->domain);
        encoder.addInt32Parameter( RnpRasserver::pmt_typelength,    tempRpcMarray->cellTypeLength);
        encoder.addInt32Parameter( RnpRasserver::pmt_currentformat, tempRpcMarray->currentFormat);
        encoder.addInt32Parameter( RnpRasserver::pmt_storageformat, tempRpcMarray->storageFormat);
	
	encoder.adjustBufferSize(tempRpcMarray->data.confarray_len);
        encoder.addOpaqueParameter(RnpRasserver::pmt_tiledata,      tempRpcMarray->data.confarray_val, tempRpcMarray->data.confarray_len);
       
       // Do not free this! "tempRpcMarray->data.confarray_val";    
       free(tempRpcMarray->domain);
       free(tempRpcMarray);

       }	
    encoder.endFragment();	
    
    /* Notez aici ca n-am unde: e adevarat ca tilele trebuie transferate si pe bucati, fiindca 
       un tiff mare creat cu select e o tila!
    */
    

    LEAVE("executeGetNextTile - out");
   }

//----------

void RnpRasDaManComm::executeUpdateQuery()
  {
    ENTER("executeUpdateQuery - in");
    RasServerEntry& rasserver = RasServerEntry::getInstance(); 
    
    decoder.getNextParameter(); const char* query   = decoder.getDataAsString();

    ExecuteUpdateRes returnStructure;
    returnStructure.token = NULL;
    
    int status = rasserver.compat_ExecuteUpdateQuery(query, returnStructure);
  
    const char *token = returnStructure.token != NULL ? returnStructure.token : "";
    
    encoder.startFragment(Rnp::fgt_OkAnswer, decoder.getCommand());
    encoder.addInt32Parameter( RnpRasserver::pmt_returnstatus, status);
    TALK( "adding return status " << status );
    encoder.addInt32Parameter( RnpRasserver::pmt_errorno,     returnStructure.errorNo);
    encoder.addInt32Parameter( RnpRasserver::pmt_lineno,      returnStructure.lineNo);
    encoder.addInt32Parameter( RnpRasserver::pmt_columnno,    returnStructure.columnNo);
    encoder.addStringParameter(RnpRasserver::pmt_errortoken,  token );
    encoder.endFragment();
    
    if(returnStructure.token) free(returnStructure.token);
    
    LEAVE("executeUpdateQuery - out");
   }

void RnpRasDaManComm::executeInitUpdate()
  {
    ENTER("executeInitUpdate - in");
  
    RasServerEntry& rasserver = RasServerEntry::getInstance(); 
    
    int status = rasserver.compat_InitUpdate();
 
    encoder.startFragment(Rnp::fgt_OkAnswer, decoder.getCommand());
    encoder.addInt32Parameter(RnpRasserver::pmt_returnstatus, status);
    TALK( "adding return status " << status );
    encoder.endFragment();     
    LEAVE("executeInitUpdate - out");
   }
   
void RnpRasDaManComm::executeStartInsertTransMDD()
  {
    ENTER("executeStartInsertTransMDD - in");
  
    RasServerEntry& rasserver = RasServerEntry::getInstance(); 
    
    decoder.getNextParameter(); const char* domain   = decoder.getDataAsString();
    decoder.getNextParameter(); int typeLength       = decoder.getDataAsInteger();
    decoder.getNextParameter(); const char* typeName = decoder.getDataAsString();
    
    int status = rasserver.compat_StartInsertTransMDD(domain, typeLength, typeName);
 
    encoder.startFragment(Rnp::fgt_OkAnswer, decoder.getCommand());
    encoder.addInt32Parameter(RnpRasserver::pmt_returnstatus, status);
    TALK( "adding return status " << status );
    encoder.endFragment();     
    
    LEAVE("executeStartInsertTransMDD - out");
   }
   
void RnpRasDaManComm::executeInsertTile()
  {
    ENTER("executeInsertTile - in");
  
    RasServerEntry& rasserver = RasServerEntry::getInstance(); 
    
    RPCMarray *rpcMarray = new RPCMarray;
    
    decoder.getNextParameter(); int persistent   = decoder.getDataAsInteger();
    decoder.getNextParameter(); rpcMarray->domain = (char*)decoder.getDataAsString();
    decoder.getNextParameter(); rpcMarray->cellTypeLength = decoder.getDataAsInteger();
    decoder.getNextParameter(); rpcMarray->currentFormat  = decoder.getDataAsInteger();
    decoder.getNextParameter(); rpcMarray->storageFormat  = decoder.getDataAsInteger();
    
    decoder.getNextParameter();
      const void* buffer = decoder.getData();
      int         length = decoder.getDataLength();
      
    rpcMarray->data.confarray_val = (char*)mymalloc(length); memcpy(rpcMarray->data.confarray_val, buffer, length);
    rpcMarray->data.confarray_len = length;

    int status = rasserver.compat_InsertTile(persistent, rpcMarray);
    
    encoder.startFragment(Rnp::fgt_OkAnswer, decoder.getCommand());
    encoder.addInt32Parameter(RnpRasserver::pmt_returnstatus, status);
    TALK( "adding return status " << status );
    encoder.endFragment();
    
    // rpcMarray->data.confarray_val is freed by Tile::Tile(...), which is stupid, but...
    delete rpcMarray;
    
    LEAVE("executeInsertTile - out");
   }
   
void RnpRasDaManComm::executeEndInsertMDD()
  {
    ENTER("executeEndInsertMDD - in");
  
    RasServerEntry& rasserver = RasServerEntry::getInstance(); 
    
    decoder.getNextParameter(); int persistent  = decoder.getDataAsInteger();
    
    int status = rasserver.compat_EndInsertMDD(persistent);
    
    encoder.startFragment(Rnp::fgt_OkAnswer, decoder.getCommand());
    encoder.addInt32Parameter(RnpRasserver::pmt_returnstatus, status);
    TALK( "adding return status " << status );
    encoder.endFragment();       

   LEAVE("executeEndInsertMDD - out");
   }
   
void RnpRasDaManComm::executeGetTypeStructure()
  {
    ENTER("executeGetTypeStructure - in");
  
    RasServerEntry& rasserver = RasServerEntry::getInstance(); 
    
    decoder.getNextParameter(); const char *typeName = decoder.getDataAsString();
    decoder.getNextParameter(); int         typeType = decoder.getDataAsInteger();
    
    char *typeStructure=NULL;
    
    int status = rasserver.compat_GetTypeStructure(typeName, typeType, typeStructure);
    
    encoder.startFragment(Rnp::fgt_OkAnswer, decoder.getCommand());
    encoder.addInt32Parameter(RnpRasserver::pmt_returnstatus, status);
    TALK( "adding return status " << status );
    encoder.addStringParameter(RnpRasserver::pmt_typestructure, typeStructure ? typeStructure : "");
    encoder.endFragment(); 
    
    if(typeStructure) 
      { 
    	free(typeStructure);
       }	
    LEAVE("executeGetTypeStructure - out");
   }
   
void RnpRasDaManComm::executeStartInsertPersMDD()
  {
    ENTER("executeStartInsertPersMDD - in");
    RasServerEntry& rasserver = RasServerEntry::getInstance(); 
    
   decoder.getNextParameter();  const char* collName   = decoder.getDataAsString();
   decoder.getNextParameter();  r_Minterval mddDomain( decoder.getDataAsString() );
   decoder.getNextParameter();  int         typeLength = decoder.getDataAsInteger();
   decoder.getNextParameter();  const char* typeName   = decoder.getDataAsString();
   decoder.getNextParameter();  r_OId       oid( decoder.getDataAsString() );

   int status = rasserver.compat_StartInsertPersMDD(collName, mddDomain, typeLength, typeName, oid);

   encoder.startFragment(Rnp::fgt_OkAnswer, decoder.getCommand());
   encoder.addInt32Parameter(RnpRasserver::pmt_returnstatus, status);
    TALK( "adding return status " << status );
   encoder.endFragment(); 
   LEAVE("executeStartInsertPersMDD - out");
  }

void RnpRasDaManComm::executeInsertMDD()
  {
   ENTER("executeInsertMDD - in");
  
   RasServerEntry& rasserver = RasServerEntry::getInstance(); 
    
   decoder.getNextParameter();  const char* collName  = decoder.getDataAsString();
   decoder.getNextParameter();  const char* typeName  = decoder.getDataAsString();
   decoder.getNextParameter();  r_OId       oid( decoder.getDataAsString() );
 
    RPCMarray *rpcMarray = new RPCMarray;
    
    decoder.getNextParameter(); rpcMarray->domain = (char*)decoder.getDataAsString();
    decoder.getNextParameter(); rpcMarray->cellTypeLength = decoder.getDataAsInteger();
    decoder.getNextParameter(); rpcMarray->currentFormat  = decoder.getDataAsInteger();
    decoder.getNextParameter(); rpcMarray->storageFormat  = decoder.getDataAsInteger();
    
    decoder.getNextParameter();
      const void* buffer = decoder.getData();
      int         length = decoder.getDataLength();
      
    rpcMarray->data.confarray_val = (char*)mymalloc(length); memcpy(rpcMarray->data.confarray_val, buffer, length);
    rpcMarray->data.confarray_len = length;
 
    int status = rasserver.compat_InsertMDD(collName, rpcMarray, typeName, oid);
   
    encoder.startFragment(Rnp::fgt_OkAnswer, decoder.getCommand());
    encoder.addInt32Parameter(RnpRasserver::pmt_returnstatus, status);
    TALK( "adding return status " << status );
    encoder.endFragment(); 
  
    LEAVE("executeInsertMDD - out");
   }

void RnpRasDaManComm::executeInsertCollection()
  {
    ENTER("executeInsertCollection - in");
    RasServerEntry& rasserver = RasServerEntry::getInstance(); 
    
    decoder.getNextParameter();  const char* collName  = decoder.getDataAsString();
    decoder.getNextParameter();  const char* typeName  = decoder.getDataAsString();
    decoder.getNextParameter();  r_OId       oid( decoder.getDataAsString() );
    
    int status = rasserver.compat_InsertCollection(collName, typeName, oid);
   
    encoder.startFragment(Rnp::fgt_OkAnswer, decoder.getCommand());
    encoder.addInt32Parameter(RnpRasserver::pmt_returnstatus, status);
    TALK( "adding return status " << status );
    encoder.endFragment(); 
  
    LEAVE("executeInsertCollection - out");
   }
   
void RnpRasDaManComm::executeDeleteCollByName()
  {
    ENTER("executeDeleteCollByName - in");
    RasServerEntry& rasserver = RasServerEntry::getInstance(); 
    
    decoder.getNextParameter();  const char* collName  = decoder.getDataAsString();
  
    int status = rasserver.compat_DeleteCollByName(collName);
   
    encoder.startFragment(Rnp::fgt_OkAnswer, decoder.getCommand());
    encoder.addInt32Parameter(RnpRasserver::pmt_returnstatus, status);
    TALK( "adding return status " << status );
    encoder.endFragment(); 
  
    LEAVE("executeDeleteCollByName - out");
   }

void RnpRasDaManComm::executeDeleteObjByOId()
  {
    ENTER("executeDeleteObjByOId - in");
    RasServerEntry& rasserver = RasServerEntry::getInstance(); 
    
    decoder.getNextParameter();  r_OId       oid( decoder.getDataAsString() );
  
    int status = rasserver.compat_DeleteObjByOId(oid);
   
    encoder.startFragment(Rnp::fgt_OkAnswer, decoder.getCommand());
    encoder.addInt32Parameter(RnpRasserver::pmt_returnstatus, status);
    TALK( "adding return status " << status );
    encoder.endFragment(); 
  
    LEAVE("executeDeleteObjByOId - out");
   }

void RnpRasDaManComm::executeRemoveObjFromColl()
  {  
    ENTER("executeRemoveObjFromColl - in");
    RasServerEntry& rasserver = RasServerEntry::getInstance(); 
    
    decoder.getNextParameter();  const char* collName  = decoder.getDataAsString();
    decoder.getNextParameter();  r_OId  oid( decoder.getDataAsString() );
  
    int status = rasserver.compat_RemoveObjFromColl(collName, oid);
   
    encoder.startFragment(Rnp::fgt_OkAnswer, decoder.getCommand());
    encoder.addInt32Parameter(RnpRasserver::pmt_returnstatus, status);
    TALK( "adding return status " << status );
    encoder.endFragment(); 
  
    LEAVE("executeRemoveObjFromColl - out");
   }

void RnpRasDaManComm::executeGetCollection()
  {
    ENTER("executeGetCollection - in");
    RasServerEntry& rasserver = RasServerEntry::getInstance(); 
    
    char* typeName      = NULL;
    char* typeStructure = NULL;
    char* collName      = NULL;
    r_OId oid;
    int status = 0;
    
    decoder.getNextParameter(); 
    if(decoder.getParameterType() == RnpRasserver::pmt_collname)
      {
        collName = strdup(decoder.getDataAsString());
	status = rasserver.compat_GetCollectionByName(collName, typeName, typeStructure, oid);
       }
    else
      {
        const char* oidstring = decoder.getDataAsString();
	oid = r_OId(oidstring);
	status = rasserver.compat_GetCollectionByName(oid, typeName, typeStructure, collName);	
       } 
       
    encoder.startFragment(Rnp::fgt_OkAnswer, decoder.getCommand());
    encoder.addInt32Parameter(RnpRasserver::pmt_returnstatus, status);
    TALK( "adding return status " << status );
    encoder.addStringParameter(RnpRasserver::pmt_typename, typeName);
    encoder.addStringParameter(RnpRasserver::pmt_typestructure, typeStructure);
    encoder.addStringParameter(RnpRasserver::pmt_oidstring, oid.get_string_representation());
    encoder.addStringParameter(RnpRasserver::pmt_collname, collName);
    encoder.endFragment(); 
    
    free((void*)typeName);
    free((void*)typeStructure);
    free((void*)collName);
        
    LEAVE("executeGetCollection - out");
   }
   
void RnpRasDaManComm::executeGetCollectionOIds()
  {
    ENTER("executeGetCollectionOIds - in");
    RasServerEntry& rasserver = RasServerEntry::getInstance(); 
    
    char* typeName      = NULL;
    char* typeStructure = NULL;
    char* collName      = NULL;
    r_OId oid;
    RPCOIdEntry* oidTable     = NULL;
    unsigned int oidTableSize = 0;
    int status = 0;
    
    decoder.getNextParameter(); 
    if(decoder.getParameterType() == RnpRasserver::pmt_collname)
      {
        collName = strdup(decoder.getDataAsString());
	status = rasserver.compat_GetCollectionOidsByName(collName, typeName, typeStructure, oid, oidTable, oidTableSize);
       }
    else
      { oid = r_OId(decoder.getDataAsString());
	status = rasserver.compat_GetCollectionOidsByOId(oid, typeName, typeStructure, oidTable, oidTableSize, collName);
       }
    encoder.startFragment(Rnp::fgt_OkAnswer, decoder.getCommand());
    encoder.addInt32Parameter(RnpRasserver::pmt_returnstatus, status);
    TALK( "adding return status " << status );
    encoder.addStringParameter(RnpRasserver::pmt_typename, typeName);
    encoder.addStringParameter(RnpRasserver::pmt_typestructure, typeStructure);
    encoder.addStringParameter(RnpRasserver::pmt_oidstring, oid.get_string_representation());
    encoder.addStringParameter(RnpRasserver::pmt_collname, collName);

    if(oidTable)
     for(int i=0;i<oidTableSize; i++)
      {
        encoder.adjustBufferSize(strlen(oidTable[i].oid));
	encoder.addStringParameter(RnpRasserver::pmt_oidstring, oidTable[i].oid);
	free(oidTable[i].oid);
       }		
    encoder.endFragment(); 
    
    free((void*)typeName);
    free((void*)typeStructure);
    free((void*)collName);
    free(oidTable);
    
   LEAVE("executeGetCollectionOIds - out");
   }

void RnpRasDaManComm::executeGetObjectType()
  {
    ENTER("executeGetObjectType - in");
    RasServerEntry& rasserver = RasServerEntry::getInstance(); 
    
    decoder.getNextParameter(); const char* oidstring = decoder.getDataAsString();
    
    r_OId oid(oidstring);
    unsigned short objType;
    
    int status=rasserver.compat_GetObjectType(oid, objType);
    
    encoder.startFragment(Rnp::fgt_OkAnswer, decoder.getCommand());
    encoder.addInt32Parameter(RnpRasserver::pmt_returnstatus, status);
    TALK( "adding return status " << status );
    encoder.addInt32Parameter(RnpRasserver::pmt_objecttype, objType);
    encoder.endFragment(); 
    
    
    LEAVE("executeGetObjectType - out");
   }

void RnpRasDaManComm::executeSetFormat()
  {
    ENTER("executeSetFormat - in");
    RasServerEntry& rasserver = RasServerEntry::getInstance(); 
    
    decoder.getNextParameter(); int whichFormat    = decoder.getDataAsInteger();
    decoder.getNextParameter(); int format         = decoder.getDataAsInteger();
    decoder.getNextParameter(); const char* params = decoder.getDataAsString();

    int status = 0;
    
    if(whichFormat == 1) 
       status = rasserver.compat_SetTransferFormat(format, params);
    else
       status = rasserver.compat_SetStorageFormat(format, params);
       
    encoder.startFragment(Rnp::fgt_OkAnswer, decoder.getCommand());
    encoder.addInt32Parameter(RnpRasserver::pmt_returnstatus, status);
    TALK( "adding return status " << status );
    encoder.endFragment(); 
       
    LEAVE("executeSetFormat - out");
   }
   
//########### until here the compatible ones ###############

void RnpRasDaManComm::executeCreateCollection()
  {
    ENTER("executeCreateCollection - in");
    RasServerEntry& rasserver = RasServerEntry::getInstance(); 
    
    const char* collName = getNextAsString(RnpRasserver::pmt_collname);
    const char* collTypeName = getNextAsString(RnpRasserver::pmt_typename);
    
    TALK("rasserver.createCollection( " << collName << ", " << collTypeName << " )" );
    r_OId roid = rasserver.createCollection(collName, collTypeName);
    
    encoder.startFragment(Rnp::fgt_OkAnswer, decoder.getCommand());
    encoder.addStringParameter(RnpRasserver::pmt_oidstring, roid.get_string_representation());
    encoder.endFragment(); 
    
    LEAVE("executeCreateCollection - out");
   }

void RnpRasDaManComm::executeCreateMDD()
{
	ENTER( "RnpRasDaManComm::executeCreateMDD" );
	RasServerEntry& rasserver = RasServerEntry::getInstance(); 
    
	const char *collName         = getNextAsString(RnpRasserver::pmt_collname);
	const char *mddTypeName      = getNextAsString(RnpRasserver::pmt_typename);
	const char *definitionDomain = getNextAsString(RnpRasserver::pmt_domain);
	bool rcindex = false;
	const char *tileDomain = 0;
    
	if(decoder.getNextParameter()) 
	{
		rcindex = decoder.getDataAsInteger() ? true : false;
		tileDomain = getNextAsString(RnpRasserver::pmt_domain);
	}
	TALK( "collName=" << collName << ", mddTypeName=" << mddTypeName << ", definitionDomain=" << definitionDomain << ", tileDomain=" << tileDomain << ", rcindex=" << rcindex );
	r_OId roid = rasserver.createMDD(collName, mddTypeName, definitionDomain, tileDomain, rcindex);

	encoder.startFragment(Rnp::fgt_OkAnswer, decoder.getCommand());
	encoder.addStringParameter(RnpRasserver::pmt_oidstring, roid.get_string_representation());
	encoder.endFragment(); 
    
	LEAVE( "RnpRasDaManComm::executeCreateMDD, oid=" << roid.get_string_representation() );
}

void RnpRasDaManComm::executeExtendMDD()
{
	ENTER( "RnpRasDaManComm::executeExtendMDD" );

	RasServerEntry& rasserver = RasServerEntry::getInstance(); 
    
	const char *oidstring    = getNextAsString(RnpRasserver::pmt_oidstring);
	const char *stripeDomain = getNextAsString(RnpRasserver::pmt_domain);
	const char *tileDomain   = getNextAsString(RnpRasserver::pmt_domain);

	r_OId mddOId = r_OId(oidstring);

	TALK( "mddOId=" << oidstring << ", stripeDomain=" << stripeDomain << ", tileDomain=" << tileDomain );
	rasserver.extendMDD(mddOId, stripeDomain, tileDomain);
    
	encoder.startFragment(Rnp::fgt_OkAnswer, decoder.getCommand());
	encoder.endFragment(); 
    
	LEAVE( "RnpRasDaManComm::executeExtendMDD" );
}

void RnpRasDaManComm::executeGetTileDomains()
{
	ENTER( "RnpRasDaManComm::executeGetTileDomains" );

	RasServerEntry& rasserver = RasServerEntry::getInstance(); 
    
	const char *oidstring    = getNextAsString(RnpRasserver::pmt_oidstring);
	const char *stripeDomain = getNextAsString(RnpRasserver::pmt_domain);
    
	r_OId mddOId = r_OId(oidstring);
    
	vector<r_Minterval> result = rasserver.getTileDomains(mddOId, stripeDomain);
    
	encoder.startFragment(Rnp::fgt_OkAnswer, decoder.getCommand());
    
	for(int i=0;i < result.size(); i++)
	{
		const char *domain = result[i].get_string_representation();
		encoder.addStringParameter(RnpRasserver::pmt_domain, domain);
	
		free((void*)domain);
	}
    
	encoder.endFragment(); 

	LEAVE( "RnpRasDaManComm::executeGetTileDomains" );
}

//######### helper functions ###########################

void RnpRasDaManComm::connectClient()
  {
    clientID = makeNewClientID();    
    TALK( "RnpRasDaManComm::connectClient(): assigned new client id 0x" << hex << clientID << dec );
   }

void RnpRasDaManComm::disconnectInternally()
{
    clientID = NoClient;
    requestCounter = 1;		// because pre-increment before request processing will not be reached when this is called
    fragmentCounter = 1;	// same phenomenon, different reason: verify needs this counter for OK'ing connect
}

void RnpRasDaManComm::disconnectClient()
  { 
    clientID = NoClient;
    requestCounter = 0;
    fragmentCounter = 0;
    rasmgrComm.informRasmgrServerAvailable();
   }
      
      
void RnpRasDaManComm::verifyClientID( RnpQuark command ) throw (r_Error)
  {
    ENTER( "RnpRasDaManComm::verifyClientID( command=" << command << " ), fragmentCounter=" << fragmentCounter << ", requestCounter=" << requestCounter );

    decoder.getFirstParameter();

    if(decoder.getParameterType() != RnpRasserver::pmt_clientid)
         {
	   RMInit::logOut << "Error: unidentified client." << endl;
           LEAVE( "RnpRasDaManComm::verifyClientID() - exception, unknown client id." );
	   throw r_Error(820); // sorry, I know, symbolic constants
          }
	  
    int verClientID = decoder.getDataAsInteger();
	TALK( "RnpRasDaManComm::verifyClientID: clientID 0x" << hex << clientID << dec << ", verClientID 0x" << hex << verClientID << dec );
    
    // it's our client, it's OK
    if(clientID == verClientID)
    {
        LEAVE( "RnpRasDaManComm::verifyClientID() - it's our client, it's OK" );
	return;
    }
    
    // connect cmd is OK too
    if(command == RnpRasserver::cmd_connect)
    {
         LEAVE( "RnpRasDaManComm::verifyClientID() - connect requested, OK" );
	 return;
    }
    
    // new client, first request, it's probably connect, so OK
    if(clientID == NoClient && fragmentCounter == 1)
    {
         LEAVE( "RnpRasDaManComm::verifyClientID() - new client, first request, it's probably connect, so OK" );
	 return;
    }
    
    // new client, same message, a new request, it's also OK (he is allowed to put more fragments in a request!)
    if(clientID != NoClient && fragmentCounter > 1 && requestCounter == 1 && verClientID == 0)
    {
         LEAVE( "RnpRasDaManComm::verifyClientID() - new client, same message, a new request (multi-fragment), so OK" );
	return;
    }
    
    RMInit::logOut << "Error: unregistered client." << endl;
    LEAVE("RnpRasDaManComm::verifyClientID(): stored clientID is 0x" << hex << clientID << dec << ", but client identified as 0x" << hex << verClientID << dec << ", fragmentCounter=" << fragmentCounter << ", requestCounter=" << requestCounter);
    throw r_Error(821);		// invalid sequence number
   }
   
int  RnpRasDaManComm::makeNewClientID()
  {
  
    // CLIENTID: 	| 0|.counter.|....timestamp ......|
    //			|31,30.....24|23...16|15...8|7...0|
    static int counter = 0;
    
    int timeNow = time(NULL);
    
    int result = (timeNow & 0xFFFFFF) + (counter << 24);
    
    counter = (counter+1) & 0x7F;
    
    TALK( "RnpRasDaManComm::makeNewClientID() -> 0x" << hex << result << " (counter now: " << counter << ")" );
    return  result;
   }

void RnpRasDaManComm::answerr_Error(r_Error &err)
{
	const char *errText = err.serialiseError();
	
	RMInit::logOut << "Error in response: (" << errText << ") " << err.what() << endl;

	encoder.startFragment(Rnp::fgt_Error, decoder.getCommand()); 
	encoder.addInt32Parameter(Rnp::ert_Other, 0);
	encoder.addStringParameter(RnpRasserver::pmt_rErrorString, errText);

	// add descriptive text -- PB 2003-nov-24
	encoder.addStringParameter(RnpRasserver::pmt_rErrorString, err.what() );

	encoder.endFragment();  
	
	delete[] errText;
}

//######################################################
RnpRasserverJob::RnpRasserverJob() throw()
  {
    TALK( "RNP: RnpRasserverJob created" );
   }
      
bool RnpRasserverJob::validateMessage() throw()
  {
    TALK( "RNP: validateMessage()" );
    return RnpServerJob::validateMessage();
   }
   
void RnpRasserverJob::executeOnAccept() throw()
  {
    TALK( "RNP: executeOnAccept()" );
    RnpServerJob::executeOnAccept();
   }
   
void RnpRasserverJob::executeOnWriteReady() throw()
  {
    TALK( "RNP: executeOnWriteReady()" );
    RnpServerJob::executeOnWriteReady();
   }
   
void RnpRasserverJob::specificCleanUpOnTimeout() throw()
  {
    TALK( "RNP: specificCleanUpOnTimeout()" );
    RnpServerJob::specificCleanUpOnTimeout();
   }
   
void RnpRasserverJob::executeOnReadError() throw()
  {
    RMInit::logOut << "Error while executing read operation." << endl;
    RnpServerJob::executeOnReadError();
   }
   
void RnpRasserverJob::executeOnWriteError() throw()
  {
    RMInit::logOut << "Error while executing write operation." << endl;
    RnpServerJob::executeOnWriteError();
   }

//#########################################################
RasserverCommunicator::RasserverCommunicator(RnpRasDaManComm* cmm) throw()
  {
    commPtr = cmm;
   }
       
bool RasserverCommunicator::executeOnTimeout() throw()
  {
   TALK( "RasserverCommunicator::executeOnTimeout()" );
    
    commPtr->checkForTimeout(); 
    
    return true;
   }

//#########################################################
ClientTimer::ClientTimer()
  {
    interval = 0;
    lastAction = 0;
    enabled = false;
   }
   
void ClientTimer::setTimeoutInterval(int seconds)
  {
    interval = seconds;
    enabled = true;
    markAction();
   }
   
void ClientTimer::markAction()
  {
    lastAction = time(NULL);
   }
   
bool ClientTimer::checkForTimeout()
  {
    if(enabled == false) return false;
    
    time_t now = time(NULL);
    
    return now >= lastAction + interval;
   }

