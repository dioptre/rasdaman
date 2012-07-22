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
#ifndef _DBMINTERVAL_HH_
#define _DBMINTERVAL_HH_

class DBMinterval;

template<class T> class DBRef;
typedef DBRef<DBMinterval> DBMintervalId;

#include "reladminif/dbobject.hh"
#include "raslib/minterval.hh"


//@ManMemo: Module: {\bf relcatalogif}.

/*@Doc:
Persistent version of r_Minterval.  it stores its attributes not very efficient.
it is used by DBMDDObj and MDDDomainType because the performance impact is neglectabel.

for a more efficient storage system refere to InlineMInterval
*/

/**
  * \ingroup Relcatalogifs
  */
class DBMinterval : public DBObject, public r_Minterval
{
public:
    DBMinterval();

    DBMinterval(const OId& id) throw (r_Error);

    DBMinterval(r_Dimension dim);

    DBMinterval(const char* dom);

    DBMinterval(const r_Minterval& old);

    DBMinterval(const DBMinterval& old);

    ~DBMinterval();
    /*@Doc:
    validates the object in the database.
    */

    virtual DBMinterval& operator=(const DBMinterval& old);
    /*@Doc:
    replaces only the r_Minterval part of the object
    */

    virtual DBMinterval& operator=(const r_Minterval& old);
    /*@Doc:
    replaces only the r_Minterval part of the object
    */

    virtual r_Bytes getMemorySize() const;
    /*@Doc:
    esimates the space taken up by this object with:
    DBObject::getMemorySize() + sizeof(r_Minterval)
        + dimensionality * (4 + 4 + 1 + 1)
    */

protected:

    virtual void insertInDb() throw (r_Error);
    /*@Doc:
    inserts the object into the database.  it uses one table
    for the fixed length attributes (oid, size, dimension) and
    another for dynamic data (lower/upper bounds/fixed ranges)
    */

    virtual void updateInDb() throw (r_Error);
    /*@Doc:
    */

    virtual void deleteFromDb() throw (r_Error);
    /*@Doc:
    */

    virtual void readFromDb() throw (r_Error);
    /*@Doc:
    */
};

#endif
