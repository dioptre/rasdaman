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
 * SOURCE: rpcclientcomm.cc
 *
 * MODULE: clientcomm
 * CLASS:  RpcClientComm
 *
 * PURPOSE:
 *
 * COMMENTS:
 *          None
*/

#include "mymalloc/mymalloc.h"

static const char rcsid[] = "@(#)clientcomm, RpcClientComm: $Id: rpcclientcomm.cc,v 1.11 2005/09/09 16:16:29 rasdev Exp $";

#include <openssl/evp.h>

#include <stdio.h>
#include <math.h>      // for ceil(), log(), exp()

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

#ifdef __VISUALC__
#include <windows.h>   // for the required defines and stuff
#include <mmsystem.h>  // for setEventTimer and killEventTimer
#include <winbase.h>
#else

#if defined(DECALPHA) || defined(LINUX)

#if !defined(_XOPEN_SOURCE_EXTENDED)
#define _XOPEN_SOURCE_EXTENDED // for gethostid
#endif

#endif

#include <unistd.h>    // for sleep(), alarm()
#include <signal.h>    // for signal()
#endif
#ifdef __VISUALC__  // do this ONLY for VisualC! Not for EARLY_TEMPLATE
#define __EXECUTABLE__
#endif

#if (defined(__VISUALC__) || defined(CYGWIN))
extern "C"
{
#include "clientcomm/clnt_control/clnt_control.h"
}
# define XDRFREE(proc, res) xdrfree( ntxdr_##proc, (char*)res )
#else
# define XDRFREE(proc, res) xdr_free( (xdrproc_t)xdr_##proc, (char*)res )
#endif

#include "rasodmg/transaction.hh"
#include "rasodmg/database.hh"
#include "rasodmg/iterator.hh"
#include "rasodmg/set.hh"
#include "rasodmg/ref.hh"
#include "rasodmg/storagelayout.hh"
#include "rasodmg/tiling.hh"

#include "raslib/minterval.hh"
#include "raslib/rmdebug.hh"
#include "raslib/rminit.hh"
#include "raslib/primitivetype.hh"
#include "raslib/complextype.hh"
#include "raslib/structuretype.hh"
#include "raslib/primitive.hh"
#include "raslib/complex.hh"
#include "raslib/structure.hh"
#include "raslib/endian.hh"
#include "raslib/parseparams.hh"

#include "clientcomm/rpcclientcomm.hh"


#ifdef __VISUALC__  // do this ONLY for VisualC! Not for EARLY_TEMPLATE
#undef __EXECUTABLE__
#endif

RMINITGLOBALS('C')

#include<stdio.h>
#include<errno.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<iostream>
#include<string.h>

#define  ALIVEINTERVAL      60
#define  TAWRITEWAITINTERVAL    10

#ifdef SOLARIS
extern "C" void  aliveSignal( int );
#endif
#ifdef __VISUALC__
void  CALLBACK TimerProc(UINT wTimerID, UINT wMsg, DWORD dwUser, DWORD dw1, DWORD dw2)
#else
void  aliveSignal( int )
#endif
{
    if(!RMInit::noTimeOut)
    {
        // get the current clientcomm object
        ClientComm* myComm = r_Database::actual_database->getComm();
        if( myComm == 0 )
        {
            RMInit::logOut << "RpcClientComm: Error: RpcClientComm object only usable within r_Database object." << endl;
            return;
        }

        myComm->triggerAliveSignal();
    }
}


RpcClientComm::RpcClientComm( const char* _rasmgrHost, int _rasmgrPort ) throw( r_Error )
    :       binding_h(NULL),
            clientID(0),
#ifdef __VISUALC__
            UINT timerid(0),
#endif
            status(0),
            serverUp(0),
            rpcActive(0),
            aliveSignalRemaining(0),
            endianServer(0),
            endianClient(0),
            serverRPCversion(0),
            transferFormat(r_Array),
            storageFormat(r_Array),
            transferFormatParams(NULL),
            storageFormatParams(NULL),
            clientParams(NULL),
            serverCompresses(0),
            exactFormat(0),
            RPCIF_PARA(0)
{
    clientParams = new r_Parse_Params();
    clientParams->add("compserver", &serverCompresses, r_Parse_Params::param_type_int);
    clientParams->add("exactformat", &exactFormat, r_Parse_Params::param_type_int);

    endianClient = (int)r_Endian::get_endianness();

    this->rasmgrHost=(char*)_rasmgrHost;
    this->rasmgrPort=_rasmgrPort;
    serverHost[0]=0;
    capability[0]=0;
    strcpy(identificationString,"rasguest:8e70a429be359b6dace8b5b2500dedb0"); // this is MD5("rasguest");
}

static int rpcRetryCounter = 0;

RpcClientComm::~RpcClientComm() throw()
{
    disconnectFromServer();
    delete clientParams;
    clientParams = NULL;
}

bool RpcClientComm::effectivTypeIsRNP() throw()
{
    return false;
}

void
RpcClientComm::executeQuery( const r_OQL_Query& query, r_Set< r_Ref_Any >& result )
throw( r_Error )
{
    if(binding_h == NULL)
    {
        RMInit::logOut << endl << "RpcClientComm::executeQuery(query, result) ERROR: CONNECTION TO SERVER ALREADY CLOSED" << endl << endl;
        throw r_Error(CONNECTIONCLOSED);
    }

    char* queryString;

    // Get the query string by using a backdoor function of r_OQL_Query
    queryString = (char*)query.get_query();

    // Finally, this is the remote procedure which sends the query and receives a
    // client Id under which the client can access the r_Marrays he is to receive
    ExecuteQueryParams* params = new ExecuteQueryParams;
    ExecuteQueryRes*    res;
    params->clientID = clientID;
    params->query = queryString;

    setRPCActive();
    rpcRetryCounter = 0;
    do
    {
        res = rpcexecutequery_1( params, binding_h );

        if( !res )
        {
            RMInit::logOut << endl << "WARNING: RPC NULL POINTER (rpcexecutequery_1)" << endl << endl;
            sleep(RMInit::clientcommSleep);
        }
        if (rpcRetryCounter > RMInit::clientcommMaxRetry)
        {
            RMInit::logOut << "RPC call 'rpcexecutequery' failed" << endl;
            throw r_Error(CLIENTCOMMUICATIONFAILURE);
        }
        rpcRetryCounter++;
    }
    while( res == 0 );
    setRPCInactive();

    delete params;

    if( res->status == 0 )
    {
        result.set_type_by_name( res->typeName );
        result.set_type_structure( res->typeStructure );

        XDRFREE(ExecuteQueryRes, res);

        getMDDCollection( result, 1 );
    }
    else if( res->status == 1 )
    {
        result.set_type_by_name( res->typeName );
        result.set_type_structure( res->typeStructure );

        XDRFREE(ExecuteQueryRes, res);

        getElementCollection( result );
    }
    else if (res->status == 2)
    {
        // Result collection is empty and nothing has to be got.
        XDRFREE(ExecuteQueryRes, res);
    }
    else if( res->status == 4 || res->status == 5 )
    {
        r_Equery_execution_failed err( res->errorNo, res->lineNo, res->columnNo, res->token );
        XDRFREE(ExecuteQueryRes, res);
        throw err;
    }
    else
    {
        r_Error err;

        if( res->status == 3 )
            err = r_Error( r_Error::r_Error_ClientUnknown );
        else
            err = r_Error( r_Error::r_Error_TransferFailed );

        XDRFREE(ExecuteQueryRes, res);

        throw err;
    }
}



void
RpcClientComm::executeQuery( const r_OQL_Query& query )
throw( r_Error )
{
    RMDBGENTER(2, RMDebug::module_clientcomm, "RpcClientComm", "executeQuery(query)")

    if(binding_h == NULL)
    {
        RMInit::logOut << endl << "RpcClientComm::executeQuery(query) ERROR: CONNECTION TO SERVER ALREADY CLOSED" << endl << endl;
        throw r_Error(CONNECTIONCLOSED);
    }

    // update -> check for read_only transaction
    if( r_Transaction::actual_transaction == 0 ||
            r_Transaction::actual_transaction->get_mode() == r_Transaction::read_only )
    {
        r_Error err = r_Error( r_Error::r_Error_TransactionReadOnly );
        throw err;
    }

    unsigned short  rpcStatus;
    unsigned short* rpcStatusPtr = 0;

    //
    // Send MDD constants to the server.
    //
    if( query.get_constants() )
    {
        r_Set< r_GMarray* >* mddConstants = (r_Set< r_GMarray* >*)query.get_constants();

        setRPCActive();
        rpcRetryCounter = 0;
        do
        {
            rpcStatusPtr = rpcinitexecuteupdate_1( &clientID, binding_h );

            if( !rpcStatusPtr )
            {
                RMInit::logOut << endl << "WARNING: RPC NULL POINTER (rpcinitexecuteupdate_1)" << endl << endl;
                sleep(RMInit::clientcommSleep);
            }
            if (rpcRetryCounter > RMInit::clientcommMaxRetry)
            {
                RMInit::logOut << "RPC call 'rpcexecuteupdate' failed" << endl;
                throw r_Error(CLIENTCOMMUICATIONFAILURE);
            }
            rpcRetryCounter++;
        }
        while( rpcStatusPtr == 0 );
        rpcStatus = *rpcStatusPtr;
        setRPCInactive();

        r_Iterator<r_GMarray*> iter = mddConstants->create_iterator();

        for( iter.reset(); iter.not_done(); iter++ )
        {
            r_GMarray* mdd = *iter;
            const r_Base_Type* baseType = mdd->get_base_type_schema();

            if( mdd )
            {
                // initiate composition of MDD at server side
                InsertTransMDDParams* params = new InsertTransMDDParams;
                params->clientID   = clientID;
                params->collName   = strdup(""); // not used
                params->domain     = mdd->spatial_domain().get_string_representation();
                params->typeLength = mdd->get_type_length();
                params->typeName   = (char*)mdd->get_type_name();

                if(binding_h == NULL)
                {
                    RMInit::logOut << endl << "RpcClientComm::executeQuery(query) ERROR: CONNECTION TO SERVER ALREADY CLOSED" << endl << endl;
                    throw r_Error(CONNECTIONCLOSED);
                }

                setRPCActive();
                rpcRetryCounter = 0;
                do
                {
                    rpcStatusPtr = rpcstartinserttransmdd_1( params, binding_h );

                    if( !rpcStatusPtr )
                    {
                        RMInit::logOut << endl << "WARNING: RPC NULL POINTER (rpcinserttransmdd_1)" << endl << endl;
                        sleep(RMInit::clientcommSleep);
                    }
                    if (rpcRetryCounter > RMInit::clientcommMaxRetry)
                    {
                        RMInit::logOut << "RPC call 'rpcexecutequery' failed" << endl;
                        throw r_Error(CLIENTCOMMUICATIONFAILURE);
                    }
                    rpcRetryCounter++;
                }
                while( rpcStatusPtr == 0 );
                rpcStatus = *rpcStatusPtr;
                setRPCInactive();

                free( params->domain );
                free( params->collName );
                delete params;

                if( rpcStatus > 0 )
                {
                    r_Error err;

                    switch( rpcStatus )
                    {
                    case 2:
                        err = r_Error( r_Error::r_Error_DatabaseClassUndefined );
                        break;
                    case 3:
                        err = r_Error( r_Error::r_Error_TypeInvalid );
                        break;
                    default:
                        err = r_Error( r_Error::r_Error_TransferFailed );
                    }
                    RMInit::logOut << "Error: rpcinitmdd() - " << err.what() << endl;
                    RMDBGEXIT(2, RMDebug::module_clientcomm, "RpcClientComm", "executeQuery(query) error occured")
                    throw err;
                }

                r_Set< r_GMarray* >* bagOfTiles;


                bagOfTiles = mdd->get_storage_layout()->decomposeMDD( mdd );

                RMDBGMIDDLE(2, RMDebug::module_clientcomm, "RpcClientComm", "decomposing into " << bagOfTiles->cardinality() << " tiles");

                r_Iterator< r_GMarray* > iter2 = bagOfTiles->create_iterator();
                r_GMarray *origTile;
                iter2.reset();

                while( iter2.not_done() )
                {
                    RPCMarray* rpcMarray;

                    origTile = *iter2;

                    // advance iter here to determine if this is the last call (not_done())
                    iter2.advance();

                    RMDBGMIDDLE(2, RMDebug::module_clientcomm, "RpcClientComm", "inserting Tile with domain " << origTile->spatial_domain() << ", " << origTile->spatial_domain().cell_count() * origTile->get_type_length() << " bytes")

                    getMarRpcRepresentation( origTile, rpcMarray, mdd->get_storage_layout()->get_storage_format(), baseType );

                    InsertTileParams* params2      = new InsertTileParams;
                    params2->clientID              = clientID;
                    params2->isPersistent          = 0;
                    params2->marray                = rpcMarray;

                    if(binding_h == NULL)
                    {
                        RMInit::logOut << endl << "RpcClientComm::executeQuery(query) ERROR: CONNECTION TO SERVER ALREADY CLOSED" << endl << endl;
                        throw r_Error(CONNECTIONCLOSED);
                    }

                    setRPCActive();
                    rpcRetryCounter = 0;
                    do
                    {
                        rpcStatusPtr = rpcinserttile_1( params2, binding_h );

                        if( !rpcStatusPtr )
                        {
                            RMInit::logOut << endl << "WARNING: RPC NULL POINTER (rpcinserttile_1)" << endl << endl;
                            sleep(RMInit::clientcommSleep);
                        }
                        if (rpcRetryCounter > RMInit::clientcommMaxRetry)
                        {
                            RMInit::logOut << "RPC call 'rpcinserttile' failed" << endl;
                            throw r_Error(CLIENTCOMMUICATIONFAILURE);
                        }
                        rpcRetryCounter++;
                    }
                    while( rpcStatusPtr == 0 );
                    rpcStatus = *rpcStatusPtr;
                    RMDBGIF(20, RMDebug::module_clientcomm, "WAITAFTERSENDTILE", \
                            RMInit::dbgOut << "Waiting 10 sec after send tile\n" << std::endl; \
                            sleep(10); \
                            RMInit::dbgOut << "Continue now\n" << std::endl; );
                    setRPCInactive();

                    // free rpcMarray structure (rpcMarray->data.confarray_val is freed somewhere else)
                    freeMarRpcRepresentation( origTile, rpcMarray );
                    delete params2;

                    // delete current tile (including data block)
                    delete origTile;

                    if( rpcStatus > 0 )
                    {
                        RMInit::logOut << "Error: rpctransfertile() - general" << endl;
                        RMDBGEXIT(2, RMDebug::module_clientcomm, "RpcClientComm", "executeQuery() error occured")
                        throw r_Error( r_Error::r_Error_TransferFailed );
                    }

                    RMDBGMIDDLE(2, RMDebug::module_clientcomm, "RpcClientComm", "OK")
                }

                EndInsertMDDParams* params3 = new EndInsertMDDParams;
                params3->clientID     = clientID;
                params3->isPersistent = 0;

                if(binding_h == NULL)
                {
                    RMInit::logOut << endl << "RpcClientComm::executeQuery(query) ERROR: CONNECTION TO SERVER ALREADY CLOSED" << endl << endl;
                    throw r_Error(CONNECTIONCLOSED);
                }

                setRPCActive();
                rpcRetryCounter = 0;
                do
                {
                    rpcStatusPtr = rpcendinsertmdd_1( params3, binding_h );

                    if( !rpcStatusPtr )
                    {
                        RMInit::logOut << endl << "WARNING: RPC NULL POINTER (rpcendinsertmdd_1)" << endl << endl;
                        sleep(RMInit::clientcommSleep);
                    }
                    if (rpcRetryCounter > RMInit::clientcommMaxRetry)
                    {
                        RMInit::logOut << "RPC call 'rpcinsertmdd' failed" << endl;
                        throw r_Error(CLIENTCOMMUICATIONFAILURE);
                    }
                    rpcRetryCounter++;
                }
                while( rpcStatusPtr == 0 );
                rpcStatus = *rpcStatusPtr;
                setRPCInactive();

                delete params3;

                // delete transient data
                bagOfTiles->remove_all();
                delete bagOfTiles;
            }
        }
    }

    //
    // Send the update query.
    //
    ExecuteQueryParams* params = new ExecuteQueryParams;
    ExecuteUpdateRes*   res;
    params->clientID = clientID;
    params->query    = (char*)query.get_query();

    if(binding_h == NULL)
    {
        RMInit::logOut << endl << "RpcClientComm::executeQuery(query) ERROR: CONNECTION TO SERVER ALREADY CLOSED" << endl << endl;
        throw r_Error(CONNECTIONCLOSED);
    }

    setRPCActive();
    rpcRetryCounter = 0;
    do
    {
        res = rpcexecuteupdate_1( params, binding_h );

        if( !res )
        {
            RMInit::logOut << endl << "WARNING: RPC NULL POINTER (rpcexecuteupdate_1)" << endl << endl;
            sleep(RMInit::clientcommSleep);
        }
        if (rpcRetryCounter > RMInit::clientcommMaxRetry)
        {
            RMInit::logOut << "RPC call 'rpcexecuteupdate' failed" << endl;
            throw r_Error(CLIENTCOMMUICATIONFAILURE);
        }
        rpcRetryCounter++;
    }
    while( res == 0 );
    setRPCInactive();

    delete params;

    rpcStatus = res->status;

    if( rpcStatus == 2 || rpcStatus == 3 )
    {
        r_Equery_execution_failed err( res->errorNo, res->lineNo, res->columnNo, res->token );

        XDRFREE(ExecuteUpdateRes, res);

        RMDBGEXIT(2, RMDebug::module_clientcomm, "RpcClientComm", "executeQuery() error occured")
        throw err;
    }

    XDRFREE(ExecuteUpdateRes, res);

    if( rpcStatus == 1 || rpcStatus > 3 )
    {
        r_Error err;

        if( rpcStatus == 1 )
            err = r_Error( r_Error::r_Error_ClientUnknown );
        else
            err = r_Error( r_Error::r_Error_TransferFailed );

        RMDBGEXIT(2, RMDebug::module_clientcomm, "RpcClientComm", "executeQuery() error occured")
        throw err;
    }
    RMDBGEXIT(2, RMDebug::module_clientcomm, "RpcClientComm", "executeQuery()")
}



