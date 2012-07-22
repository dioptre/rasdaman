/*
 * This file is part of rasdaman community.
 *
 * Rasdaman community is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Rasdaman community is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with rasdaman community.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright 2003 - 2010 Peter Baumann / rasdaman GmbH.
 *
 * For more information please see <http://www.rasdaman.org>
 * or contact Peter Baumann via <baumann@rasdaman.com>.
*/

/**
 * INCLUDE: structuretype.hh
 *
 * MODULE:  raslib
 * CLASS:   r_Structure_Type
 *
 * COMMENTS:
 *
*/

#ifndef _D_STRUCTURE_TYPE_
#define _D_STRUCTURE_TYPE_

#if (defined(__VISUALC__) && !defined(__EXECUTABLE__))
#define __EXECUTABLE__
#include "raslib/itertype.hh"
#undef  __EXECUTABLE__
#else
#include "raslib/itertype.hh"
#endif

class r_Error;
#include "raslib/basetype.hh"
#include "raslib/attribute.hh"


//@ManMemo: Module: {\bf raslib}

/*@Doc:
  This class represents all user defined structured types in the
  ODMG conformant representation of the RasDaMan type system.
*/

class r_Structure_Type : public r_Base_Type
{
public:
    /// typedef for iterator iterating through all attributes;
    typedef r_IterType<r_Attribute> attribute_iterator;
    /// default constructor.
    r_Structure_Type();
    /// constructor getting name of type and type id.
    r_Structure_Type( char* newTypeName, unsigned int newNumAttrs, r_Attribute* newAttrs, int offset = 0 );
    /// copy constructor
    r_Structure_Type( const r_Structure_Type& oldObj );
    /// assignment operator.
    const r_Structure_Type& operator=( const r_Structure_Type& oldObj );
    /// destructor.
    virtual ~r_Structure_Type();

    /// clone operation
    virtual r_Type* clone() const;

    /// retrieve id of the type.
    virtual r_Type::r_Type_Id type_id() const;

    /// check, if type is primitive or structured.
    virtual bool isStructType() const;

    /// check, if this type is compatible with myType (e.g. check the structure ignoring then names of atributtes)
    virtual bool compatibleWith(const r_Structure_Type* myType) const;

    /// returns attribute iterator at begin position.
    attribute_iterator defines_attribute_begin() const;
    /// returns attribute iterator at end position (behind last attribute).
    attribute_iterator defines_attribute_end() const;
    /// return attribute specified by name.
    r_Attribute resolve_attribute(const char* name) const throw( r_Error );
    /// return attribute specified by number starting with zero.
    r_Attribute resolve_attribute(unsigned int number) const throw( r_Error );
    /// subscript operator to access attributes by index
    r_Attribute operator[]( unsigned int number ) const throw( r_Error );

    /// get number of attributes
    unsigned int count_elements() const;

    /// converts array of cells from NT byte order to Unix byte order.
    virtual void convertToLittleEndian(char* cells, r_Area noCells) const;

    /// converts array of cells from Unix byte order to NT byte order.
    virtual void convertToBigEndian(char* cells, r_Area noCells) const;

    /// writes state of object to specified stream
    virtual void print_status( std::ostream& s = std::cout ) const;

    /// prints values of a structured type
    virtual void print_value( const char* storage,  std::ostream& s = std::cout  ) const;

protected:
    unsigned int numAttrs;
    r_Attribute* myAttributes;
};

//@Doc: write the status of a structure type to a stream
extern std::ostream &operator<<( std::ostream &str, const r_Structure_Type &type );

#endif
