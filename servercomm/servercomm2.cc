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
 * SOURCE: servercomm2.cc
 *
 * MODULE: servercomm
 * CLASS:  ServerComm
 *
 * PURPOSE:
 *
 * COMMENTS:
 *  - FIXME: catch exceptions in all operations
 *  - return values & their meaning, see servercomm.hh
 *  - FIXME: "client not registered" delivers sometimes 1, sometimes 3
 *
*/

#include "mymalloc/mymalloc.h"

static const char rcsid[] = "@(#)servercomm2, ServerComm: $Id: servercomm2.cc,v 1.121 2005/09/07 23:23:31 rasdev Exp $";

// after some time please take this and everything related to it out (26.06.2001)
#define ANDREAS_2306

#include <iostream>
#ifdef __APPLE__
#include <sys/malloc.h>
#else
#include <malloc.h>
#endif
#include <string.h>
#include <math.h>      // for log(), exp(), floor()
#include <ctime>         // time
#include <iomanip>

#ifdef PURIFY
#include <purify.h>
#endif

#ifdef RMANBENCHMARK
// to control GenericCompression::decompTimer and GenericCompression::compTimer
#include "tilemgr/gencompression.hh"
#endif

#ifdef SOLARIS
#define PORTMAP        // define to use function declarations for old interfaces
#include <rpc/rpc.h>

extern int _rpcpmstart;

// function prototype with C linkage
extern "C" int gethostname(char *name, int namelen);
#else  // HPUX
#include <rpc/rpc.h>
#endif

#include <rpc/pmap_clnt.h>

#include "raslib/rmdebug.hh"
#include "debug.hh"
#include "raslib/rminit.hh"
#include "raslib/error.hh"
#include "raslib/minterval.hh"
#include "raslib/mddtypes.hh"
#include "raslib/basetype.hh"
#include "raslib/endian.hh"
#include "raslib/odmgtypes.hh"
#include "raslib/parseparams.hh"

#include "servercomm/servercomm.hh"
#include "catalogmgr/typefactory.hh"

#include "mddmgr/mddcoll.hh"
#include "mddmgr/mddobj.hh"
#include "mddmgr/mddcolliter.hh"
#include "tilemgr/tile.hh"

#include "relcatalogif/mddtype.hh"
#include "relcatalogif/mdddomaintype.hh"
#include "relcatalogif/settype.hh"
#include "reladminif/eoid.hh"

#include "qlparser/qtmdd.hh"
#include "qlparser/qtatomicdata.hh"
#include "qlparser/qtcomplexdata.hh"
#include "qlparser/qtintervaldata.hh"
#include "qlparser/qtmintervaldata.hh"
#include "qlparser/qtpointdata.hh"
#include "qlparser/qtstringdata.hh"

// console output describing successful/unsuccessful actions
#define MSG_OK          "ok"
#define MSG_FAILED      "failed"

// include and extern declarations for the query parsing
#include "qlparser/querytree.hh"
extern int           yyparse(void *);
extern void          yyreset();
#ifdef NOPRE
char* ppInBuf = 0;
char* ppOutBuf = 0;
void ppreset()
{
    RMInit::logOut << "Error: Preprocessor not compiled in." << std::endl;
    RMInit::dbgOut << "Error: Preprocessor not compiled in." << std::endl;
    throw r_Error(ILLEGALSTATEREACHED);
}

int ppparse()
{
    RMInit::logOut << "Error: Preprocessor not compiled in." << std::endl;
    RMInit::dbgOut << "Error: Preprocessor not compiled in." << std::endl;
    throw r_Error(ILLEGALSTATEREACHED);
}
#else
extern char*         ppInBuf;
extern char*         ppOutBuf;
extern void          ppreset();
extern int       ppparse();
#endif
extern bool          udfEnabled;

extern QueryTree*    parseQueryTree;
extern ParseInfo*    parseError;
extern char*         beginParseString;
extern char*         iterParseString;
extern unsigned long maxTransferBufferSize;
extern char*         dbSchema;

MDDColl*      mddConstants=0;

ServerComm::ClientTblElt* currentClientTblElt=0;

// Once again a function prototype. The first one is for the RPC dispatcher
// function located in the server stub file rpcif_svc.c and the second one
// is for the garbage collection function pointed to by the signal handler.
extern "C"
{
    // static void rpcif_1( struct svc_req*, register SVCXPRT* );
    char* rpcif_1( struct svc_req*, register SVCXPRT* );
    void garbageCollection( int );
}

// This is needed in httpserver.cc
char globalHTTPSetTypeStructure[4096];

// constant for clientID
const char* ServerComm::HTTPCLIENT = "HTTPClient";

///ensureTileFormat returns the following:
const int ServerComm::ENSURE_TILE_FORMAT_OK=0;
const int ServerComm::ENSURE_TILE_FORMAT_BAD=-1;

/*************************************************************************
 * Method name...: openDB( unsigned long callingClientId,
 *                         const char*   dbName ,
 *                         const char*   userName )
 ************************************************************************/
unsigned short
ServerComm::openDB( unsigned long callingClientId,
                    const char* dbName,
                    const char* userName )
{
    RMDBGENTER( 4, RMDebug::module_servercomm, "ServerComm",  "openDB" )

    unsigned short returnValue=0;

    RMInit::logOut << "Request: 'open DB', name = " << dbName << "'..." << std::flush;

    ClientTblElt* context = getClientContext( callingClientId );

    if( context != 0 )
    {
        // release transfer collection/iterator
        context->releaseTransferStructures();

        // open the database
        try
        {
            context->database.open( dbName );
        }
        catch(r_Error& err)
        {
            if(err.get_kind() == r_Error::r_Error_DatabaseUnknown)
            {
                RMInit::logOut << "Error: database does not exist." << endl;
                returnValue = 2;
            }
            else if(err.get_kind() == r_Error::r_Error_DatabaseOpen)
            {
                // ignore re-open to be fault tolerant -- PB 2004-dec-16
                // RMInit::logOut << "Error: database is already open." << endl;
                returnValue = 3;
            }
            else
            {
                RMInit::logOut << "Error: exception " << err.get_errorno() << ": " << err.what() << endl;
                //should be something else.  but better than no message about the problem at all
                returnValue = 2;
            }
        }

        if( returnValue == 0 )
        {
            // database was successfully opened, so assign db and user name
            delete[] context->baseName;
            context->baseName = new char[strlen( dbName )+1];
            strcpy( context->baseName, dbName );

            delete[] context->userName;
            context->userName = new char[strlen( userName )+1];
            strcpy( context->userName, userName );

            RMInit::logOut << MSG_OK << std::endl;
        }

        context->release();

        // ignore "already open" error to be more fault tolerant -- PB 2004-dec-16
        if( returnValue == 3 )
        {
            RMInit::logOut << "Warning: database already open for user '" << userName << "', ignoring command." << endl;
            returnValue = 0;
        }
    }
    else
    {
        RMInit::logOut << "Error: client not registered." << std::endl;
        returnValue = 1;
    }

    RMDBGEXIT( 4, RMDebug::module_servercomm, "ServerComm",  "openDB" )
    return returnValue;
}



/*************************************************************************
 * Method name...: closeDB( unsigned long callingClientId )
 ************************************************************************/
unsigned short
ServerComm::closeDB( unsigned long callingClientId )
{
    RMDBGENTER( 4, RMDebug::module_servercomm, "ServerComm",  "closeDB" )
    unsigned short returnValue;

    RMInit::logOut << "Request: 'close DB'..." << std::flush;

    ClientTblElt* context = getClientContext( callingClientId );

    if( context != 0 )
    {
        // release transfer collection/iterator
        context->releaseTransferStructures();

        // If the current transaction belongs to this client, abort it.
        if( transactionActive == callingClientId )
        {
            RMInit::logOut << "Warning: transaction is open; aborting this transaction..." << std::flush;

            context->transaction.abort();
            transactionActive = 0;
        }

        // close the database
        context->database.close();

        // reset database name
        delete[] context->baseName;
        context->baseName = new char[5];
        strcpy( context->baseName, "none" );

        returnValue = 0;

        context->release();

#ifdef PURIFY
        purify_new_leaks();
#endif

        RMInit::logOut << MSG_OK << endl;
    }
    else
    {
        RMInit::logOut << "Error: client not registered." << std::endl;
        returnValue = 1;
    }

    RMDBGEXIT( 4, RMDebug::module_servercomm, "ServerComm",  "closeDB" )
    return returnValue;
}



/*************************************************************************
 * Method name...: createDB( char* name )
 ************************************************************************/
unsigned short
ServerComm::createDB( char* name )
{
    unsigned short returnValue;

    // FIXME: what about client id? -- PB 2005-aug-27

    RMInit::logOut << "Request: 'create DB', name = " << name << "'..." << std::flush;

    DatabaseIf* tempDbIf = new DatabaseIf;

    // create the database
    try
    {
        tempDbIf->createDB( name, dbSchema );
        RMInit::logOut << MSG_OK << endl;
    }
    catch(r_Error& myErr)
    {
        RMInit::logOut << "Error: exception " << myErr.get_errorno() << ": " << myErr.what() << std::endl;
    }
    catch(std::bad_alloc)
    {
        RMInit::logOut << "Error: cannot allocate memory." << std::endl;
        throw;
    }
    catch(...)
    {
        RMInit::logOut << std::endl << "Error: Unspecified exception." << std::endl;
    }

    delete tempDbIf;

    // FIXME: set proper return value on failure (update .hh!) -- PB 2005-aug-27
    returnValue = 0;

    return returnValue;
}



/*************************************************************************
 * Method name...: destroyDB( char* name )
 ************************************************************************/
unsigned short
ServerComm::destroyDB( char* name )
{
    // Note: why no check for client id here? -- PB 2005-aug-25

    unsigned short returnValue = 0;

    RMInit::logOut << "Request: 'destroy DB', name = " << name << "'..." << std::flush;

    DatabaseIf* tempDbIf = new DatabaseIf;

    // begin a temporary transaction because persistent data (as databases)
    // can only be manipulated within active transactions.
    // tempTaIf->begin(tempDbIf);

    // destroy the database
    tempDbIf->destroyDB( name );

    // commit the temporary transaction
    // tempTaIf->commit();

    delete tempDbIf;

    RMInit::logOut << MSG_OK << std::endl;

    return returnValue;
}



/*************************************************************************
 * Method name...: beginTA( unsigned long callingClientId )
 ************************************************************************/
unsigned short
ServerComm::beginTA( unsigned long callingClientId,
                     unsigned short readOnly )
{
    RMDBGENTER( 4, RMDebug::module_servercomm, "ServerComm",  "beginTA" )
    unsigned short returnValue;

    RMInit::logOut << "Request: 'begin TA', mode = " << ( readOnly ? "read" : "write" ) << "..." << std::flush;

    ClientTblElt* context = getClientContext( callingClientId );

    if( context == 0 )
    {
        RMInit::logOut << "Error: client not registered." << std::endl;
        returnValue = 1;
    }
    else if ( transactionActive )
    {
        RMInit::logOut << "Error: transaction already active." << std::endl;
        returnValue = 2;
        context->release();
    }
    else
    {
        // release transfer collection/iterator
        context->releaseTransferStructures();

#ifdef RMANBENCHMARK
        if( RManBenchmark > 0 )
            context->taTimer = new RMTimer("ServerComm", "transaction");
#endif
        try
        {
            // start the transaction
            context->transaction.begin( &(context->database), readOnly );
            RMInit::logOut << MSG_OK << endl;
        }
        catch(r_Error& err)
        {
            RMInit::logOut << "Error: exception " << err.get_errorno() << ": " << err.what() << std::endl;
            context->release();
            throw;
        }

        // lock the semaphor
        transactionActive = callingClientId;
        returnValue = 0;

        context->release();
    }

    RMDBGEXIT( 4, RMDebug::module_servercomm, "ServerComm",  "beginTA" )
    return returnValue;
}



/*************************************************************************
 * Method name...: commitTA( unsigned long callingClientId )
 ************************************************************************/
unsigned short
ServerComm::commitTA( unsigned long callingClientId )
{
    RMDBGENTER( 4, RMDebug::module_servercomm, "ServerComm",  "commitTA" )
    unsigned short returnValue;

    ClientTblElt* context = getClientContext( callingClientId );

    RMInit::logOut << "Request: 'commit TA'..." << std::flush;

    if( context != 0 )
    {

#ifdef RMANBENCHMARK
        RMTimer* commitTimer = 0;
        if( RManBenchmark > 0 )
            commitTimer = new RMTimer("ServerComm", "commit");
#endif

        // release transfer collection/iterator within the transaction they are created
        context->releaseTransferStructures();

        // commit the transaction
        context->transaction.commit();

        // unlock the semaphor
        transactionActive = 0;

        returnValue = 0;

#ifdef RMANBENCHMARK
        if( commitTimer ) delete commitTimer;
#endif

        context->release();

        RMInit::logOut << MSG_OK << endl;
    }
    else
    {
        RMInit::logOut << "Error: client not registered." << std::endl;
        returnValue = 1;
    }

#ifdef RMANBENCHMARK
    if( context->taTimer )
        delete context->taTimer;
    context->taTimer = 0;
#endif

    RMDBGEXIT( 4, RMDebug::module_servercomm, "ServerComm",  "commitTA" )
    return returnValue;
}



/*************************************************************************
 * Method name...: abortTA( unsigned long callingClientId )
 ************************************************************************/