void
RpcClientComm::insertColl( const char* collName, const char* typeName, const r_OId& oid )
throw( r_Error )
{
    if(binding_h == NULL)
    {
        RMInit::logOut << endl << "RpcClientComm::insertColl(collName, typeName, oid ) ERROR: CONNECTION TO SERVER ALREADY CLOSED" << endl << endl;
        throw r_Error(CONNECTIONCLOSED);
    }

    // update -> check for read_only transaction
    if( r_Transaction::actual_transaction == 0 ||
            r_Transaction::actual_transaction->get_mode() == r_Transaction::read_only )
    {
        r_Error err = r_Error( r_Error::r_Error_TransactionReadOnly );
        throw err;
    }

    unsigned short  rpcStatus;
    unsigned short* rpcStatusPtr = 0;

    InsertCollParams* params = new InsertCollParams;
    params->clientID = clientID;
    params->collName = (char*)collName;
    params->typeName = (char*)typeName;
    params->oid      = (char*)oid.get_string_representation();

    setRPCActive();
    rpcRetryCounter = 0;
    do
    {
        rpcStatusPtr = rpcinsertcoll_1( params, binding_h );

        if( !rpcStatusPtr )
        {
            RMInit::logOut << endl << "WARNING: RPC NULL POINTER (rpcinsertcoll_1)" << endl << endl;
            sleep(RMInit::clientcommSleep);
        }
        if (rpcRetryCounter > RMInit::clientcommMaxRetry)
        {
            RMInit::logOut << "RPC call 'rpcinsertcoll' failed" << endl;
            throw r_Error(CLIENTCOMMUICATIONFAILURE);
        }
        rpcRetryCounter++;
    }
    while( rpcStatusPtr == 0 );
    rpcStatus = *rpcStatusPtr;
    setRPCInactive();

    delete params;

    if( rpcStatus > 0 )
    {
        r_Error err;

        switch( rpcStatus )
        {
        case 1:
            err = r_Error( r_Error::r_Error_ClientUnknown );
            break;
        case 2:
            err = r_Error( r_Error::r_Error_DatabaseClassUndefined );
            break;
        case 3:
            err = r_Error( r_Error::r_Error_NameNotUnique );
            break;
        default:
            err = r_Error( r_Error::r_Error_General );
        }
        RMInit::logOut << "Error: rpcCreateMDDCollection() - " << err.what() << endl;
        throw err;
    }
}



void
RpcClientComm::deleteCollByName( const char* collName )
throw( r_Error )
{
    if(binding_h == NULL)
    {
        RMInit::logOut << endl << "RpcClientComm::deleteCollByName(collName) ERROR: CONNECTION TO SERVER ALREADY CLOSED" << endl << endl;
        throw r_Error(CONNECTIONCLOSED);
    }

    // update -> check for read_only transaction
    if( r_Transaction::actual_transaction == 0 ||
            r_Transaction::actual_transaction->get_mode() == r_Transaction::read_only )
    {
        r_Error err = r_Error( r_Error::r_Error_TransactionReadOnly );
        throw err;
    }

    unsigned short  rpcStatus;
    unsigned short* rpcStatusPtr=0;

    NameSpecParams* params = new NameSpecParams;
    params->clientID = clientID;
    params->name     = (char*)collName;

    setRPCActive();
    rpcRetryCounter = 0;
    do
    {
        rpcStatusPtr = rpcdeletecollbyname_1( params, binding_h );

        if( !rpcStatusPtr )
        {
            RMInit::logOut << endl << "WARNING: RPC NULL POINTER (rpcdeletecollbyname_1)" << endl << endl;
            sleep(RMInit::clientcommSleep);
        }
        if (rpcRetryCounter > RMInit::clientcommMaxRetry)
        {
            RMInit::logOut << "RPC call 'rpcdeletecollbyname' failed" << endl;
            throw r_Error(CLIENTCOMMUICATIONFAILURE);
        }
        rpcRetryCounter++;
    }
    while( rpcStatusPtr == 0 );
    rpcStatus = *rpcStatusPtr;
    setRPCInactive();

    delete params;

    if( rpcStatus > 0 )
    {
        r_Error err;

        switch( rpcStatus )
        {
        case 1:
            err = r_Error( r_Error::r_Error_ClientUnknown );
            break;
        case 2:
            err = r_Error( r_Error::r_Error_ObjectUnknown );
            break;
        default:
            err = r_Error( r_Error::r_Error_General );
        }
        RMInit::logOut << "Error: rpcInsertMDD() - " << err.what() << endl;
        throw err;
    }
}



void
RpcClientComm::deleteObjByOId( const r_OId& oid )
throw( r_Error )
{
    if(binding_h == NULL)
    {
        RMInit::logOut << endl << "RpcClientComm::deleteObjectByOId(oid) ERROR: CONNECTION TO SERVER ALREADY CLOSED" << endl << endl;
        throw r_Error(CONNECTIONCLOSED);
    }

    // update -> check for read_only transaction
    if( r_Transaction::actual_transaction == 0 ||
            r_Transaction::actual_transaction->get_mode() == r_Transaction::read_only )
    {
        r_Error err = r_Error( r_Error::r_Error_TransactionReadOnly );
        throw err;
    }

    unsigned short  rpcStatus;
    unsigned short* rpcStatusPtr;

    OIdSpecParams* params = new OIdSpecParams;
    params->clientID = clientID;
    params->oid      = (char*) oid.get_string_representation();

    setRPCActive();
    rpcRetryCounter = 0;
    do
    {
        rpcStatusPtr = rpcdeleteobjbyoid_1( params, binding_h );

        if( !rpcStatusPtr )
        {
            RMInit::logOut << endl << "WARNING: RPC NULL POINTER (rpcdeleteobjbyoid_1)" << endl << endl;
            sleep(RMInit::clientcommSleep);
        }
        if (rpcRetryCounter > RMInit::clientcommMaxRetry)
        {
            RMInit::logOut << "RPC call 'rpcdeleteobjbyoid' failed" << endl;
            throw r_Error(CLIENTCOMMUICATIONFAILURE);
        }
        rpcRetryCounter++;
    }
    while( rpcStatusPtr == 0 );
    rpcStatus = *rpcStatusPtr;
    setRPCInactive();
    delete params;

    if( rpcStatus > 0 )
    {
        r_Error err;

        switch( rpcStatus )
        {
        case 1:
            err = r_Error( r_Error::r_Error_ClientUnknown );
            break;
        case 2:
            err = r_Error( r_Error::r_Error_ObjectUnknown );
            break;
        default:
            err = r_Error( r_Error::r_Error_General );
        }
        RMInit::logOut << "Error: rpcInsertMDD() - " << err.what() << endl;
        throw err;
    }
}



void
RpcClientComm::removeObjFromColl( const char* collName, const r_OId& oid )
throw( r_Error )
{
    if(binding_h == NULL)
    {
        RMInit::logOut << endl << "RpcClientComm::removeObjFromColl(collName, oid) ERROR: CONNECTION TO SERVER ALREADY CLOSED" << endl << endl;
        throw r_Error(CONNECTIONCLOSED);
    }

    // update -> check for read_only transaction
    if( r_Transaction::actual_transaction == 0 ||
            r_Transaction::actual_transaction->get_mode() == r_Transaction::read_only )
    {
        r_Error err = r_Error( r_Error::r_Error_TransactionReadOnly );
        throw err;
    }

    unsigned short  rpcStatus;
    unsigned short* rpcStatusPtr;

    RemoveObjFromCollParams* params = new RemoveObjFromCollParams;
    params->clientID = clientID;
    params->collName = (char*) collName;
    params->oid      = (char*) oid.get_string_representation();

    setRPCActive();
    rpcRetryCounter = 0;
    do
    {
        rpcStatusPtr = rpcremoveobjfromcoll_1( params, binding_h );

        if( !rpcStatusPtr )
        {
            RMInit::logOut << endl << "WARNING: RPC NULL POINTER (rpcremoveobjfromcoll_1)" << endl << endl;
            sleep(RMInit::clientcommSleep);
        }
        if (rpcRetryCounter > RMInit::clientcommMaxRetry)
        {
            RMInit::logOut << "RPC call 'rpcremoveobjfromcoll' failed" << endl;
            throw r_Error(CLIENTCOMMUICATIONFAILURE);
        }
        rpcRetryCounter++;
    }
    while( rpcStatusPtr == 0 );
    rpcStatus = *rpcStatusPtr;
    setRPCInactive();

    delete params;

    if( rpcStatus > 0 )
    {
        r_Error err;

        switch( rpcStatus )
        {
        case 1:
            err = r_Error( r_Error::r_Error_ClientUnknown );
            break;
        case 2:
        case 3:
            err = r_Error( r_Error::r_Error_ObjectUnknown );
            break;
        default:
            err = r_Error( r_Error::r_Error_General );
        }
        RMInit::logOut << "Error: rpcInsertMDD() - " << err.what() << endl;
        throw err;
    }
}



