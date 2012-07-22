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
 * INCLUDE: endian.hh
 *
 * MODULE:  raslib
 * CLASS:   r_Endian
 *
 * COMMENTS:
 *      None
*/

#ifndef _R_ENDIANTOOLS_HH_
#define _R_ENDIANTOOLS_HH_

#include <iostream>

class r_Minterval;
class r_Base_Type;
class r_Primitive_Type;
#include "raslib/odmgtypes.hh"
#include "raslib/mddtypes.hh"

//@ManMemo: Module {\bf raslib}

/*@Doc:
  Class to check the endianness of the host machine and to
  swap the endianness of types and arrays. Don't instantiate,
  static members only.
*/

class r_Endian
{
public:
    /// endianness identifiers
    enum r_Endianness
    {
        r_Endian_Big,
        r_Endian_Little
    };

    /// swap endianness of atomic types
    static r_Short swap( r_Short val );
    static r_UShort swap( r_UShort val );
    static r_Long swap( r_Long val );
    static r_ULong swap( r_ULong val );
    static r_Float swap( r_Float val );
    static r_Double swap( r_Double val );

    /// query host machine's endianness
    static r_Endianness get_endianness( void );

    /// change the endianness of a linear array of size <size> and type <type>
    static void swap_array( const r_Primitive_Type *type, r_Bytes size, const void *src, void *dest );

    /// change the endianness of a linear array of size <size>, type implicit
    static void swap_array( r_Bytes size, const r_Octet *src, r_Octet *dest ); // dummy
    static void swap_array( r_Bytes size, const r_Char *src, r_Char *dest ); // dummy
    static void swap_array( r_Bytes size, const r_Short *src, r_Short *dest );
    static void swap_array( r_Bytes size, const r_UShort *src, r_UShort *dest );
    static void swap_array( r_Bytes size, const r_Long *src, r_Long *dest );
    static void swap_array( r_Bytes size, const r_ULong *src, r_ULong *dest );
    static void swap_array( r_Bytes size, const r_Float *src, r_Float *dest );
    static void swap_array( r_Bytes size, const r_Double *src, r_Double *dest );

    /// change the endianness of a linear array of size <size> with type size <tsize>
    static void swap_array( r_Bytes size, r_Bytes tsize, const void *src, void *dest);

    /// change the endianness of one type member of an array for identical domains
    /// for src and dest; step is the increment between cells
    static void swap_array( const r_Primitive_Type *type, const r_Minterval &srcDom,
                            const r_Minterval &srcIterDom, const void *src, void *dest,
                            r_ULong step );

    /// change the endianness of the entire tile for identical domains for src and dest
    static void swap_array( const r_Base_Type *type, const r_Minterval &srcDom,
                            const r_Minterval &srcIterDom, const void *src, void *dest );

    /// change the endianness of one type member of an array for the generic case;
    /// step is the increment between cells.
    static void swap_array( const r_Primitive_Type *type, const r_Minterval &srcDom,
                            const r_Minterval &srcIterDom, const r_Minterval &destDom,
                            const r_Minterval &destIterDom, const void *src, void *dest,
                            r_ULong step );

    /// change the endianness of the entire tile for the generic case
    static void swap_array( const r_Base_Type *type, const r_Minterval &srcDom,
                            const r_Minterval &srcIterDom, const r_Minterval &destDom,
                            const r_Minterval &destIterDom, const void *src, void *dest );
};

//@ManMemo: Module: {\bf raslib}
/**
  Output stream operator for enum of type {\tt const} \Ref{r_Minterval}.
  */
extern std::ostream& operator<<( std::ostream& s, r_Endian::r_Endianness& e );


#endif