unsigned short
ServerComm::abortTA( unsigned long callingClientId )
{
    RMDBGENTER( 4, RMDebug::module_servercomm, "ServerComm",  "abortTA" )
    unsigned short returnValue;

    RMInit::logOut << "Request: 'abort TA'..." << std::flush;

    ClientTblElt* context = getClientContext( callingClientId );

    if( context != 0 )
    {
        // release transfer collection/iterator within the transaction they are created
        context->releaseTransferStructures();

        // abort the transaction
        context->transaction.abort();

        // unlock the semaphor
        transactionActive = 0;

        returnValue = 0;

        context->release();

        RMInit::logOut << MSG_OK << endl;
    }
    else
    {
        RMInit::logOut << "Error: client not registered." << std::endl;
        returnValue = 1;
    }

#ifdef RMANBENCHMARK
    if( context->taTimer ) delete context->taTimer;
    context->taTimer = 0;
#endif

    RMDBGEXIT( 4, RMDebug::module_servercomm, "ServerComm",  "abortTA" )
    return returnValue;
}


/*************************************************************************
 * Method name...: isTAOpen()
 * as right now only one transaction can be active per server,
 * we only have to check the sema.
 * returns:
 *  true    iff a transaction is open
 ************************************************************************/
bool
ServerComm::isTAOpen( unsigned long callingClientId )
{
    RMDBGENTER( 4, RMDebug::module_servercomm, "ServerComm",  "isTAOpen" )

    RMInit::logOut << "Request: 'is TA open'..." << std::flush;

    bool returnValue = transactionActive;

    RMInit::logOut << MSG_OK << (transactionActive?"yes.":"no.") << endl;

    RMDBGEXIT( 4, RMDebug::module_servercomm, "ServerComm",  "isTAOpen" )
    return returnValue;
}



unsigned short
ServerComm::insertColl( unsigned long callingClientId,
                        const char*   collName,
                        const char*   typeName,
                        r_OId&        oid       )
{
    RMDBGENTER( 4, RMDebug::module_servercomm, "ServerComm",  "insertColl" )
    unsigned short returnValue = 0;

    RMInit::logOut << "Request: 'insert collection', collection name = '" << collName << "', type = '" << typeName << "'..." << std::flush;

    ClientTblElt* context = getClientContext( callingClientId );

    if( context != 0 )
    {
        // delete old transfer structures
        context->releaseTransferStructures();

        //
        // create the collenction
        //

        // get collection type
        CollectionType* collType = (CollectionType*)TypeFactory::mapSetType( typeName );

        if( collType )
        {
            try
            {
                MDDColl* coll = MDDColl::createMDDCollection(collName, OId(oid.get_local_oid()), collType);
                delete coll;
                RMInit::logOut << MSG_OK << endl;
            }
            catch( r_Error& obj )
            {
                if (obj.get_kind() == r_Error::r_Error_NameNotUnique)
                {
                    RMInit::logOut << "Error: collection exists already." << std::endl;
                    returnValue = 3;
                }
                else
                {
                    RMInit::logOut << "Error: cannot create collection: " << obj.get_errorno() << " " << obj.what() << std::endl;
                    //this should be another code...
                    returnValue = 3;
                }
            }

        }
        else
        {
            RMInit::logOut << "Error: unknown collection type: '" << typeName << "'." << std::endl;
            returnValue = 2;
        }

        //
        // done
        //

        context->release();
    }
    else
    {
        RMInit::logOut << "Error: client not registered." << std::endl;
        returnValue = 1;
    }

    RMDBGEXIT( 4, RMDebug::module_servercomm, "ServerComm",  "insertColl" )
    return returnValue;
}



/*************************************************************************
 * Method name...: deleteCollByName( unsigned long callingClientId,
 *                                      const char*   collName )
 ************************************************************************/
unsigned short
ServerComm::deleteCollByName( unsigned long callingClientId,
                              const char*   collName         )
{
    RMDBGENTER( 4, RMDebug::module_servercomm, "ServerComm", "deleteCollByName(" << callingClientId << ", " << collName << ")")
    unsigned short returnValue;

    RMInit::logOut << "Request: 'delete collection by name', name = '" << collName << "'..." << std::flush;

    ClientTblElt* context = getClientContext( callingClientId );

    if( context != 0 )
    {
        // delete old transfer structures
        context->releaseTransferStructures();

        //
        // delete collenction
        //

        // delete root object with collection name
        if (MDDColl::dropMDDCollection(collName))
        {
            RMDBGMIDDLE( 4, RMDebug::module_servercomm, "ServerComm", "collection dropped")
            RMInit::logOut << MSG_OK << std::endl;
            returnValue = 0;
        }
        else
        {
            RMDBGMIDDLE( 4, RMDebug::module_servercomm, "ServerComm", "did not drop collection")
            RMInit::logOut << "Error: collection does not exist." << std::endl;
            returnValue = 2;
        }

        //
        // done
        //

        context->release();
    }
    else
    {
        RMInit::logOut << "Error: client not registered." << std::endl;
        returnValue = 1;
    }

    RMDBGEXIT( 4, RMDebug::module_servercomm, "ServerComm", "deleteCollByName(" << callingClientId << ", " << collName << ") " << returnValue)
    return returnValue;
}



unsigned short
ServerComm::deleteObjByOId( unsigned long callingClientId,
                            r_OId&        oid              )
{
    RMDBGENTER( 4, RMDebug::module_servercomm, "ServerComm", "deleteObjByOId(" << callingClientId << ", " << oid << ")");
    unsigned short returnValue;

    RMInit::logOut << "Request: 'delete MDD by OID', oid = '" << oid << "'..." << std::flush;

    ClientTblElt* context = getClientContext( callingClientId );

    if( context != 0 )
    {
        // delete old transfer structures
        context->releaseTransferStructures();

        // determine type of object
        OId oidIf( oid.get_local_oid() );

        RMDBGMIDDLE( 4, RMDebug::module_servercomm, "ServerComm", "OId of object " << oidIf)
        OId::OIdType objType = oidIf.getType();

        switch( objType )
        {
        case OId::MDDOID:
            // FIXME: why not deleted?? -- PB 2005-aug-27
            RMInit::logOut << "found MDD object; NOT deleted yet..." << MSG_OK << std::endl;
            RMDBGMIDDLE( 4, RMDebug::module_servercomm, "ServerComm", "not deleting mdd object")
            returnValue = 0;
            break;
        case OId::MDDCOLLOID:
            RMInit::logOut << "deleting collection..." << std::flush;
            // delete root object with collection name
            if (MDDColl::dropMDDCollection(oidIf))
            {
                RMInit::logOut << MSG_OK << std::endl;
                RMDBGMIDDLE( 4, RMDebug::module_servercomm, "ServerComm", "deleted mdd coll")
                returnValue = 0;
            }
            else
            {
                RMInit::logOut << "Error: Collection does not exist." << std::endl;
                RMDBGMIDDLE( 4, RMDebug::module_servercomm, "ServerComm", "did not delete mdd coll")
                returnValue = 2;
            }
            break;
        default:
            RMInit::logOut << "Error: object has unknown type: " << objType << std::endl;
            returnValue = 2;
        }

        //
        // done
        //

        context->release();
    }
    else
    {
        RMInit::logOut << "Error: client not registered." << std::endl;
        returnValue = 1;
    }

    RMDBGEXIT( 4, RMDebug::module_servercomm, "ServerComm", "deleteObjByOId(" << callingClientId << ", " << oid << ")" << returnValue)
    return returnValue;
}



unsigned short
ServerComm::removeObjFromColl( unsigned long callingClientId,
                               const char*   collName,
                               r_OId&        oid              )
{
    RMDBGENTER( 4, RMDebug::module_servercomm, "ServerComm", "removeObjFromColl(" << callingClientId << ", " << collName << ", " << oid << ")")
    unsigned short returnValue;

    RMInit::logOut << "Request: 'remove MDD from collection', collection name = '" << collName << "', oid = '" << oid << "'..." << std::flush;

    ClientTblElt* context = getClientContext( callingClientId );

    if( context != 0 )
    {
        // delete old transfer structures
        context->releaseTransferStructures();

        OId oidIf( oid.get_local_oid() );

        RMDBGMIDDLE( 4, RMDebug::module_servercomm, "ServerComm", "mdd object oid " << oidIf)

        // open collection
        MDDColl* coll = 0;

        try
        {
            coll = MDDColl::getMDDCollection(collName);
            RMDBGMIDDLE( 4, RMDebug::module_servercomm, "ServerComm", "retrieved mdd coll")
        }
        catch(r_Error& obj)
        {
            // collection name invalid
            if (obj.get_kind() == r_Error::r_Error_ObjectUnknown)
            {
                RMInit::logOut << "Error: collection not found." << std::endl;
                returnValue = 2;
            }
            else
            {
                RMInit::logOut << "Error " << obj.get_errorno() << ": " << obj.what() << std::endl;
                // there should be another return code
                returnValue = 2;
            }
            coll = NULL;
        }
        catch(std::bad_alloc)
        {
            RMInit::logOut << "Error: cannot allocate memory." << std::endl;
            throw;
        }
        catch(...)
        {
            // collection name invalid
            RMInit::logOut << "Error: unspecified exception." << std::endl;
            returnValue = 2;
        }

        if( coll )
        {
            if (coll->isPersistent())
            {
                RMDBGMIDDLE( 4, RMDebug::module_servercomm, "ServerComm", "retrieved persistent mdd coll")

                OId collId;
                coll->getOId(collId);
                RMDBGMIDDLE( 4, RMDebug::module_servercomm, "ServerComm", "mdd coll oid " << collId)
                MDDColl::removeMDDObject(collId, oidIf);

                // no error management yet -> returnValue = 3

                RMInit::logOut << MSG_OK << std::endl;
                returnValue = 0;

                delete coll;
            }
        }

        //
        // done
        //

        context->release();
    }
    else
    {
        RMInit::logOut << "Error: client not registered." << std::endl;
        returnValue = 1;
    }

    RMDBGEXIT( 4, RMDebug::module_servercomm, "ServerComm", "removeObjFromColl(" << callingClientId << ", " << collName << ", " << oid << ") " << returnValue)
    return returnValue;
}


