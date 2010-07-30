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
 * INCLUDE: servercomm.hh
 *
 * MODULE:  servercomm
 * CLASS:   ServerComm
 *
 * COMMENTS:
 *			None
*/

#ifndef _SERVERCOMM_
#define _SERVERCOMM_

// Put it in front of any typedef bool ... because o2 is using bool as a variable. 
// #include "o2template_CC.hxx"

#include <list>

#include <rpc/rpc.h>
#include "raslib/error.hh"
#include "raslib/oid.hh"
#include "raslib/minterval.hh"
#include "raslib/rmdebug.hh"

#include "reladminif/adminif.hh"
#include "reladminif/databaseif.hh"
#include "reladminif/transactionif.hh"
#include "relcatalogif/basetype.hh"

#include "servercomm/callbackmgr.hh"

#ifndef _RPCIF_
  #define _RPCIF_
  #include "clientcomm/rpcif.h"
#endif

#ifdef DECALPHA
#include <rpc/svc.h>
#endif

// forward declarations
class QtData;
class MDDObj;
class MDDCollIter;
class MDDColl;
class r_Parse_Params;

//@ManMemo: Module: {\bf servercomm}

/*@Doc:
  The class servercomm describes the one and only server communication object
  that can exist in a RasDaMan RPC server. It manages listening for client and
  maps incoming calls to the respective remote procedures (which reside in the
  file manager.cc). These remote procedures are global functions
  which mainly concern with RPC call processing and finally call the methods 
  of this servercomm class to forward client requests.
*/

class ServerComm
{
  public:
  
    /// the class defines an entry of the client table
    class ClientTblElt
    {
      public:
        /// default constructor
        ClientTblElt( const char* clientIdText, unsigned long clientId );	
        /**
          Default constructor that takes the information to be placed in the
	  clientIdText field of the client table entry and the unique ID to
	  be placed in the clientId field.
	*/

        /// destructor
	~ClientTblElt();

        /// release client context
        void release();
        /**
          Releasing the client context means to decrease the currentUsers counter
          and to update lastActionTime.
        */
        
	void endRequest();
	 
	/// releases transfer collection/iterator
	void releaseTransferStructures();	
	/**
	  The method releases transfer collection and iterator. As the collection is a
	  persistent one, care has to be taken that creation and deletion is done
	  within the same transaction.
	*/
	
        /// unique client identification assigned by the server
	unsigned long  clientId;

        /// counter indicating the number of current users
        unsigned int   currentUsers;

	/// binding information about the client (IP address and TCP port number)
	char*          clientIdText;

	/// Name of the client user name (if available)
	char*          userName;
	
	/// Name of the actual database (if one is open)
	char*          baseName;
	
	/// time when the database was opened (for curiosity purposes)
	unsigned long  creationTime;

	/// time of the client's last action (for garbage collection purposes)
	unsigned long  lastActionTime;
	
        /// convert raw array data to this data format before transfer
        r_Data_Format  transferFormat;
        char*          transferFormatParams;
        /// send data to client in the exact transfer format
        int            exactFormat;
        /// store array data in this data format in the database
        r_Data_Format  storageFormat;
        char*          storageFormatParams;

        /// the tile data converted into the transfer format, if required
        void*          encodedData;
        unsigned long  encodedSize;
        /// for establishing the compression ratio
        unsigned long  totalRawSize;
        unsigned long  totalTransferedSize;

	/// pointer to an MDD collection
        MDDColl*       transferColl;
        /**
           For collection of MDD constants with an update query.
        */

        /// pointer to an iterator for collection transferColl
	MDDCollIter*   transferCollIter;
	
        /// pointer to the query result which is currently in transfer
        std::vector<QtData*>* transferData;
        /**
           For the result of the last query (NULL if the result is completely delivered to the client).
        */

        /// point to an iterator for transfer data
        std::vector<QtData*>::iterator* transferDataIter;

	/// pointer to a persistent MDD object for tile based transfers
	MDDObj*    assembleMDD;

        /// pointer to an MDD object for tile base transfer
        MDDObj*        transferMDD;
	
	/// std::vector storing tiles of actual MDD for transfer
	std::vector< Tile* >* transTiles;
	
	/// iterator for the std::vector above
	std::vector< Tile* >::iterator* tileIter;
	
