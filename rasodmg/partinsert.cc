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
 * INCLUDE: partinsert.cc
 *
 * MODULE:  rasodmg
 * CLASS:   r_Partial_Insert
 *
 * COMMENTS:
 *      None
*/

#include <string.h>
#include <stdio.h>
#include <vector>

#include "raslib/rminit.hh"
#include "raslib/rmdebug.hh"
#include "raslib/minterval.hh"
#include "raslib/error.hh"
#include "rasodmg/storagelayout.hh"
#include "rasodmg/alignedtiling.hh"
#include "rasodmg/gmarray.hh"
#include "rasodmg/database.hh"
#include "rasodmg/oqlquery.hh"
#include "rasodmg/partinsert.hh"



// format string for creating collections; parameters: collection name, set type
const char *r_Partial_Insert::format_create = "CREATE COLLECTION %s %s";
// format string for updating the MDD; parameters: collection name, local oid
const char *r_Partial_Insert::format_update = "UPDATE %s AS x SET x ASSIGN $1 WHERE OID(x) = %f";


r_Partial_Insert::r_Partial_Insert( r_Database &usedb, const char *collname, const char *mddtype, const char *settype, const r_Storage_Layout &stl ) : mydb(usedb)
{
    init_share(collname, mddtype, settype);
    mystl = stl.clone();
}


r_Partial_Insert::r_Partial_Insert( r_Database &usedb, const char *collname, const char *mddtype, const char *settype, const r_Minterval &dom, unsigned int tsize ) : mydb(usedb)
{
    init_share(collname, mddtype, settype);
    r_Aligned_Tiling *tilingObj = new r_Aligned_Tiling(dom, tsize * dom.cell_count());
    mystl = new r_Storage_Layout(tilingObj);
}


r_Partial_Insert::r_Partial_Insert( const r_Partial_Insert &src ) : mydb(src.mydb)
{
    init_share(src.collName, src.mddType, src.setType);
    mystl = src.mystl->clone();
}


r_Partial_Insert::~r_Partial_Insert( void )
{
    if (collName != NULL)
        delete [] collName;
    if (mddType != NULL)
        delete [] mddType;
    if (setType != NULL)
        delete [] setType;
    if (mystl != NULL)
        delete mystl;
}


void r_Partial_Insert::init_share( const char *collname, const char *mddtype, const char *settype )
{
    collName = new char[strlen(collname)+1];
    strcpy(collName, collname);
    mddType = new char[strlen(mddtype)+1];
    strcpy(mddType, mddtype);
    setType = new char[strlen(settype)+1];
    strcpy(setType, settype);
    doUpdate = 0;
}

int r_Partial_Insert::update( r_GMarray *mddPtr,
                              r_Data_Format transferFormat,
                              const char* transferFormatParams,
                              r_Data_Format storageFormat,
                              const char* storageFormatParams
                            )
{
    RMDBGENTER(0, RMDebug::module_rasodmg, "r_Partial_Insert", "update()");
    try
    {
        mddPtr->set_storage_layout(mystl->clone());
        mddPtr->set_type_by_name(mddType);
    }
    catch (r_Error &err)
    {
        RMInit::logOut << "r_Partial_Insert::update(): unable to set storage_layout for the currend MDD: "
                       << err.what() << endl;
        return -1;
    }

    if (doUpdate == 0)
    {
        char *queryBuffer = new char[strlen(format_create) + strlen(collName) + strlen(setType)];
        sprintf(queryBuffer, format_create, collName, setType);
        // first try creating the collection
        try
        {
            // this is basically 3 bytes to long, but that doesn't matter
            myta.begin();
            mydb.set_transfer_format(transferFormat, transferFormatParams);
            mydb.set_storage_format(storageFormat, storageFormatParams);
            r_OQL_Query query(queryBuffer);
            r_oql_execute(query);
            myta.commit();
            RMDBGMIDDLE(0, RMDebug::module_rasodmg, "r_Partial_Insert", "update(): created new collection " << collName << " with type " << setType );
        }
        catch (r_Error &err)
        {
            RMDBGMIDDLE(0, RMDebug::module_rasodmg, "r_Partial_Insert", "update(): can't create collection: " << err.what() );
            myta.abort();
            // failure to create the collection is not an error
        }
        delete [] queryBuffer;

        // now create the root object
        try
        {
            myta.begin();
            mydb.set_transfer_format(transferFormat, transferFormatParams);
            mydb.set_storage_format(storageFormat, storageFormatParams);
            r_Ref<r_GMarray> mddp = new (&mydb, mddType) r_GMarray(*mddPtr);
            r_Ref<r_Set<r_Ref<r_GMarray> > > mddCollPtr;
            mddCollPtr = (r_Ref<r_Set<r_Ref<r_GMarray> > >)(mydb.lookup_object(collName));
            mddCollPtr->insert_element(mddp);
            myOId = mddp->get_oid();
            myta.commit();
            RMDBGMIDDLE(0, RMDebug::module_rasodmg, "r_Partial_Insert", "update(): reated root object OK, oid = " << myOId );
            doUpdate = 1;
        }
        catch (r_Error &err)
        {
            RMInit::logOut << "r_Partial_Insert::update(): unable to create root object: "
                           << err.what() << endl;
            myta.abort();
            return -1;
        }
    }
    else
    {
        char *queryBuffer = new char[strlen(format_update) + strlen(collName) + 32];
        sprintf(queryBuffer, format_update, collName, myOId.get_local_oid());

        // try the update
        try
        {
            myta.begin();
            mydb.set_transfer_format(transferFormat, transferFormatParams);
            mydb.set_storage_format(storageFormat, storageFormatParams);
            RMDBGMIDDLE(1, RMDebug::module_rasodmg, "r_Partial_Insert", "update(): QUERY: " << queryBuffer );
            r_OQL_Query query(queryBuffer);
            query << (*mddPtr);
            r_oql_execute(query);
            myta.commit();
            RMDBGMIDDLE(0, RMDebug::module_rasodmg, "r_Partial_Insert", "update(): update object OK" );
        }
        catch (r_Error &err)
        {
            RMInit::logOut << "r_Partial_Insert::update(): failed to update marray: "
                           << err.what() << endl;
            myta.abort();
            delete [] queryBuffer;
            RMDBGEXIT(1, RMDebug::module_rasodmg, "r_Partial_Insert", "failed");
            return -1;
        }
        delete [] queryBuffer;
    }
    RMDBGEXIT(1, RMDebug::module_rasodmg, "r_Partial_Insert", "update(): exiting OK" );

    return 0;
}
