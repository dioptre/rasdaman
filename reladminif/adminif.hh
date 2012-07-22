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
#ifndef _ADMINIF_HH_
#define _ADMINIF_HH_

/************************************************************************
 *
= *
 * PURPOSE:
 *
 *
 * COMMENTS:
 *
 ***********************************************************************/

class AdminIf;
class DatabaseIf;

class r_Error;

#include <iostream>
#include "lists.h"

class AdminIf;

//@ManMemo: Module: {\bf reladminif}.

/*@Doc:
An AdminIf instance has to be obtained, before any work with the base
DBMS can be done. Only one instance can exist at any time. The class
follows the singleton design pattern (Gamma et. al. p. 127ff.).

{\bf Functionality}

At the moment, AdminIf is responsible for the state of a transaction.
persistent objects rely on this class to decide if they should write
changes back into the db. It also carries a list of compressed tiles.
Before a session is opened, no persistence capable classes can be used (e.g.
\Ref{BLOBTile})! Remember to also create a \Ref{DatabaseIf} instance
before using a persistence capable class.

{\bf Example}

{\tt AdminIf* myAdmin = AdminIf::instance();}

...

{\tt delete myAdmin;}

*/

const int SYSTEMNAME_MAXLEN=256;
/**
  * \defgroup Reladminifs Reladminif Classes
  */

/**
  * \ingroup Reladminifs
  */

class AdminIf
{
public:
    static AdminIf* instance();
    /*@Doc:
    satic function used to access instance of AdminIf and start session.
    */

    static DatabaseIf* getCurrentDatabaseIf();
    /*@Doc:
    static function used to access the current databaseif object
    */

    static void setCurrentDatabaseIf(DatabaseIf* db);
    /*@Doc:
    static function used to store the current databaseif object in AdminIf
    this function should only be called by DatabaseIf, DatabasIf
    is responsible for setting the databaseif object to
    NULL at destruction time.
    */

    ~AdminIf();
    /*@Doc:
    issues a ROLLBACK WORK RELEASE
    deinitializes the ObjectBroker
    deinitializes benchmark timers
    */

    static char* getSystemName();
    /*@Doc:
    returns Oracle on oracle
    */

    static void setReadOnlyTA(bool newReadOnlyTA);
    /*@Doc:
    sets readOnlyTA, should only be used by \Ref{TransactionIf}
    */

    static bool isReadOnlyTA();
    /*@Doc:
    checks for read only TA.
    */

    static bool isAborted();
    /*@Doc:
    used by DBObject::validate() to determine if it should execute the persistency functions
    */

    static void setAborted(bool newAborted);
    /*@Doc:
    used by transactionif to set the aborted status of the transaction
    */

protected:
    AdminIf() throw (r_Error);
    /*@Doc:
    constructor, can not be used from outside.
    initializes the objectbroker
    does a CONNECT,  when successful sets validConnection to true else false,
    then a ROLLBACK WORK RELEASE
    throws exception if connection fails
    */

private:
    static AdminIf* myInstance;
    /*@Doc:
    pointer to instance (just needed for Singleton pattern).
    */

    static DatabaseIf* myDatabaseIf;
    /*@Doc:
    pointer to the current DatabaseIf object
    */

    static bool validConnection;
    /*@Doc:
    flag for error when opening session: false if error.
    */

    static bool readOnlyTA;
    /*@Doc:
    flag for read only transactions
    */

    static const char dbmsName[SYSTEMNAME_MAXLEN];
    /*@Doc:
    holds the specific name of dbms
    */

    static char systemName[SYSTEMNAME_MAXLEN+1];
    /*@Doc:
    Store the dbms name using dbmsName
    */

    static bool _isAborted;
    /*@Doc:
    flag for aborted transactions
    */
};

#endif