unsigned short
ServerComm::insertMDD( unsigned long  callingClientId,
                       const char*    collName,
                       RPCMarray*     rpcMarray,
                       const char*    typeName,
                       r_OId&         oid              )
{
    RMDBGENTER( 4, RMDebug::module_servercomm, "ServerComm",  "insertMDD" )
    unsigned short returnValue = 0;

    RMInit::logOut << "Request: 'insert MDD type', type = '" << typeName << "', collection = '" << collName << "'..." << std::flush;

    ClientTblElt* context = getClientContext( callingClientId );
    r_Data_Format myDataFmt = r_Array;
    r_Data_Format myCurrentFmt = r_Array;

    if( context != 0 )
    {
        // delete old transfer structures
        context->releaseTransferStructures();

        //
        // insert the object into the collection
        //

        // Determine the type of the MDD to be inserted.
        const MDDType* mddType = TypeFactory::mapMDDType( typeName );
        if( mddType )
        {
            if( mddType->getSubtype() != MDDType::MDDONLYTYPE )
            {
                //
                // open the collection
                //

                MDDColl* collection=0;
                MDDColl* almost = 0;

                try
                {
                    almost = MDDColl::getMDDCollection( collName );
                    if (almost->isPersistent())
                        collection = (MDDColl*)almost;
                    else
                    {
                        RMInit::logOut << "Error: inserting into system collection is illegal." << std::endl;
                        context->release(); //!!!
                        throw r_Error(SYSTEM_COLLECTION_NOT_WRITABLE);
                    }
                }
                catch(std::bad_alloc)
                {
                    RMInit::logOut << "Error: cannot allocate memory." << std::endl;
                    context->release(); //!!!
                    throw;
                }
                catch (r_Error& err)
                {
                    RMInit::logOut << "Error " << err.get_errorno() << ": " << err.what() << std::endl;
                    returnValue = 5;
                    context->release(); //!!!
                    throw;
                }
                catch(...)
                {
                    returnValue = 5;
                    RMInit::logOut << "Error: unspecific exception during collection read." << std::endl;
                    context->release();
                    return returnValue;
                }

                //
                // check MDD and collection type for compatibility
                //

                r_Minterval   domain( rpcMarray->domain );

                RMDBGIF(4, RMDebug::module_servercomm, "ServerComm", \
                        char* collTypeStructure = collection->getCollectionType()->getTypeStructure(); \
                        char* mddTypeStructure  = mddType->getTypeStructure(); \
                        RMInit::dbgOut << std::endl << "Collection type structure.: " << collTypeStructure << std::endl \
                        << "MDD type structure........: " << mddTypeStructure << std::endl \
                        << "MDD domain................: " << domain << std::endl; \
                        free( collTypeStructure ); \
                        free( mddTypeStructure ); )

                if( !mddType->compatibleWithDomain( &domain ) )
                {
                    // free resources
                    collection->releaseAll();
                    delete collection;

                    // return error
                    returnValue = 4;
                    RMInit::logOut << "Error: MDD type is not compatible wrt. its domain: " << domain << std::endl;

                    context->release();

                    return returnValue;
                }

                if( !collection->getCollectionType()->compatibleWith( mddType ) )
                {
                    // free resources
                    collection->releaseAll();
                    delete collection;

                    // return error
                    returnValue = 3;
                    RMInit::logOut << "Error: MDD and collection types are incompatible." << std::endl;

                    context->release();

                    return returnValue;
                }

                //
                // create persistent MDD object
                //

                MDDBaseType*  mddBaseType = (MDDBaseType*)mddType;
                char*         dataPtr  = rpcMarray->data.confarray_val;
                unsigned long dataSize = rpcMarray->data.confarray_len;
                // reset data area from rpc structure so that it is not deleted
                // deletion is done by TransTile resp. Tile
                rpcMarray->data.confarray_len = 0;
                rpcMarray->data.confarray_val = 0;
                int           getMDDData=0;
                const BaseType*     baseType = mddBaseType->getBaseType();
                unsigned long byteCount  = domain.cell_count() * rpcMarray->cellTypeLength;
                //r_Data_Format storageFormat = (r_Data_Format)(rpcMarray->storageFormat);

                MDDObj*   mddObj=0;
                StorageLayout ms;
                ms.setTileSize(StorageLayout::DefaultTileSize);
                ms.setIndexType(StorageLayout::DefaultIndexType);
                ms.setTilingScheme(StorageLayout::DefaultTilingScheme);
                if (domain.dimension() == StorageLayout::DefaultTileConfiguration.dimension())
                    ms.setTileConfiguration(StorageLayout::DefaultTileConfiguration);

                try
                {
                    mddObj = new MDDObj(mddBaseType, domain, OId(oid.get_local_oid()), ms);
                }
                catch(std::bad_alloc)
                {
                    RMInit::logOut << "Error: cannot allocate memory." << std::endl;
                    context->release(); //!!!
                    throw;
                }
                catch (r_Error& obj)
                {
                    RMInit::logOut << "Error " << obj.get_errorno() << ": " << obj.what() << std::endl;
                    context->release(); //!!!
                    throw;
                }
                catch(...)
                {
                    returnValue = 6;
                    RMInit::logOut << "Error: unspecific exception during creation of persistent object." << std::endl;

                    context->release();

                    return returnValue;
                }

                myDataFmt = (r_Data_Format)(rpcMarray->storageFormat);
                myCurrentFmt = (r_Data_Format)(rpcMarray->currentFormat);
                RMInit::dbgOut  << "oid " << oid
                                << ", domain " << domain
                                << ", cell length " << rpcMarray->cellTypeLength
                                << ", data size " << dataSize
                                << ", rpc storage " << myDataFmt
                                << ", rpc transfer " << myCurrentFmt << " " << std::flush;

                // store in the specified storage format; the current tile format afterwards will be the
                // requested format if all went well, but use the (new) current format to be sure.
                // Don't repack here, however, because it might be retiled before storage.
                if(ensureTileFormat(myCurrentFmt, myDataFmt, domain,
                                    baseType, dataPtr, dataSize, 0, 1, context->storageFormatParams) != ENSURE_TILE_FORMAT_OK)
                {
                    //FIXME returnValue
                    returnValue = 6;
                    RMInit::logOut << "Error: illegal tile format for creating object." << std::endl;

                    context->release();

                    return returnValue;
                }

                // if compressed, getMDDData is != 0
                if (myCurrentFmt != r_Array)
                    getMDDData = dataSize;

                // This should check the compressed size rather than the raw data size
                if( RMInit::tiling && dataSize > StorageLayout::DefaultTileSize )
                {
                    r_Range edgeLength = (r_Range)floor(exp((1/(r_Double)domain.dimension())*
                                                            log((r_Double)StorageLayout::DefaultTileSize/rpcMarray->cellTypeLength)));

                    if (edgeLength <1)
                        edgeLength=1;

                    r_Minterval tileDom( domain.dimension() );
                    for( int i=0; i<tileDom.dimension(); i++ )
                        tileDom << r_Sinterval( (r_Range)0, (r_Range)(edgeLength-1) );

                    Tile* entireTile = 0;
                    myCurrentFmt = r_Array;
                    entireTile = new Tile( domain, baseType, dataPtr, getMDDData, myDataFmt );

                    vector< Tile *>* tileSet = entireTile->splitTile( tileDom );
                    if (entireTile->isPersistent())
                        entireTile->setPersistent(0);
                    delete entireTile;

                    RMInit::logOut << "creating " << tileSet->size() << " tile(s)..." << std::flush;

                    for( vector<Tile*>::iterator iter = tileSet->begin(); iter != tileSet->end(); iter++ )
                        mddObj->insertTile( *iter );

                    // delete the vector again
                    delete tileSet;
                }
                else
                {
                    Tile* tile = 0;

                    tile = new Tile( domain, baseType, dataPtr, getMDDData, myDataFmt );
                    RMInit::dbgOut << "insertTile created new TransTile (" << myDataFmt << "), ";

                    RMInit::dbgOut << "one tile..." << std::flush;
                    mddObj->insertTile( tile );
                }

                collection->insert( mddObj );

                // free transient memory
                collection->releaseAll();

                delete collection;

                //
                // done
                //

                RMInit::logOut << MSG_OK << std::endl;
            }
            else
            {
                RMInit::logOut << "Error: MDD type name '" << typeName << "' has no base type." << std::endl;
                returnValue = 2;
            }
        }
        else
        {
            RMInit::logOut << "Error: MDD type name '" << typeName << "' not found." << std::endl;
            returnValue = 2;
        }

        context->release();
    }
    else
    {
        RMInit::logOut << "Error: client not registered." << std::endl;
        returnValue = 1;
    }

    RMDBGEXIT( 4, RMDebug::module_servercomm, "ServerComm",  "insertMDD " << returnValue)
    return returnValue;
}


unsigned short
ServerComm::insertTileSplitted( unsigned long  callingClientId,
                                int            isPersistent,
                                RPCMarray*     rpcMarray,
                                r_Minterval*   tileSize)
{
    RMDBGENTER( 4, RMDebug::module_servercomm, "ServerComm",  "insertTileSplitted" )
    unsigned short returnValue = 0;

    RMInit::logOut << "Request: 'insert tile'..." << std::flush;

    ClientTblElt* context = getClientContext( callingClientId );

    if( context != 0 )
    {
        BaseType* baseType = NULL;

        if( isPersistent )
            baseType = (BaseType*)context->assembleMDD->getCellType();
        else
            baseType = (BaseType*)context->transferMDD->getCellType();

        // The type of the tile has to be the one of the MDD.
        // type check missing

        if( baseType != NULL )
        {
            r_Minterval   domain( rpcMarray->domain );
            char*         dataPtr  = rpcMarray->data.confarray_val;
            unsigned long dataSize = rpcMarray->data.confarray_len;
            // reset data area from rpc structure so that it is not deleted
            // deletion is done by TransTile resp. Tile
            rpcMarray->data.confarray_len = 0;
            rpcMarray->data.confarray_val = 0;
            int           getMDDData    = 0;
            r_Data_Format myDataFmt = (r_Data_Format)(rpcMarray->storageFormat);
            r_Data_Format myCurrentFmt = (r_Data_Format)(rpcMarray->currentFormat);
            RMDBGMIDDLE( 2, RMDebug::module_server, "ServerComm", "insertTileSplitted - rpc storage  format : " << myDataFmt)
            RMDBGMIDDLE( 2, RMDebug::module_server, "ServerComm", "insertTileSplitted - rpc transfer format : " << myCurrentFmt)
            // store in specified storage format; use (new) current format afterwards
            // Don't repack here because of possible retiling.
            if(ensureTileFormat(myCurrentFmt, myDataFmt, domain,
                                baseType, dataPtr, dataSize, 0, 1, context->storageFormatParams) != ENSURE_TILE_FORMAT_OK)
            {
                //FIXME returnValue
                returnValue = 1;

                context->release();
                RMDBGEXIT( 4, RMDebug::module_servercomm, "ServerComm",  "insertTileSplitted - ensureTileFormat Failed" )

                return returnValue;
            }

            if (myCurrentFmt != r_Array)
                getMDDData = dataSize;

            Tile* tile = 0;

            RMInit::dbgOut << "insertTile created new TransTile (" << myDataFmt << "), ";
            myDataFmt = r_Array;
            tile = new Tile( domain, baseType, dataPtr, getMDDData, myDataFmt );

            // for java clients only: check endianness and split tile if necessary
            if(strcmp(context->clientIdText, ServerComm::HTTPCLIENT) == 0)
            {
                // check endianess
                r_Endian::r_Endianness serverEndian = r_Endian::get_endianness();
                if(serverEndian != r_Endian::r_Endian_Big)
                {
                    RMInit::dbgOut << "changing endianness..." << std::flush;

                    // we have to swap the endianess
                    char *tpstruct;
                    r_Base_Type *useType;
                    tpstruct = baseType->getTypeStructure();
                    useType = (r_Base_Type*)(r_Type::get_any_type(tpstruct));

                    char* tempT = (char*)mymalloc(sizeof(char) * tile->getSize());

                    // change the endianness of the entire tile for identical domains for src and dest
                    r_Endian::swap_array(useType, domain, domain, tile->getContents(), tempT);
                    tile->setContents(tempT);

                    delete useType;
                    free(tpstruct);
                }

                // Split the tile!
                vector< Tile *>* tileSet = tile->splitTile( *tileSize );
                RMInit::dbgOut << "inserting split tile...";
                for( vector<Tile*>::iterator iter = tileSet->begin(); iter != tileSet->end(); iter++ )
                {
                    if( isPersistent )
                        context->assembleMDD->insertTile( *iter );
                    else
                        context->transferMDD->insertTile( *iter );
                }
                // delete the vector again
                delete tile;
                delete tileSet;
            }
            // for c++ clients: insert tile
            else
            {
                //insert one single tile
                // later, we should take into consideration the default server tile-size!
                RMInit::logOut << "inserting single tile...";
                if( isPersistent )
                    context->assembleMDD->insertTile( tile );
                else
                    context->transferMDD->insertTile( tile );
                //do not access tile again, because it was already deleted in insertTile
            }
            //
            // done
            //

            RMInit::logOut << MSG_OK << std::endl;
        }
        else
            RMInit::logOut << "Error: tile and MDD base type do not match." << std::endl;

        context->release();
    }
    else
    {
        RMInit::logOut << "Error: client not registered." << std::endl;
        returnValue = 1;
    }

    RMDBGEXIT( 4, RMDebug::module_servercomm, "ServerComm",  "insertTileSplitted" )
    return returnValue;
}


unsigned short
ServerComm::insertTile( unsigned long  callingClientId,
                        int            isPersistent,
                        RPCMarray*     rpcMarray )
{
    // no log here, is done in RNP comm.

    unsigned short returnValue = insertTileSplitted(callingClientId, isPersistent, rpcMarray, NULL);

    return returnValue;
}


unsigned short
ServerComm::startInsertPersMDD( unsigned long  callingClientId,
                                const char*    collName,
                                r_Minterval    &domain,
                                unsigned long  typeLength,
                                const char*    typeName,
                                r_OId&         oid          )
{
    RMDBGENTER( 4, RMDebug::module_servercomm, "ServerComm",  "startInsertPersMDD" )
    unsigned short returnValue = 0;

    RMInit::logOut << "Request: 'start inserting persistent MDD type', type = '" << typeName
                   << "', collection = '" << collName << "', domain = " << domain << ", cell size = " << typeLength
                   << ", " << domain.cell_count()*typeLength << "..." << std::flush;

    ClientTblElt* context = getClientContext( callingClientId );

    if( context != 0 )
    {
        // delete old transfer structures
        context->releaseTransferStructures();

        // Determine the type of the MDD to be inserted.
        const MDDType* mddType = TypeFactory::mapMDDType( typeName );

        if( mddType )
        {
            if( mddType->getSubtype() != MDDType::MDDONLYTYPE )
            {
                MDDBaseType* mddBaseType = (MDDBaseType*)mddType;

                try
                {
                    // store PersMDDColl for insert operation at the end of the transfer
                    context->transferColl = MDDColl::getMDDCollection( collName );
                    if (!context->transferColl->isPersistent())
                    {
                        RMInit::logOut << "Error: inserting into system collection is illegal." << std::endl;
                        context->release(); //!!!
                        throw r_Error(SYSTEM_COLLECTION_NOT_WRITABLE);
                    }
                }
                catch (r_Error& obj)
                {
                    RMInit::logOut << "Error " << obj.get_errorno() << ": " << obj.what() << std::endl;
                    context->release(); //!!!
                    throw;
                }
                catch(std::bad_alloc)
                {
                    RMInit::logOut << "Error: cannot allocate memory." << std::endl;
                    context->release(); //!!!
                    throw;
                }
                catch(...)
                {
                    returnValue = 5;
                    RMInit::logOut << "Error: unspecific exception while opening collection." << std::endl;

                    context->release();

                    return returnValue;
                }

                //
                // check MDD and collection type for compatibility
                //

                RMDBGIF(4, RMDebug::module_servercomm, "ServerComm", \
                        char* collTypeStructure = context->transferColl->getCollectionType()->getTypeStructure(); \
                        char* mddTypeStructure  = mddType->getTypeStructure(); \
                        RMInit::dbgOut << std::endl << "Collection type structure.: " << collTypeStructure << std::endl \
                        << "MDD type structure........: " << mddTypeStructure << std::endl \
                        << "MDD domain................: " << domain << std::endl; \
                        free( collTypeStructure ); \
                        free( mddTypeStructure ); )

                if( !mddType->compatibleWithDomain( &domain ) )
                {
                    // free resources
                    context->transferColl->releaseAll();
                    delete context->transferColl;
                    context->transferColl = 0;

                    // return error
                    returnValue = 4;
                    RMInit::logOut << "Error: MDD type not compatible wrt. its domain: " << domain << MSG_FAILED << std::endl;

                    context->release();

                    return returnValue;
                }

                if( !context->transferColl->getCollectionType()->compatibleWith( mddType ) )
                {
                    // free resources
                    context->transferColl->releaseAll();
                    delete context->transferColl;
                    context->transferColl = 0;

                    // return error
                    returnValue = 3;
                    RMInit::logOut << "Error: incompatible MDD and collection types." << std::endl;

                    context->release();

                    return returnValue;
                }

                //
                // Create persistent MDD for further tile insertions
                //

                StorageLayout ms;
                ms.setTileSize(StorageLayout::DefaultTileSize);
                ms.setIndexType(StorageLayout::DefaultIndexType);
                ms.setTilingScheme(StorageLayout::DefaultTilingScheme);
                if (domain.dimension() == StorageLayout::DefaultTileConfiguration.dimension())
                    ms.setTileConfiguration(StorageLayout::DefaultTileConfiguration);
                try
                {
                    context->assembleMDD = new MDDObj( mddBaseType, domain, OId( oid.get_local_oid() ), ms );
                }
                catch (r_Error& err)
                {
                    RMInit::logOut << "Error: while creating persistent tile: " << err.get_errorno() << ": " << err.what() << std::endl;
                    context->release(); //!!!
                    throw;
                }
                catch(std::bad_alloc)
                {
                    RMInit::logOut << "Error: cannot allocate memory." << std::endl;
                    context->release(); //!!!
                    throw;
                }
                catch(...)
                {
                    returnValue = 6;
                    RMInit::logOut << "Error: unspecific exception during creation of persistent object." << std::endl;

                    context->release();

                    return returnValue;
                }

                RMInit::logOut << MSG_OK << std::endl;
            }
            else
            {
                RMInit::logOut << "Error: MDD type '" << typeName << "' has no base type..." << std::endl;
                returnValue = 2;
            }
        }
        else
        {
            RMInit::logOut << "Error: MDD type name '" << typeName << "' not found." << std::endl;
            returnValue = 2;
        }

        context->release();
    }
    else
    {
        RMInit::logOut << "Error: client not registered." << std::endl;
        returnValue = 1;
    }

    RMDBGEXIT( 4, RMDebug::module_servercomm, "ServerComm",  "startInsertPersMDD" )
    return returnValue;
}


