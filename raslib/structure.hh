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
 * INCLUDE: structure.hh
 *
 * MODULE:  raslib
 * CLASS:   r_Structure
 *
 * COMMENTS:
 *
 * The class represents a structured value.
 *
*/

#ifndef _D_STRUCTURE_
#define _D_STRUCTURE_

#include <iostream>

class r_Error;

#include "raslib/scalar.hh"

class r_Structure_Type;

//@ManMemo: Module: {\bf raslib}

/*@Doc:

 Class \Ref{r_Structure} represents a structured value.

*/

class r_Structure : public r_Scalar
{
public:
    /// constructs a structured type value
    r_Structure( const char* newBuffer, const r_Structure_Type* newType );

    /// copy constructor
    r_Structure( const r_Structure& obj );

    /// destructor
    virtual ~r_Structure();

    /// clone operator
    virtual r_Scalar* clone() const;

    /// operator for assigning a structure
    virtual const r_Structure& operator= ( const r_Structure& );

    /// debug output
    virtual void print_status(std::ostream& s) const;

    /// returns true to indicate that this is a structured value
    virtual bool isStructure() const;

    /// get number of elements
    unsigned int count_elements() const;

    /// get buffer
    const char* get_buffer() const;

    /// access an element by name
    /// throws TypeInvalid and r_Eindex_violation
    const r_Scalar& operator[]( const char* name ) const throw( r_Error );

    /// access an element by number
    /// throws TypeInvalid and NameInvalid
    const r_Scalar& operator[]( unsigned int ) const throw( r_Error );

private:
    /// number of elements
    unsigned int numElements;

    /// array of pointers to elements
    r_Scalar** elements;

    /// char representation
    char* valueBuffer;
};



//@ManMemo: Module: {\bf raslib}
/**
  Output stream operator for objects of type {\tt const} \Ref{r_Structure}.
*/
extern std::ostream& operator<<( std::ostream& s, const r_Structure& oid );
#endif