	/// std::vector storing pointers to transient tiles
	std::vector< Tile* >* deletableTiles;
        /**
          The tiles referenced by these pointers are border tiles dynamically created in getNextMDD().
          They do not belong to any MDD object, and, therefore, they have to be deleted explicitly.
	*/

	/// bytes to transfer in actual tile (valid only if tile is larger than {\tt MAXTRANSBYTES})
	unsigned long bytesToTransfer;

        /// std::vector of persistent MDD collections in use
        std::vector< MDDColl* >* persMDDCollections;
	
        /// object representing the actual database
	DatabaseIf     database;

        /// object representing the actual transaction (only one at a time possible)
	TransactionIf  transaction;

        /// pointer to a timer for recording transaction time
        RMTimer* taTimer;      

        /// pointer to a timer for recording transfer time
        RMTimer* transferTimer;      
	
        /// pointer to a timer for recording evaluation time
        RMTimer* evaluationTimer;      
	
        /// parameter object
        r_Parse_Params *clientParams;

      private:
        /// empty private definition prevents of using the copy constructor
	ClientTblElt( const ClientTblElt& ){};
    };

    /// default constructor
    ServerComm();

    /// constructor getting the client time out and the time interval for management routines, together with listen port, rasmgr host and port and the server name
    ServerComm( unsigned long timeOut, unsigned long managementInterval , unsigned long listenPort, char* rasmgrHost, unsigned int rasmgrPort, char* serverName);  

    /// destructor
    virtual ~ServerComm();

    /// forces the server to listen for client calls
    virtual void startRpcServer() throw( r_Error );

    /// stops the server
    virtual void stopRpcServer();

    // informs RasMGR:
    void informRasMGR(int);
    #define SERVER_DOWN        0
    #define SERVER_AVAILABLE   1
    // 2 is server crushed, but it's generated by rasmgr!
    // regularly signal the rasmgr that we are available
    #define SERVER_REGULARSIG  3
    
    /// adds an entry to the client table (used in RasServerEntry)
    void addClientTblEntry( ClientTblElt *context ) throw ( r_Error );
    /**
      Adds the context entry passed to the client table.
      Throws an exception if context==NULL.
    */
    
    /// deletes an entry of the client table (must be public because it is used in the global garbage collection function)
    unsigned short deleteClientTblEntry( unsigned long ClientId );    
    /**
      Deletes the entry of the client table corresponding to the given client id.
      If no corresponding id is found, false is returned.     
    */
    
    // quick hack function used when stopping server to abort transaction and close db
    void abortEveryThingNow();
        
    /// print server status with client table content to {\tt s}
    virtual void printServerStatus( ostream& s=cout );

    /// get server status
    virtual void getServerStatus( ServerStatRes &returnStruct );

    /// the client table which holds information about the calling clients
    static std::list<ClientTblElt*>  clientTbl;

    /// last used client ID (this is increased by one to get the clientId for the next client)
    static unsigned long clientCount;

    /// inactivity timeout in seconds after which pending client data is deleted
    const unsigned long clientTimeout;

    /// do a garbage collection every {\tt garbageCollectionInterval} seconds (ONC RPC only)
    const unsigned long garbageCollectionInterval;    

    /// flag for active o2 transaction (stores the clientID of the owner of the active transaction, or 0 if none open)
    unsigned long transactionActive;

    /// memory used by malloc in ordinary blocks (set in dumpClientTable)
    long memUsed;

    /// stores a pointer to the actual servercomm object, only one can exist at a time
    static ServerComm* actual_servercomm; 

    /**
      the callback manager object for garbage collection, license check, ...
      Always instantiated with default size, resized on startup if required.
    */
    CallBackManager callback_mgr;

    //@Man: Communication methods
    //@{
    ///

      /// process the client's alive signal
      virtual unsigned short aliveSignal( unsigned long client );
      /**
        The method take the alive signal of a client and updates the last action time.
      
        Return values:
        \begin{tabular}{lll}
        0 && operation was successfull\\
        1 && client context not found\\
        \end{tabular}    
      */

      ///
      /// open database
      virtual unsigned short openDB( unsigned long callingClientId, const char* dbName, const char* userName );
      /**
        The method opens the database with {\tt dbName}. The return value means the following:
       
        \begin{tabular}{lll}
        0 && database successfully opened\\
        1 && client context not found\\
        2 && database does not exist\\
        3 && database is already open\\
        \end{tabular}
      */    
    