void
RpcClientComm::insertMDD( const char* collName, r_GMarray* mar )
throw( r_Error )
{
    RMDBGENTER(2, RMDebug::module_clientcomm, "RpcClientComm", "insertMDD(" << collName << ", marray)")

    if(binding_h == NULL)
    {
        RMInit::logOut << endl << "RpcClientComm::insertMDD(collName, mar) ERROR: CONNECTION TO SERVER ALREADY CLOSED" << endl << endl;
        throw r_Error(CONNECTIONCLOSED);
    }

    // update -> check for read_only transaction
    if( r_Transaction::actual_transaction == 0 ||
            r_Transaction::actual_transaction->get_mode() == r_Transaction::read_only )
    {
        r_Error err = r_Error( r_Error::r_Error_TransactionReadOnly );
        RMDBGEXIT(2, RMDebug::module_clientcomm, "RpcClientComm", "insertMDD(" << collName << ", marray) error")
        throw err;
    }

    unsigned short  rpcStatus;
    unsigned short* rpcStatusPtr=0;
    r_Minterval     spatdom;
    r_Bytes         marBytes;
    RPCMarray*      rpcMarray;
    r_Bytes         tileSize = 0;

    // get the spatial domain of the r_GMarray
    spatdom = mar->spatial_domain();

    // determine the amount of data to be transferred
    marBytes = mar->get_array_size();

    RMDBGMIDDLE(2, RMDebug::module_clientcomm, "RpcClientComm", "inserting MDD with domain " << spatdom << ", cell length " << mar->get_type_length() << ", " << marBytes << " bytes" )

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
        InsertPersMDDParams* params = new InsertPersMDDParams;
        params->clientID   = clientID;
        params->collName   = (char*)collName;
        params->domain     = spatdom.get_string_representation();
        params->typeLength = mar->get_type_length();
        params->typeName   = (char*)mar->get_type_name();
        params->oid        = (char*)mar->get_oid().get_string_representation();

        setRPCActive();
        rpcRetryCounter = 0;
        do
        {
            rpcStatusPtr = rpcstartinsertpersmdd_1( params, binding_h );

            if( !rpcStatusPtr )
            {
                RMInit::logOut << endl << "WARNING: RPC NULL POINTER (rpcstartinsertpersmdd_1)" << endl << endl;
                sleep(RMInit::clientcommSleep);
            }
            if (rpcRetryCounter > RMInit::clientcommMaxRetry)
            {
                RMInit::logOut << "RPC call 'rpcstartinsertpersmdd' failed" << endl;
                throw r_Error(CLIENTCOMMUICATIONFAILURE);
            }
            rpcRetryCounter++;
        }
        while( rpcStatusPtr == 0 );
        rpcStatus = *rpcStatusPtr;
        setRPCInactive();

        free( params->domain );
        delete params;

        if( rpcStatus > 0 )
        {
            r_Error err;

            switch( rpcStatus )
            {
            case 2:
                err = r_Error( r_Error::r_Error_DatabaseClassUndefined );
                break;
            case 3:
                err = r_Error( r_Error::r_Error_CollectionElementTypeMismatch );
                break;
            case 4:
                err = r_Error( r_Error::r_Error_TypeInvalid );
                break;
            default:
                err = r_Error( r_Error::r_Error_TransferFailed );
            }
            RMInit::logOut << "Error: rpcInsertMDDObj() - " << err.what() << endl;
            RMDBGEXIT(2, RMDebug::module_clientcomm, "RpcClientComm", "insertMDD(" << collName << ", marray) error")
            throw err;
        }

        r_Set< r_GMarray* >* bagOfTiles;


        bagOfTiles = mar->get_storage_layout()->decomposeMDD( mar );

        RMDBGMIDDLE(2, RMDebug::module_clientcomm, "RpcClientComm", "decomposing into " << bagOfTiles->cardinality() << " tiles")

        r_Iterator< r_GMarray* > iter = bagOfTiles->create_iterator();
        r_GMarray *origTile;
        iter.reset();

        while( iter.not_done() )
        {
            origTile = *iter;

            // advance iter here to determine if this is the last call (not_done())
            iter.advance();

            RMDBGMIDDLE(2, RMDebug::module_clientcomm, "RpcClientComm", "inserting Tile with domain " << origTile->spatial_domain() << ", " << origTile->spatial_domain().cell_count() * origTile->get_type_length() << " bytes")

            getMarRpcRepresentation( origTile, rpcMarray, mar->get_storage_layout()->get_storage_format(), baseType );

            InsertTileParams* params2 = new InsertTileParams;
            params2->clientID     = clientID;
            params2->isPersistent = 1;
            params2->marray       = rpcMarray;

            if(binding_h == NULL)
            {
                RMInit::logOut << endl << "RpcClientComm::insertMDD(collName, mar) ERROR: CONNECTION TO SERVER ALREADY CLOSED" << endl << endl;
                throw r_Error(CONNECTIONCLOSED);
            }

            setRPCActive();
            rpcRetryCounter = 0;
            do
            {
                rpcStatusPtr = rpcinserttile_1( params2, binding_h );

                if( !rpcStatusPtr )
                {
                    RMInit::logOut << endl << "WARNING: RPC NULL POINTER (rpcinserttile_1)" << endl << endl;
                    sleep(RMInit::clientcommSleep);
                }
                if (rpcRetryCounter > RMInit::clientcommMaxRetry)
                {
                    RMInit::logOut << "RPC call 'rpcinserttile' failed" << endl;
                    throw r_Error(CLIENTCOMMUICATIONFAILURE);
                }
                rpcRetryCounter++;
            }
            while( rpcStatusPtr == 0 );
            rpcStatus = *rpcStatusPtr;
            setRPCInactive();

            // free rpcMarray structure (rpcMarray->data.confarray_val is freed somewhere else)
            freeMarRpcRepresentation( origTile, rpcMarray );
            delete params2;

            // delete current tile (including data block)
            delete origTile;

            if( rpcStatus > 0 )
            {
                RMInit::logOut << "Error: rpcInsertMDD() - general" << endl;
                RMDBGEXIT(2, RMDebug::module_clientcomm, "RpcClientComm", "insertMDD(" << collName << ", marray) error")
                throw r_Error( r_Error::r_Error_TransferFailed );
            }

            RMDBGMIDDLE(2, RMDebug::module_clientcomm, "RpcClientComm", "OK")
        }

        EndInsertMDDParams* params3 = new EndInsertMDDParams;
        params3->clientID     = clientID;
        params3->isPersistent = 1;

        if(binding_h == NULL)
        {
            RMInit::logOut << endl << "RpcClientComm::insertMDD(collName, mar) ERROR: CONNECTION TO SERVER ALREADY CLOSED" << endl << endl;
            throw r_Error(CONNECTIONCLOSED);
        }

        setRPCActive();
        rpcRetryCounter = 0;
        do
        {
            rpcStatusPtr = rpcendinsertmdd_1( params3, binding_h );

            if( !rpcStatusPtr )
            {
                RMInit::logOut << endl << "WARNING: RPC NULL POINTER (rpcendinsertmdd_1)" << endl << endl;
                sleep(RMInit::clientcommSleep);
            }
            if (rpcRetryCounter > RMInit::clientcommMaxRetry)
            {
                RMInit::logOut << "RPC call 'rpcendinsertmdd' failed" << endl;
                throw r_Error(CLIENTCOMMUICATIONFAILURE);
            }
            rpcRetryCounter++;
        }
        while( rpcStatusPtr == 0 );
        rpcStatus = *rpcStatusPtr;
        setRPCInactive();

        delete params3;

        // delete transient data
        bagOfTiles->remove_all();
        delete bagOfTiles;
    }
    else // begin: MDD is transferred in one piece
    {
        RMDBGMIDDLE(2, RMDebug::module_clientcomm, "RpcClientComm", ", one tile" )

        getMarRpcRepresentation( mar, rpcMarray, mar->get_storage_layout()->get_storage_format(), baseType );

        InsertMDDParams* params = new InsertMDDParams;
        params->clientID = clientID;
        params->collName = (char*)collName;
        params->marray   = rpcMarray;
        params->typeName = (char*)mar->get_type_name();
        params->oid      = (char*)mar->get_oid().get_string_representation();

        if(binding_h == NULL)
        {
            RMInit::logOut << endl << "RpcClientComm::insertMDD(collName, mar) ERROR: CONNECTION TO SERVER ALREADY CLOSED" << endl << endl;
            throw r_Error(CONNECTIONCLOSED);
        }

        setRPCActive();
        rpcRetryCounter = 0;
        do
        {
            rpcStatusPtr = rpcinsertmdd_1( params, binding_h );

            if( !rpcStatusPtr )
            {
                RMInit::logOut << endl << "WARNING: RPC NULL POINTER (rpcinsertmdd_1)" << endl << endl;
                sleep(RMInit::clientcommSleep);
            }
            if (rpcRetryCounter > RMInit::clientcommMaxRetry)
            {
                RMInit::logOut << "RPC call 'rpcinsertmdd' failed" << endl;
                throw r_Error(CLIENTCOMMUICATIONFAILURE);
            }
            rpcRetryCounter++;
        }
        while( rpcStatusPtr == 0 );
        rpcStatus = *rpcStatusPtr;
        setRPCInactive();

        freeMarRpcRepresentation( mar, rpcMarray );
        delete params;

        RMDBGMIDDLE(2, RMDebug::module_clientcomm, "RpcClientComm", "ok" )

        if( rpcStatus > 0 )
        {
            r_Error err;

            switch( rpcStatus )
            {
            case 2:
                err = r_Error( r_Error::r_Error_DatabaseClassUndefined );
                break;
            case 3:
                err = r_Error( r_Error::r_Error_CollectionElementTypeMismatch );
                break;
            case 4:
                err = r_Error( r_Error::r_Error_TypeInvalid );
                break;
            default:
                err = r_Error( r_Error::r_Error_TransferFailed );
            }
            RMInit::logOut << "Error: rpcInsertMDD() - " << err.what() << endl;
            throw err;
        }

    } // end: MDD i transferred in one piece
}

r_Ref_Any
RpcClientComm::getMDDByOId( const r_OId& oid )
throw( r_Error )
{

    if(binding_h == NULL)
    {
        RMInit::logOut << endl << "RpcClientComm::getMDDByOId(oid) ERROR: CONNECTION TO SERVER ALREADY CLOSED" << endl << endl;
        throw r_Error(CONNECTIONCLOSED);
    }

    RMDBGENTER(2, RMDebug::module_clientcomm, "RpcClientComm", "getMDDByOId(" << oid << ")")

    r_Ref_Any       mddResult;

    r_GMarray*      marray       = 0;
    unsigned short  tileStatus   = 0;
    unsigned short  rpcStatus    = 0;
    unsigned short* rpcStatusPtr = 0;

    OIdSpecParams params;
    GetMDDRes*     thisResult = 0;
    params.clientID = clientID;
    params.oid      = (char*)oid.get_string_representation();

    setRPCActive();
    rpcRetryCounter = 0;
    do
    {
        thisResult = rpcgetmddbyoid_1( &params, binding_h );

        if( !thisResult )
        {
            RMInit::logOut << endl << "WARNING: RPC NULL POINTER (rpcgetmddbyoid_1)" << endl << endl;
            sleep(RMInit::clientcommSleep);
        }
        if (rpcRetryCounter > RMInit::clientcommMaxRetry)
        {
            RMInit::logOut << "RPC call 'rpcgetmddbyoid' failed" << endl;
            throw r_Error(CLIENTCOMMUICATIONFAILURE);
        }
        rpcRetryCounter++;
    }
    while( thisResult == 0 );
    setRPCInactive();

    rpcStatus = thisResult->status;

    if( rpcStatus != 0 )
    {
        r_Error err;

        switch( rpcStatus )
        {
        case 1:
            err = r_Error( r_Error::r_Error_ClientUnknown );
            break;
        case 2:
        case 3:
            err = r_Error( r_Error::r_Error_ObjectUnknown );
            break;
        default:
            err = r_Error( r_Error::r_Error_TransferFailed );
        }

        XDRFREE(GetMDDRes, thisResult);

        RMDBGEXIT(2, RMDebug::module_clientcomm, "RpcClientComm", "failed")
        throw err;
    }

    r_Ref< r_GMarray > mdd;
    getMDDCore(mdd, thisResult, 0);
    mddResult = mdd;

    setRPCActive();
    RMDBGIF(20, RMDebug::module_clientcomm, "WAITENDTRANSFERSTART", \
            RMInit::dbgOut << "Waiting 100 sec before end transfer\n" << std::endl; \
            sleep(100); \
            RMInit::dbgOut << "Continue now\n" << std::endl; );

    if(binding_h == NULL)
    {
        RMInit::logOut << endl << "RpcClientComm::getMDDByOId(oid) ERROR: CONNECTION TO SERVER ALREADY CLOSED" << endl << endl;
        throw r_Error(CONNECTIONCLOSED);
    }

    rpcRetryCounter = 0;
    do
    {
        rpcStatusPtr = rpcendtransfer_1( &clientID, binding_h );
        if (rpcRetryCounter > RMInit::clientcommMaxRetry)
        {
            RMInit::logOut << "RPC call 'rpcendtransfer' failed" << endl;
            throw r_Error(CLIENTCOMMUICATIONFAILURE);
        }
        rpcRetryCounter++;

        if( !rpcStatusPtr )
        {
            RMInit::logOut << endl << "WARNING: RPC NULL POINTER (rpcendtransfer_1)" << endl << endl;
            sleep(RMInit::clientcommSleep);
        }
    }
    while( rpcStatusPtr == 0 );
    rpcStatus = *rpcStatusPtr;
    RMDBGIF(20, RMDebug::module_clientcomm, "WAITENDTRANSFEREND", \
            RMInit::dbgOut << "Waiting 100 sec after end transfer\n" << std::endl; \
            sleep(100); \
            RMInit::dbgOut << "Continue now\n" << std::endl; );
    setRPCInactive();

    RMDBGEXIT(2, RMDebug::module_clientcomm, "RpcClientComm", "ok")

    return mddResult;
}



