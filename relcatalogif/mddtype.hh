// -*-C++-*- (for Emacs)

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
 * PURPOSE:
 *   The MDDType class is used as a type for MDDs where nothing
 *   is specified. It also is a superclass for types for MDDs
 *   specifying more.
 *
 *
 * COMMENTS:
 *
 ************************************************************/

#ifndef _MDDTYPE_HH_
#define _MDDTYPE_HH_

#include "catalogmgr/ops.hh"
#include "type.hh"
#include "raslib/minterval.hh"

class OId;

//@ManMemo: Module: {\bf relcatalogif}.

/*@Doc:
  The MDDType class is used as a type for MDDs where nothing
  is specified. It also is a superclass for types for MDDs
  specifying more.
*/

/**
  * \ingroup Relcatalogifs
  */
class MDDType : public Type
{
public:
    enum MDDTypeEnum { MDDONLYTYPE, MDDBASETYPE, MDDDOMAINTYPE, MDDDIMENSIONTYPE };
    /*@Doc:
    enum used for runtime typing.
    could be superceded by OId::OIdType
    */

    virtual char* getTypeStructure() const;
    /*@Doc:
    returns type as string:
        marray <>
    */

    MDDType(const OId& id) throw (r_Error);

    MDDType();
    /*@Doc:
    constructor.
    */

    MDDType(const char* newTypeName);
    /*@Doc:
    constructor using type name.
    */

    MDDType(const MDDType& old);
    /*@Doc:
    copy constructor.
    */

    MDDType& operator=(const MDDType& old);
    /*@Doc:
    assignment operator.
    */

    virtual void print_status( ostream& s ) const;
    /*@Doc:
    writes the state of the object to the specified stream:
        \tr_Marray<>
    */

    MDDType::MDDTypeEnum getSubtype() const;
    /*@Doc:
    return subclass of MDDType (runtime typing)
    */

    virtual ~MDDType();
    /*@Doc:
    virtual destructor.
    */

    virtual int compatibleWith(const Type* aType) const;
    /*@Doc:
    check for compatibility of MDDTypes:
        if aType is a MDDTYPE Type (don't confuse with MDDType!!)
    */

    virtual int compatibleWithDomain(const r_Minterval* aDomain) const;
    /*@Doc:
    check for compatibility with a certain domain.
    always returns 1.
    */

    virtual r_Bytes getMemorySize() const;
    /*@Doc:
    the memory space is computed by:
        sizeof(MDDType::MDDTypeEnum) + DBNamedObject::getMemorySize();
    */

protected:

    virtual void insertInDb() throw (r_Error);

    virtual void readFromDb() throw (r_Error);

    virtual void deleteFromDb() throw (r_Error);

    MDDTypeEnum mySubclass;
    /*@Doc:
    used for runtime typing and comparison operation.
    */
};

#endif