      /// close current database
      virtual unsigned short closeDB( unsigned long callingClientId );
      /**
        The return value has the following meaning:
        \begin{tabular}{lll}
        0 && operation was successful\\
        1 && client context not found\\
        \end{tabular}    
      */

      /// create a database
      virtual unsigned short createDB( char* name );

      /// destroy a database
      virtual unsigned short destroyDB( char* name );

      ///
      /// open transaction
      virtual unsigned short beginTA( unsigned long callingClientId, unsigned short readOnly=0 );
      /**
        The return value has the following meaning:
        \begin{tabular}{lll}
        0 && operation was successful\\
        1 && client context not found\\
        2 && other transaction already active\\
        \end{tabular}    
      */


      /// commit current transaction
      virtual unsigned short commitTA( unsigned long callingClientId );
      /**
        The return value has the following meaning:
        \begin{tabular}{lll}
        0 && operation was successful\\
        1 && client context not found\\
        \end{tabular}    
      */


      /// abort current transaction
      virtual unsigned short abortTA( unsigned long callingClientId );
      /**
        The return value has the following meaning:
        \begin{tabular}{lll}
        0 && operation was successful\\
        1 && client context not found\\
        \end{tabular}    
      */

      /// is transaction open currently?
      /**
        The return value has the following meaning:
        \begin{tabular}{lll}
        true && a transaction is open\\
        false && no transaction is open\\
        \end{tabular}    
      */
      
      virtual bool isTAOpen( unsigned long callingClientId );

      ///    
      /// executes a retrieval query and prepares the result for transfer with \Ref{getNextMDD}.
      virtual unsigned short executeQuery( unsigned long callingClientId, const char* query, ExecuteQueryRes &returnStructure );
      /**
        Executes a query and puts the result in the actual transfer collection.
        The first parameter is the unique client id
        for which the query should be executed. The second parameter is the
        query itself represented as a string.
      
        Return values
        \begin{tabular}{lll}
        0 && operation was successful - result collection holds MDD elements\\
        1 && operation was successful - result collection holds non-MDD elements\\
        2 && operation was successful - result collection has no elements\\
        3 && client context not found\\
        4 && parse errror\\
        5 && execution error\\
        \end{tabular}    

        Communication protocol (return value = 0)
        \begin{tabular}{lll}
        \Ref{executeQuery} && \\
        ->                 && \Ref{getNextMDD} \\
                           && ->               && \Ref{getNextTile} \\
                           &&                  && : \\
                           && :\\
        \Ref{endTransfer} \\
        \end{tabular}    

        Communication protocol (return value = 1)
        \begin{tabular}{lll}
        \Ref{executeQuery} && \\
        ->                 && \Ref{getNextElement} \\
                           && :\\
        \Ref{endTransfer} \\
        \end{tabular}    
      */
    
      ///
      /// get the domain of the next MDD of the actual transfer collection
      virtual unsigned short getNextMDD( unsigned long  callingClientId,
					 r_Minterval    &mddDomain,
					 char*          &typeName,
					 char*          &typeStructure,
					 r_OId          &oid,
					 unsigned short &currentFormat             );
      /**
        The Method gets the domain of the next MDD of the actual transfer collection. 
        The first parameter is the unique client id. The second parameter returns the
        domain of the MDD to be transfered. {\tt typeName} returns the name of the
        MDD type and its structure.
        Transfer of MDD data is tile-based using the method \Ref{getNextTile}.
      
        Return values:
        \begin{tabular}{lll}
        0 && operation was successful, at least one MDD is left in the transfer collection\\
        1 && nothing left in the transfer collection\\
        2 && client context not found, no tiles in the MDD object, no actual transfer collection \\
        \end{tabular}    
      */

      /// get the next scalar element in the actual transfer collection.
      virtual unsigned short getNextElement( unsigned long callingClientId,
					     char*         &buffer,
					     unsigned int  &bufferSize );
      /**
        The Method gets the next non-MDD element in the actual transfer collection.
        The first parameter is the unique client id. The second parameter returns a 
        pointer to the memory occupied by the next element and the third one delivers
        the size of the buffer.
      
        Return values:
        \begin{tabular}{lll}
        0 && operation was successful, at least one element is left in the transfer collection\\
        1 && operation succesful, nothing left in the transfer collection\\
        2 && client context not found, no tiles in the MDD object, no actual transfer collection \\
        \end{tabular}    
      */

