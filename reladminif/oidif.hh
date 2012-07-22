#ifndef _OIDIF_HH_
#define _OIDIF_HH_

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

//@ManMemo: Module: {\bf adminif}.

class OId;

#include <iostream>

#include "raslib/error.hh"

#ifdef RMANBENCHMARK
#include "raslib/rmdebug.hh"
#endif

//@ManMemo: Module: {\bf reladminif}.
/*@Doc:
the oid is a structure containing a counter and a type field.  based on the type
it is possible to determine the type of the object the oid is refering to.
the counter is used to pinpoint the exact instance of the object.

currently there are 19 different persistent classes.

the counter and type information is encoded into a double:
ID_MULTIPLIER * counter + type;
the counters for each oid type are stored in the database.  their exact value is read
when the transaction starts.   the values are updated in the database at the end of a
transaction.  this can be a problem with multiple concurrent open read/write
transactions.
*/

/**
  * \ingroup Reladminifs
  */
class OId
{
public:
    enum OIdType {  INVALID = 0,
                    MDDOID,
                    MDDCOLLOID,
                    MDDTYPEOID,
                    MDDBASETYPEOID,
                    MDDDIMTYPEOID,
                    MDDDOMTYPEOID,
                    STRUCTTYPEOID,
                    SETTYPEOID,
                    BLOBOID,
                    DBMINTERVALOID,
                    STORAGEOID,
                    MDDHIERIXOID,
                    DBTCINDEXOID,
                    INLINETILEOID,
                    INNEROID,
                    ATOMICTYPEOID,
                    UDFOID,
                    UDFPACKAGEOID,
                    MDDRCIXOID
                 };
    /*@Doc:
    every persistent class needs a unique OIdType.
    There is as always an exception: INNEROID is only used by DBTCIndex internally
    */

    typedef int OIdCounter;
    /*@Doc:
    every persistent object needs a unique OIdCounter within all persistent objects
    with the same OIdType.
    */

    typedef double OIdPrimitive;
    /*@Doc:
    an oid can be converted from and to a primitive of this type.
    */

    static OIdPrimitive ID_MULTIPLIER;
    /*@Doc:
    is used to calculate the actual id and type from a given double
    */

    static void allocateOId(OId& id, OIdType type, OIdCounter howMany = 1);
    /*@Doc:
    allocates a OId for an object of the specified type or a whole bunch of them.
    */

    static void deinitialize();
    /*@Doc:
    writes the current state of the oid counters back
    into the database.
    */

    static void initialize();
    /*@Doc:
    reads the state of the oid counters from the database.
    */

    OId::OIdType getType() const;
    /*@Doc:
    Returns type of the object with this OId.
    */

    OId(const OId& oldOId);
    /*@Doc:
    Copy constructor
    */

    OId(OIdCounter newId, OIdType type);
    /*@Doc:
    New OId with counter = newId, oidtype = type
    */

    OId(OIdPrimitive oidd);
    /*@Doc:
    generate a oid from a double.
    */

    OId();
    /*@Doc:
    invalid oid
    */

    OIdCounter getCounter() const;
    /*@Doc:
    returns the counter part of the oid.
    */

    void print_status(std::ostream& s = std::cout) const;
    /*@Doc:
    prints a double
    */

    operator double() const;
    /*@Doc:
    converts the oid to a double:
    oid * OId::ID_MULTIPLIER + oidtype;
    */

    static const char* counterNames[];
    /*@Doc:
    holds the names of the counters in RAS_ADMIN, to go with counterIds
    */

    static unsigned int maxCounter;

    OId& operator=(const OId& old);

    bool operator== (const OId& one) const;

    bool operator!= (const OId& one) const;

    bool operator< (const OId& old) const;

    bool operator> (const OId& old) const;

    bool operator<= (const OId& old) const;

    bool operator>= (const OId& old) const;

#ifdef RMANBENCHMARK
    static RMTimer oidAlloc;
    static RMTimer oidResolve;
#endif
protected:
    // protection agains writing back unloaded counters => inconsistent DB!!
    static bool loadedOk;


    OIdCounter oid;
    /*@Doc:
    the counter inside the oid
    */

    OIdType oidtype;
    /*@Doc:
    the type of object
    */

    static OIdCounter nextMDDOID;
    /*@Doc:
    counter which holds the next oid
    */

    static OIdCounter nextMDDCOLLOID;
    /*@Doc:
    counter which holds the next oid
    */

    static OIdCounter nextMDDTYPEOID;
    /*@Doc:
    counter which holds the next oid
    */

    static OIdCounter nextMDDBASETYPEOID;
    /*@Doc:
    counter which holds the next oid
    */

    static OIdCounter nextMDDDIMTYPEOID;
    /*@Doc:
    counter which holds the next oid
    */

    static OIdCounter nextMDDDOMTYPEOID;
    /*@Doc:
    counter which holds the next oid
    */

    static OIdCounter nextSTRUCTTYPEOID;
    /*@Doc:
    counter which holds the next oid
    */

    static OIdCounter nextSETTYPEOID;
    /*@Doc:
    counter which holds the next oid
    */

    static OIdCounter nextBLOBOID;
    /*@Doc:
    counter which holds the next oid
    */

    static OIdCounter nextDBMINTERVALOID;
    /*@Doc:
    counter which holds the next oid
    */

    static OIdCounter nextSTORAGEOID;
    /*@Doc:
    counter which holds the next oid
    */

    static OIdCounter nextMDDHIERIXOID;
    /*@Doc:
    counter which holds the next oid
    */

    //static OIdCounter nextDBTCINDEXOID;
    /*@Doc:
    this counter is not used because mddhierix takes care of that
    */

    //static OIdCounter nextINLINETILEOID;
    /*@Doc:
    not used because they are the same as bloboid counter
    */

    static OIdCounter nextATOMICTYPEOID;
    /*@Doc:
    not used now because they are hard coded
    */

    static OIdCounter nextMDDRCIXOID;
    /*@Doc:
    counter which holds the next oid
    */

    static OIdCounter nextUDFOID;
    /*@Doc:
    counter which holds the next oid
    */

    static OIdCounter nextUDFPACKAGEOID;
    /*@Doc:
    counter which holds the next oid
    */

    static OIdCounter* counterIds[];
    /*@Doc:
    holds all OIdCounters of next* sort, to go with the counterNames.
    */
};

extern std::ostream& operator<<(std::ostream& in, const OId& d);

extern std::ostream& operator<<(std::ostream& in, OId::OIdType d);

extern bool operator== (const OId::OIdPrimitive one, const OId& two);

extern bool operator== (const OId& two, const OId::OIdPrimitive one);

#endif