/*************************************************************************
 * Method name...: executeQuery( unsigned long   callingClientId,
 *                               const char*     query
 *                               ExecuteQueryRes &returnStructure )
 ************************************************************************/
unsigned short
ServerComm::executeQuery( unsigned long callingClientId,
                          const char* query,
                          ExecuteQueryRes &returnStructure )
{
    RMDBGENTER( 4, RMDebug::module_servercomm, "ServerComm",  "executeQuery" )
    unsigned short returnValue=0;

    // set all to zero as default. They are not really applicable here.
    returnStructure.errorNo       = 0;
    returnStructure.lineNo        = 0;
    returnStructure.columnNo      = 0;

    RMInit::logOut << "Request: '" << query << "'..." << std::flush;

    ClientTblElt* context = getClientContext( callingClientId );

    if( context != 0 )
    {
#ifdef RMANBENCHMARK
        Tile::relTimer.start();
        Tile::relTimer.pause();
        Tile::opTimer.start();
        Tile::opTimer.pause();
#endif

#ifdef PURIFY
        purify_printf( "%s\n", query );
#endif

        // delete old transfer collection/iterator
        context->releaseTransferStructures();

        //
        // execute the query
        //

        QueryTree* qtree = new QueryTree();   // create a query tree object...
        parseQueryTree   = qtree;             // ...and assign it to the global parse query tree pointer;

        currentClientTblElt = context;      // assign current client table element (temporary)

        int ppRet = 0;
        int parserRet = 0;

        udfEnabled = 0; // Forced for RNP, but only temporary...
        if(udfEnabled)
        {
            //
            // preprocess
            //
            RMInit::logOut << "preprocessing..." << std::flush;
            ppInBuf = (char *)query;
            ppreset();
            ppRet = ppparse();

            RMInit::dbgOut << "new query: '" << ppOutBuf << "'..." << std::flush;

            // initialize the input string parameters
            beginParseString = ppOutBuf;
            iterParseString  = ppOutBuf;
        }
        else
        {
            beginParseString = (char*)query;
            iterParseString  = (char*)query;
        }

        yyreset();

        RMInit::logOut << "parsing..." << std::flush;

        parserRet=yyparse(0);
        if((ppRet == 0) && (parserRet == 0))
        {
            try
            {
                RMDBGIF(1, RMDebug::module_servercomm, "ServerComm::executeQuery", \
                        qtree->printTree( 2, RMInit::logOut);
                       );

                RMInit::logOut << "checking semantics..." << std::flush;
                qtree->checkSemantics();

                RMDBGIF(1, RMDebug::module_servercomm, "ServerComm::executeQuery", \
                        qtree->printTree( 2, RMInit::logOut );
                       );

#ifdef RMANBENCHMARK
                if( RManBenchmark > 0 )
                    context->evaluationTimer = new RMTimer("ServerComm", "evaluation");
#endif
                //qtree->checkSemantics();
                //qtree->printTree( 2, std::cout );
                RMInit::logOut << "evaluating..." << std::flush;
                context->transferData = qtree->evaluateRetrieval();
            }
            catch( ParseInfo& info )
            {
                // this is the old error handling which has been here for quite some time
                // dealing with errors when release data
                context->releaseTransferStructures();

                returnValue = 5;           // execution error

                // set the error values of the return structure
                returnStructure.errorNo    = info.getErrorNo();
                returnStructure.lineNo     = info.getLineNo();
                returnStructure.columnNo   = info.getColumnNo();
                returnStructure.token      = strdup( info.getToken().c_str() );

                RMInit::logOut << "Error: cannot parse query (1)." << std::endl;
                info.printStatus( RMInit::logOut );
            }
            catch( r_Ebase_dbms& myErr )
            {
                RMInit::logOut << "Error: base DBMS exception: " << myErr.what() << std::endl;

                // release data
                context->releaseTransferStructures();
                context->release(); //!!!

                //delete parser data
                parseQueryTree      = 0;
                currentClientTblElt = 0;
                delete qtree;

                returnValue = 42;           // general serialisable exception

                throw;
            }
            catch( r_Error& myErr )
            {
                RMInit::logOut << "Error: " << myErr.get_errorno() << " " << myErr.what() << std::endl;

                // release data
                context->releaseTransferStructures();
                context->release(); //!!!

                //delete parser data
                parseQueryTree      = 0;
                currentClientTblElt = 0;
                delete qtree;

                returnValue = 5;

                throw;
            }
            catch(std::bad_alloc)
            {
                RMInit::logOut << "Error: cannot allocate memory." << std::endl;

                // release data
                context->releaseTransferStructures();
                context->release(); //!!!

                //delete parser data
                parseQueryTree      = 0;
                currentClientTblElt = 0;
                delete qtree;

                throw;
            }
            catch(...)
            {
                RMInit::logOut << "Error: unspecific exception." << std::endl;

                context->releaseTransferStructures();
                context->release(); //!!!

                //delete parser data
                parseQueryTree      = 0;
                currentClientTblElt = 0;
                delete qtree;

                returnValue = 5;

                throw;
            }

            if( returnValue == 0 )
            {
                if( context->transferData != 0 )
                {
                    // create the transfer iterator
                    context->transferDataIter    = new vector<QtData*>::iterator;
                    *(context->transferDataIter) = context->transferData->begin();

                    //
                    // set typeName and typeStructure
                    //

                    // The type of first result object is used to determine the type of the result
                    // collection.
                    if( *(context->transferDataIter) != context->transferData->end() )
                    {
                        QtData* firstElement = (**(context->transferDataIter));

                        if( firstElement->getDataType() == QT_MDD )
                        {
                            QtMDD* mddObj = (QtMDD*)firstElement;
                            const BaseType* baseType = mddObj->getMDDObject()->getCellType();
                            r_Minterval     domain   = mddObj->getLoadDomain();

                            MDDType* mddType = new MDDDomainType( "tmp", (BaseType*)baseType, domain );
                            SetType* setType = new SetType( "tmp", mddType );

                            returnStructure.typeName      = strdup( setType->getTypeName() );
                            returnStructure.typeStructure = setType->getTypeStructure();  // no copy

                            TypeFactory::addTempType( setType );
                            TypeFactory::addTempType( mddType );
                        }
                        else
                        {
                            returnValue = 1;       // evaluation ok, non-MDD elements

                            returnStructure.typeName      = strdup("");

                            // hack set type
                            char* elementType = firstElement->getTypeStructure();
                            returnStructure.typeStructure = (char*)mymalloc( strlen(elementType) + 6 );
                            sprintf( returnStructure.typeStructure, "set<%s>", elementType );
                            free( elementType );
                        }

                        strcpy(globalHTTPSetTypeStructure, returnStructure.typeStructure);

                        RMInit::logOut << MSG_OK << ", result type '" << returnStructure.typeStructure << "', " << context->transferData->size() << " element(s)." << std::endl;
                    }
                    else
                    {
                        RMInit::logOut << MSG_OK << ", result is empty." << std::endl;
                        returnValue = 2;         // evaluation ok, no elements

                        returnStructure.typeName      = strdup("");
                        returnStructure.typeStructure = strdup("");
                    }
                }
                else
                {
                    RMInit::logOut << MSG_OK << ", result is empty." << std::endl;
                    returnValue = 2;         // evaluation ok, no elements
                }
            }
        }
        else
        {
            if(ppRet)
            {
                RMInit::logOut << MSG_OK << ",result is empty." << std::endl;
                returnValue = 2;         // evaluation ok, no elements
            }
            else    // parse error
            {
                if( parseError )
                {
                    returnStructure.errorNo    = parseError->getErrorNo();
                    returnStructure.lineNo     = parseError->getLineNo();
                    returnStructure.columnNo   = parseError->getColumnNo();
                    returnStructure.token      = strdup( parseError->getToken().c_str() );

                    delete parseError;
                    parseError = 0;
                }
                else
                {
                    returnStructure.errorNo = 309;
                    RMInit::logOut << "Internal Error: Unknown parse error.";
                }

                yyreset(); // reset the input buffer of the scanner

                RMInit::logOut << "Error: cannot parse query (2)." << std::endl;
                returnValue = 4;
            }
        }

        parseQueryTree      = 0;
        currentClientTblElt = 0;
        delete qtree;

        //
        // done
        //

#ifdef RMANBENCHMARK

        // Evaluation timer can not be stopped because some time spent in the transfer
        // module is added to this phase.
        if( context->evaluationTimer )
            context->evaluationTimer->pause();

        if( RManBenchmark > 0 )
            context->transferTimer = new RMTimer("ServerComm", "transfer");
#endif

        // In case of an error or the result set is empty, no endTransfer()
        // is called by the client.
        // Therefore, some things have to be release here.
        if( returnValue >= 2)
        {
#ifdef RMANBENCHMARK
            Tile::opTimer.stop();
            Tile::relTimer.stop();
            Tile::opTimer.stop();
            if( context->evaluationTimer )
                delete context->evaluationTimer;
            context->evaluationTimer = 0;

            if( context->transferTimer )
                delete context->transferTimer;
            context->transferTimer = 0;

            RMTimer* releaseTimer = 0;

            if( RManBenchmark > 0 )
                releaseTimer = new RMTimer("ServerComm", "release");
#endif

            // release transfer collection/iterator
            context->releaseTransferStructures();

#ifdef RMANBENCHMARK
            if( releaseTimer )
                delete releaseTimer;
#endif
        }
        context->release();
    }
    else
    {
        RMInit::logOut << "Error: client not registered." << std::endl;
        returnValue = 3;
    }

    RMDBGEXIT( 4, RMDebug::module_servercomm, "ServerComm",  "executeQuery" )
    return returnValue;
}



unsigned short
ServerComm::initExecuteUpdate( unsigned long callingClientId )
{
    RMDBGENTER( 4, RMDebug::module_servercomm, "ServerComm",  "initExecuteUpdate" )
    unsigned short returnValue = 0;

    RMInit::logOut << "Request: 'initialize update'..." << std::flush;

    ClientTblElt* context = getClientContext( callingClientId );

    if( context != 0 )
    {
        // delete old transfer structures
        context->releaseTransferStructures();

        MDDType* mddType = new MDDType( "tmp" );
        SetType* setType = new SetType( "tmp", mddType );

        TypeFactory::addTempType( mddType );
        TypeFactory::addTempType( setType );

        // create a transient collection for storing MDD constants
        context->transferColl = new MDDColl( setType );

        context->release();

        RMInit::logOut << MSG_OK << std::endl;
    }
    else
    {
        RMInit::logOut << "Error: client not registered." << std::endl;
        returnValue = 1;
    }

    RMDBGEXIT( 4, RMDebug::module_servercomm, "ServerComm",  "initExecuteUpdate" )
    return returnValue;
}



unsigned short
ServerComm::startInsertTransMDD( unsigned long callingClientId,
                                 r_Minterval   &domain, unsigned long typeLength,
                                 const char*   typeName )
{
    RMDBGENTER( 4, RMDebug::module_servercomm, "ServerComm",  "startInsertTransMDD" )
    unsigned short returnValue = 0;

    RMInit::logOut << "Request: 'insert MDD', type '"
                   << typeName <<"', domain " << domain << ", cell length " << typeLength  << ", "
                   << domain.cell_count()*typeLength << " bytes..." << std::flush;

    ClientTblElt* context = getClientContext( callingClientId );

    if( context != 0 )
    {

        RMDBGMIDDLE(1, RMDebug::module_servercomm, "ServerComm", "startInsertTransMDD(...) TRANSFER " << context->transferFormat << ", EXACT " << (bool)context->exactFormat);

        // Determine the type of the MDD to be inserted.
        const MDDType* mddType = TypeFactory::mapMDDType( typeName );

        if( mddType )
        {
            if( mddType->getSubtype() != MDDType::MDDONLYTYPE )
            {
                MDDBaseType* mddBaseType = (MDDBaseType*)mddType;

                if( !mddType->compatibleWithDomain( &domain ) )
                {
                    // return error
                    returnValue = 3;
                    RMInit::logOut << "Error: MDD type incompatible wrt. domain: " << domain << std::endl;

                    context->release();

                    return returnValue;
                }

                // create for further insertions
                context->transferMDD = new MDDObj( mddBaseType, domain );

                RMInit::logOut << MSG_OK << std::endl;
            }
            else
            {
                RMInit::logOut << "Error: MDD type has no base type." << std::endl;
                returnValue = 2;
            }
        }
        else
        {
            RMInit::logOut << "Error: MDD type not found." << std::endl;
            returnValue = 2;
        }

        context->release();
    }
    else
    {
        RMInit::logOut << "Error: client not registered." << std::endl;
        returnValue = 1;
    }

    RMDBGEXIT( 4, RMDebug::module_servercomm, "ServerComm",  "startInsertTransMDD" )
    return returnValue;
}