      /// get an MDD by OId
      virtual unsigned short getMDDByOId( unsigned long  callingClientId,
					  r_OId          &oid,
					  r_Minterval    &mddDomain,
					  char*          &typeName,
					  char*          &typeStructure,
					  unsigned short &currentFormat );
      /**
        The Method gets an MDD by OId {\tt oid}. If the MDD is found, it is initialized as transfer 
        object and can be picked up by \Ref{getNextTile} calls (tile-based transfer).

        Additionally, the method returns domain, type name, and type structure of the found MDD
        object by reference parameters.

        Return values:
        \begin{tabular}{lll}
        0 && operation was successful\\
        1 && client context not found\\
        2 && object with this oid not found\\
        3 && object has no tiles
        \end{tabular}    

        Communication protocol
        \begin{tabular}{lll}
        \Ref{getMDDByOId} \\
        ->                && \Ref{getNextTile} \\
                          && : \\
        \Ref{endTransfer} \\
        \end{tabular}    
      */

      /// get next tile of the actual MDD of the actual transfer collection
      virtual unsigned short getNextTile( unsigned long callingClientId,
					  RPCMarray**   rpcMarray );
      /**
        The Method gets the next tile of the actual MDD of the actual transfer collection. 
        The first parameter is the unique client id. The second parameter is the
        RPC representation of the Marray representing the tile. If a tile is too large to be
        transferred in one piece, the data is split. To get the rest of the data, consecutively
        use this method.
      
        Return values:
        \begin{tabular}{lll}
        0 && operation was successful, no further MDDs are left\\
        1 && operation was successful, at least one MDD is left in the transfer collection\\
        2 && operation was successful, at least one tile is left in the actual MDD\\
        3 && operation was successful, at least one block is left in the actual tile\\
        4 && client context not found, no tiles in the MDD object, no actual transfer collection \\
          && or nothing left in the collection\\
        \end{tabular}

        Examples of valid return value chains:
        \begin{itemize}
        \item To be transferred: 1 MDD consisting of 1 tile (which is too large)\\
        \begin{verbatim}
        3 ->...-> 3 -> 0
        \end{verbatim}
        \item To be transferred: 1 MDD consisting of 2 tiles (the first is too large)\\
        \begin{verbatim}
        3 ->...-> 3 -> 2 -> 0
        |--------------|    |
            1st tile     2nd tile
        \end{verbatim}
        \item To be transferred: 2 MDDs, each consisting of 1 tile (none too large)\\
        \begin{verbatim}
        1 -> 0
        \end{verbatim}
        \item To be transferred: 3 MDDs, the first (A) consisting of 1 tile (not too large),\\
        the second (B) consisting of 2 tiles (B1, B2, of which the first is too large),
        the third (C) consisting of 2 tiles (C1, C2, of which the second is too large),
        \begin{verbatim}
        1 -> 3 ->...-> 3 -> 2 -> 1 -> 2 -> 3 ->...-> 3 -> 0
        |    |--------------|    |    |    |--------------|
        |           B1          B2    C1          C2
        |    |-------------------|    |-------------------|
        A              B                        C
        \end{verbatim}
        \end{itemize}
      */

      /// process the client's alive signal
      virtual unsigned short endTransfer( unsigned long client );
      /**
        The method terminates a transfer session and releases all transfer structures.
      
        Return values:
        \begin{tabular}{lll}
        0 && operation was successfull\\
        1 && client context not found\\
        \end{tabular}    
      */

      ///
      /// prepares transfer of MDD constants and execution of update query
      virtual unsigned short initExecuteUpdate( unsigned long callingClientId );
      /**
        Return values:
        \begin{tabular}{lll}
        0 && operation was successful\\
        1 && client context not found\\
        \end{tabular}    

        Communication protocol
        \begin{tabular}{lll}
        \Ref{initExecuteUpdate} && \\
        ->                      && \Ref{startInsertTransMDD} \\
                                && ->                        && \Ref{insertTile} \\
                                &&                           && :\\
                                && \Ref{endInsertMDD}\\
                                && :\\
        \Ref{executeUpdate}     && \\
        \end{tabular}    

        Note: Method \Ref{executeUpdate} can be invoked without the \Ref{initExecuteUpdate} 
              prolog in case of no constant MDD objects.
      */