r_Ref_Any
RpcClientComm::getCollByName( const char* collName )
throw( r_Error )
{
    if(binding_h == NULL)
    {
        RMInit::logOut << endl << "RpcClientComm::getCollByName(collName) ERROR: CONNECTION TO SERVER ALREADY CLOSED" << endl << endl;
        throw r_Error(CONNECTIONCLOSED);
    }

    RMDBGENTER(2, RMDebug::module_clientcomm, "RpcClientComm", "getCollByName(" << collName << ")")
    r_Set< r_Ref_Any >* set       = 0;
    unsigned short      rpcStatus = 0;

    NameSpecParams* params     = new NameSpecParams;
    GetCollRes*     thisResult = 0;
    params->clientID = clientID;
    params->name     = (char*)collName;

    setRPCActive();
    rpcRetryCounter = 0;
    do
    {
        thisResult = rpcgetcollbyname_1( params, binding_h );

        if( !thisResult )
        {
            RMInit::logOut << endl << "WARNING: RPC NULL POINTER (rpcgetcollbyname_1)" << endl << endl;
            sleep(RMInit::clientcommSleep);
        }
        if (rpcRetryCounter > RMInit::clientcommMaxRetry)
        {
            RMInit::logOut << "RPC call 'rpcgetcollbyname' failed" << endl;
            throw r_Error(CLIENTCOMMUICATIONFAILURE);
        }
        rpcRetryCounter++;
    }
    while( thisResult == 0 );
    setRPCInactive();

    delete params;

    rpcStatus = thisResult->status;

    if( rpcStatus != 0 && rpcStatus != 1 )
    {
        r_Error err;

        switch( rpcStatus )
        {
        case 2:
            err = r_Error( r_Error::r_Error_ObjectUnknown );
            break;
        case 3:
            err = r_Error( r_Error::r_Error_ClientUnknown );
            break;
        default:
            err = r_Error( r_Error::r_Error_TransferFailed );
        }

        XDRFREE(GetCollRes, thisResult);

        RMDBGEXIT(2, RMDebug::module_clientcomm, "RpcClientComm", "failed")
        throw err;
    }

    RMDBGMIDDLE(2, RMDebug::module_clientcomm, "RpcClientComm", "ok")

    // create the set
    r_OId rOId( thisResult->oid );
    set = new ( r_Database::actual_database, r_Object::read, rOId ) r_Set< r_Ref_Any >;

    // initialize data elements
    set->set_type_by_name  ( thisResult->typeName );
    set->set_type_structure( thisResult->typeStructure );
    set->set_object_name   ( thisResult->collName );

    // now the transfer structure of rpcgetcollbyname can be freed
    XDRFREE(GetCollRes, thisResult);

    // get collection elements
    if( rpcStatus == 0 )
        getMDDCollection( *set, 0 );
    // else rpcStatus == 1 -> Result collection is empty and nothing has to be got.

    RMDBGEXIT(2, RMDebug::module_clientcomm, "RpcClientComm", "getCollByName(" << collName << ")")
    return r_Ref_Any( set->get_oid(), set );
}



r_Ref_Any
RpcClientComm::getCollByOId( const r_OId& oid )
throw( r_Error )
{
    if(binding_h == NULL)
    {
        RMInit::logOut << endl << "RpcClientComm::getCollByOId(oid) ERROR: CONNECTION TO SERVER ALREADY CLOSED" << endl << endl;
        throw r_Error(CONNECTIONCLOSED);
    }

    RMDBGENTER(2, RMDebug::module_clientcomm, "RpcClientComm", "getCollByOId(" << oid << ")")
    r_Set< r_Ref_Any >* set       = 0;
    unsigned short             rpcStatus = 0;

    OIdSpecParams* params     = new OIdSpecParams;
    GetCollRes*    thisResult = 0;
    params->clientID = clientID;
    params->oid      = (char*)oid.get_string_representation();

    setRPCActive();
    rpcRetryCounter = 0;
    do
    {
        thisResult = rpcgetcollbyoid_1( params, binding_h );

        if( !thisResult )
        {
            RMInit::logOut << endl << "WARNING: RPC NULL POINTER (rpcgetcollbyoid_1)" << endl << endl;
            sleep(RMInit::clientcommSleep);
        }
        if (rpcRetryCounter > RMInit::clientcommMaxRetry)
        {
            RMInit::logOut << "RPC call 'rpcgetcollbyoid' failed" << endl;
            throw r_Error(CLIENTCOMMUICATIONFAILURE);
        }
        rpcRetryCounter++;
    }
    while( thisResult == 0 );
    setRPCInactive();

    delete params;

    rpcStatus = thisResult->status;

    if( rpcStatus != 0 && rpcStatus != 1 )
    {
        r_Error err;

        switch( rpcStatus )
        {
        case 2:
            err = r_Error( r_Error::r_Error_ObjectUnknown );
            break;
        case 3:
            err = r_Error( r_Error::r_Error_ClientUnknown );
            break;
        default:
            err = r_Error( r_Error::r_Error_TransferFailed );
        }

        XDRFREE(GetCollRes, thisResult);

        RMDBGEXIT(2, RMDebug::module_clientcomm, "RpcClientComm", "failed")
        throw err;
    }

    RMDBGMIDDLE(2, RMDebug::module_clientcomm, "RpcClientComm", "ok")

    // create the set
    r_OId rOId( thisResult->oid );
    set = new ( r_Database::actual_database, r_Object::read, rOId )  r_Set< r_Ref_Any >;

    // initialize data elements
    set->set_type_by_name  ( thisResult->typeName );
    set->set_type_structure( thisResult->typeStructure );
    set->set_object_name   ( thisResult->collName );

    // now the transfer structure can be freed
    XDRFREE(GetCollRes, thisResult);

    // get collection elements
    if( rpcStatus == 0 )
        getMDDCollection( *set, 0 );
    // else rpcStatus == 1 -> Result collection is empty and nothing has to be got.

    RMDBGEXIT(2, RMDebug::module_clientcomm, "RpcClientComm", "failed")
    return r_Ref_Any( set->get_oid(), set );
}



r_Ref_Any
RpcClientComm::getCollOIdsByName( const char* collName )
throw( r_Error )
{
    if(binding_h == NULL)
    {
        RMInit::logOut << endl << "RpcClientComm::getCollOIdsByName(collName) ERROR: CONNECTION TO SERVER ALREADY CLOSED" << endl << endl;
        throw r_Error(CONNECTIONCLOSED);
    }
    RMDBGENTER(2, RMDebug::module_clientcomm, "RpcClientComm", "getCollOIdsByName(" << collName << ")")
    r_Set< r_Ref<r_GMarray> >* set = 0;
    unsigned short rpcStatus       = 0;

    NameSpecParams* params     = new NameSpecParams;
    GetCollOIdsRes* thisResult = 0;
    params->clientID = clientID;
    params->name     = (char*)collName;

    setRPCActive();
    rpcRetryCounter = 0;
    do
    {
        thisResult = rpcgetcolloidsbyname_1( params, binding_h );

        if( !thisResult )
        {
            RMInit::logOut << endl << "WARNING: RPC NULL POINTER (rpcgetcolloidsbyname_1)" << endl << endl;
            sleep(RMInit::clientcommSleep);
        }
        if (rpcRetryCounter > RMInit::clientcommMaxRetry)
        {
            RMInit::logOut << "RPC call 'rpcgetcolloidsbyname' failed" << endl;
            throw r_Error(CLIENTCOMMUICATIONFAILURE);
        }
        rpcRetryCounter++;
    }
    while( thisResult == 0 );
    setRPCInactive();

    delete params;

    rpcStatus = thisResult->status;

    if( rpcStatus != 0 && rpcStatus != 1 )
    {
        r_Error err;

        switch( rpcStatus )
        {
        case 2:
            err = r_Error( r_Error::r_Error_ObjectUnknown );
            break;
        case 3:
            err = r_Error( r_Error::r_Error_ClientUnknown );
            break;
        default:
            err = r_Error( r_Error::r_Error_TransferFailed );
        }

        XDRFREE(GetCollOIdsRes, thisResult);

        RMDBGEXIT(2, RMDebug::module_clientcomm, "RpcClientComm", "failed")
        throw err;
    }

    RMDBGMIDDLE(2, RMDebug::module_clientcomm, "RpcClientComm", "ok")

    // create the set
    r_OId rOId( thisResult->oid );
    set = new ( r_Database::actual_database, r_Object::read, rOId )  r_Set< r_Ref< r_GMarray > >;

    set->set_type_by_name  ( thisResult->typeName );
    set->set_type_structure( thisResult->typeStructure );
    set->set_object_name   ( thisResult->collName );

    // fill set with oids
    if( rpcStatus == 0 )
    {
        for( unsigned int i=0; i<thisResult->oidTable.oidTable_len; i++ )
        {
            set->insert_element( r_Ref<r_GMarray>( r_OId( thisResult->oidTable.oidTable_val[i].oid ) ), 1 );

            RMDBGMIDDLE(2, RMDebug::module_clientcomm, "RpcClientComm", "oid " << i << ": " << thisResult->oidTable.oidTable_val[i].oid)
        }
    }

    // now the transfer structure can be freed
    XDRFREE(GetCollOIdsRes, thisResult);

    RMDBGEXIT(2, RMDebug::module_clientcomm, "RpcClientComm", "getCollOIdsByName")
    return r_Ref_Any( set->get_oid(), set );
}



r_Ref_Any
RpcClientComm::getCollOIdsByOId( const r_OId& oid )
throw( r_Error )
{
    if(binding_h == NULL)
    {
        RMInit::logOut << endl << "RpcClientComm::getCollOIdsByOId(oid) ERROR: CONNECTION TO SERVER ALREADY CLOSED" << endl << endl;
        throw r_Error(CONNECTIONCLOSED);
    }
    RMDBGENTER(2, RMDebug::module_clientcomm, "RpcClientComm", "getCollOIdsByOId(" << oid << ")")
    r_Set< r_Ref<r_GMarray> >* set = 0;
    unsigned short rpcStatus       = 0;

    OIdSpecParams*  params     = new OIdSpecParams;
    GetCollOIdsRes* thisResult = 0;
    params->clientID = clientID;
    params->oid      = (char*)oid.get_string_representation();

    setRPCActive();
    rpcRetryCounter = 0;
    do
    {
        thisResult = rpcgetcolloidsbyoid_1( params, binding_h );

        if( !thisResult )
        {
            RMInit::logOut << endl << "WARNING: RPC NULL POINTER (rpcgetcolloidsbyoid_1)" << endl << endl;
            sleep(RMInit::clientcommSleep);
        }
        if (rpcRetryCounter > RMInit::clientcommMaxRetry)
        {
            RMInit::logOut << "RPC call 'rpcgetcolloidsbyoid' failed" << endl;
            throw r_Error(CLIENTCOMMUICATIONFAILURE);
        }
        rpcRetryCounter++;
    }
    while( thisResult== 0 );
    setRPCInactive();

    delete params;

    rpcStatus = thisResult->status;

    if( rpcStatus != 0 && rpcStatus != 1 )
    {
        r_Error err;

        switch( rpcStatus )
        {
        case 2:
            err = r_Error( r_Error::r_Error_ObjectUnknown );
            break;
        case 3:
            err = r_Error( r_Error::r_Error_ClientUnknown );
            break;
        default:
            err = r_Error( r_Error::r_Error_TransferFailed );
        }

        XDRFREE(GetCollOIdsRes, thisResult);

        RMDBGEXIT(2, RMDebug::module_clientcomm, "RpcClientComm", "failed")
        throw err;
    }

    RMDBGMIDDLE(2, RMDebug::module_clientcomm, "RpcClientComm", "ok")

    // create the set
    r_OId rOId( thisResult->oid );
    set = new ( r_Database::actual_database, r_Object::read, rOId )  r_Set< r_Ref< r_GMarray > >;

    set->set_type_by_name  ( thisResult->typeName );
    set->set_type_structure( thisResult->typeStructure );
    set->set_object_name   ( thisResult->collName );

    // fill set with oids
    if( rpcStatus == 0 )
    {
        for( unsigned int i=0; i<thisResult->oidTable.oidTable_len; i++ )
        {
            set->insert_element( r_Ref<r_GMarray>( r_OId( thisResult->oidTable.oidTable_val[i].oid ) ), 1 );
            RMDBGMIDDLE(2, RMDebug::module_clientcomm, "RpcClientComm", "contains oid #" << i << ":" << thisResult->oidTable.oidTable_val[i].oid)
        }
    }

    // now the transfer structure can be freed
    XDRFREE(GetCollOIdsRes, thisResult);

    RMDBGEXIT(2, RMDebug::module_clientcomm, "RpcClientComm", "getCollOIdsByOId")
    return r_Ref_Any( set->get_oid(), set );
}