unsigned short
ServerComm::endInsertMDD( unsigned long callingClientId,
                          int isPersistent )
{
    RMDBGENTER( 4, RMDebug::module_servercomm, "ServerComm",  "endInsertMDD" )
    unsigned short returnValue = 0;

    RMInit::logOut << "Request: 'end insert MDD'..." << std::flush;

    ClientTblElt* context = getClientContext( callingClientId );

    if( context != 0 )
    {
        if( isPersistent )
        {
            // we are finished with this MDD Object, so insert it into the collection
            context->transferColl->insert( context->assembleMDD );

            // reset assembleMDD, because otherwise it is tried to be freed
            context->assembleMDD = 0;

            // free transfer structure
            context->releaseTransferStructures();

            // old: context->transferColl->releaseAll(); caused a crash because releaseAll() is not idempotent
        }
        else
        {
            // we are finished with this MDD Object, so insert it into the collection
            context->transferColl->insert( context->transferMDD );

            // reset transferMDD
            context->transferMDD = 0;

            // Do not delete the transfer structure because the transient set
            // of MDD objects will be used as constants for executeUpdate().
        }

        RMInit::logOut << MSG_OK << std::endl;

        context->release();
    }
    else
    {
        RMInit::logOut << "Error: client not registered." << std::endl;
        returnValue = 1;
    }

    RMDBGEXIT( 4, RMDebug::module_servercomm, "ServerComm",  "endInsertMDD" )
    return returnValue;
}



unsigned short
ServerComm::executeUpdate( unsigned long callingClientId,
                           const char* query,
                           ExecuteUpdateRes &returnStructure )
{
    RMDBGENTER( 4, RMDebug::module_servercomm, "ServerComm",  "executeUpdate" )

    RMInit::logOut << "Request: '" << query << "'..." << std::flush;

#ifdef RMANBENCHMARK
    Tile::relTimer.start();
    Tile::relTimer.pause();
    Tile::opTimer.start();
    Tile::opTimer.pause();
#endif

    unsigned short returnValue = 0;

    ClientTblElt* context = getClientContext( callingClientId );

    if( context != 0 )
    {
#ifdef PURIFY
        purify_printf( "%s\n", query );
#endif

        //
        // execute the query
        //

        QueryTree* qtree = new QueryTree();   // create a query tree object...
        parseQueryTree   = qtree;             // ...and assign it to the global parse query tree pointer;

        mddConstants     = context->transferColl; // assign the mdd constants collection to the global pointer (temporary)
        currentClientTblElt = context;        // assign current client table element (temporary)

        int ppRet = 0;
        udfEnabled = false; // forced for RNP tests
        if(udfEnabled)
        {
            //
            // preprocess
            //
            RMInit::logOut << "preprocessing..." << std::flush;
            ppInBuf = (char *)query;
            ppreset();
            ppRet = ppparse();

            if(ppOutBuf)
                RMInit::dbgOut << "new query: '" << ppOutBuf << "'" << std::endl;
            else
                RMInit::dbgOut << "new query: empty." << std::endl;

            // initialize the input string parameters
            beginParseString = ppOutBuf;
            iterParseString  = ppOutBuf;
        }
        else
        {
            beginParseString = (char*)query;
            iterParseString  = (char*)query;
        }

        yyreset();

        RMInit::logOut << "parsing..." << std::flush;

        if( ppRet == 0 && yyparse(0) == 0 )
        {
            try
            {
                RMDBGIF(1, RMDebug::module_servercomm, "ServerComm::executeUpdate", \
                        qtree->printTree( 2, RMInit::logOut );
                       );

                RMInit::logOut << "checking semantics..." << std::flush;

                qtree->checkSemantics();

                RMDBGIF(1, RMDebug::module_servercomm, "ServerComm::executeUpdate", \
                        qtree->printTree( 2, RMInit::logOut );
                       );

#ifdef RMANBENCHMARK
                if( RManBenchmark > 0 )
                    context->evaluationTimer = new RMTimer("ServerComm", "evaluation");
#endif

                RMInit::logOut << "evaluating..." << std::flush;
                qtree->evaluateUpdate();

                // release data
                context->releaseTransferStructures();

                RMInit::logOut << MSG_OK << std::endl;
            }
            catch( ParseInfo& info )
            {
                // release data
                context->releaseTransferStructures();

                returnValue = 3;           // evaluation error

                // set the error values of the return structure
                returnStructure.errorNo    = info.getErrorNo();
                returnStructure.lineNo     = info.getLineNo();
                returnStructure.columnNo   = info.getColumnNo();
                returnStructure.token      = strdup( info.getToken().c_str() );

                RMInit::logOut << "Error: cannot parse query (1)." << std::endl;
                info.printStatus( RMInit::logOut );
            }
            catch(r_Error &err)
            {
                context->releaseTransferStructures();
                context->release();
                RMInit::logOut << "Error: " << err.get_errorno() << " " << err.what() << std::endl;
                throw;
            }
        }
        else
        {
            if(ppRet)
            {
                RMInit::logOut << MSG_OK << std::endl;
                returnValue = 0;
            }
            else    // parse error
            {
                if( parseError )
                {
                    returnStructure.errorNo    = parseError->getErrorNo();
                    returnStructure.lineNo     = parseError->getLineNo();
                    returnStructure.columnNo   = parseError->getColumnNo();
                    returnStructure.token      = strdup( parseError->getToken().c_str() );

                    delete parseError;

                    RMInit::logOut << "Error: cannot parse query (2)." << std::endl;
                    parseError = 0;
                }
                else
                {
                    returnStructure.errorNo = 309;
                    RMInit::logOut << "Error: unspecific internal parser error." << endl;
                }

                yyreset(); // reset the input buffer of the scanner

                returnValue = 2;
            }
        }

        parseQueryTree      = 0;
        mddConstants        = 0;
        currentClientTblElt = 0;
        delete qtree;

        // delete set of mdd constants
        context->releaseTransferStructures();

        //
        // done
        //

        context->release();
    }
    else
    {
        RMInit::logOut << "Error: client not registered." << std::endl;
        returnValue = 1;
    }


#ifdef RMANBENCHMARK
    // stop evaluation timer
    if( context->evaluationTimer )
    {
        delete context->evaluationTimer;
        context->evaluationTimer = 0;
    }

    Tile::opTimer.stop();
    Tile::relTimer.stop();
#endif

    RMDBGEXIT( 4, RMDebug::module_servercomm, "ServerComm",  "executeUpdate" )
    return returnValue;
}



unsigned short
ServerComm::getCollByName( unsigned long callingClientId,
                           const char*   collName,
                           char*         &typeName,
                           char*         &typeStructure,
                           r_OId         &oid             )
{
    RMDBGENTER( 4, RMDebug::module_servercomm, "ServerComm",  "ServerComm::getCollByName" )

    RMInit::logOut << "Request: 'get collection by name', name = " << collName << "'..." << std::flush;

    unsigned short returnValue=0;

    ClientTblElt* context = getClientContext( callingClientId );

    if( context != 0 )
    {
        //
        // create the actual transfer collenction
        //

        // delete old transfer collection/iterator
        context->releaseTransferStructures();

        // create the transfer collection
        try
        {
            context->transferColl = MDDColl::getMDDCollection( collName );
            RMDBGMIDDLE( 4, RMDebug::module_server, "ServerComm", "retrieved mdd collection" )
        }
        catch(std::bad_alloc)
        {
            RMInit::logOut << "Error: cannot allocate memory." << std::endl;
            context->release(); //!!!
            throw;
        }
        catch (r_Error& err)
        {
            RMInit::logOut << "Error " << err.get_errorno() << " " << err.what() << std::endl;
            context->release(); //!!!
            throw;
        }
        catch(...)
        {
            returnValue = 2;  // collection name invalid
            RMInit::logOut << "Error: unspecific exception." << std::endl;
        }

        if( returnValue == 0 )
        {
            // create the transfer iterator
            context->transferCollIter = context->transferColl->createIterator();
            context->transferCollIter->reset();

            // set typeName and typeStructure
            CollectionType* collectionType = (CollectionType*) context->transferColl->getCollectionType();

            if( collectionType )
            {
                typeName      = strdup( collectionType->getTypeName() );
                typeStructure = collectionType->getTypeStructure();  // no copy !!!

                // set oid in case of a persistent collection
                if( context->transferColl->isPersistent() )
                {
                    EOId eOId;
                    if (context->transferColl->isPersistent())
                    {
                        if (context->transferColl->getEOId(eOId) == true)
                            oid = r_OId( eOId.getSystemName(), eOId.getBaseName(), eOId.getOId() );
                    }
                }
                RMInit::logOut << MSG_OK << std::endl;
            }
            else
            {
                RMInit::logOut << "Warning: cannot obtain collection type information." << endl;
                typeName      = strdup("");
                typeStructure = strdup("");
            }

            if( !context->transferCollIter->notDone() )
            {
                RMInit::logOut << MSG_OK << ", result empty.";
                returnValue = 1;

                // delete transfer collection/iterator
                context->releaseTransferStructures();
            }
        }

        //
        // done
        //

        context->release();
    }
    else
    {
        RMInit::logOut << "Error: client not registered." << std::endl;
        returnValue = 3;
    }

    RMDBGEXIT( 4, RMDebug::module_servercomm, "ServerComm",  " ServerComm::getCollByName" )

    return returnValue;
}



unsigned short
ServerComm::getCollByOId( unsigned long callingClientId,
                          r_OId         &oid,
                          char*         &typeName,
                          char*         &typeStructure,
                          char*         &collName       )
{
    RMDBGENTER( 4, RMDebug::module_servercomm, "ServerComm",  " ServerComm::getCollByOId" )

    RMInit::logOut << "Request: 'get collection by OID', oid = " << oid << "..." << std::flush;

    unsigned short returnValue=0;

    ClientTblElt* context = getClientContext( callingClientId );

    if( context != 0 )
    {
        // delete old transfer collection/iterator
        context->releaseTransferStructures();

        // check type and existence of oid
        OId oidIf( oid.get_local_oid() );
        OId::OIdType objType = oidIf.getType();

        if( objType == OId::MDDCOLLOID )
        {
            //
            // get collection
            //

            try
            {
                RMDBGMIDDLE( 4, RMDebug::module_server, "ServerComm",
                             std::endl << "  execute new PersMDDColl(" << oid << ")" )
                context->transferColl = MDDColl::getMDDCollection(oidIf);
                RMDBGMIDDLE( 4, RMDebug::module_server, "ServerComm", "  ok" )
            }
            catch(std::bad_alloc)
            {
                RMInit::logOut << "Error: cannot allocate memory." << std::endl;
                throw;
            }
            catch (r_Error& err)
            {
                RMInit::logOut << "Error " << err.get_errorno() << " " << err.what() << std::endl;
                throw;
            }
            catch(...)  // not found (?)
            {
                returnValue = 2;
                RMInit::logOut << "Error: unspecific exception." << endl;
            }

            //
            // create the actual transfer collenction
            //

            if( returnValue == 0 )
            {
                // get collection name
                collName = strdup(context->transferColl->getName());

                // create the transfer iterator
                context->transferCollIter = context->transferColl->createIterator();
                context->transferCollIter->reset();

                // set typeName and typeStructure
                CollectionType* collectionType = (CollectionType*) context->transferColl->getCollectionType();

                if( collectionType )
                {
                    typeName      = strdup( collectionType->getTypeName() );
                    typeStructure = collectionType->getTypeStructure();  // no copy !!!

                    // set oid in case of a persistent collection
                    if( context->transferColl->isPersistent() )
                    {
                        EOId eOId;

                        if (context->transferColl->getEOId(eOId) == true)
                            oid = r_OId( eOId.getSystemName(), eOId.getBaseName(), eOId.getOId() );
                    }
                    RMInit::logOut << MSG_OK << std::endl;
                }
                else
                {
                    RMInit::logOut << MSG_OK << ", but warning: cannot obtain type information." << endl;
                    typeName      = strdup("");
                    typeStructure = strdup("");
                }

                if( !context->transferCollIter->notDone() )
                {
                    RMInit::logOut << MSG_OK << ", result empty." << endl;
                    returnValue = 1;

                    // delete transfer collection/iterator
                    context->releaseTransferStructures();
                }
            }

        }
        else
        {
            returnValue = 2; // oid does not belong to a collection object
            RMInit::logOut << "Error: oid does not belong to a collection object." << std::endl;
        }

        //
        // done
        //

        context->release();
    }
    else
    {
        RMInit::logOut << "Error: client not registered." << std::endl;
        returnValue = 3;
    }

    RMDBGEXIT( 4, RMDebug::module_servercomm, "ServerComm",  " ServerComm::getCollByOId" )

    return returnValue;
}



