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
 * INCLUDE: clientcomm.hh
 *
 * MODULE:  clientcomm
 * CLASS:   ClientComm
 *
 * COMMENTS:
 *      ClientComm is now an abstract class, with two subclasses: RpcClintComm and RnpClientComm
 *      This will change again when RPC will be dropped forever
 *
*/

#ifndef _CLIENTCOMM_
#define _CLIENTCOMM_

#include "raslib/rminit.hh"
#include "raslib/error.hh"
#include "rasodmg/oqlquery.hh"
#include "rasodmg/marray.hh"

#include "raslib/primitivetype.hh"


template <class T> class r_Set;
class r_Ref_Any;
class r_Base_Type;
class r_Parse_Params;


/**
*   @defgroup ClientComm ClientComm
*
*   @file clientcomm.hh
*
*   @ingroup ClientComm
*/
//  @Doc

/*  The class ClientComm represents {\bf one} connection between
    {\bf one} client entity (e.g. an object of class \Ref{r_Database}) and
    the server. Therefore, the host name has to be provided at
    the constructor.
*/

class ClientComm
{
public:

    /// destructor (closes the connection and releases resources)
    virtual ~ClientComm() throw();

    // the class is not necessary singleton, but the type of the actual object depends on the environment
    static ClientComm* createObject(const char* rasmgrName, int rasmgrPort);

    /** Methods for setting the protocol. Default will be RNP, but until release 5.2 it is RPC,
        unless you compile with -DDEFAULTRNP
        Also, if the environment variable RMANPROTOCOL is set, it's value is used to set the protocol
    and the 'in-code' setting is ignored
    RMANPROTOCOL can be:
        - RNP       -> RNP is used
        - COMPAT    -> raslib uses RPC, rasj uses HTTP
        - RPC       -> raslib uses RPC, rasj uses RNP
        - HTTP      -> raslib uses RNP, rasj uses HTTP
        every other value is ignored and the 'in-code' setting is used

    NOTE: This methods will be removed in the next version
      */

    static  void useRNP() throw();
    static  void useRPC() throw();
    static  bool internalSettingIsRNP() throw();

    virtual bool effectivTypeIsRNP() throw() = 0;

    //@Man: Database methods
    //@{
    ///
    //RNP: all made pure

    /// open database
    virtual int openDB( const char* database ) = 0;
    /// close current database
    virtual int closeDB() = 0;
    /// create a database
    virtual int createDB( const char* name ) throw(r_Error) = 0;
    /// destroy a database
    virtual int destroyDB( const char* name ) throw(r_Error) = 0;

    ///
    //@}

    //@Man: Transaction methods
    //@{
    ///

    /// begin transaction
    virtual int openTA( unsigned short readOnly = 0 ) throw(r_Error) =0;
    /// commit current transaction
    virtual int commitTA() throw(r_Error) =0;
    /// abort current transaction
    virtual int abortTA() =0;

    ///
    //@}

    //@Man: MDD methods
    //@{
    ///

    /// inserts a MDD object in an existing MDD collection on the server
    virtual void insertMDD( const char* collName, r_GMarray* mar ) throw( r_Error ) =0;
    /// gets MDD object by oid
    virtual r_Ref_Any getMDDByOId( const r_OId& oid ) throw( r_Error ) =0;

    ///
    //@}

    //@Man: Collection methods
    //@{
    ///

    /// creates an empty MDD collection on the server
    virtual void insertColl( const char* collName, const char* typeName, const r_OId& oid ) throw( r_Error ) =0;
    /// deletes an MDD collection by name
    virtual void deleteCollByName( const char* collName ) throw( r_Error ) =0;
    /// deletes an object by oid (right now, objects are collection only)
    virtual void deleteObjByOId( const r_OId& oid ) throw( r_Error ) =0;
    /// removes an object from a collection
    virtual void removeObjFromColl( const char* name, const r_OId& oid ) throw ( r_Error ) =0;
    /// gets collection by name
    virtual r_Ref_Any getCollByName( const char* name ) throw( r_Error ) =0;
    /// gets collection by oid
    virtual r_Ref_Any getCollByOId ( const r_OId& oid ) throw( r_Error ) =0;
    /// gets collection references by name
    virtual r_Ref_Any getCollOIdsByName( const char* name ) throw( r_Error ) =0;
    /// gets collection references by oid
    virtual r_Ref_Any getCollOIdsByOId ( const r_OId& oid ) throw( r_Error ) =0;