int
RpcClientComm::createDB( const char* name )
throw(r_Error)
{
    if(binding_h == NULL)
    {
        RMInit::logOut << endl << "RpcClientComm::createDB(name) ERROR: CONNECTION TO SERVER ALREADY CLOSED" << endl << endl;
        throw r_Error(CONNECTIONCLOSED);
    }

    unsigned short  rpcStatus=0;
    unsigned short* rpcStatusPtr=0;

    setRPCActive();
    rpcRetryCounter = 0;
    do
    {
        rpcStatusPtr = rpccreatedb_1( (char**)&name, binding_h );

        if( !rpcStatusPtr )
        {
            RMInit::logOut << endl << "WARNING: RPC NULL POINTER (rpccreatedb_1)" << endl << endl;
            sleep(RMInit::clientcommSleep);
        }
        if (rpcRetryCounter > RMInit::clientcommMaxRetry)
        {
            RMInit::logOut << "RPC call 'rpccreatedb' failed" << endl;
            throw r_Error(CLIENTCOMMUICATIONFAILURE);
        }
        rpcRetryCounter++;
    }
    while( rpcStatusPtr == 0 );
    rpcStatus = *rpcStatusPtr;
    setRPCInactive();

    return rpcStatus;
}


int
RpcClientComm::destroyDB( const char* name )
throw(r_Error)
{
    if(binding_h == NULL)
    {
        RMInit::logOut << endl << "RpcClientComm::destroyDB(name) ERROR: CONNECTION TO SERVER ALREADY CLOSED" << endl << endl;
        throw r_Error(CONNECTIONCLOSED);
    }

    unsigned short  rpcStatus=0;
    unsigned short* rpcStatusPtr=0;

    setRPCActive();
    rpcRetryCounter = 0;
    do
    {
        rpcStatusPtr = rpcdestroydb_1( (char**)&name, binding_h );

        if( !rpcStatusPtr )
        {
            RMInit::logOut << endl << "WARNING: RPC NULL POINTER (rpcdestroydb_1)" << endl << endl;
            sleep(RMInit::clientcommSleep);
        }
        if (rpcRetryCounter > RMInit::clientcommMaxRetry)
        {
            RMInit::logOut << "RPC call 'rpcdestroydb' failed" << endl;
            throw r_Error(CLIENTCOMMUICATIONFAILURE);
        }
        rpcRetryCounter++;
    }
    while( rpcStatusPtr == 0 );
    rpcStatus = *rpcStatusPtr;
    setRPCInactive();

    return rpcStatus;
}


int
RpcClientComm::openDB( const char* database )
{
    strcpy(dataBase,database);

    connectToServer(1); // this means read-only

    int answer=executeOpenDB(database);

    if(answer==0) answer=executeCloseDB();
    // else the DB is not open and makes ugly log output on the server

    disconnectFromServer();
    return answer;
}

int
RpcClientComm::executeOpenDB( const char* database )
{

    if(binding_h == NULL)
    {
        RMInit::logOut << endl << "RpcClientComm::executeOpenDB(database) ERROR: CONNECTION TO SERVER ALREADY CLOSED" << endl << endl;
        return CONNECTIONCLOSED;
    }

    RMDBGENTER(2, RMDebug::module_clientcomm, "RpcClientComm", "executeOpenDB(" << database << ")")
    // Send first "I'm alive" signal in ALIVEINTERVAL seconds
#ifdef WIN32
    timerid = timeSetEvent(ALIVEINTERVAL * 1000, 0, TimerProc, 0, TIME_PERIODIC);
#else
    alarm( ALIVEINTERVAL );
#endif
    OpenDBParams* params     = new OpenDBParams;
    OpenDBRes*    thisResult = 0;
    params->dbName   = (char*)database;
    params->userName = (char*)RMInit::userName;
    params->capability = capability;
    int*          dummyParam = new int(0);// dummy
    int*          endianResult = NULL;
    ServerVersionRes* versionResult = NULL;

    setRPCActive();
    versionResult = rpcgetserverversion_1( dummyParam, binding_h );
    RMDBGMIDDLE(2, RMDebug::module_clientcomm, "RpcClientComm", "server version " << versionResult->serverVersionNo << ", rpc version " << versionResult->rpcInterfaceVersionNo)
    // don't forget to add 0.5, otherwise rounding errors!
    serverRPCversion = (int)(1000.0 * versionResult->rpcInterfaceVersionNo + 0.5);
    if (serverRPCversion != RPCVERSION)
    {
        RMDBGMIDDLE(2, RMDebug::module_clientcomm, "RpcClientComm", "RPC interface version mismatch: client (" << RPCVERSION/1000.0 << "), server (" << versionResult->rpcInterfaceVersionNo << ")")
        RMInit::logOut << "Client Server Communication incompatible" << endl;
        setRPCInactive();
        RMDBGEXIT(2, RMDebug::module_clientcomm, "RpcClientComm", "executeOpenDB(" << database << ") " << 4)
        return 4;   // servercomm::openDB creates codes 1-3.
    }

    if(binding_h == NULL)
    {
        RMInit::logOut << endl << "RpcClientComm::executeOpenDB(database) ERROR: CONNECTION TO SERVER ALREADY CLOSED" << endl << endl;
        return CONNECTIONCLOSED;
    }

    rpcRetryCounter = 0;
    do
    {
        thisResult = rpcopendb_1( params, binding_h );

        if( !thisResult )
        {
            RMInit::logOut << endl << "WARNING: RPC NULL POINTER (rpcopendb_1)" << endl << endl;
            sleep(RMInit::clientcommSleep);
        }
        if (rpcRetryCounter > RMInit::clientcommMaxRetry)
        {
            RMInit::logOut << "RPC call 'rpcopendb' failed" << endl;
            throw r_Error(CLIENTCOMMUICATIONFAILURE);
        }
        rpcRetryCounter++;
    }
    while( thisResult == 0 );

    if(binding_h == NULL)
    {
        RMInit::logOut << endl << "RpcClientComm::executeOpenDB(database) ERROR: CONNECTION TO SERVER ALREADY CLOSED" << endl << endl;
        return CONNECTIONCLOSED;
    }

    endianResult = rpcgetserverendian_1( dummyParam, binding_h );

    setRPCInactive();

    delete params;
    delete dummyParam;

    clientID = thisResult->clientID;
    endianServer = *endianResult;
    //cout << "server endianness: " << endianServer << ", client: " << endianClient << endl;

    RMDBGEXIT(2, RMDebug::module_clientcomm, "RpcClientComm", "executeOpenDB(" << database << ") " << thisResult->status)
    return thisResult->status;
}


int
RpcClientComm::closeDB()
{
    // We decided that it is not necessary to do anything for closeDB, the database is already closed by others
    RMInit::logOut << "Fake closeDB" << endl;

    return 0;// answer;
}

int
RpcClientComm::executeCloseDB()
{
    if(binding_h == NULL)
    {
        RMInit::logOut << endl << "RpcClientComm::executeCloseDB(database) ERROR: CONNECTION TO SERVER ALREADY CLOSED" << endl << endl;
        return CONNECTIONCLOSED;
    }

    unsigned short  rpcStatus=0;
    unsigned short* rpcStatusPtr=0;

    // Suspend "I'm alive" signal
#ifdef __VISUALC__
    timeKillEvent(timerid);
#else
    alarm( 0 );
#endif

    setRPCActive();
    rpcRetryCounter = 0;
    do
    {
        rpcStatusPtr =  rpcclosedb_1( &clientID, binding_h );

        if( !rpcStatusPtr )
        {
            RMInit::logOut << endl << "WARNING: RPC NULL POINTER (rpcclosedb_1)" << endl << endl;
            sleep(RMInit::clientcommSleep);
        }
        if (rpcRetryCounter > RMInit::clientcommMaxRetry)
        {
            RMInit::logOut << "RPC call 'rpcclosedb' failed" << endl;
            throw r_Error(CLIENTCOMMUICATIONFAILURE);
        }
        rpcRetryCounter++;
    }
    while( rpcStatusPtr == 0 );
    rpcStatus = *rpcStatusPtr;
    setRPCInactive();

    return rpcStatus;
}

int
RpcClientComm::openTA( unsigned short readOnly )
throw(r_Error)
{
    int answer=0;
    connectToServer(readOnly);

    answer=executeOpenDB(dataBase);

    if(answer==0) executeOpenTA(readOnly);

    //If there is an error CONNECTIONCLOSED, we report this, it is important to know
    if(answer == CONNECTIONCLOSED)
    {
        throw r_Error(CONNECTIONCLOSED);
    }

    return answer;
}

int
RpcClientComm::executeOpenTA( unsigned short readOnly )
{
    if(binding_h == NULL)
    {
        RMInit::logOut << endl << "RpcClientComm::executeOpenTA(database) ERROR: CONNECTION TO SERVER ALREADY CLOSED" << endl << endl;
        return CONNECTIONCLOSED;
    }

    unsigned short  rpcStatus=0;
    unsigned short* rpcStatusPtr=0;
    int             secsWaited = 0;

    BeginTAParams params;
    params.clientID = clientID;
    params.readOnly = readOnly;
    params.capability = capability;

    setRPCActive();
    rpcRetryCounter = 0;
    do
    {
        rpcStatusPtr = rpcbeginta_1( &params, binding_h );

        if( !rpcStatusPtr )
        {
            RMInit::logOut << endl << "WARNING: RPC NULL POINTER (rpcbeginta_1)" << endl << endl;
            sleep(RMInit::clientcommSleep);
        }
        if (rpcRetryCounter > RMInit::clientcommMaxRetry)
        {
            RMInit::logOut << "RPC call 'rpcbeginta' failed" << endl;
            throw r_Error(CLIENTCOMMUICATIONFAILURE);
        }
        rpcRetryCounter++;
    }
    while( rpcStatusPtr == 0 );

    rpcStatus = *rpcStatusPtr;

    RMDBGIF(20, RMDebug::module_tools, "WAITRECEIVEDTILE", \
            RMInit::dbgOut << "Waiting 100 sec after receive tile\n" << std::endl; \
            sleep(100); \
            RMInit::dbgOut << "Continue now\n" << std::endl; );
    setRPCInactive();

    /* not necessary with V5
      int             sleepCntr = 1;
      while( rpcStatus == 2 )
      {
        if( sleepCntr == 64 ) sleepCntr /= 2;
        RMInit::logOut << "Another transaction is already active, sleeping " << sleepCntr*2 << " secs..." << endl;
        sleepCntr *= 2;
        secsWaited += sleepCntr;
    #ifndef __VISUALC__
        sleep( sleepCntr );
    #else
        Sleep( sleepCntr*1000 );
    #endif
        setRPCActive();
        do
        {
          rpcStatusPtr = rpcbeginta_1( &params, binding_h );

          if( !rpcStatusPtr )
        {
            RMInit::logOut << endl << "WARNING: RPC NULL POINTER (rpcbeginta_1)" << endl << endl;
        sleep(RMInit::clientcommSleep);
        }
        }while( rpcStatusPtr == 0 );
        rpcStatus = *rpcStatusPtr;
        setRPCInactive();
      }
    */
    return rpcStatus;
}

int
RpcClientComm::commitTA()
throw(r_Error)
{
    int answer=executeCommitTA();

    if(answer==0) answer=executeCloseDB();

    //If there is an error CONNECTIONCLOSED, we report this, it is important to know
    if(answer == CONNECTIONCLOSED)
    {
        throw r_Error(CONNECTIONCLOSED);
    }

    disconnectFromServer();

    return answer;
}

int
RpcClientComm::executeCommitTA()
{
    if(binding_h == NULL)
    {
        RMInit::logOut << endl << "RpcClientComm::executeCommitTA(database) ERROR: CONNECTION TO SERVER ALREADY CLOSED" << endl << endl;
        return CONNECTIONCLOSED;
    }

    unsigned short  rpcStatus=0;
    unsigned short* rpcStatusPtr=0;

    setRPCActive();
    rpcRetryCounter = 0;
    do
    {
        rpcStatusPtr = rpccommitta_1( &clientID, binding_h );

        if( !rpcStatusPtr )
        {
            {
                RMInit::logOut << endl << "WARNING: RPC NULL POINTER (rpccommitta_1)" << endl << endl;
                sleep(RMInit::clientcommSleep);
            }
        }
        if (rpcRetryCounter > RMInit::clientcommMaxRetry)
        {
            RMInit::logOut << "RPC call 'rpccommitta' failed" << endl;
            throw r_Error(CLIENTCOMMUICATIONFAILURE);
        }
        rpcRetryCounter++;
    }
    while( rpcStatusPtr == 0 );
    rpcStatus = *rpcStatusPtr;
    setRPCInactive();

    return rpcStatus;
}

int
RpcClientComm::abortTA()
{
    int answer=0;
    try
    {
        answer = executeAbortTA();

        if(answer==0) answer=executeCloseDB();

        disconnectFromServer();
    }
    catch (r_Error& e)
    {
        RMInit::logOut << "RpcClientComm::abortTA() caught error: " << e.get_errorno() << " " << e.what() << std::endl;
        answer = 1;
    }

    //If there is an error CONNECTIONCLOSED, we ignore this, it is in abort

    return answer;
}

