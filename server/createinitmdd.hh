#ifndef CREATE_INIT_MDD_HH
#define CREATE_INIT_MDD_HH
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
 * COMMENTS:
 *
 * These are tools for fast, low level acces to MDD and Colelctions
 * The main purpose is speeding up initialization and import operations
 *
 ************************************************************/


#include "mddmgr/mddcoll.hh"
#include "mddmgr/mddobj.hh"
#include "mddmgr/mddcolliter.hh"
#include "tilemgr/tile.hh"
#include "catalogmgr/typefactory.hh"
#include "relcatalogif/mddtype.hh"
#include "relcatalogif/mdddomaintype.hh"
#include "relcatalogif/settype.hh"
#include "reladminif/eoid.hh"
#include "raslib/basetype.hh"
#include "raslib/oid.hh"
#include "servercomm/servercomm.hh"

/**
  * \defgroup Servers Server Classes
  */

/**
  * \ingroup Servers
  */
class FastCollectionCreator
{
public:
    FastCollectionCreator(const char *collName, const char* collTypeName);

    r_OId createCollection();

private:
    // allow only [A-Z,a-z,_]
    void verifyName( const char* name ) throw(r_Error);

    const char *collectionName;
    const char *collectionTypeName;

};


/**
  * \ingroup Servers
  */
class FastMDDCreator
{
public:
    FastMDDCreator();
    ~FastMDDCreator();

    void  setCollectionName(const char *collName);
    void  setMDDTypeName(const char* mddTypeName);

    r_OId createMDD(const char *domain);
    r_OId createRCxMDD(const char *domain, const char *tileDomain);

    void addStripe(r_OId mddOId, const char *stripeDomain, const char *tileDomain);

    vector<r_Minterval> getTileDomains(r_OId mddOId, const char *stripeDomain);
private:
    void verifyCompatibility(MDDColl *collection) throw(r_Error);

    void createCompressedTileData(r_Minterval&, const BaseType* baseType);

    std::string collectionName;
    std::string mddTypeName;

    r_Minterval definitionInterval;

    r_Data_Format storageFormat;
    const char *formatParams;

    r_OId mddOId;
    int   cellSize;

    char *comprData;
    int   comprDataSize;

    MDDObj      *mymdd;
};

#endif
