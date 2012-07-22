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
 * INCLUDE: indexds.hh
 *
 * MODULE:  indexmgr
 * CLASS:   IndexDS
 *
 * COMMENTS:
 *
*/

#ifndef _INDEXDS_HH_
#define _INDEXDS_HH_

#include "reladminif/dbobject.hh"
#include "reladminif/lists.h"
#include "reladminif/oidif.hh"

/**
 * @defgroup indexmgr indexmgr
 *
 * @file indexds.hh
 *
 * @ingroup indexmgr

 This is an interface class.  It is abstract.  It supplies the signature that
 is required to allow SDirIndexLogic to manage this object.
*/

class IndexDS   :   public DBObject
{
public:
    IndexDS():DBObject() {}

    IndexDS(const OId& id):DBObject(id) {}

    virtual r_Minterval getCoveredDomain() const = 0;
    /*@Doc:
        returns the domain which is covered by the entries of this index.
        when some one has time:
            this should not be persistent
            this is not the same as assigned domain
            this should be updated automatically when removing and adding entries, and is
    */

    virtual r_Minterval getAssignedDomain() const = 0;
    /*@Doc:
        returns the domain this index is responsible for (assigned domain).
        when some one has time:
            this is persistent and should be
            this should not be the same as covered domain, but is
            this should not be updated when removing entries, but is because of above
    */

    virtual r_Minterval getObjectDomain(unsigned int pos) const = 0;
    /*@Doc:
        Returns the (assigned) domain for the object in position {\ttpos}.
        The object domain is always a assigned domain, because we do not read the domain from the entries (blobtiles do not store one).
        At some point this of course makes storing the assigned domain obsolete (because it is stored at the parent, in case of the root it is the covered domain which can be computed from the entries)
    */

    virtual r_Dimension getDimension() const = 0;
    /*@Doc:
        Returns the dimensionality of this index.
    */

    virtual unsigned int getSize() const = 0;
    /*@Doc:
        Only the number of elements in this index node is returned.
    */

    virtual bool isValid() const = 0;
    /*@Doc:
        true when domains of entries of nodes are completely covered
        by the assigned domains of the nodes and assigned domains of
        leafs intersect with each of their entries domains.  The assigned domains may not intersect.
        false otherwise.
    */

    virtual bool isUnderFull() const = 0;
    /*@Doc:
        Returns true when the index does not have the minimal number of entries (it is a candidate for merging).
        Otherwise false.
    */

    virtual bool isOverFull() const = 0;
    /*@Doc:
        Same as above but vice versa.
    */

    virtual bool isSameAs(const IndexDS* pix) const = 0;
    /*@Doc:
        Compares two index data structures:
            When both are persistent, their oids are compared.
            When both are transient there memory address is compared.
            Else they are not equal.
    */

    virtual bool removeObject(unsigned int pos) = 0;
    /*@Doc:
        Removes an object from this index. It returns true if the object was successfully removed.
    */

    virtual bool removeObject(const KeyObject& theKey) = 0;
    /*@Doc:
        Removes an object from this index. It returns true if the object was successfully removed.
    */

    virtual void insertObject(const KeyObject& theKey, unsigned int pos) = 0;
    /*@Doc:
        Inserts the object in theKey at the sppecified position.
        Position 0 means at the front.
    */

    virtual void setAssignedDomain(const r_Minterval& domain) = 0;
    /*@Doc:
        Sets the assigned domain.
    */

    virtual void setObject(const KeyObject& theKey, unsigned int pos) = 0;
    /*@Doc:
        Sets the entry and domain at position pos.
    */

    virtual void setObjectDomain(const r_Minterval& dom, unsigned int pos) = 0;
    /*@Doc:
        Sets the assigned domain of the object at position pos to dom.
    */

    virtual const KeyObject& getObject(unsigned int pos) const = 0;
    /*@Doc:
        The requested object will be placed in result.
    */

    virtual void getObjects(KeyObjectVector& objs) const = 0;
    /*@Doc:
        Push all entries into the vector
    */

    virtual unsigned int getOptimalSize() const = 0;
    /*@Doc:
        Computes the optimal number of entries for this index.
        Should be moved to a place where StorageLayout is accessible.
    */

    virtual void freeDS() = 0;
    /*@Doc:
        Marks the persistent database index data structure for
        deletion all entries contained in it will be deleted
        from the database along with it.
    */

    virtual OId::OIdPrimitive getIdentifier() const = 0;

    virtual IndexDS* getNewInstance() const = 0;
};

#endif