int
RpcClientComm::executeAbortTA()
{
    if(binding_h == NULL)
    {
        RMInit::logOut << endl << "RpcClientComm::executeAbortTA(database) ERROR: CONNECTION TO SERVER ALREADY CLOSED" << endl << endl;
        return CONNECTIONCLOSED;
    }

    unsigned short  rpcStatus=0;
    unsigned short* rpcStatusPtr=0;

    setRPCActive();
    rpcRetryCounter = 0;
    do
    {
        rpcStatusPtr = rpcabortta_1( &clientID, binding_h );

        if( !rpcStatusPtr )
        {
            {
                RMInit::logOut << endl << "WARNING: RPC NULL POINTER (rpcabortta_1)" << endl << endl;
                sleep(RMInit::clientcommSleep);
            }
        }
        if (rpcRetryCounter > RMInit::clientcommMaxRetry)
        {
            RMInit::logOut << "RPC call 'rpcabortta' failed" << endl;
            throw r_Error(CLIENTCOMMUICATIONFAILURE);
        }
        rpcRetryCounter++;
    }
    while( rpcStatusPtr == 0 );
    rpcStatus = *rpcStatusPtr;
    setRPCInactive();

    return rpcStatus;
}


r_OId
RpcClientComm::getNewOId( unsigned short objType )
throw(r_Error)
{
    if(binding_h == NULL)
    {
        RMInit::logOut << endl << "RpcClientComm::getNewOId(objType) ERROR: CONNECTION TO SERVER ALREADY CLOSED" << endl << endl;
        throw r_Error(CONNECTIONCLOSED);
    }

    // update -> check for read_only transaction
    if( r_Transaction::actual_transaction == 0 ||
            r_Transaction::actual_transaction->get_mode() == r_Transaction::read_only )
    {
        r_Error err = r_Error( r_Error::r_Error_TransactionReadOnly );
        throw err;
    }

    unsigned short rpcStatus = 0;

    NewOIdParams* params     = new NewOIdParams;
    OIdRes*       thisResult = 0;
    params->clientID = clientID;
    params->objType  = objType;

    setRPCActive();
    rpcRetryCounter = 0;
    do
    {
        thisResult = rpcgetnewoid_1( params, binding_h );

        if( !thisResult )
        {
            RMInit::logOut << endl << "WARNING: RPC NULL POINTER (rpcgetnewoid_1)" << endl << endl;
            sleep(RMInit::clientcommSleep);
        }
        if (rpcRetryCounter > RMInit::clientcommMaxRetry)
        {
            RMInit::logOut << "RPC call 'rpcgetnewoid' failed" << endl;
            throw r_Error(CLIENTCOMMUICATIONFAILURE);
        }
        rpcRetryCounter++;
    }
    while( thisResult == 0 );
    setRPCInactive();

    delete params;

    rpcStatus = thisResult->status;

    r_OId oid( thisResult->oid );

    // now the transfer structure of rpcgetcollbyname can be freed
    XDRFREE(OIdRes, thisResult);

    if( rpcStatus != 0 )
    {
        r_Error err;

        switch( rpcStatus )
        {
        case 1:
            err = r_Error( r_Error::r_Error_ClientUnknown );
            break;
        case 2:
            err = r_Error( r_Error::r_Error_CreatingOIdFailed );
            break;
        default:
            err = r_Error( r_Error::r_Error_TransferFailed );
        }
        throw err;
    }

    return oid;
}


unsigned short
RpcClientComm::getObjectType( const r_OId& oid )
throw(r_Error)
{
    if(binding_h == NULL)
    {
        RMInit::logOut << endl << "RpcClientComm::getObjectType(oid) ERROR: CONNECTION TO SERVER ALREADY CLOSED" << endl << endl;
        throw r_Error(CONNECTIONCLOSED);
    }

    OIdSpecParams* params     = new OIdSpecParams;
    ObjectTypeRes* thisResult = 0;
    params->clientID = clientID;
    params->oid      = (char*)oid.get_string_representation();

    setRPCActive();
    rpcRetryCounter = 0;
    do
    {
        thisResult = rpcgetobjecttype_1( params, binding_h );

        if( !thisResult )
        {
            RMInit::logOut << endl << "WARNING: RPC NULL POINTER (rpcgetobjecttype_1)" << endl << endl;
            sleep(RMInit::clientcommSleep);
        }
        if (rpcRetryCounter > RMInit::clientcommMaxRetry)
        {
            RMInit::logOut << "RPC call 'rpcgetobjexttype' failed" << endl;
            throw r_Error(CLIENTCOMMUICATIONFAILURE);
        }
        rpcRetryCounter++;
    }
    while( thisResult == 0 );
    setRPCInactive();

    delete params;

    unsigned short rpcStatus = thisResult->status;
    unsigned short objType   = thisResult->objType;

    // now the transfer structure can be freed
    XDRFREE(ObjectTypeRes, thisResult);

    if( rpcStatus != 0 )
    {
        r_Error err;

        switch( rpcStatus )
        {
        case 1:
            err = r_Error( r_Error::r_Error_ClientUnknown );
            break;
        case 2:
            err = r_Error( r_Error::r_Error_ObjectUnknown );
            break;
        default:
            err = r_Error( r_Error::r_Error_TransferFailed );
        }
        throw err;
    }

    return objType;
}


char*
RpcClientComm::getTypeStructure( const char* typeName, r_Type_Type typeType )
throw(r_Error)
{
    if(binding_h == NULL)
    {
        RMInit::logOut << endl << "RpcClientComm::getTypeStructure(typeName, typeType) ERROR: CONNECTION TO SERVER ALREADY CLOSED" << endl << endl;
        throw r_Error(CONNECTIONCLOSED);
    }

    GetTypeStructureParams* params     = new GetTypeStructureParams;
    GetTypeStructureRes*    thisResult = 0;
    params->clientID = clientID;
    params->typeName = (char*)typeName;
    params->typeType = typeType;

    setRPCActive();
    rpcRetryCounter = 0;
    do
    {
        thisResult = rpcgettypestructure_1( params, binding_h );

        if( !thisResult )
        {
            RMInit::logOut << endl << "WARNING: RPC NULL POINTER (rpcgettypestructure_1)" << endl << endl;
            sleep(RMInit::clientcommSleep);
        }
        if (rpcRetryCounter > RMInit::clientcommMaxRetry)
        {
            RMInit::logOut << "RPC call 'rpcgettypestructure' failed" << endl;
            throw r_Error(CLIENTCOMMUICATIONFAILURE);
        }
        rpcRetryCounter++;
    }
    while( thisResult == 0 );
    setRPCInactive();

    delete params;

    char*          typeStructure = 0;
    unsigned short rpcStatus     = thisResult->status;

    if( rpcStatus == 0 )
    {
        typeStructure = new char[strlen(thisResult->typeStructure) + 1];
        strcpy(typeStructure, thisResult->typeStructure);
        // this has to be freed by rpc:thisResult->typeStructure = 0;
    }

    // now the transfer structure can be freed
    XDRFREE(GetTypeStructureRes, thisResult);

    if( rpcStatus != 0 )
    {
        r_Error err;

        switch( rpcStatus )
        {
        case 2:
            err = r_Error( r_Error::r_Error_DatabaseClassUndefined );
            break;
        default:
            err = r_Error( r_Error::r_Error_TransferFailed );
        }
        throw err;
    }

    return typeStructure;
}


void
RpcClientComm::getMarRpcRepresentation( const r_GMarray* mar, RPCMarray*& rpcMarray,
                                        r_Data_Format initStorageFormat,
                                        const r_Base_Type *baseType)
{
    RMDBGENTER(2, RMDebug::module_clientcomm, "RpcClientComm", "getMarRpcRepresentation(...)");

    // allocate memory for the RPCMarray data structure and assign its fields
    rpcMarray                 = (RPCMarray*)mymalloc( sizeof(RPCMarray) );
    rpcMarray->domain         = mar->spatial_domain().get_string_representation();
    rpcMarray->cellTypeLength = mar->get_type_length();

    void* arrayData = NULL;
    r_ULong arraySize=0;

    if (initStorageFormat == r_Array)
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
}


void
RpcClientComm::freeMarRpcRepresentation( const r_GMarray* mar, RPCMarray* rpcMarray )
{
    if (rpcMarray->data.confarray_val != ((r_GMarray*)mar)->get_array())
    {
        delete [] rpcMarray->data.confarray_val;
    }
    free( rpcMarray->domain );
    free( rpcMarray );
}

void
RpcClientComm::getMDDCollection( r_Set< r_Ref_Any >& mddColl, unsigned int isQuery )
throw(r_Error)
{
    if(binding_h == NULL)
    {
        RMInit::logOut << endl << "RpcClientComm::getMDDCollection(mddColl, isQuery) ERROR: CONNECTION TO SERVER ALREADY CLOSED" << endl << endl;
        throw r_Error(CONNECTIONCLOSED);
    }
    RMDBGENTER(2, RMDebug::module_clientcomm, "RpcClientComm", "getMDDCollection(...)")
    unsigned short tileStatus=0;
    unsigned short mddStatus = 0;
//  r_Minterval    mddDomain;

    RMInit::logOut << endl;

    while( mddStatus == 0 ) // repeat until all MDDs are transferred
    {
        r_Ref<r_GMarray> mddResult;

        GetMDDRes* thisResult = 0;

        // Get spatial domain of next MDD
        setRPCActive();
        rpcRetryCounter = 0;
        do
        {
            thisResult = rpcgetnextmdd_1( &clientID, binding_h );

            if( !thisResult )
            {
                RMInit::logOut << endl << "WARNING: RPC NULL POINTER (rpcgetnextmdd_1)" << endl << endl;
                sleep(RMInit::clientcommSleep);
            }
            if (rpcRetryCounter > RMInit::clientcommMaxRetry)
            {
                RMInit::logOut << "RPC call 'rpcgetnextmdd' failed" << endl;
                throw r_Error(CLIENTCOMMUICATIONFAILURE);
            }
            rpcRetryCounter++;
        }
        while( thisResult == 0 );
        setRPCInactive();

        mddStatus = thisResult->status;

        RMDBGMIDDLE(2, RMDebug::module_clientcomm, "RpcClientComm", "read MDD")

        if( mddStatus == 2 )
        {
            RMInit::logOut << "Error: getMDDCollection(...) - no transfer collection or empty transfer collection" << endl;
            throw r_Error( r_Error::r_Error_TransferFailed );
        }
        else
            tileStatus = 0 ? 10 : 0;

        //  create r_Minterval
        //  mddDomain = r_Minterval( thisResult->domain );

        tileStatus = getMDDCore(mddResult, thisResult, isQuery);

        // finally, insert the r_Marray into the set

        mddColl.insert_element( mddResult, 1 );

        if( tileStatus == 0 ) // if this is true, we're done with this collection
            break;

        RMDBGMIDDLE(2, RMDebug::module_clientcomm, "RpcClientComm", "ok");

    } // end while( mddStatus == 0 )

    if(binding_h == NULL)
    {
        RMInit::logOut << endl << "RpcClientComm::getMDDCollection(mddColl, isQuery) ERROR: CONNECTION TO SERVER ALREADY CLOSED" << endl << endl;
        throw r_Error(CONNECTIONCLOSED);
    }

    unsigned short* rpcStatusPtr = 0;

    setRPCActive();
    rpcRetryCounter = 0;
    do
    {
        rpcStatusPtr = rpcendtransfer_1( &clientID, binding_h );

        if( !rpcStatusPtr )
        {
            RMInit::logOut << endl << "WARNING: RPC NULL POINTER (rpcendtransfer_1)" << endl << endl;
            sleep(RMInit::clientcommSleep);
        }
        if (rpcRetryCounter > RMInit::clientcommMaxRetry)
        {
            RMInit::logOut << "RPC call 'rpcendtransfer' failed" << endl;
            throw r_Error(CLIENTCOMMUICATIONFAILURE);
        }
        rpcRetryCounter++;
    }
    while( rpcStatusPtr == 0 );
    setRPCInactive();
}



