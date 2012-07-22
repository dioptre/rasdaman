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
// -*-C++-*- (for Emacs)

/*************************************************************
 *
 *
 * PURPOSE:
 *   The CollectionType class is the superclass for the class
 *   SetType.
 *
 *
 * COMMENTS:
 *
 ************************************************************/

#ifndef _COLLECTIONTYPE_HH_
#define _COLLECTIONTYPE_HH_

class CollectionType;

#include <iostream>
#include "catalogmgr/ops.hh"
#include "type.hh"
#include "reladminif/externs.h"

class MDDType;

//@ManMemo: Module: {\bf relcatalogif}.

/*@Doc:
  CollectionType is the base class for classes that deal with
  collections of MDDs (the only subclass at the moment is SetType).
*/

/**
  * \ingroup Relcatalogifs
  */
class CollectionType : public Type
{
public:
    const MDDType* getMDDType() const;
    /*@Doc:
    returns MDDType of collection.
    */
    /*
        MDDType* getMDDType() const;
    */


    void print_status( ostream& s ) const;
    /*@Doc:
    writes the state of the object to the specified stream:
    d_MYNAME <MYMDDTYPE->printStatus >
    */

    CollectionType(const MDDType* newMDDType);
    /*@Doc:
    constructor receiving pointer to an MDDType (or subclass).
    */

    CollectionType();
    /*@Doc:
    default constructor, cannot be used.
    */

    CollectionType(const OId& id) throw (r_Error);
    /*@Doc:
    */

    CollectionType(const CollectionType& old);
    /*@Doc:
    */

    CollectionType& operator=(const CollectionType& old);
    /*@Doc:
    */

    virtual ~CollectionType();
    /*@Doc:
    virtual destructor.
    */

    virtual int compatibleWith(const Type* aType) const;
    /*@Doc:
    overloaded from Type.
    returns true if myMDDType is compatible with the type.
    */

    virtual r_Bytes getMemorySize() const;
    /*@Doc:
    returns DBNamedObject::getMemorySize() +
        myMDDType->getMemorySize() + sizeof(MDDType*);
    */

protected:
    const MDDType* myMDDType;
    /*@Doc:
    persistent pointer to MDDType of collection.
    */

    CollectionType(const char* name);
    /*@Doc:
    */

    CollectionType(const char* name,const MDDType* newMDDType);
    /*@Doc:
    */
};

#endif

