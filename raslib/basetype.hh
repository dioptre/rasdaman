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
 * INCLUDE: basetype.hh
 *
 * MODULE:  raslib
 * CLASS:   r_Base_Type
 *
 * COMMENTS:
 *          None
*/

#ifndef _D_BASETYPE_
#define _D_BASETYPE_

#include "raslib/type.hh"



//@ManMemo: Module: {\bf raslib}

/**
  This class is the superclass of the types r_Structure_Type and r_Primitive_Type in the
  representation of the RasDaMan type system.
*/

class r_Base_Type : public r_Type
{
public:
    /// default constructor.
    r_Base_Type();

    /// constructor getting name of basetype.
    r_Base_Type(const char* newTypeName, r_Bytes newSize);

    /// copy constructor
    r_Base_Type(const r_Base_Type& oldObj);

    /// assignment operator.
    const r_Base_Type& operator=(const r_Base_Type& oldObj);

    /// destructor.
    virtual ~r_Base_Type();

    /// check, if type is a base type (primitive type or structure type).
    virtual bool isBaseType() const;

    /// retrieve size of the type.
    r_Bytes size() const;

    /// prints value of a primitive type or values of a structured type
    virtual void print_value(const char* storage, std::ostream& s = std::cout) const = 0;

protected:
    /// storing size of type in bytes
    r_Bytes typeSize;

};

#endif