      /// executes an update query
      virtual unsigned short executeUpdate( unsigned long callingClientId, const char* query, ExecuteUpdateRes &returnStructure );
      /**
        Executes an update query.
        The first parameter is the unique client id
        for which the query should be executed. The second parameter is the
        query itself represented as a string.
      
        Return values:
        \begin{tabular}{lll}
        0 && operation was successful\\
        1 && client context not found\\
        2 && parse errror\\
        3 && execution error\\
        \end{tabular}    
      */
    
      ///
      /// prepares an MDD (transient) for transfer of tiles
      virtual unsigned short startInsertTransMDD( unsigned long callingClientId, 
						  r_Minterval   &domain, 
						  unsigned long typeLength,
						  const char*   typeName );
      /**
        Creates an object for tile based transfer with method \Ref{insertTile}.

        The first parameter is the unique client id for which the MDD should be created.
        The second parameter is the
        name of the collection to insert the MDD object. The third parameter holds the
        spatial domain of the following MDD object and {\tt typeLength} specifies the size of
        the base type in bytes. The last one gives the type structure as string representation.
      
        Return values:
        \begin{tabular}{lll}
        0 && operation was successful\\
        1 && client context not found\\
        2 && MDD type name not found\\
        3 && MDD and its type are incompatible\\
        \end{tabular}    
      */

      /// create a new persistent MDD object for tile based transfers
      virtual unsigned short startInsertPersMDD( unsigned long callingClientId, 
						 const char*   collName,
						 r_Minterval&  domain, 
						 unsigned long typeLength,
						 const char*   typeName,
						 r_OId&        oid         );
      /**
        Creates an object for tile based transfer with method \Ref{insertTile} to be 
        inserted into the specified MDD collection.

        Parameters
        \begin{tabular}{lll}
        {\tt callingClientId}  && unique client id of the calling client\\
        {\tt collName}         && name of the collection to insert the MDD object\\
        {\tt domain}           && spatial domain\\
        {\tt typeLength}       && size of base type in bytes\\
        {\tt typeName}         && type structure as string representation\\
        {\tt oid}              && object identifier\\
        \end{tabular}    

        Return values
        \begin{tabular}{lll}
        0 && operation was successful\\
        1 && client context not found\\
        2 && MDD type name not found\\
        3 && types of MDD and collection are incompatible\\
        4 && MDD and its type are incompatible\\
        5 && collection does not exist\\
        6 && creation of persistent object failed\\
        \end{tabular}    

        Communication protocol
        \begin{tabular}{lll}
        \Ref{startInsertPersMDD} && \\
        ->                       && \Ref{insertTile} \\
                                 && :\\
        \Ref{endInsertMDD} && \\
        \end{tabular}    
      */

      /// insert a tile into a persistent MDD object
      virtual unsigned short insertTile( unsigned long callingClientId, 
					 int isPersistent,
					 RPCMarray* rpcMarray );
      /**
        Inserts a tile into the current MDD object.
      
        Parameters
        \begin{tabular}{lll}
        {\tt callingClientId}  && unique client id of the calling client\\
        {\tt isPersistent}     && determines wheather it is a persistent or a transient tile\\
        {\tt rpcMarray}        && RPC representation of the tile\\
        \end{tabular}    

        Return values:
        \begin{tabular}{lll}
        0 && operation was successful\\
        1 && client context not found\\
        2 && base type name of inserting tile is not supported\\
        \end{tabular}    
      */



      // inserts a tile into a persistent MDD object splitting it up according to 
      // parameter tileSize
      virtual unsigned short insertTileSplitted( unsigned long callingClientId, 
					 int isPersistent,
					 RPCMarray* rpcMarray,
                                         r_Minterval* tileSize );
      /**
        Splits and inserts a tile into the current MDD object.
      
        Parameters
        \begin{tabular}{lll}
        {\tt callingClientId}  && unique client id of the calling client\\
        {\tt isPersistent}     && determines wheather it is a persistent or a transient tile\\
        {\tt rpcMarray}        && RPC representation of the tile\\
        {\tt tileSize}         && r_Minterval specifying the tile-size\\
        \end{tabular}    

        Return values:
        \begin{tabular}{lll}
        0 && operation was successful\\
        1 && client context not found\\
        2 && base type name of inserting tile is not supported\\
        \end{tabular}    
      */