unsigned short
ServerComm::getCollOIdsByName( unsigned long callingClientId,
                               const char*   collName,
                               char*         &typeName,
                               char*         &typeStructure,
                               r_OId         &oid,
                               RPCOIdEntry*  &oidTable,
                               unsigned int  &oidTableSize     )
{
    RMDBGENTER( 4, RMDebug::module_servercomm, "ServerComm",  "getCollOIdsByName" )

    RMInit::logOut << "Request: 'get collection OIds by name', name = " << collName << "'..." << std::flush;

    unsigned short returnValue=0;

    ClientTblElt* context = getClientContext( callingClientId );

    if( context != 0 )
    {
        //
        // get collection
        //

        MDDColl* coll = 0;
        MDDColl* almost = 0;

        try
        {
            RMDBGMIDDLE( 4, RMDebug::module_server, "ServerComm", "retrieving collection " << collName)
            almost = MDDColl::getMDDCollection( collName );
            RMDBGMIDDLE( 4, RMDebug::module_server, "ServerComm", "retrieved collection " << collName)
            if (!almost->isPersistent())
            {
                RMDBGMIDDLE( 4, RMDebug::module_server, "ServerComm", "retrieved system collection")
                RMInit::logOut << "Error: trying to get oid of system collection: " << collName << std::endl;
                throw r_Error(SYSTEM_COLLECTION_HAS_NO_OID);
            }
            else
            {
                RMDBGMIDDLE( 4, RMDebug::module_server, "ServerComm", "retrieved persistent collection")
                coll = (MDDColl*)almost;
            }
        }
        catch(std::bad_alloc)
        {
            RMInit::logOut << "Error: cannot allocate memory." << std::endl;
            throw;
        }
        catch (r_Error& err)
        {
            RMDBGMIDDLE( 4, RMDebug::module_server, "ServerComm", "caught exception")
            RMInit::logOut << "Error " << err.get_errorno() << ": " << err.what() << std::endl;
            returnValue = 2;  // collection name invalid
        }
        catch(...)
        {
            RMDBGMIDDLE( 4, RMDebug::module_server, "ServerComm", "caught exception")
            returnValue = 2;  // collection name invalid
            RMInit::logOut << "Error: unspecific exception." << std::endl;
        }
        RMDBGMIDDLE( 4, RMDebug::module_server, "ServerComm", "after exception catching")

        if( returnValue == 0 )
        {
            // set typeName and typeStructure
            CollectionType* collectionType = (CollectionType*) coll->getCollectionType();

            if( collectionType )
            {
                typeName      = strdup( collectionType->getTypeName() );
                typeStructure = collectionType->getTypeStructure();  // no copy !!!

                // set oid
                EOId eOId;

                if (coll->getEOId(eOId) == true)
                    oid = r_OId( eOId.getSystemName(), eOId.getBaseName(), eOId.getOId() );
            }
            else
            {
                RMInit::logOut << "Warning: no type information available..." << std::flush;
                typeName      = strdup("");
                typeStructure = strdup("");
            }

            if( coll->getCardinality() )
            {
                // create iterator
                MDDCollIter* collIter = coll->createIterator();
                int          i;

                oidTableSize = coll->getCardinality();
                oidTable     = (RPCOIdEntry*) mymalloc( sizeof(RPCOIdEntry) * oidTableSize );

                TALK( oidTableSize << " elements..." );

                for( collIter->reset(), i=0; collIter->notDone(); collIter->advance(), i++ )
                {
                    MDDObj* mddObj = collIter->getElement();

                    if( mddObj->isPersistent() )
                    {
                        EOId eOId;

                        if( ((MDDObj*)mddObj)->getEOId( &eOId ) == 0 )
                            oidTable[i].oid = strdup( r_OId( eOId.getSystemName(), eOId.getBaseName(), eOId.getOId() ).get_string_representation() );
                        else
                            oidTable[i].oid = strdup("");
                        mddObj = 0;
                    }
                    else
                        oidTable[i].oid = strdup("");
                }

                delete collIter;

                RMInit::logOut << MSG_OK << ", " << coll->getCardinality() << " result(s)." << endl;
            }
            else
            {
                RMInit::logOut << MSG_OK << ", result empty." << endl;
                returnValue = 1;
            }

            delete coll;
        }

        //
        // done
        //

        context->release();
    }
    else
    {
        RMInit::logOut << "Error: client not registered." << std::endl;
        returnValue = 3;
    }

    RMDBGEXIT( 4, RMDebug::module_servercomm, "ServerComm",  "getCollOIdsByName " << returnValue)

    return returnValue;
}


unsigned short
ServerComm::getCollOIdsByOId( unsigned long callingClientId,
                              r_OId         &oid,
                              char*         &typeName,
                              char*         &typeStructure,
                              RPCOIdEntry*  &oidTable,
                              unsigned int  &oidTableSize,
                              char*         &collName        )
{
    RMDBGENTER( 4, RMDebug::module_servercomm, "ServerComm",  "getCollOIdsByOId" )

    RMInit::logOut << "Request: 'get collection OIDs by OId', oid = " << oid << "..." << std::flush;

    unsigned short returnValue=0;

    ClientTblElt* context = getClientContext( callingClientId );

    if( context != 0 )
    {
        // check type and existence of oid
        OId oidIf( oid.get_local_oid() );
        OId::OIdType objType = oidIf.getType();

        if( objType == OId::MDDCOLLOID )
        {
            //
            // get collection
            //

            MDDColl* coll = 0;

            try
            {
                RMDBGMIDDLE( 4, RMDebug::module_server, "ServerComm", "get mdd coll by oid " << oidIf)
                coll = MDDColl::getMDDCollection(oidIf);
                RMDBGMIDDLE( 4, RMDebug::module_server, "ServerComm", "retrieved mdd coll" )
            }
            catch(std::bad_alloc)
            {
                RMInit::logOut << "Error: cannot allocate memory." << std::endl;
                throw;
            }
            catch (r_Error& err)
            {
                RMInit::logOut << "Error " << err.get_errorno() << ": " << err.what() << std::endl;
                returnValue = 2;  // collection name invalid
                if (err.get_kind() != r_Error::r_Error_RefNull)
                    throw;
            }
            catch(...)
            {
                returnValue = 2;  // collection name invalid
                RMInit::logOut << "Error: unknown collection name." << std::endl;
            }

            if( returnValue == 0 )
            {
                // get collection name
                collName = strdup(coll->getName());

                // set typeName and typeStructure
                CollectionType* collectionType = (CollectionType*) coll->getCollectionType();

                if( collectionType )
                {
                    typeName      = strdup( collectionType->getTypeName() );
                    typeStructure = collectionType->getTypeStructure();  // no copy !!!

                    // set oid
                    EOId eOId;

                    if (coll->getEOId(eOId) == true)
                        oid = r_OId( eOId.getSystemName(), eOId.getBaseName(), eOId.getOId() );
                }
                else
                {
                    RMInit::logOut << "Warning: no type information available..." << std::flush;
                    typeName      = strdup("");
                    typeStructure = strdup("");
                }

                if( coll->getCardinality() )
                {
                    // create iterator
                    MDDCollIter* collIter = coll->createIterator();
                    int          i;

                    oidTableSize = coll->getCardinality();
                    oidTable     = (RPCOIdEntry*) mymalloc( sizeof(RPCOIdEntry) * oidTableSize );

                    TALK( oidTableSize << " elements..." );

                    for( collIter->reset(), i=0; collIter->notDone(); collIter->advance(), i++ )
                    {
                        MDDObj* mddObj = collIter->getElement();

                        if( mddObj->isPersistent() )
                        {
                            EOId eOId;

                            if( ((MDDObj*)mddObj)->getEOId( &eOId ) == 0 )
                                oidTable[i].oid = strdup( r_OId( eOId.getSystemName(), eOId.getBaseName(), eOId.getOId() ).get_string_representation() );
                            else
                                oidTable[i].oid = strdup("");
                        }
                        else
                            oidTable[i].oid = strdup("");
                    }

                    delete collIter;
                    //coll->releaseAll();

                    RMInit::logOut << MSG_OK << ", " << coll->getCardinality() << " result(s)." << endl;
                }
                else
                {
                    RMInit::logOut << MSG_OK << ", result empty." << endl;
                    returnValue = 1;
                }

                delete coll;
            }
        }
        else
        {
            returnValue = 2; // oid does not belong to a collection object
            RMInit::logOut << "Error: not a collection oid: " << oid << std::endl;
        }

        //
        // done
        //

        context->release();
    }
    else
    {
        RMInit::logOut << "Error: client not registered." << std::endl;
        returnValue = 3;
    }

    RMDBGEXIT( 4, RMDebug::module_servercomm, "ServerComm",  "getCollOIdsByOId " << returnValue)
    return returnValue;
}



unsigned short
ServerComm::getNextMDD( unsigned long   callingClientId,
                        r_Minterval     &mddDomain,
                        char*           &typeName,
                        char*           &typeStructure,
                        r_OId           &oid,
                        unsigned short  &currentFormat     )
{
    RMDBGENTER( 4, RMDebug::module_servercomm, "ServerComm",  "getNextMDD" )

    RMInit::logOut << "Request (continuing): 'get next MDD'..." << std::flush;

    unsigned short returnValue = 0;

    ClientTblElt* context = getClientContext( callingClientId );

    if( context != 0 )
    {
        try
        {
            if( context->transferData && context->transferDataIter && *(context->transferDataIter) != context->transferData->end() )
            {
                //
                // convert the mdd to transfer to rpc data structures
                //

                // get the MDD object to be transfered
                QtMDD*  mddData = (QtMDD*) **(context->transferDataIter);
                MDDObj* mddObj  = mddData->getMDDObject();

                // initialize mddDomain to give it back
                mddDomain = mddData->getLoadDomain();

                TALK( "domain " << mddDomain );

                //
                // initialize tiles to transfer
                //

#ifdef RMANBENCHMARK
                // pause transfer timer and resume evaluation timer
                if( context->transferTimer   )
                    context->transferTimer->pause();
                if( context->evaluationTimer )
                    context->evaluationTimer->resume();
#endif

                if( mddObj->getCurrentDomain() == mddData->getLoadDomain() )
                    context->transTiles = mddObj->getTiles();
                else
                {
                    // If the load domain is different from the current domain, we have
                    // a persitent MDD object. The border tiles have to be cut (and
                    // therefore copied) in order to be ready for transfering them.
                    // These temporary border tiles are added to the deletableTiles list
                    // which is deleted at the end.

                    context->transTiles = mddObj->intersect( mddData->getLoadDomain() );

                    // iterate over the tiles
                    for( vector<Tile*>::iterator iter = context->transTiles->begin(); iter != context->transTiles->end(); iter++ )
                    {
                        // get relevant area of source tile
                        r_Minterval sourceTileDomain( mddData->getLoadDomain().create_intersection( (*iter)->getDomain() ) );

                        if( sourceTileDomain != (*iter)->getDomain() )
                        {
                            // create a new transient tile and copy the transient data
                            Tile* newTransTile = new Tile( sourceTileDomain, mddObj->getCellType() );
                            newTransTile->copyTile( sourceTileDomain, *iter, sourceTileDomain );

                            // replace the tile in the list with the new one
                            *iter = newTransTile;

                            // add the new tile to deleteableTiles
                            if( !(context->deletableTiles) )
                                context->deletableTiles = new vector<Tile*>();
                            context->deletableTiles->push_back( newTransTile );
                        }
                    }
                }

#ifdef RMANBENCHMARK
                // In order to be sure that reading tiles from disk is done
                // in the evaluation phase, the contents pointers of each tile
                // are got.
                char* benchmarkPointer;

                for( vector<Tile*>::iterator benchmarkIter = context->transTiles->begin();
                        benchmarkIter != context->transTiles->end(); benchmarkIter++ )
                    benchmarkPointer = (*benchmarkIter)->getContents();

                // pause evaluation timer and resume transfer timer
                if( context->evaluationTimer )
                    context->evaluationTimer->pause();
                if( context->transferTimer   )
                    context->transferTimer->resume();
#endif

                // initialize tile iterator
                context->tileIter   = new vector<Tile*>::iterator;
                *(context->tileIter) = context->transTiles->begin();

                const BaseType* baseType = mddObj->getCellType();

                TALK( "cell length " << baseType->getSize() );

                //
                // set typeName and typeStructure
                //
                // old: typeName = strdup( mddObj->getCellTypeName() ); not known for the moment being
                typeName      = strdup("");

                // create a temporary mdd type for the moment being
                r_Minterval typeDomain( mddData->getLoadDomain() );
                MDDType* mddType = new MDDDomainType( "tmp", (BaseType*)baseType, typeDomain );
                TypeFactory::addTempType( mddType );

                typeStructure = mddType->getTypeStructure();  // no copy !!!

                // I'm not sure about this code...
#if 0
                // determine data format from the 1st tile
                if( context->transTiles->size() && (*(context->transTiles))[0]->getDataFormat() == r_TIFF )
                    currentFormat = r_TIFF;
                else
                    currentFormat = r_Array;
#else
                if (context->transTiles->size())
                    currentFormat = (*(context->transTiles))[0]->getDataFormat();
                else
                    currentFormat = r_Array;
#endif

                // set oid in case of persistent MDD objects
                if( mddObj->isPersistent() )
                {
                    EOId eOId;

                    if( ((MDDObj*)mddObj)->getEOId( &eOId ) == 0 )
                        oid = r_OId( eOId.getSystemName(), eOId.getBaseName(), eOId.getOId() );
                }

                //
                //
                //

                if( context->transTiles->size() > 0 )
                {
                    RMInit::logOut << MSG_OK << ", " << context->transTiles->size() << " more tile(s)" << endl;
                }
                else   // context->transTiles->size() == 0
                {
                    returnValue = 2;
                    RMInit::logOut << "Error: no tiles in MDD object." << std::endl;
                }

                context->totalTransferedSize = 0;
                context->totalRawSize = 0;
            }
            else
            {
                if( context->transferDataIter && *(context->transferDataIter) == context->transferData->end() )
                {
                    returnValue = 1;  // nothing left in the collection
                    RMInit::logOut << MSG_OK << ", no more tiles." << std::endl;
                    context->releaseTransferStructures();
                }
                else
                {
                    returnValue = 2;  // no actual transfer collection
                    RMInit::logOut << "Error: no transfer collection. " << std::endl;
                }
            }

            //
            // done
            //

            context->release();
        }
        catch( r_Ebase_dbms& myErr )
        {
            RMInit::logOut << "Error: base DBMS exception (kind " << myErr.get_kind() << ", errno " << myErr.get_errorno() << ") " << myErr.what() << std::endl;
            returnValue = 42;
            throw;
        }
        catch( r_Error& myErr )
        {
            RMInit::logOut << "Error: (kind " << myErr.get_kind() << ", errno " << myErr.get_errorno() << ") " << myErr.what() << std::endl;
            throw;
        }
        catch(std::bad_alloc)
        {
            RMInit::logOut << "Error: cannot allocate memory." << std::endl;
            throw;
        }
        catch(...)
        {
            RMInit::logOut << "Error: unspecified exception." << std::endl;
        }
    }
    else
    {
        RMInit::logOut << "Error: client not registered." << std::endl;
        returnValue = 2;
    }


    RMDBGEXIT( 4, RMDebug::module_servercomm, "ServerComm",  "getNextMDD" )

    return returnValue;
}


