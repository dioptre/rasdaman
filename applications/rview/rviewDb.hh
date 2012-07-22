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
 *  Purpose:
 *
 *  Database abstraction layer. All database accesses are performed though
 *  the functions provided here. This includes creating / deleting / looking
 *  up collections, inserting objects into collections and executing RasQL
 *  queries.
 *
 *  COMMENTS:
 *          none
 */

/**
*   @file rviewDb.hh
*
*   @ingroup Applications
*/


#ifndef _RVIEW_DB_H_
#define _RVIEW_DB_H_



#ifdef __GNUG__
#pragma interface
#endif




#include "rasodmg/database.hh"
#include "rasodmg/transaction.hh"
#include "rasodmg/set.hh"
#include "rasodmg/ref.hh"
#include "rasodmg/marray.hh"
#include "rasodmg/gmarray.hh"
#include "rasodmg/iterator.hh"
#include "rasodmg/oqlquery.hh"


#include "rviewUtils.hh"



class r_Fast_Base_Scale;

class rviewDatabase
{
public:

    rviewDatabase(void);
    ~rviewDatabase(void);
    int  open(const char *srvname, int srvport, const char *dbname,
              const char *username, const char *userpassword);
    void close(void);
    bool isOpen(void);
    int  createCollection(const char *collName, rviewBaseType bt);
    int  deleteCollection(const char *collName);
    int  lookupCollection(collection_desc *desc);
    static r_Ref<r_GMarray> getScaledObject(r_Fast_Base_Scale *scaler, const r_Minterval &trimDom, double scale);
    r_Fast_Base_Scale *lookupScaledObject(collection_desc *desc, double scale);
    int  insertObject(const char *collName, r_Ref<r_GMarray> mddObj, r_Minterval *domain=NULL);
    int  executeQuery(collection_desc *desc, const char *query, r_Ref<r_GMarray> *updateMdd=NULL, bool showProgress=TRUE);
    int  getMinterval(r_Minterval &dom, const char *collName, const double *loid=NULL);
    const r_Database *getDatabase(void) const;
    int getErrorInfo(int &line, int &col) const;


protected:

    int collectionToDesc(r_Set<r_Ref<r_GMarray> > &mddColl, collection_desc *desc);

    int ensureDatabase(void);

    DynamicString server;
    int port;
    DynamicString database;
    DynamicString username;
    DynamicString userpassword;
    DynamicString lastTransferParams;
    DynamicString lastStorageParams;
    r_Data_Format lastTransferFormat;
    r_Data_Format lastStorageFormat;

    bool dbOpen;
    r_Database dbase;
    // For errors in queries
    int line, col;
};

#endif
