#ifndef _TRANSACTIONIF_HH_
#define _TRANSACTIONIF_HH_

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
/************************************************************************
 *
 *
 * PURPOSE:
 *
 *
 * COMMENTS:
 *
 ***********************************************************************/

//@ManMemo: Module: {\bf transactionif}.

/*@Doc:
TransactionIf has the functionality to begin, commit and abort a base
DBMS transaction.
*/

class TransactionIf;
class DatabaseIf;

#include <iostream>

#include "raslib/error.hh"
/**
  * \ingroup Reladminifs
  */
class TransactionIf
{
public:
    void begin(DatabaseIf* currBase, bool readOnly = false) throw ( r_Error );
    /*@Doc:
    begin a TA.
    By default, the TA started is a write TA. If {\tt readOnly} is set to 1,
    the TA will be read only.
    */

    void commit() throw ( r_Error );
    /*@Doc:
    commit a transaction.  make all changes permanent.
    r_Error is thrown when any objects have problems
    writting themselves to the database.
    AdminIf::compCompTiles(), TypeFactory::freeTempTypes,
    ObjectBroker::clearBroker, OId::deinitialize are called.
    COMMIT is issued.  the database is closed.
    the benchmark timers are stopped.
    */

    void abort();
    /*@Doc:
    abort a TA.  don't make changes permanent.
    AdminIf::compCompTiles(), TypeFactory::freeTempTypes,
    ObjectBroker::clearBroker, OId::deinitialize are called.
    ROLLBACK is issued.  the database is closed.
    the benchmark timers are stopped.
    */

    DatabaseIf* getDatabaseIf();
    /*@Doc:
    returns the DatabaseIf object associated with this transaction.
    */

private:
    void begin( bool readOnly = false ) throw ( r_Error );
    /*@Doc:
    begin a TA. Internal usage only.
    This function does not work if the appropriate initializations have'nt
    been done. For that reason, it is only appropriate for internal usage.
    By default, the TA started is a write TA. If {\tt readOnly} is set to 1,
    the TA will be read only.
    benchmark timers are started.
    */

    bool isReadOnly;
    /*@Doc:
    keeps track if the current TA is read only or not
    */

    static DatabaseIf* lastBase;
    /*@Doc:
    stores the last base used in begin();
    */
};

#endif