void
RpcClientComm::getElementCollection( r_Set< r_Ref_Any >& resultColl )
throw(r_Error)
{
    if(binding_h == NULL)
    {
        RMInit::logOut << endl << "RpcClientComm::getElementCollection(resultColl) ERROR: CONNECTION TO SERVER ALREADY CLOSED" << endl << endl;
        throw r_Error(CONNECTIONCLOSED);
    }
    unsigned short rpcStatus = 0;

    RMInit::logOut << endl << " got set of type " << resultColl.get_type_structure() << endl;

    while( rpcStatus == 0 ) // repeat until all elements are transferred
    {
        GetElementRes* thisResult = 0;

        if(binding_h == NULL)
        {
            RMInit::logOut << endl << "RpcClientComm::getElementCollection(resultColl) ERROR: CONNECTION TO SERVER ALREADY CLOSED" << endl << endl;
            throw r_Error(CONNECTIONCLOSED);
        }

        setRPCActive();
        rpcRetryCounter = 0;
        do
        {
            thisResult = rpcgetnextelement_1( &clientID, binding_h );

            if( !thisResult )
            {
                RMInit::logOut << endl << "WARNING: RPC NULL POINTER (rpcgetnextelement_1)" << endl << endl;
                sleep(RMInit::clientcommSleep);
            }
            if (rpcRetryCounter > RMInit::clientcommMaxRetry)
            {
                RMInit::logOut << "RPC call 'rpcgetnextelement' failed" << endl;
                throw r_Error(CLIENTCOMMUICATIONFAILURE);
            }
            rpcRetryCounter++;
        }
        while( thisResult == 0 );
        setRPCInactive();

        rpcStatus = thisResult->status;

        if( rpcStatus == 2 )
        {
            RMInit::logOut << "Error: getElementCollection(...) - no transfer collection or empty transfer collection" << endl;
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
        {
            element = new r_Primitive( thisResult->data.confarray_val, (r_Primitive_Type*) elementType );
            r_Transaction::actual_transaction->add_object_list( r_Transaction::SCALAR, (void*) element );
        }
        break;

        case r_Type::COMPLEXTYPE1:
        case r_Type::COMPLEXTYPE2:
            element = new r_Complex(thisResult->data.confarray_val, (r_Complex_Type *)elementType);
            r_Transaction::actual_transaction->add_object_list(r_Transaction::SCALAR, (void *)element);
            break;

        case r_Type::STRUCTURETYPE:
        {
            element = new r_Structure( thisResult->data.confarray_val, (r_Structure_Type*) elementType );
            r_Transaction::actual_transaction->add_object_list( r_Transaction::SCALAR, (void*) element );
        }
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


        RMInit::logOut << " got element" << endl;

        // now the transfer structure of rpcgetnextmdd can be freed
        XDRFREE(GetElementRes, thisResult);

        // insert element into result set
        resultColl.insert_element( element, 1 );
    }


    if(binding_h == NULL)
    {
        RMInit::logOut << endl << "RpcClientComm::getElementCollection(resultColl) ERROR: CONNECTION TO SERVER ALREADY CLOSED" << endl << endl;
        throw r_Error(CONNECTIONCLOSED);
    }

    unsigned short* rpcStatusPtr = 0;

    setRPCActive();
    rpcRetryCounter = 0;
    do
    {
        rpcStatusPtr = rpcendtransfer_1( &clientID, binding_h );

        if( !rpcStatusPtr )
        {
            RMInit::logOut << endl << "WARNING: RPC NULL POINTER (rpcendtransfer_1)" << endl << endl;
            sleep(RMInit::clientcommSleep);
        }
        if (rpcRetryCounter > RMInit::clientcommMaxRetry)
        {
            RMInit::logOut << "RPC call 'rpcendtransfer' failed" << endl;
            throw r_Error(CLIENTCOMMUICATIONFAILURE);
        }
        rpcRetryCounter++;
    }
    while( rpcStatusPtr == 0 );
    setRPCInactive();
}

unsigned short
RpcClientComm::getMDDCore( r_Ref< r_GMarray > &mdd, GetMDDRes *thisResult, unsigned int isQuery )
throw( r_Error )
{
    //  create r_Minterval and oid
    r_Minterval mddDomain( thisResult->domain );
    r_OId       rOId     ( thisResult->oid );
    r_GMarray  *marray;

    //cout << "getMDDCore..." << endl;
    if( isQuery )
        marray = new( r_Database::actual_database, r_Object::transient, rOId ) r_GMarray();
    else
        marray = new( r_Database::actual_database, r_Object::read     , rOId ) r_GMarray();

    marray->set_spatial_domain( mddDomain );
    marray->set_type_by_name  ( thisResult->typeName );
    marray->set_type_structure( thisResult->typeStructure );

    r_Data_Format currentFormat = (r_Data_Format)(thisResult->currentFormat);
    currentFormat = r_Array;
    marray->set_current_format( currentFormat );

    r_Data_Format decompFormat;

    const r_Base_Type *baseType = marray->get_base_type_schema();

    // now the transfer structure of rpcgetnextmdd can be freed
    XDRFREE(GetMDDRes, thisResult);

    //RMDBGOUTFLUSH( 2, "domain " << mddDomain << " ... " )

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

        if(binding_h == NULL)
        {
            RMInit::logOut << endl << "RpcClientComm::getMDDCore(mdd, thisResult, isQuery) ERROR: CONNECTION TO SERVER ALREADY CLOSED" << endl << endl;
            throw r_Error(CONNECTIONCLOSED);
        }

        setRPCActive();
        rpcRetryCounter = 0;
        do
        {
            tileRes = rpcgetnexttile_1( &clientID, binding_h );

            if( !tileRes )
            {
                RMInit::logOut << endl << "WARNING: RPC NULL POINTER (rpcgetnexttile_1)" << endl << endl;
                sleep(RMInit::clientcommSleep);
            }
            if (rpcRetryCounter > RMInit::clientcommMaxRetry)
            {
                RMInit::logOut << "RPC call 'rpcgetnexttile' failed" << endl;
                throw r_Error(CLIENTCOMMUICATIONFAILURE);
            }
            rpcRetryCounter++;
        }
        while( tileRes == 0 );
        RMDBGIF(20, RMDebug::module_tools, "WAITRECEIVEDTILE", \
                RMInit::dbgOut << "Waiting 100 sec after receive tile\n" << std::endl; \
                sleep(100); \
                RMInit::dbgOut << "Continue now\n" << std::endl; );
        setRPCInactive();


        tileStatus = tileRes->status;

        if( tileStatus == 4 )
        {
            XDRFREE(GetTileRes, tileRes);

            RMInit::logOut << "Error: rpcGetNextTile(...) - no tile to transfer or empty transfer collection" << endl;
            throw r_Error( r_Error::r_Error_TransferFailed );
        }

        // take cellTypeLength for current MDD of the first tile
        if( tileCntr == 0 )
            marray->set_type_length( tileRes->marray->cellTypeLength );

        tileDomain = r_Minterval( tileRes->marray->domain );
        memCopyLen = tileDomain.cell_count() * marray->get_type_length(); // cell type length of the tile must be the same
        if (memCopyLen < tileRes->marray->data.confarray_len)
            memCopyLen = tileRes->marray->data.confarray_len; // may happen when compression expands
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
            XDRFREE(GetTileRes, tileRes);

            while( subStatus == 3 )
            {

                if(binding_h == NULL)
                {
                    RMInit::logOut << endl << "RpcClientComm::getMDDCore(mdd, thisResult, isQuery) ERROR: CONNECTION TO SERVER ALREADY CLOSED" << endl << endl;
                    throw r_Error(CONNECTIONCLOSED);
                }

                setRPCActive();
                rpcRetryCounter = 0;
                do
                {
                    tileRes = rpcgetnexttile_1( &clientID, binding_h );

                    if( !tileRes )
                    {
                        RMInit::logOut << endl << "WARNING: RPC NULL POINTER (rpcgetnexttile_1)" << endl << endl;
                        sleep(RMInit::clientcommSleep);
                    }
                    if (rpcRetryCounter > RMInit::clientcommMaxRetry)
                    {
                        RMInit::logOut << "RPC call 'rpcgetnexttile' failed" << endl;
                        throw r_Error(CLIENTCOMMUICATIONFAILURE);
                    }
                    rpcRetryCounter++;
                }
                while( tileRes == 0 );
                setRPCInactive();

                subStatus = tileRes->status;

                if( subStatus == 4 )
                {
                    XDRFREE(GetTileRes, tileRes);

                    RMInit::logOut << "Error: rpcGetNextTile(...) - no tile to transfer or empty transfer collection" << endl;
                    throw r_Error( r_Error::r_Error_TransferFailed );
                }

                // RMInit::logOut << "Status: " << subStatus << endl;
                // RMInit::logOut << "BlockOffset: " << blockOffset << " Size: " << tileRes->marray->data.confarray_len << endl;
                concatArrayData(tileRes->marray->data.confarray_val, tileRes->marray->data.confarray_len, memCopy, memCopyLen, blockOffset);
                XDRFREE(GetTileRes, tileRes);
            }

            tileStatus = subStatus;
            break;

        default: // tileStatus = 0,3 last block of the current tile

            // Tile arrives as one block.
            concatArrayData(tileRes->marray->data.confarray_val, tileRes->marray->data.confarray_len, memCopy, memCopyLen, blockOffset);
            // RMInit::logOut << "Internal size: " << tileRes->marray->data.confarray_len << endl;

            XDRFREE(GetTileRes, tileRes);
            break;
        }

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

    return tileStatus;
}

int RpcClientComm::concatArrayData( const char *source, unsigned long srcSize, char *&dest, unsigned long &destSize, unsigned long &destLevel )
{
    if (destLevel + srcSize > destSize)
    {
        // need to extend dest
        unsigned long newSize = destLevel + srcSize;
        char *newArray;

        // allocate a little extra if we have to extend
        newSize = newSize + newSize / 16;

//    RMDBGOUT( 1, "RpcClientComm::concatArrayData(): need to extend from " << destSize << " to " << newSize );

        if ((newArray = new char[newSize]) == NULL)
            return -1;

        memcpy(newArray, dest, destLevel);
        delete [] dest;
        dest = newArray;
        destSize = newSize;
    }

    memcpy(dest + destLevel, source, srcSize);
    destLevel += srcSize;

    return 0;
}


void RpcClientComm::triggerAliveSignal()
{
    aliveSignalRemaining = 1;

    sendAliveSignal();
}


void RpcClientComm::sendAliveSignal()
{
    if( aliveSignalRemaining && !checkRPCActive() )
    {
        aliveSignalRemaining = 0;

        unsigned long myID = getClientID();

        // tell the server I'm alive

        // determine my binding handle
        CLIENT* myHandle = getBindingHandle();

        unsigned short* rpcStatusPtr = 0;

        setRPCActive();
        rpcRetryCounter = 0;
        do
        {
            rpcStatusPtr = rpcalive_1( &myID, myHandle );

            if( !rpcStatusPtr )
            {
                RMInit::logOut << endl << "WARNING: RPC NULL POINTER (rpcalive_1)" << endl << endl;
                sleep(RMInit::clientcommSleep);
            }
            if (rpcRetryCounter > RMInit::clientcommMaxRetry)
            {
                RMInit::logOut << "RPC call 'rpcalive' failed" << endl;
                throw r_Error(CLIENTCOMMUICATIONFAILURE);
            }
            rpcRetryCounter++;
        }
        while( rpcStatusPtr == 0 );
        setRPCInactive();
        RMDBGONCE(3, RMDebug::module_clientcomm, "RpcClientComm", "sent alive signal")

#ifdef __VISUALC__
        timeKillEvent(timerid);
        timerid = timeSetEvent(ALIVEINTERVAL * 1000, 0, TimerProc, NULL, TIME_PERIODIC);
#else
        // Re-initialize the signal handler to point to this function
        signal( SIGALRM, aliveSignal );

        // Reset the alarm
        alarm( ALIVEINTERVAL );
#endif
    }
}


int RpcClientComm::setTransferFormat( r_Data_Format format, const char* formatParams )
{
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

    SetServerTransferParams* params = new SetServerTransferParams;

    params->clientID = getClientID();
    params->format = (unsigned short)format;
    if (transferFormatParams == NULL)
        params->formatParams = "";
    else
        params->formatParams = transferFormatParams;

    CLIENT* myHandle = getBindingHandle();

    unsigned short* rpcStatusPtr = 0;

    setRPCActive();
    rpcRetryCounter = 0;
    do
    {
        rpcStatusPtr = rpcsetservertransfer_1( params, myHandle );

        if (!rpcStatusPtr)
        {
            RMInit::logOut << endl << "WARNING: RPC NULL POINTER (rpcsetservertransfer_1)" << endl << endl;
            sleep(RMInit::clientcommSleep);
        }
        if (rpcRetryCounter > RMInit::clientcommMaxRetry)
        {
            RMInit::logOut << "RPC call 'rpcsetservertransfer' failed" << endl;
            throw r_Error(CLIENTCOMMUICATIONFAILURE);
        }
        rpcRetryCounter++;
    }
    while ( rpcStatusPtr == 0 );
    setRPCInactive();

    delete params;

    return (int)(*rpcStatusPtr);
}


int RpcClientComm::setStorageFormat( r_Data_Format format, const char *formatParams )
{
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

    SetServerTransferParams *params = new SetServerTransferParams;

    params->clientID = getClientID();
    params->format = (unsigned short)format;
    if (storageFormatParams == NULL)
        params->formatParams = "";
    else
        params->formatParams = storageFormatParams;

    CLIENT *myHandle = getBindingHandle();

    unsigned short *rpcStatusPtr = 0;

    setRPCActive();
    rpcRetryCounter = 0;
    do
    {
        rpcStatusPtr = rpcsetserverstorage_1( params, myHandle );
        if( !rpcStatusPtr )
        {
            RMInit::logOut << endl << "WARNING: RPC NULL POINTER (rpcsetserverstorage_1)" << endl << endl;
            sleep(RMInit::clientcommSleep);
        }
        if (rpcRetryCounter > RMInit::clientcommMaxRetry)
        {
            RMInit::logOut << "RPC call 'rpcsetserverstorage' failed" << endl;
            throw r_Error(CLIENTCOMMUICATIONFAILURE);
        }
        rpcRetryCounter++;
    }
    while ( rpcStatusPtr == NULL );
    setRPCInactive();

    delete params;

    return (int)(*rpcStatusPtr);
}


void
RpcClientComm::setRPCActive()
{
    rpcActive = 1;
}



void
RpcClientComm::setRPCInactive()
{
    rpcActive = 0;
    sendAliveSignal();
}


int
RpcClientComm::checkRPCActive()
{
    return rpcActive;
}

