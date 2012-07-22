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
 * INCLUDE: rpcclientcomm.hh
 *
 * MODULE:  clientcomm
 * CLASS:   RpcClientComm
 *
 * COMMENTS:
 *      None
*/

// Everything commented out with /* RNP   RNP*/ is done because of the switch to RNP

#ifndef RPCCLIENTCOMM_HH
#define RPCCLIENTCOMM_HH

#include "clientcomm.hh"

/* RNP
#include "raslib/rminit.hh"
#include "raslib/error.hh"
#include "rasodmg/oqlquery.hh"
#include "rasodmg/marray.hh"

#include "raslib/primitivetype.hh"
RNP */

#ifdef __VISUALC__
extern "C"
{
#include <rpc/rpc.h>
}
#else
#include <rpc/rpc.h>
#endif

#ifndef _RPCIF_
#define _RPCIF_
#include "clientcomm/rpcif.h"
#endif


/* RNP
template <class T> class r_Set;
class r_Ref_Any;
class r_Base_Type;
class r_Parse_Params;
RNP */


/**
*   @file rpcclientcomm.hh
*   @Doc:
*   The class ClientComm represents {\bf one} RPC connection between
*   {\bf one} client entity (e.g. an object of class \Ref{r_Database}) and
*   the server. Therefore, the host name has to be provided at
*   the constructor.
*   Basically, all methods of a Clientcomm object map directly
*   to the corresponding RPC calls. All public clientcomm methods
*   may throw exceptions of type {\tt r_Error_TransferFailed} if the
*   communication breaks down during RPC execution.
*
*   @ingroup ClientComm
*/

class RpcClientComm : public ClientComm
{
public:
    /// constructor getting the host name of the rasmgr host and it's listening port (default 7001).
    RpcClientComm( const char* rasmgrHost, int rasmgrPort = RASMGRPORT ) throw( r_Error );

    /*@Doc:
      May throw an exception of type {\tt r_Error_HostInvalid} if the hostname
      cannot be successfully resolved and an exception of type {\tt r_Error_ServerInvalid}
      if there is no RasDaMan RPC server running on the designated host.
    */

    /// destructor (closes the connection and releases resources)
    ~RpcClientComm() throw();

    bool effectivTypeIsRNP() throw();

    //@Man: Database methods
    //@{
    ///

    /// open database
    int openDB( const char* database );
    /// close current database
    int closeDB();
    /// create a database
    int createDB( const char* name ) throw(r_Error);
    /// destroy a database
    int destroyDB( const char* name ) throw(r_Error);

    ///
    //@}

    //@Man: Transaction methods
    //@{
    ///

    /// begin transaction
    int openTA( unsigned short readOnly = 0 ) throw(r_Error);
    /// commit current transaction
    int commitTA() throw(r_Error);
    /// abort current transaction
    int abortTA();

    ///
    //@}

    //@Man: MDD methods
    //@{
    ///

    /// inserts a MDD object in an existing MDD collection on the server
    void insertMDD( const char* collName, r_GMarray* mar ) throw( r_Error );
    /// gets MDD object by oid
    r_Ref_Any getMDDByOId( const r_OId& oid ) throw( r_Error );

    ///
    //@}

    //@Man: Collection methods
    //@{
    ///

    /// creates an empty MDD collection on the server
    void insertColl( const char* collName, const char* typeName, const r_OId& oid ) throw( r_Error );
    /// deletes an MDD collection by name
    void deleteCollByName( const char* collName ) throw( r_Error );
    /// deletes an object by oid (right now, objects are collection only)
    void deleteObjByOId( const r_OId& oid ) throw( r_Error );
    /// removes an object from a collection
    void removeObjFromColl( const char* name, const r_OId& oid ) throw ( r_Error );
    /// gets collection by name
    r_Ref_Any getCollByName( const char* name ) throw( r_Error );
    /// gets collection by oid
    r_Ref_Any getCollByOId ( const r_OId& oid ) throw( r_Error );
    /// gets collection references by name
    r_Ref_Any getCollOIdsByName( const char* name ) throw( r_Error );
    /// gets collection references by oid
    r_Ref_Any getCollOIdsByOId ( const r_OId& oid ) throw( r_Error );

    ///
    //@}

    //@Man: Query methods
    //@{
    ///

    /// query execution
    void executeQuery( const r_OQL_Query& query, r_Set< r_Ref_Any >& result ) throw( r_Error );
    /*@Doc:
      Executes a retrieval query of type \Ref{r_OQL_Query} and returns the result. Every
      MDD object of the MDD collection is fetched from the server and inserted
      in the resulting \Ref{r_Set}.
    */

    /// update execution
    void executeQuery( const r_OQL_Query& query ) throw( r_Error );
    /*@Doc:
      Executes an update query of type \Ref{r_OQL_Query}.
    */

    ///
    //@}


    //@Man: System methods
    //@{
    ///

    /// get new oid
    r_OId getNewOId( unsigned short objType ) throw(r_Error);

    /// get oid type
    unsigned short getObjectType( const r_OId& oid ) throw(r_Error);

    /// get type structure
    /// dallocate using delete []
    char* getTypeStructure( const char* typeName, r_Type_Type typeType ) throw(r_Error);

    ///
    //@}

    /// provides read access to my clientID
    inline unsigned long getClientID() const;

    /// provides read access to my binding handle
    inline CLIENT* getBindingHandle() const;

    //@Man: Methods for asynchronious alive signal concept
    //@{