unsigned short
ServerComm::getNextElement( unsigned long   callingClientId,
                            char*           &buffer,
                            unsigned int    &bufferSize)
{
    RMDBGENTER(1, RMDebug::module_servercomm, "ServerComm", "getNextElement(...)")

    RMInit::logOut << "Request (continuing): 'get next element'..." << std::flush;

    unsigned short returnValue = 0;

    ClientTblElt* context = getClientContext( callingClientId );

    if( context != 0 )
    {
        RMDBGMIDDLE(1, RMDebug::module_servercomm, "ServerComm", "getNextElement(...) TRANSFER " << context->transferFormat << ", EXACT " << (bool)context->exactFormat);

        if( context->transferData && context->transferDataIter &&
                *(context->transferDataIter) != context->transferData->end() )
        {

            //
            // convert data element to rpc data structures
            //
            // Buffer is allocated and has to be freed by the caller using free().

            // get next object to be transfered
            try
            {
                QtData*  dataObj = **(context->transferDataIter);

                switch( dataObj->getDataType() )
                {
                case QT_STRING:
                {
                    QtStringData* stringDataObj       = (QtStringData*)dataObj;
                    bufferSize = stringDataObj->getStringData().length();
                    buffer     = (char*)mymalloc( bufferSize );
                    memcpy( (void*)buffer, (void*)stringDataObj->getStringData().c_str(), bufferSize );
                }
                break;
                case QT_INTERVAL:
                {
                    QtIntervalData*  intervalDataObj  = (QtIntervalData*)dataObj;
                    char*            stringData       = intervalDataObj->getIntervalData().get_string_representation();
                    bufferSize = strlen( stringData );
                    buffer     = (char*)mymalloc( bufferSize );
                    memcpy( (void*)buffer, (void*)stringData, bufferSize );
                    free( stringData );
                }
                break;
                case QT_MINTERVAL:
                {
                    QtMintervalData* mintervalDataObj = (QtMintervalData*)dataObj;
                    char*            stringData       = mintervalDataObj->getMintervalData().get_string_representation();
                    bufferSize = strlen( stringData );
                    buffer     = (char*)mymalloc( bufferSize );
                    memcpy( (void*)buffer, (void*)stringData, bufferSize );
                    free( stringData );
                }
                break;
                case QT_POINT:
                {
                    QtPointData* pointDataObj         = (QtPointData*)dataObj;
                    char*            stringData       = pointDataObj->getPointData().get_string_representation();
                    bufferSize = strlen( stringData );
                    buffer     = (char*)mymalloc( bufferSize );
                    memcpy( (void*)buffer, (void*)stringData, bufferSize );

                    free( stringData );
                }
                break;
                default:
                    if( dataObj->isScalarData() )
                    {
                        QtScalarData* scalarDataObj = (QtScalarData*)dataObj;
                        bufferSize = scalarDataObj->getValueType()->getSize();
                        buffer     = (char*)mymalloc( bufferSize );
                        memcpy( (void*)buffer, (void*)scalarDataObj->getValueBuffer(), bufferSize );
                        // server endianess
                        r_Endian::r_Endianness serverEndian = r_Endian::get_endianness();

                        // change endianess if necessary
                        // currently only one client is active at one time
                        //  if((context->clientId == 1) && (strcmp(context->clientIdText, ServerComm::HTTPCLIENT) == 0) &&  (serverEndian != r_Endian::r_Endian_Big))
                        if( (strcmp(context->clientIdText, ServerComm::HTTPCLIENT) == 0) && (serverEndian != r_Endian::r_Endian_Big))
                        {
                            RMInit::logOut << "changing endianness..." << std::flush;
                            // calling client is a http-client(java -> always BigEndian) and server has LittleEndian
                            switch(scalarDataObj->getDataType())
                            {
                            case QT_USHORT:
                            {
                                r_UShort tmp = *(r_UShort*)buffer;
                                *(r_UShort*)buffer = r_Endian::swap(tmp);
                            }
                            break;

                            case QT_SHORT:
                            {
                                r_Short tmp = *(r_Short*)buffer;
                                *(r_Short*)buffer = r_Endian::swap(tmp);
                            }
                            break;

                            case QT_LONG:
                            {
                                r_Long tmp = *(r_Long*)buffer;
                                *(r_Long*)buffer = r_Endian::swap(tmp);
                            }
                            break;

                            case QT_ULONG:
                            {
                                r_ULong tmp = *(r_ULong*)buffer;
                                *(r_ULong*)buffer = r_Endian::swap(tmp);
                            }
                            break;

                            case QT_FLOAT:
                            {
                                r_Float tmp = *(r_Float*)buffer;
                                *(r_Float*)buffer = r_Endian::swap(tmp);
                            }
                            break;

                            case QT_DOUBLE:
                            {
                                r_Double tmp = *(r_Double*)buffer;
                                *(r_Double*)buffer = r_Endian::swap(tmp);
                            }
                            break;

                            default:
                            {
                                RMDBGENTER( 0, RMDebug::module_servercomm, "ServerComm",  "getNextElement(...) bad dataType "  << scalarDataObj->getDataType());
                            }
                            break;
                            }
                        }
                    }
                    break;
                }
            }
            catch( r_Ebase_dbms& myErr)
            {
                RMInit::logOut << "Error: base BMS exception (kind " << myErr.get_kind() << ", errno " << myErr.get_errorno() << ") " << myErr.what() << std::endl;
                throw;
            }
            catch (r_Error& err)
            {
                RMInit::logOut << "Error: exception (kind " << err.get_kind() << ", errno " << err.get_errorno() << ") " << err.what() << std::endl;
                throw;
            }

            // increment list iterator
            (*(context->transferDataIter))++;

            if( *(context->transferDataIter) != context->transferData->end() )
            {
                returnValue = 0;
                RMInit::logOut << MSG_OK << ", some more tile(s) left." << endl;
            }
            else
            {
                returnValue = 1;
                RMInit::logOut << MSG_OK << ", no more tiles." << std::endl;
            }
        }
        else
        {
            if( context->transferDataIter && *(context->transferDataIter) == context->transferData->end() )
            {
                returnValue = 1;  // nothing left in the collection
                TALK( "nothing left..." << MSG_OK );
                context->releaseTransferStructures();
            }
            else
            {
                returnValue = 2;  // no actual transfer collection
                RMInit::logOut << "Error: no transfer collection." << endl;
            }
        }

        //
        // done
        //

        context->release();
    }
    else
    {
        RMInit::logOut << "Error: client not registered." << std::endl;
        returnValue = 2;
    }

    RMDBGEXIT(1, RMDebug::module_servercomm, "ServerComm", "getNextElement(...)")
    return returnValue;
}



unsigned short
ServerComm::getMDDByOId( unsigned long   callingClientId,
                         r_OId           &oid,
                         r_Minterval     &mddDomain,
                         char*           &typeName,
                         char*           &typeStructure,
                         unsigned short  &currentFormat   )
{
    RMDBGENTER( 4, RMDebug::module_servercomm, "ServerComm",  "getMDDByOId" )

    RMInit::logOut << "Request: 'get MDD by OId', oid = " << oid << "..." << std::flush;

    unsigned short returnValue = 0;

    ClientTblElt* context = getClientContext( callingClientId );

    if( context != 0 )
    {
        // delete old transfer collection/iterator
        context->releaseTransferStructures();

        // check type and existence of oid
        OId oidIf( oid.get_local_oid() );
        OId::OIdType objType = oidIf.getType();

        if( objType == OId::MDDOID )
        {
            // get MDD object
            try
            {
                context->transferMDD = new MDDObj( oidIf );
            }
            catch(std::bad_alloc)
            {
                RMInit::logOut << "Error: cannot allocate memory." << std::endl;
                context->release();
                throw;
            }
            catch (r_Error& err)
            {
                RMInit::logOut << "Error: (kind " << err.get_kind() << ", errno " << err.get_errorno() << ") " << err.what() << std::endl;
                context->release();
                throw;
            }
            catch(...)
            {
                returnValue = 2;
                RMInit::logOut << "Error: unspecified exception." << endl;
            }

            if( !returnValue )
            {
                //
                // convert the mdd to transfer to rpc data structures
                //

                // initialize mddDomain to give it back
                mddDomain = context->transferMDD->getCurrentDomain();

                TALK( "domain " << mddDomain );

                // initialize context fields
                context->transTiles  = context->transferMDD->getTiles();
                context->tileIter    = new vector<Tile*>::iterator;
                *(context->tileIter) = context->transTiles->begin();

                const BaseType* baseType = context->transferMDD->getCellType();

                TALK( "cell length " << baseType->getSize() );

                //
                // set typeName and typeStructure
                //
                // old: typeName = strdup( context->transferMDD->getCellTypeName() ); not known for the moment being

                typeName      = strdup("");

                // create a temporary mdd type for the moment being
                MDDType* mddType = new MDDDomainType( "tmp", (BaseType*)baseType, context->transferMDD->getCurrentDomain() );
                TypeFactory::addTempType( mddType );

                typeStructure = mddType->getTypeStructure();  // no copy !!!

                // I'm not sure about this code either
#if 0
                // determine data format from the 1st tile
                if( context->transTiles->size() && (*(context->transTiles))[0]->getDataFormat() == r_TIFF )
                    currentFormat = r_TIFF;
                else
                    currentFormat = r_Array;
#else
                if (context->transTiles->size())
                    currentFormat = (*(context->transTiles))[0]->getDataFormat();
                else
                    currentFormat = r_Array;
#endif

                // set oid in case of persistent MDD objects
                if( context->transferMDD->isPersistent() )
                {
                    EOId eOId;

                    if( ((MDDObj*)(context->transferMDD))->getEOId( &eOId ) == 0 )
                        oid = r_OId( eOId.getSystemName(), eOId.getBaseName(), eOId.getOId() );
                }

                //
                //
                //

                if( context->transTiles->size() > 0 )
                {
                    RMInit::logOut << MSG_OK << ", got " << context->transTiles->size() << " tile(s)." << endl;
                }
                else   // context->transTiles->size() == 0
                {
                    returnValue = 3;
                    RMInit::logOut << "Error: no tiles in MDD object." << endl;
                }
            }
        }
        else
        {
            returnValue = 2; // oid does not belong to an MDD object
            RMInit::logOut << "Error: oid does not belong to an MDD object." << endl;
        }

        //
        // done
        //

        context->release();
    }
    else
    {
        RMInit::logOut << "Error: client not registered." << std::endl;
        returnValue = 1; // client context not found
    }

    context->totalRawSize = 0;
    context->totalTransferedSize = 0;

    RMDBGEXIT( 4, RMDebug::module_servercomm, "ServerComm",  "getMDDByOId" )

    return returnValue;
}