const char *
RpcClientComm::getExtendedErrorInfo()
throw(r_Error)
{
    if(binding_h == NULL)
    {
        RMInit::logOut << endl << "RpcClientComm::getMDDCollection(mddColl, isQuery) ERROR: CONNECTION TO SERVER ALREADY CLOSED" << endl << endl;
        throw r_Error(CONNECTIONCLOSED);
    }

    static char *errorInfo = NULL;

    GetExtendedErrorInfo *result=NULL;

    int dummy;

    if(errorInfo) delete[] errorInfo;

    setRPCActive();

    rpcRetryCounter = 0;
    do
    {

        result = rpcgeterrorinfo_1(&dummy, binding_h);
        if( !result )
        {
            RMInit::logOut << endl << "WARNING: RPC NULL POINTER (rpcalive_1)" << endl << endl;
            sleep(RMInit::clientcommSleep);
        }
        if (rpcRetryCounter > RMInit::clientcommMaxRetry)
        {
            RMInit::logOut << "RPC call 'rpcgeterrorinfo' failed" << endl;
            throw r_Error(CLIENTCOMMUICATIONFAILURE);
        }
        rpcRetryCounter++;

    }
    while(!result);

    setRPCInactive();

    errorInfo = new char[strlen(result->errorText)+1];
    strcpy(errorInfo,result->errorText);

    return errorInfo;
}


#define MAXMSG 512

int RpcClientComm::readWholeMessage(int socket,char *destBuffer,int buffSize)
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
            if(errno == EINTR) continue; // read was interrupted by signal

            return -1; // another error
        }
        totalLength+=redNow;

        if(destBuffer[totalLength-1]==0) break; // THE END
    }
    return totalLength;
}

int RpcClientComm::writeWholeMessage(int socket,char *destBuffer,int buffSize)
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
            if(errno == EINTR) continue; // read was interrupted by signal

            return -1; // another error
        }
        totalLength+=writeNow;

        if( totalLength==buffSize ) break; // THE END
    }
    return totalLength;
}

void
RpcClientComm::setMaxRetry(unsigned int newMaxRetry)
{
    RMInit::clientcommMaxRetry = newMaxRetry;
}

unsigned int
RpcClientComm::getMaxRetry()
{
    return RMInit::clientcommMaxRetry;
}


static void pause(int retryCount)
{
    unsigned int milisec = 50 + retryCount * 50;
    if(milisec > 1000) milisec = 1000;

    timeval tv;
    tv.tv_sec  = milisec / 1000;
    tv.tv_usec = milisec * 1000;

    select(0,NULL,NULL,NULL,&tv);
}

int
RpcClientComm::getFreeServer(unsigned short readOnly)
{
    //RMInit::logOut << "getFreeServer in"<<endl;
    for(int retryCount=0;; retryCount++)
    {
        try
        {
            executeGetFreeServer(readOnly);

            // if no error, we have the server, so break
            break;
        }
        catch(r_Error &e)
        {
            int errorno = e.get_errorno();
            //cerr<<"errorno="<<errorno;
            if(( errorno==801 || errorno==805 || errorno==806) && retryCount < RMInit::clientcommMaxRetry)
            {
                //cerr<<"  retry="<<retryCount<<endl;
                RMInit::logOut << "Connection to RasDaMan failed with " << errorno << ": retry " << retryCount << endl;
                pause(retryCount);
            }
            else throw;
        }
    }
    //RMInit::logOut << "getFreeServer out"<<endl;
    return 1;
}
int
RpcClientComm::executeGetFreeServer(unsigned short readOnly)
{
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
    sprintf(header,"POST getfreeserver HTTP/1.1\r\nAccept: text/plain\r\nUserAgent: RasClient/1.0\r\nAuthorization: ras %s\r\nContent-length:",identificationString);
    sprintf(body,"%s RPC %s %s",dataBase,(readOnly ? "ro":"rw"), myRasmgrID);
    sprintf(message,"%s %d\r\n\r\n%s",header,strlen(body)+1,body);

    struct protoent* getprotoptr = getprotobyname("tcp");

    struct hostent *hostinfo = gethostbyname(rasmgrHost);
    if(hostinfo==NULL)
    {
        RMInit::logOut << "Error locating RasMGR" << rasmgrHost <<" ("<<strerror(errno)<<')'<<endl;
        throw r_Error( r_Error::r_Error_ServerInvalid );
    }

    sockaddr_in internetSocketAddress;

    internetSocketAddress.sin_family=AF_INET;
    internetSocketAddress.sin_port=htons(rasmgrPort);
    internetSocketAddress.sin_addr=*(struct in_addr*)hostinfo->h_addr;


    int sock;
    bool ok = false;
    int retry;
    for(retry=0; retry<RMInit::clientcommMaxRetry * 40 ; retry++) // this has to be 5000 or so, now that counter is 120 default (later we'll make this better)
    {
        sock=socket(PF_INET,SOCK_STREAM,getprotoptr->p_proto);
        //cout<<"Socket="<<sock<<" protocol(tcp)="<<getprotoptr->p_proto<<endl;
        if(sock<0)   //cerr<<"getFreeServer: cannot open socket to RasMGR, ("<<strerror(errno)<<')'<<endl;
        {
            if(retry==0) RMInit::logOut << "getFreeServer: cannot open socket to RasMGR, ("<<strerror(errno)<<')'<<endl;
            sleep(RMInit::clientcommSleep);
            continue;
        }

        if(connect(sock,(struct sockaddr*)&internetSocketAddress,sizeof(internetSocketAddress)) < 0)
        {
            if(retry==0) RMInit::logOut <<"getFreeServer: Connection to RasMGR failed! ("<<strerror(errno)<<')'<<endl;
            close(sock);
            sleep(RMInit::clientcommSleep);
            continue;
        }

        ok = true;
        break;
    }
    if(retry)  RMInit::logOut << "getFreeServer: tried " << retry+1 << " times " <<endl;

    if( !ok )
    {
        RMInit::logOut << "getFreeServer: I give up, sorry" <<endl;
        close(sock);
        throw r_Error( r_Error::r_Error_ServerInvalid );
    }

    //write_to_server
    int nbytes=writeWholeMessage(sock,message,strlen(message)+1);

    if(nbytes<0)
    {
        RMInit::logOut << "Error writing message to RasMGR" << rasmgrHost << " ("<<strerror(errno)<<')' << endl;
        close(sock);
        throw r_Error( r_Error::r_Error_ServerInvalid );
    }

    //wait and read answer
    nbytes=readWholeMessage(sock,message,MAXMSG);
    close(sock);

    if(nbytes<0)
    {
        RMInit::logOut << "Error reading answer from RasMGR" << rasmgrHost <<" ("<<strerror(errno)<<')'<<endl;
        throw r_Error( r_Error::r_Error_ServerInvalid );
    }

    // and now, analize answer
    // first line is: HTTP/1.1 code answertext(CRLF)
    char *p=strstr(message," "); //looks for the first white space to locate status-code

    int statusCode=strtoul( p, (char **)NULL, 10);

    char *pEOL=strstr(p,"\r\n"); // locate CRLF
    if(!pEOL)
    {
        RMInit::logOut << "Invalid answer from RasMGR" << endl;
        throw r_Error( r_Error::r_Error_ServerInvalid );
    }

    if(statusCode==200)
    {
        // It's OK
        char *addr = strstr(message,"\r\n\r\n")+4; //looks for the address of server

        addr = strtok(addr," \r\n\t");       //isolates the RasMGR host name

        char *portString = strtok(NULL," \r\n\t"); //looks for the rpc_prognum, sended as string

        char *capab      = strtok(NULL," \r\n\t");

        if(portString && addr && capab)
        {
            strcpy(serverHost,addr);
            RPCIF_PARA= strtoul( portString, (char **)NULL, 0); // requires 0x if base16
            strcpy(capability,capab);
            //cout<<"Got server="<<serverHost<<" servnr=0x"<<hex<<RPCIF_PARA<<dec<<endl;
        }
        else
        {
            RMInit::logOut << "Invalid answer from RasMGR" << endl;
            throw r_Error( r_Error::r_Error_ServerInvalid );
        }

    }
    else
    {
        char *errText = strstr(message,"\r\n\r\n")+4;
        //cerr<<"cucu Error "<<errText<<endl;
        //RMInit::logOut << "Error "<<errText<< endl;

        int errorCode = strtoul(errText, (char**)NULL, 0);
        //cerr <<" throw "<< errorCode <<endl;

        switch(errorCode)
        {
        case 802:
        case 803:
        case 804:
            throw r_Error( r_Error::r_Error_AccesDenied,errorCode);
            break;
        case 801:
        case 805:
        case 806:
            throw r_Error( r_Error::r_Error_SystemOverloaded,errorCode);
            break;
        case 807:
            throw r_Error( r_Error::r_Error_DatabaseUnknown,errorCode);
            break;
        default :
            throw r_Error( r_Error::r_Error_General,808 );
            break;
        }
    }
    return 1;
}


const char*
RpcClientComm::getServerName()
{
    return serverHost;
}


int
RpcClientComm::connectToServer(unsigned short readOnly)
{
    disconnectFromServer(); // just to be sure
    getFreeServer(readOnly);

#if (defined(__VISUALC__) || defined(CYGWIN))
    RMInit::logOut << "Initializing the NT-RPC ..." << flush;
    rpc_nt_init();
    RMInit::logOut << "OK" << endl;
#endif
    RMInit::logOut << "Creating the binding..." << flush;
#if (defined(__VISUALC__) || defined(CYGWIN))
    binding_h = client_create( (char *) serverHost, RPCIF_PARA, RPCIFVERS, "tcp" );
#else
    binding_h = clnt_create( (char *) serverHost, RPCIF_PARA, RPCIFVERS, "tcp" );
    if( !binding_h )
    {
        cout << endl;
        clnt_pcreateerror("");
    }
#endif
    if( !binding_h )
    {
        RMInit::logOut << "FAILED" << endl;
        throw r_Error( r_Error::r_Error_ServerInvalid );
    }
    else
    {
        RMInit::logOut << "OK" << endl;
        serverUp = 1;
    }

    /* Default timeout can be changed using clnt_control() */
    // moved constant into raslib/riminit.hh, changed 25->3  -- PB 2005-sep-09
    static struct timeval timeout = { RPC_TIMEOUT, 0 };

#if (defined(__VISUALC__) || defined(CYGWIN))
    client_control( binding_h, CLGET_TIMEOUT, (char *)&timeout );
#else
    clnt_control( binding_h, CLGET_TIMEOUT, (char *)&timeout );
#endif
    RMInit::logOut << "Timeout: " << timeout.tv_sec << " sec " << timeout.tv_usec << " microsec" << endl;

    timeout.tv_sec  = RMInit::timeOut;
    timeout.tv_usec = 0;
#if (defined(__VISUALC__) || defined(CYGWIN))
    client_control( binding_h, CLSET_TIMEOUT, (char *)&timeout );
#else
    clnt_control( binding_h, CLSET_TIMEOUT, (char*)&timeout );
#endif

    RMInit::logOut << "Timeout set to " << timeout.tv_sec / 60. << " min." << endl;

#ifndef __VISUALC__
    // Install a signal handler for the alive signal
    signal( SIGALRM, aliveSignal );
#endif
    return 1;
}

int RpcClientComm::disconnectFromServer() throw()
{
    if(!binding_h)
    {
        RMInit::logOut << "Disconnect from server: no binding" << endl;
        return -1;
    }
    else
    {
        RMInit::logOut << "Disconnect from server: binding ok" << endl;
    }

#ifdef __VISUALC__
    RMInit::logOut << "Deleting the binding...";
    client_destroy(binding_h);
    RMInit::logOut << "OK" << endl;
    timeKillEvent(timerid);
    RMInit::logOut << "Deactivating the NT-RPC feature...";
    rpc_nt_exit();
    RMInit::logOut << "OK" << endl;
#else
    RMInit::logOut << "Deleting the binding...";
#ifdef CYGWIN
    client_destroy(binding_h);
    rpc_nt_exit();
#else
    clnt_destroy(binding_h);
#endif
    RMInit::logOut << "OK" << endl;

    if (storageFormatParams != NULL)
        free(storageFormatParams);
    storageFormatParams = NULL;

    if (transferFormatParams != NULL)
        free(transferFormatParams);
    transferFormatParams = NULL;

    binding_h = NULL;

    // suspend alarm timer for the periodical alive signal
    alarm( 0 );
#endif

    return 0;
}

// we will make this nicer after the D-day (this means when we change to para-proc
int messageDigest(const char *input,char *output,const char *mdName);

void
RpcClientComm::setUserIdentification(const char *userName, const char *plainTextPassword)
{
    char digest[33]="";
    messageDigest(plainTextPassword,digest,"MD5");
    sprintf(identificationString,"%s:%s",userName,digest);
}

int messageDigest(const char *input,char *output,const char *mdName)
{

    EVP_MD_CTX mdctx;
    const EVP_MD *md;
    unsigned int md_len, i;
    unsigned char md_value[100];

    OpenSSL_add_all_digests();

    md = EVP_get_digestbyname(mdName);

    if(!md) return 0;

    EVP_DigestInit(&mdctx, md);
    EVP_DigestUpdate(&mdctx,input, strlen(input));
    EVP_DigestFinal(&mdctx, md_value, &md_len);

    for(i = 0; i < md_len; i++) sprintf(output+i+i,"%02x", md_value[i]);

    return strlen(output);
}

unsigned long
RpcClientComm::getClientID() const
{
    return clientID;
}


CLIENT*
RpcClientComm::getBindingHandle() const
{
    return binding_h;
}


void RpcClientComm::setTimeoutInterval(int seconds) { };

int  RpcClientComm::getTimeoutInterval()
{
    return 0;
};