    /// triggers an alive signal
    void triggerAliveSignal();
    /**
      First, it sets the switch {\tt aliveSignalRemaining} saying that an alive signal
      should be send to the server. Then it calls {\tt sendAliveSignal()} to send it
      immediately if possible.
    */

    /// send an alive signal if necessary and possible
    void sendAliveSignal();
    /**
      Sends an alive signal to the server if the switch {\tt aliveSignalRemaining} is
      set and no other RPC is active. If a signal can be sent, {\tt aliveSignalRemaining}
      is set to 0 again.
    */

    /// set the preferred transfer format
    int setTransferFormat( r_Data_Format format, const char* formatParams=NULL );

    int writeWholeMessage(int socket,char *destBuffer,int buffSize);
    int readWholeMessage(int socket,char *destBuffer,int buffSize);

    /// set the preferred storage format
    int setStorageFormat( r_Data_Format format, const char *formatParams=NULL );

    /// sets a mutex for RPC
    void setRPCActive();

    /// frees the mutex for RPC and calls sendAliveSignal() for sending remaining signals
    void setRPCInactive();

    /// checks if an RPC is active
    int checkRPCActive();

    /// get extended error information
    const char *getExtendedErrorInfo() throw(r_Error);

    /// get real server name (the dinamic one, assigned by the RasMGR)
    const char* getServerName();

    /// user identification for RasMGR
    void setUserIdentification(const char *userName, const char *plainTextPassword);

    /// set maximum retry to get a server
    void setMaxRetry(unsigned int newMaxRetry);

    /// get maximum retry to get a server
    unsigned int  getMaxRetry();

    ///
    //@}

    void setTimeoutInterval(int seconds);

    int  getTimeoutInterval();

private:
    // binding handle
    CLIENT* binding_h;

    /// client ID assigned to me by the server (used to index the client table)
    unsigned long clientID;

#ifdef __VISUALC__
    // save the timerid for later killing it
    UINT timerid;
#endif

    // status variable of the last RPC library function call
    unsigned long status;

    /// status: server is running
    int serverUp;

    /// determines if an RPC is active
    int rpcActive;

    /// switch determining if a alive signal should be send to the server
    int aliveSignalRemaining;

    /// internal function for reading an MDD from the database
    unsigned short getMDDCore( r_Ref<r_GMarray> &mdd, GetMDDRes *thisResult, unsigned int isQuery ) throw( r_Error );

    /// concatenate data to an array, making sure there are no overflows (used by getMDDCore())
    int concatArrayData( const char *source, unsigned long srcSize, char *&dest,
                         unsigned long &destSize, unsigned long &destLevel );

    /// internal function for converting a \Ref{r_GMarray} into its RPC representation
    void getMarRpcRepresentation( const r_GMarray* mar, RPCMarray*& rpcMarray,
                                  r_Data_Format initStorageFormat = r_Array,
                                  const r_Base_Type *bt = NULL);

    /// internal function for freeing data allocated by getMarRpcRepresentation()
    void freeMarRpcRepresentation( const r_GMarray* mar, RPCMarray* rpcMarray );

    /// internal function for client/server protocol handling of MDD collection transfer
    void getMDDCollection( r_Set< r_Ref_Any >& result, unsigned int isQuery ) throw(r_Error);

    /// internal function for client/server protocol handling of non-MDD collection transfer
    void getElementCollection( r_Set< r_Ref_Any >& result ) throw(r_Error);

    /// endianness of client and server (0 means big endian)
    int endianServer;
    int endianClient;

    /// version of server's RPC interface
    int serverRPCversion;

    /// data format for transfer compression
    r_Data_Format transferFormat;
    /// storage format for inserting new tiles
    r_Data_Format storageFormat;
    /// transfer format parameters
    char* transferFormatParams;
    /// storage format parameters
    char *storageFormatParams;
    /// parameter object for configuration
    r_Parse_Params *clientParams;
    /**
       Possible parameters:

       \begin{tabular}{lcl}
       compserver && int && on insert the server compresses the data, rather than the client\\
       exactformat && int && the server must provide data in the exact transfer data format\\
       \end{tabular}

       compserver has to be provided in storageFormatParams during insert
       exactformat has to be provided in transferFormatParams during download
    */
    /// policy is compress-on-server
    int serverCompresses;
    /// policy is exact
    int exactFormat;

    /// dinamically connects to RPC-Server, requesting a readOnly/read-write connection
    int connectToServer(unsigned short readOnly);

    /// disconnects from RPC-Server after completed request
    int disconnectFromServer() throw();

    /// requests a free server from the rasmgr, retrying maxRetry times
    int getFreeServer(unsigned short readOnly);

    /// requests a free server from the rasmgr
    int executeGetFreeServer(unsigned short readOnly);

    /// open database
    int executeOpenDB( const char* database );

    /// issues "close current database" - request
    int executeCloseDB();

    /// issues "begin transaction" - request
    int executeOpenTA( unsigned short readOnly = 0 );

    /// issues "commit current transaction" - request
    int executeCommitTA();

    /// issues "abort current transaction" - request
    int executeAbortTA();

    /// the name of the rasmgr host
    char *rasmgrHost;

    /// the listening port of the rasmgr
    int  rasmgrPort;

    /// the name of the server host
    char serverHost[100]; //can't be just a pointer, it's never stored elsewhere

    /// RPC prognum of the server
    unsigned long RPCIF_PARA;

    /// the name of the opened database, needed because it will be opened again and again, in a hidden operation
    /// all 3 will be dinamic allocated strings when we change to parallel-proc
    char dataBase[100];

    // the capability
    char capability[100];

    /// user identification string
    char identificationString[100];

};

//RNP #include "clientcomm.icc"


#endif