      /// finnishes the MDD creation and inserts the MDD into the collection
      virtual unsigned short endInsertMDD( unsigned long callingClientId, 
					   int isPersistent );
      /**
        Parameters
        \begin{tabular}{lll}
        {\tt callingClientId}  && unique client id of the calling client\\
        {\tt isPersistent}     && determines wheather it is a persistent or a transient MDD\\
        \end{tabular}            
      */

      ///
      /// insert object into collection    
      virtual unsigned short insertMDD( unsigned long callingClientId,
					const char* collName,
					RPCMarray *rpcMarray, 
					const char* typeName, r_OId& oid );
      /**
        Inserts an object into an MDD collection. It is transfered in one piece.

        Parameters
        \begin{tabular}{lll}
        {\tt callingClientId}  && unique client id of the calling client\\
        {\tt collName}         && name of the collection to insert the MDD object\\
        {\tt rpcMarray}        && RPC representation of the MDD object\\
        {\tt typeName}         && type structure as string representation\\
        {\tt oid}              && object identifier\\
        \end{tabular}    

        Return values:
        \begin{tabular}{lll}
        0 && operation was successful\\
        1 && client context not found\\
        2 && MDD type name not found\\
        3 && types of MDD and collection are incompatible\\
        4 && MDD and its type are incompatible\\
        5 && collection does not exist\\
        6 && creation of persistent object failed\\
        \end{tabular}    
      */

      ///
      /// prepare an MDD collection for transfer with getNextMDD()
      virtual unsigned short getCollByName( unsigned long callingClientId,
					    const char*   collName,
					    char*         &typeName,
					    char*         &typeStructure,
					    r_OId         &oid             );
      /**
        ATTENTION: This function is not used at the moment. It hast
        to be adapted to transferData.

        Prepares an MDD collection for transfer with getNextMDD().

        Parameters
        \begin{tabular}{lll}
        {\tt callingClientId}  && unique client id of the calling client\\
        {\tt collName}         && name of the collection to be got\\
        {\tt typeName}         && returns name of the collection type\\
        {\tt typeStructure}    && returns structure of the collection type\\
        {\tt oid}              && returns oid of the collection\\
        \end{tabular}    

        The first parameter is the unique client id. The second parameter is the
        name of the collection to get. {\tt typeName} returns the name of the 
        collection type and {\tt typeStructure} its type structure.
      
        Return values:
        \begin{tabular}{lll}
        0 && operation was successful - collection has some elements\\
        1 && operation was successful - collection has no elements\\
        2 && collection is not known\\
        3 && client context not found\\
        \end{tabular}    
      */

      /// prepare an MDD collection for transfer with getNextMDD()
      virtual unsigned short getCollByOId( unsigned long callingClientId,
					   r_OId         &oid,
					   char*         &typeName,
					   char*         &typeStructure,
					   char*         &collName       );
      /**
        ATTENTION: This function is not used at the moment. It hast
        to be adapted to transferData.

        Prepares an MDD collection for transfer with \Ref{getNextMDD}.

        Parameters
        \begin{tabular}{lll}
        {\tt callingClientId}  && unique client id of the calling client\\
        {\tt oid}              && oid of the collection to be got\\
        {\tt typeName}         && returns name of the collection type\\
        {\tt typeStructure}    && returns structure of the collection type\\
        {\tt collName}         && returns name of collection\\
        \end{tabular}    

        Return values:
        \begin{tabular}{lll}
        0 && operation was successful - collection has some elements\\
        1 && operation was successful - collection has no elements\\
        2 && collection is not known\\
        3 && client context not found\\
        \end{tabular}    
      */

      /// gets oids of the collection specified by name
      virtual unsigned short getCollOIdsByName( unsigned long callingClientId,
						const char*   collName,
						char*         &typeName,
						char*         &typeStructure,
						r_OId         &oid,
						RPCOIdEntry*  &oidTable,
						unsigned int  &oidTableSize     );
      /**
        Gets the collection of oids of the collection with {\tt collName}.

        Parameters
        \begin{tabular}{lll}
        {\tt callingClientId}  && unique client id of the calling client\\
        {\tt collName}         && name of the collection to be got\\
        {\tt typeName}         && returns name of the collection type\\
        {\tt typeStructure}    && returns structure of the collection type\\
        {\tt oid}              && returns object identifier\\
        {\tt oidTable}         && returns an array of pointers to oids\\
        {\tt oidTableSize}     && returns the no of elements in the table\\
        \end{tabular}    

        Return values:
        \begin{tabular}{lll}
        0 && operation was successful - collection has some elements\\
        1 && operation was successful - collection has no elements\\
        2 && collection is not known\\
        3 && client context not found\\
        \end{tabular}    
      */

