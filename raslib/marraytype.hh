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
 * INCLUDE: marraytype.hh
 *
 * MODULE:  raslib
 * CLASS:   r_Marray_Type
 *
 * COMMENTS:
 *
*/

#ifndef _D_MARRAY_TYPE_
#define _D_MARRAY_TYPE_

#include "raslib/type.hh"

class r_Base_Type;
class r_Error;

//@ManMemo: Module: {\bf raslib}

/*@Doc:
  This class represents the marray type in the ODMG conformant
  representation of the RasDaMan type system.
*/


class r_Marray_Type : public r_Type
{
public:
    /// constructor getting basetype
    r_Marray_Type(const r_Base_Type&);

    /// copy constructor
    /// if base type is NULL an exception will be raised.
    /// this is possible
    r_Marray_Type(const r_Marray_Type&) throw (r_Error);

    /// assignment operator
    /// if base type is NULL an exception will be raised.
    /// this is possible
    const r_Marray_Type& operator=(const r_Marray_Type&) throw (r_Error);

    bool isMarrayType() const;

    /// get base type
    const r_Base_Type& base_type() const;

    /// clone operation
    virtual r_Type* clone() const;

    /// retrieve id of the type.
    virtual r_Type::r_Type_Id type_id() const;

    /// converts array of cells from NT byte order to Unix byte order.
    virtual void convertToLittleEndian(char* cells, r_Area noCells) const;

    /// converts array of cells from Unix byte order to NT byte order.
    virtual void convertToBigEndian(char* cells, r_Area noCells) const;

    /// writes state of object to specified stream
    virtual void print_status(std::ostream& s = std::cout) const;

    /// destructor
    ~r_Marray_Type();

protected:
    /// default constructor
    /// should be used by noone
    r_Marray_Type();

    /// base type
    r_Base_Type* baseType;
};

//@Doc: write the status of a marray type to a stream
extern std::ostream &operator<<( std::ostream &str, const r_Marray_Type &type );

#endif