    ///
    //@}

    //@Man: Query methods
    //@{
    ///

    /// query execution
    virtual void executeQuery( const r_OQL_Query& query, r_Set< r_Ref_Any >& result ) throw( r_Error ) =0;
    /*@Doc:
      Executes a retrieval query of type \Ref{r_OQL_Query} and returns the result. Every
      MDD object of the MDD collection is fetched from the server and inserted
      in the resulting \Ref{r_Set}.
    */

    /// update execution
    virtual void executeQuery( const r_OQL_Query& query ) throw( r_Error ) =0;
    /*@Doc:
      Executes an update query of type \Ref{r_OQL_Query}.
    */

    ///
    //@}


    //@Man: System methods
    //@{
    ///

    /// get new oid
    virtual r_OId getNewOId( unsigned short objType ) throw(r_Error) = 0;

    /// get oid type
    virtual unsigned short getObjectType( const r_OId& oid ) throw(r_Error) = 0;

    enum r_Type_Type
    {
        r_SetType_Type = 1,
        r_MDDType_Type = 2
    };

    /// get type structure
    /// deallocate using delete []
    virtual char* getTypeStructure( const char* typeName, r_Type_Type typeType ) throw(r_Error) =0;

    ///
    //@}

    /// changes endianness of MDD data
    static int changeEndianness( r_GMarray* mdd, const r_Base_Type* bt=NULL );
    /// changes the endianness of MDD data and keeps the original untouched
    static int changeEndianness( const r_GMarray* mdd, void *newData, const r_Base_Type* bt=NULL );

    /// provides read access to my clientID
    virtual unsigned long getClientID() const  = 0;


    //@Man: Methods for asynchronious alive signal concept
    //@{

    /// triggers an alive signal
    virtual void triggerAliveSignal() = 0;
    /**
      First, it sets the switch {\tt aliveSignalRemaining} saying that an alive signal
      should be send to the server. Then it calls {\tt sendAliveSignal()} to send it
      immediately if possible.
    */

    /// send an alive signal if necessary and possible
    virtual void sendAliveSignal() = 0;
    /**
      Sends an alive signal to the server if the switch {\tt aliveSignalRemaining} is
      set and no other RPC is active. If a signal can be sent, {\tt aliveSignalRemaining}
      is set to 0 again.
    */

    /// set the preferred transfer format
    virtual int setTransferFormat( r_Data_Format format, const char* formatParams=NULL ) =0;

    /// set the preferred storage format
    virtual int setStorageFormat( r_Data_Format format, const char *formatParams=NULL ) =0;

    /// get extended error information
    virtual const char *getExtendedErrorInfo() throw(r_Error)  =0;

    /// get real server name (the dinamic one, assigned by the RasMGR)
    const char* getServerName();

    /// user identification for RasMGR
    virtual void setUserIdentification(const char *userName, const char *plainTextPassword) =0;

    /// set maximum retry to get a server
    virtual void setMaxRetry(unsigned int newMaxRetry) = 0;

    /// get maximum retry to get a server
    virtual unsigned int  getMaxRetry() = 0;

    /// set and get communication timeout interval. Only RNP really uses it
    virtual void setTimeoutInterval(int seconds) = 0;
    virtual int  getTimeoutInterval() = 0;

    ///
    //@}

protected:
    /// constructor getting nothing
    ClientComm() throw( r_Error );

private:

    static bool currentProtocolIsRNP;
};

#endif