      /// gets oids of the collection specified by name
      virtual unsigned short getCollOIdsByOId( unsigned long callingClientId,
					       r_OId         &oid,
					       char*         &typeName,
					       char*         &typeStructure,
					       RPCOIdEntry*  &oidTable,
					       unsigned int  &oidTableSize,
					       char*         &collName       );
      /**
        Gets the collection of oids of the collection with {\tt collName}.

        Parameters
        \begin{tabular}{lll}
        {\tt callingClientId}  && unique client id of the calling client\\
        {\tt oid}              && oid of the collection to be got\\
        {\tt typeName}         && returns name of the collection type\\
        {\tt typeStructure}    && returns structure of the collection type\\
        {\tt oidTable}         && returns an array of pointers to oids\\
        {\tt oidTableSize}     && returns the no of elements in the table\\
        {\tt collName}         && returns name of collection\\
        \end{tabular}    

        Return values:
        \begin{tabular}{lll}
        0 && operation was successful - collection has some elements\\
        1 && operation was successful - collection has no elements\\
        2 && collection is not known\\
        3 && client context not found\\
        \end{tabular}    
      */

      ///   
      /// create new MDD collection
      virtual unsigned short insertColl( unsigned long callingClientId, 
					 const char* collName,
					 const char* typeName,
					 r_OId&      oid       );
      /**
        Creates a new MDD collection.

        Parameters
        \begin{tabular}{lll}
        {\tt callingClientId}  && unique client id of the calling client\\
        {\tt collName}         && name of the collection to be created\\
        {\tt typeName}         && name of the collection type\\
        {\tt oid}              && object identifier\\
        \end{tabular}    

        Return values:
        \begin{tabular}{lll}
        0 && operation was successful\\
        1 && client context not found\\
        2 && collection type name not found\\
        3 && collection name exists already in the db
        \end{tabular}    
      */
  
      ///    
      /// delete MDD collection
      virtual unsigned short deleteCollByName( unsigned long callingClientId, 
					       const char* collName );
      /**
        Deletes an MDD collection. The first parameter is the unique client id
        for which the collection should be deleted. The second parameter is the
        name for the collection to be deleted.
      
        Return values:
        \begin{tabular}{lll}
        0 && operation was successful\\
        1 && client context not found\\
        2 && collection with name does not exist\\
        \end{tabular}    
      */
    
      /// delete object by oid
      virtual unsigned short deleteObjByOId( unsigned long callingClientId, r_OId& oid );
      /**
        Deletes the object with {\tt oid}.
        The first parameter is the unique client id for which the object should be
        deleted.
      
        Return values:
        \begin{tabular}{lll}
        0 && operation was successful\\
        1 && client context not found\\
        2 && object with oid does not exist\\
        \end{tabular}    
      */
    
      ///    
      /// remove object specified by oid from collection specified by name
      virtual unsigned short removeObjFromColl( unsigned long callingClientId, 
						const char* collName, r_OId& oid );
      /**
        The method removes the object with {\\t oid} from collection with {\tt collName}.
        The first parameter is the unique client id for which the object should be removed.
      
        Return values:
        \begin{tabular}{lll}
        0 && operation was successful\\
        1 && client context not found\\
        2 && specified collection does not exist\\
        3 && specified object does not exist in the collection\\
        \end{tabular}    
      */

      ///    
      /// get new object identifier
      virtual unsigned short getNewOId( unsigned long callingClientId,
					unsigned short objType, r_OId& oid );
      /**
        Creates a new oid and gives it back by the refernce parameter {\tt oid}.
        {\tt objType} determines the type of object for which that oid is allocated. The folowing
        values are supported: 1 = MDD,  2 = Collection.
      
        Return values:
        \begin{tabular}{lll}
        0 && operation was successful\\
        1 && client context not found\\
        2 && error while creating oid\\
        \end{tabular}    
      */

