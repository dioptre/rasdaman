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
 *   The MDDBaseType class is used as a type for MDDs where
 *   the base type and the dimensionality is specified.
 *
 *
 * COMMENTS:
 *
 ************************************************************/

#ifndef _MDDDIMENSIONTYPE_HH_
#define _MDDDIMENSIONTYPE_HH_

#include <iostream>
#include "raslib/mddtypes.hh"
#include "catalogmgr/ops.hh"
#include "mddbasetype.hh"

class OId;

//@ManMemo: Module: {\bf relcatalogif}.

/*@Doc:
  The MDDBaseType class is used as a type for MDDs where
  the base type and the dimensionality is specified.
*/

/**
  * \ingroup Relcatalogifs
  */
class MDDDimensionType : public MDDBaseType
{
public:
    virtual char* getTypeStructure() const;
    /*@Doc:
    */

    virtual r_Bytes getMemorySize() const;
    /*@Doc:
    the memory size is computed by:
    MDDBaseType::getMemorySize() + sizeof(r_Dimension);
    */

    MDDDimensionType(const OId& id) throw (r_Error);
    /*@Doc:
    */

    MDDDimensionType(const char* newTypeName, const BaseType* newBaseType, r_Dimension newDimension);
    /*@Doc:
    constructor.
    */

    MDDDimensionType();
    /*@Doc:
    default constructor, cannot be used.
    */

    MDDDimensionType(const MDDDimensionType& old);
    /*@Doc:
    copy constructor.
    */

    MDDDimensionType& operator=(const MDDDimensionType& old);
    /*@Doc:
    assignment operator.
    */

    virtual void print_status( ostream& s ) const;
    /*@Doc:
    writes the state of the object to the specified stream
    */

    r_Dimension getDimension() const;
    /*@Doc:
    return dimensionality
    */

    virtual ~MDDDimensionType();
    /*@Doc:
    virtual destructor.
    */

    virtual int compatibleWith(const Type* aType) const;
    /*@Doc:
    is compatible if:
        aType is MDDDimType or MDDDomType and
        the basetypes are compatible
        and dimensionality is the same
    */

protected:

    virtual void insertInDb() throw (r_Error);

    virtual void readFromDb() throw (r_Error);

    virtual void deleteFromDb() throw (r_Error);

    r_Dimension myDimension;
    /*@Doc:
    dimensionality.
    */
};

#endif