unsigned short
ServerComm::getNextTile( unsigned long   callingClientId,
                         RPCMarray**     rpcMarray )
{
    RMDBGENTER( 4, RMDebug::module_servercomm, "ServerComm",  "getNextTile" )

    RMInit::logOut << "Request (continuing): 'get next tile',..." << std::flush;

    unsigned long  transOffset = 0;
    unsigned long  transSize = 0;
    unsigned short statusValue = 0;
    unsigned short returnValue = 0;

    // initialize the result parameter for failure cases
    *rpcMarray = 0;

    ClientTblElt* context = getClientContext( callingClientId );

    if( context != 0 )
    {
        if( context->transTiles && context->tileIter )
        {
            Tile* resultTile = **(context->tileIter);
            r_Minterval mddDomain = resultTile->getDomain();
            void* useTransData;
            unsigned long totalSize;

            // allocate memory for the output parameter rpcMarray
            *rpcMarray = (RPCMarray*)mymalloc( sizeof( RPCMarray ) );

            if ( context->bytesToTransfer == 0 )
            {
                // free old data
                if (context->encodedData != NULL)
                {
                    free(context->encodedData);
                    context->encodedData = NULL;
                    context->encodedSize = 0;
                }
            }

            // note: transfer compression affects the current format, not the storage format.
            if ( context->encodedData == NULL )
            {
                totalSize = resultTile->getCompressedSize();
                //this is bad because useTransData is char* although it is not modified
                useTransData = (char*)resultTile->getContents();
                (*rpcMarray)->currentFormat = resultTile->getDataFormat();
                RMDBGMIDDLE(4, RMDebug::module_servercomm, "ServerComm", "using tile format " << (r_Data_Format)(*rpcMarray)->currentFormat)
            }
            else
            {
                totalSize = context->encodedSize;
                useTransData = context->encodedData;
                (*rpcMarray)->currentFormat = context->transferFormat;
                //FILE *fp = fopen("trans_data.raw", "wb"); fwrite(useTransData, 1, totalSize, fp); fclose(fp);
                RMDBGMIDDLE(4, RMDebug::module_servercomm, "ServerComm", "using transfer format " << (r_Data_Format)(*rpcMarray)->currentFormat)
            }
            // Preserve storage format
            (*rpcMarray)->storageFormat = resultTile->getDataFormat();
            RMDBGMIDDLE(4, RMDebug::module_servercomm, "ServerComm", "rpc storage  " << (r_Data_Format)(*rpcMarray)->storageFormat)
            RMDBGMIDDLE(4, RMDebug::module_servercomm, "ServerComm", "rpc current  " << (r_Data_Format)(*rpcMarray)->currentFormat)

            transSize = totalSize;

            if( totalSize > maxTransferBufferSize )
            {
                // if there is the rest of a tile to transfer, do it!
                if( context->bytesToTransfer )
                {
                    TALK( " resuming block transfer..." );
                    transOffset = totalSize - context->bytesToTransfer;
                    if( context->bytesToTransfer > maxTransferBufferSize )
                    {
                        transSize = maxTransferBufferSize;
                        statusValue = 1;
                    }
                    else
                    {
                        transSize = context->bytesToTransfer;
                        statusValue = 2;
                    }

                    context->bytesToTransfer -= transSize;
                }
                else // transfer first block of too large tile
                {
                    TALK( " has to be split..." );
                    transSize = maxTransferBufferSize;
                    context->bytesToTransfer = totalSize - transSize;
                    statusValue = 1;
                }
            }
            else    // resultTile->getSize() <= maxTransferBufferSize
                statusValue = 3;

            context->totalTransferedSize += transSize;

            // 1. convert domain
            (*rpcMarray)->domain = mddDomain.get_string_representation();

            // 2. copy data pointers
            TALK( " domain " << mddDomain << ", " << transSize << " bytes" );

            // allocate memory for the output parameter data and assign its fields
            (*rpcMarray)->data.confarray_len = (unsigned int)transSize;
            (*rpcMarray)->data.confarray_val = ((char*)useTransData) + transOffset;

            // 3. store cell type length
            (*rpcMarray)->cellTypeLength = resultTile->getType()->getSize();

            // increment iterator only if tile is transferred completely
            if( statusValue > 1 )
            {
                context->totalRawSize += resultTile->getSize();
                (*context->tileIter)++;
            }

            // delete tile vector and increment transfer collection iterator if tile iterator is exhausted
            if( (*context->tileIter) == context->transTiles->end() )
            {

                // delete tile vector transTiles (tiles are deleted when the object is deleted)
                if( context->transTiles )
                {
                    delete context->transTiles;
                    context->transTiles = 0;
                }

                // delete tile iterator
                if( context->tileIter )
                {
                    delete context->tileIter;
                    context->tileIter = 0;
                }

                if( context->transferDataIter )
                {
                    (*(context->transferDataIter))++;

                    if( *(context->transferDataIter) != context->transferData->end() )
                    {
                        returnValue = 1;
                        TALK( " some MDDs left..." );
                        RMInit::logOut << MSG_OK << ", some MDD(s) left."  << endl;
                    }
                    else
                    {
                        // no elements left -> delete collection and iterator

                        // Memory of last tile is still needed for the last byte transfer,
                        // therefore, do not release memory now, but with any next RPC call.
                        // context->releaseTransferStructures();

                        returnValue = 0;
                        RMInit::logOut << MSG_OK << ", all MDDs fetched."  << endl;
                    }
                }
                else
                {
                    returnValue = 0;
                    RMInit::logOut << MSG_OK << ", MDD transfer complete."  << endl;
                }

                if ((context->totalTransferedSize != context->totalRawSize) && (context->totalRawSize != 0))
                {
                    TALK( "(compressed using " <<  context->transferFormat << " to " << ((r_Double)(100 * context->totalTransferedSize)) / context->totalRawSize << "%) " );
                }
            }
            else
            {
                if( statusValue == 1 )   // at least one block in actual tile is left
                {
                    RMInit::logOut << MSG_OK << ", some block(s) left."  << endl;
                    returnValue = 3;
                }
                else  // tiles left in actual MDD
                {
                    RMInit::logOut << MSG_OK << ", some tile(s) left."  << endl;
                    returnValue = 2;
                }
            }
        }
        else    // no actual transfer collection or nothing left in the collection
        {
            returnValue = 4;
            RMInit::logOut << "Error: no transfer collection or nothing left in collection."  << endl;
        }

        context->release();
    }
    else
    {
        // client context not found
        RMInit::logOut << "Error: client not registered." << std::endl;
        returnValue = 4;
    }

    RMDBGEXIT( 4, RMDebug::module_servercomm, "ServerComm",  "getNextTile" )
    return returnValue;
}


unsigned short
ServerComm::endTransfer( unsigned long client )
{
    RMDBGENTER( 4, RMDebug::module_servercomm, "ServerComm",  "endTransfer" )
    unsigned short returnValue = 0;

    RMInit::logOut << "Client " << client << " called: endTransfer..." << std::flush;

    ClientTblElt* context = getClientContext( client );

    if( context )
    {
#ifdef RMANBENCHMARK
        Tile::relTimer.stop();
        Tile::opTimer.stop();
        if( context->evaluationTimer ) delete context->evaluationTimer;
        context->evaluationTimer = 0;
        if( context->transferTimer ) delete context->transferTimer;
        context->transferTimer = 0;
        RMTimer* releaseTimer = 0;

        if( RManBenchmark > 0 )
            releaseTimer = new RMTimer("ServerComm", "release");
#endif
        // release transfer collection/iterator
        context->releaseTransferStructures();

#ifdef RMANBENCHMARK
        if( releaseTimer ) delete releaseTimer;
#endif

        context->release();

        RMInit::logOut << MSG_OK << std::endl;
    }
    else
    {
        RMInit::logOut << "Error: client not registered." << std::endl;
        returnValue = 1;
    }

    RMDBGEXIT( 4, RMDebug::module_servercomm, "ServerComm",  "endTransfer" )
    return returnValue;
}



/*************************************************************************
 * Method name...: aliveSignal( unsigned long client )
 ************************************************************************/
unsigned short
ServerComm::aliveSignal( unsigned long client )
{
    unsigned short returnValue = 0;

    RMInit::logOut << "Client " << client << " called: endTransfer..." << std::flush;

    ClientTblElt* context = getClientContext( client );

    if( context )
    {
        // set the time of the client's last action to now
        context->lastActionTime = time( NULL );

        returnValue = 1;

        context->release();

        RMInit::logOut << MSG_OK << std::endl;
    }
    else
    {
        RMInit::logOut << "Error: client not registered." << std::endl;
    }

    return returnValue;
}



unsigned short
ServerComm::getNewOId( unsigned long callingClientId,
                       unsigned short objType,
                       r_OId& oid )
{
    RMDBGENTER( 4, RMDebug::module_servercomm, "ServerComm",  "getNewOId" )
    unsigned short returnValue = 0;

    RMInit::logOut << "Request: 'get new OId for " << (objType==1?"MDD":"collection") << " type'..." << std::flush;

    ClientTblElt* context = getClientContext( callingClientId );

    if( context != 0 )
    {
        EOId eOId;

        if( objType == 1 )
            EOId::allocateEOId( eOId, OId::MDDOID );
        else // objType == 2
            EOId::allocateEOId( eOId, OId::MDDCOLLOID );

        RMDBGMIDDLE( 4, RMDebug::module_servercomm, "ServerComm", "allocated " << eOId)
        oid = r_OId( eOId.getSystemName(), eOId.getBaseName(), eOId.getOId() );

        context->release();

        RMInit::logOut << MSG_OK << std::endl;
    }
    else
    {
        RMInit::logOut << "Error: client not registered." << std::endl;
        returnValue = 1;
    }

    RMDBGEXIT( 4, RMDebug::module_servercomm, "ServerComm",  "getNewOId " << returnValue)
    return returnValue;
}



unsigned short
ServerComm::getObjectType( unsigned long callingClientId,
                           r_OId& oid,
                           unsigned short &objType )
{
    RMDBGENTER( 4, RMDebug::module_servercomm, "ServerComm",  "getObjectType" )
    unsigned short returnValue = 0;

    RMInit::logOut << "Request: 'get object type by OID', oid = " << oid << "..." << std::flush;

    ClientTblElt* context = getClientContext( callingClientId );

    if( context != 0 )
    {
        OId oidIf( oid.get_local_oid() );

        objType = oidIf.getType();

        if( objType == OId::INVALID )
        {
            // oid not found
            RMInit::logOut << "Error: no type for this oid." << std::endl;
            returnValue = 2;
        }
        else
        {
            RMInit::logOut << "type is " << (objType==1?"MDD":"collection") << "..." << MSG_OK << endl;
        }

        context->release();
    }
    else
    {
        RMInit::logOut << "Error: client not registered." << endl;
        returnValue = 1;
    }

    RMDBGEXIT( 4, RMDebug::module_servercomm, "ServerComm",  "getObjectType" )
    return returnValue;
}



unsigned short
ServerComm::getTypeStructure( unsigned long  callingClientId,
                              const char* typeName,
                              unsigned short typeType,
                              char* &typeStructure )
{
    RMDBGENTER( 4, RMDebug::module_servercomm, "ServerComm",  "getTypeStructure" )
    unsigned short returnValue = 0;

    RMInit::logOut << "Request: 'get type structure', type = '" << typeName << "'..." << std::flush;

    ClientTblElt* context = getClientContext( callingClientId );
    if (context == 0)
    {
        RMInit::logOut << "Error: client not registered." << std::endl;
        returnValue = 1;
    }

    if (returnValue==0 && !transactionActive)
    {
        RMInit::logOut << "Error: no transaction open." << endl;
        returnValue = 1;
    }

    if (returnValue==0)
    {
        if( typeType == 1 )
        {
            // get collection type
            CollectionType* collType = (CollectionType*)TypeFactory::mapSetType( (char*)typeName );

            if( collType )
                typeStructure = collType->getTypeStructure(); // no copy
            else
                returnValue = 2;
        }
        else if( typeType == 2 )
        {
            // get MDD type
            const MDDType* mddType = TypeFactory::mapMDDType( typeName );

            if( mddType )
                typeStructure = mddType->getTypeStructure(); // no copy
            else
                returnValue = 2;
        }
        else    // base type not implemented
        {
            returnValue = 2;
        }

        if( returnValue == 2 )
            RMInit::logOut << "Error: unknown type." << endl;
        else
            RMInit::logOut << MSG_OK << endl;

        context->release();
    }

    RMDBGEXIT( 4, RMDebug::module_servercomm, "ServerComm", "getTypeStructure" )
    return returnValue;
}


unsigned short
ServerComm::setTransferMode( unsigned long callingClientId,
                             unsigned short format,
                             const char* formatParams )
{
    RMDBGENTER(4, RMDebug::module_servercomm, "ServerComm", "setTransferMode(" << callingClientId << ", " << format << ", " << formatParams)

    RMInit::logOut << "Request: 'set transfer mode', format = '" << format << "', params = '" << formatParams << "'..." << std::flush;

    unsigned short retval = 1;

    ClientTblElt* context = getClientContext( callingClientId );

    if (context != 0)
    {
        r_Data_Format fmt = (r_Data_Format)format;
        if (context->transferFormatParams != NULL)
        {
            delete [] context->transferFormatParams;
            context->transferFormatParams = NULL;
            // revert the transfer format strictness
            context->exactFormat = 0;
        }
        if (formatParams != NULL)
        {
            context->transferFormatParams = new char[strlen(formatParams)+1];
            strcpy(context->transferFormatParams, formatParams);
            // extract any occurrences of ``exactformat''
            context->clientParams->process(context->transferFormatParams);
        }
        context->transferFormat = fmt;

        RMInit::logOut << MSG_OK << std::endl;
        retval = 0;

        context->release();
        RMDBGMIDDLE(1, RMDebug::module_servercomm, "ServerComm", "setTransferMode(...) current transfer format :" << context->transferFormat)
        RMDBGMIDDLE(1, RMDebug::module_servercomm, "ServerComm", "setTransferMode(...)current transfer params :" << context->transferFormatParams );
    }
    else
    {
        RMInit::logOut << "Error: client not registered." << std::endl;
        retval = 1;
    }

    RMDBGEXIT(4, RMDebug::module_servercomm, "ServerComm",  "setTransferMode " << retval)
    return retval;
}

unsigned short
ServerComm::setStorageMode( unsigned long callingClientId,
                            unsigned short format,
                            const char* formatParams )
{
    RMDBGENTER(4, RMDebug::module_servercomm, "ServerComm", "setStorageMode(" << callingClientId << ", " << format << ", " << formatParams)

    RMInit::logOut << "Request: 'set storage mode', format = " << format << ", params = " << formatParams << "..." << std::flush;

    unsigned short retval = 1;

    ClientTblElt* context = getClientContext( callingClientId );

    if (context != 0)
    {
        r_Data_Format fmt = r_Array;

        if (context->storageFormatParams != NULL)
        {
            delete [] context->storageFormatParams;
            context->storageFormatParams = NULL;
        }
        if (formatParams != NULL)
        {
            context->storageFormatParams = new char[strlen(formatParams)+1];
            strcpy(context->storageFormatParams, formatParams);
        }
        context->storageFormat = fmt;

        RMInit::logOut << MSG_OK << std::endl;
        retval = 0;

        context->release();
        RMDBGMIDDLE(1, RMDebug::module_servercomm, "ServerComm", "setStorageMode(...) current storage format :" << context->storageFormat)
        RMDBGMIDDLE(1, RMDebug::module_servercomm, "ServerComm", "setStorageMode(...) current storage params :" << context->storageFormatParams)
    }
    else
    {
        RMInit::logOut << "Error: client not registered." << std::endl;
        retval = 1;
    }

    RMDBGEXIT(4, RMDebug::module_servercomm, "ServerComm",  "setStorageMode " << retval)
    return retval;
}

int
ServerComm::ensureTileFormat( r_Data_Format &hasFmt,
                              r_Data_Format needFmt,
                              const r_Minterval &dom,
                              const BaseType *type,
                              char *&data,
                              unsigned long &size,
                              int repack,
                              int owner,
                              const char *params )
{
    RMDBGENTER(2, RMDebug::module_servercomm, "ServerComm", "ensureTileFormat(" << hasFmt << ", " << needFmt << ", " << dom << ", " << type->getName() << ", data, " << size << ", repack " << repack << ", owner " << owner << ", params " << params << ")")
    int status = ENSURE_TILE_FORMAT_OK;

    RMDBGMIDDLE(2, RMDebug::module_servercomm, "ServerComm", "ensureTileFormat(...) #Size 1=" << size);

    RMDBGMIDDLE(2, RMDebug::module_servercomm, "ServerComm", "ensureTileFormat(...) #Size 3=" << size);
    RMDBGEXIT(2, RMDebug::module_servercomm, "ServerComm", "ensureTileFormat(...) " << ENSURE_TILE_FORMAT_OK)
    return status;
}