      /// get type of object by oid
      virtual unsigned short getObjectType( unsigned long callingClientId, 
					    r_OId& oid, unsigned short &objType );
      /**
        Determines the type of the object indicated by {\tt oid}. The type is returned by the 
        reference parameter {\tt objType}. The folowing types are supported: 1 = MDD,  2 = Collection.
      
        Return values:
        \begin{tabular}{lll}
        0 && operation was successful\\
        1 && client context not found\\
        2 && oid not found\\
        \end{tabular}    
      */

      /// get type structure of a type name
      virtual unsigned short getTypeStructure( unsigned long  callingClientId, 
					       const char* typeName,
					       unsigned short typeType, 
					       char* &typeStructure);
      /**
        Determines the type structure of the type specified by {\tt typeName}. The type 
	either can be a set type (typeType=1), an mdd type (typeType=2), or a base type 
	(typeType = 3).
      
        Return values:
        \begin{tabular}{lll}
        0 && operation was successful\\
        1 && client context not found\\
        2 && type name not found\\
        \end{tabular}    
      */

      /// set the data format used for transferring data to the client
      virtual unsigned short setTransferMode( unsigned long callingClientId,
					      unsigned short format, const char* formatParams );
      /**
	Sets the data format used by the server to transfer data to the client to
	format which is of type r_Data_Format.

	Return values:
	\begin{tabular}{lll}
	0 && operation was successful\\
	1 && client context not found\\
	2 && unknown or unsupported data format\\
	\end{tabular}
      */

      /// set the data format for storing data into the database
      virtual unsigned short setStorageMode( unsigned long callingClientId,
					     unsigned short format, const char *formatParams );
      /**
	 return values exactly like setTransferMode()
      */
    ///
    //@}    
    
    /// returns a pointer to the context of the calling client, 0 it there is no context
    virtual ClientTblElt* getClientContext( unsigned long ClientId );
    /**
      Returns a pointer to the context of the calling client. This is done by
      searching the client table maintained by the server for the given client id.
      If there is no context corresponding to the client id, 0 is returned.

      Attention: After a client context was successfully received it has to be 
                 released using its member function release();
    */
   
    // get, set and clear extended error info.
    const char *getExtendedErrorInfo();

    void setExtendedErrorInfo(const char*);
  
    void clearExtendedErrorInfo(); 
    
    void clientEndRequest();
   
    // constant for clientID
    static const char* HTTPCLIENT;   

  private:
    /// copy constructor is private and therefore can not be used
    ServerComm( const ServerComm& );//and then why this? : clientTimeout(3600), garbageCollectionInterval(600){;};

  protected:
    /// make sure a tile has the correct data format, converting if necessary
    static int ensureTileFormat( r_Data_Format &hasFmt, r_Data_Format needFmt,
				 const r_Minterval &dom, const BaseType *type,
				 char *&data, unsigned long &size, int repack,
				 int owner, const char *params = NULL);
    ///returns the following:
    static const int ENSURE_TILE_FORMAT_OK;
    static const int ENSURE_TILE_FORMAT_BAD;
				   
    /// pointer to the actual administration interface object
    AdminIf* admin;
    
    char *errorText;

    unsigned long listenPort;
    char*         rasmgrHost;
    unsigned int  rasmgrPort;
    char*         serverName;
    
    bool          isHttpServer;
    //this trick didn't work and we are out of time
   // ClientTblElt  *uniqueClientContext;
};

    
/// indirect caller for rpcif_1
void rpcif_1_caller(struct svc_req *rqstp,SVCXPRT *transp);


/******************************************************************************************
***  This class shouldn't be here, later it will be put in its own file
******************************************************************************************/


class AccessControl
   {
     public:
       AccessControl();
       ~AccessControl();
       void initSyncro(const char *);
       void setServerName(const char *serverName);

       void resetForNewClient();
       int  crunchCapability(const char*); 
       /*   0 - ok
          804 - capability refused
       */
       
       void wantToRead(); // both throw
       void wantToWrite();
       bool isClient();
     private:
       int messageDigest(const char *input,char *output,const char *mdName);
       double initDeltaT;
       char serverName[100];
       
       bool okToRead;
       bool okToWrite;
       bool weHaveClient;
    };
extern AccessControl accessControl;       
   
#include "servercomm.icc"

#endif
