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
 * INCLUDE: sinterval.hh
 *
 * MODULE:  raslib
 * CLASS:   r_Sinterval
 *
 * COMMENTS:
 *
*/

#ifndef _D_SINTERVAL_
#define _D_SINTERVAL_

#ifdef __VISUALC__
// Disable warning about exception specification.
#pragma warning( disable : 4290 )
#endif

class r_Error;
class r_Eno_interval;

#include <iostream>

#include "raslib/point.hh"

//@ManMemo: Module: {\bf raslib}

/*@Doc:

 The class represents an interval with lower and upper bound.
 Operations on the interval are defined according to the
 ODMG-93 standard.
 The operations union, difference, and intersection are
 defined according to the following table:

    | ...  fixed bound \\
    * ...  open bound


 \begin{verbatim}

 class   orientation       union    difference  intersection
 -----------------------------------------------------------
   1     |-a-| |-b-|       error    a           error

   2     |-a-|             [a1,b2]  [a1,b1]     [b1,a2]
            |-b-|

   3     |--a--|           a        error       b
          |-b-|

   4     |-b-|             [b1,a2]  [b2,a2]     [a1,b2]
            |-a-|

   5     |--b--|           b        error       a
          |-a-|

   6     |-b-| |-a-|       error    a           error

   7     |-a-|-b-|         [a1,b2]  a           [a2,a2]

   8     |-b-|-a-|         [b1,a2]  a           [b2,b2]

   9     |--a--|           a        [a1,b1]     b
           |-b-|

  10     |--a--|           a        [b2,a2]     b
         |-b-|

  11     |-a-|             a        error       a
         |-b-|

  12     |--b--|           b        error       a
         |-a-|

  13     |--b--|           b        error       a
           |-a-|

  -----------------------------------------------------

  14     |--a--*           a        error       b
          |-b-|

  15     |--a--*           a        [b2,a2]     b
         |-b-|

  16     |-b-| |-a-*       error    a           error

  17     |-b-|-a-*         [b1,a2]  a           [b2,b2]

  18      |--a--*          [b1,a2]  [b2,a2]     [a1,b2]
         |-b-|

  -----------------------------------------------------

  19     *--a--|           a        error       b
          |-b-|

  20     *--a--|           a        [a1,b1]     b
           |-b-|

  21     *-a-| |-b-|       error    a           error

  22     *-a-|-b-|         [a1,b2]  a           [a2,a2]

  23     *--a--|           [a1,b2]  [a1,b1]     [b1,a2]
            |-b-|

  -----------------------------------------------------

  24     |--b--*           b        error       a
          |-a-|

  25     |--b--*           b        error       a
         |-a-|

  26     |-a-| |-b-*       error    a           error

  27     |-a-|-b-*         [a1,b2]  a           [a2,a2]

  28      |--b--*          [a1,b2]  [a1,b1]     [b1,a2]
         |-a-|

  -----------------------------------------------------

  29     *--b--|           b        error       a
          |-a-|

  30     *--b--|           b        error       a
           |-a-|

  31     *-b-| |-a-|       error    a           error

  32     *-b-|-a-|         [b1,a2]  a           [b2,b2]

  33     *--b--|           [b1,a2]  [b2,a2]     [a1,b2]
            |-a-|

  -----------------------------------------------------

  34     *-a-| |-b-*       error    a           error

  35     *-a-|-b-*         [a1,b2]  a           [a2,a2]

  36     *-a-|             [a1,b2]  [a1,b1]     [b1,a2]
            |-b-*

  -----------------------------------------------------

  37     *-b-| |-a-*       error    a           error

  38     *-b-|-a-*         [b1,a2]  a           [b2,b2]

  39     *-b-|             [b1,a2]  [a1,b1]     [a1,b2]
            |-a-*

  -----------------------------------------------------

  40     *-a-|             b        error       a
          *-b-|

  41     *-a-|             a        error       a
         *-b-|

  42     *-b-|             a        [b2,a2]     b
          *-a-|

  -----------------------------------------------------

  43     |-a-*             a        [a1,b1]     b
          |-b-*

  44     |-a-*             a        error       a
         |-b-*

  45     |-b-*             b        error       a
          |-a-*

  -----------------------------------------------------
  46     *-a-* |-b-|       a        error       b

  47     *-b-* |-a-|       b        error       a

  48     *-a-*             a        [b2,a2]     b
          *-b-|

  49     *-a-*             a        [a1,b1]     b
          |-b-*

  50     *-b-*             b        error       a
          *-a-|

  51     *-b-*             b        error       a
          |-a-*

  52     *-a-*             a        error       a
         *-b-*

 \end{verbatim}

 Attention: The difference operation has to be reconsidered in future
 concerning a discrete interpretation of the intervals.

 The closure operation defines an interval which is the smallest
 interval containing the two operands.
 The method {\tt intersects_with()} returns 0 in the error cases of the
 intersection operation and 1 otherwise.

*/

class r_Sinterval
{
public:
    /// default constructor creates an interval with open bounds
    r_Sinterval();

    /// constructor taking string representation (e.g. *:200 )
    r_Sinterval( char* ) throw(r_Eno_interval);

    /// constructor for an interval with fixed bounds
    r_Sinterval( r_Range low, r_Range high ) throw( r_Eno_interval );

    //@Man: Constructors for intervals with at least one open bound.
    //@{
    ///
    r_Sinterval( char,        r_Range high );
    ///
    r_Sinterval( r_Range low, char         );
    ///
    r_Sinterval( char,        char         );
    ///
    //@}

    /// equal operator
    bool operator==( const r_Sinterval& ) const;

    /**
      Two intervals are equal if they have the same lower and upper bound.
    */

    /// non equal operator - negation of equal operator
    bool operator!=( const r_Sinterval& ) const;

    //@Man: Read/Write methods:
    //@{
    ///
    inline r_Range  low () const;
    ///
    inline r_Range  high() const;
    ///
    inline bool      is_low_fixed() const;
    ///
    inline bool      is_high_fixed() const;

    ///
    void set_low     ( r_Range low  ) throw( r_Eno_interval );
    ///
    void set_high    ( r_Range high ) throw( r_Eno_interval );
    ///
    inline void set_low ( char );
    ///
    inline void set_high( char );

    /// get the size of one dimensional interval as range.
    r_Range get_extent() const throw(r_Error);
    /*@Doc:
      Returns a range with high() - low() + 1 of this interval.
    */

    ///
    void set_interval( r_Range low, r_Range high ) throw( r_Eno_interval );
    ///
    void set_interval( char,        r_Range high );
    ///
    void set_interval( r_Range low, char         );
    ///
    void set_interval( char,        char         );
    ///
    //@}

    /// determines if the self interval intersects with the delivered one
    bool intersects_with( const r_Sinterval& ) const;

    //@Man: Methods/Operators for the union operation:
    //@{
    ///
    r_Sinterval& union_of           ( const r_Sinterval&, const r_Sinterval& )
    throw( r_Eno_interval );
    ///
    r_Sinterval& union_with         ( const r_Sinterval& )
    throw( r_Eno_interval );
    ///
    r_Sinterval& operator+=         ( const r_Sinterval& )
    throw( r_Eno_interval );
    ///
    r_Sinterval  create_union       ( const r_Sinterval& ) const
    throw( r_Eno_interval );
    ///
    r_Sinterval  operator+          ( const r_Sinterval& ) const
    throw( r_Eno_interval );
    ///
    //@}

    //@Man: Methods/Operators for the difference operation:
    //@{
    ///
    r_Sinterval& difference_of       ( const r_Sinterval&, const r_Sinterval& )
    throw( r_Eno_interval );
    ///
    r_Sinterval& difference_with     ( const r_Sinterval& )
    throw( r_Eno_interval );
    ///
    r_Sinterval& operator-=          ( const r_Sinterval& )
    throw( r_Eno_interval );
    ///
    r_Sinterval  create_difference   ( const r_Sinterval& ) const
    throw( r_Eno_interval );
    ///
    r_Sinterval  operator-           ( const r_Sinterval& ) const
    throw( r_Eno_interval );
    ///
    //@}

    //@Man: Methods/Operators for the intersection operation:
    //@{
    ///
    r_Sinterval& intersection_of     ( const r_Sinterval&, const r_Sinterval& )
    throw( r_Eno_interval );
    ///
    r_Sinterval& intersection_with   ( const r_Sinterval& )
    throw( r_Eno_interval );
    ///
    r_Sinterval& operator*=          ( const r_Sinterval&)
    throw( r_Eno_interval);
    ///
    r_Sinterval  create_intersection ( const r_Sinterval& ) const
    throw( r_Eno_interval );
    ///
    r_Sinterval  operator*           ( const r_Sinterval& ) const
    throw( r_Eno_interval );
    ///
    //@}

    //@Man: Methods/Operators for the closure operation:
    //@{
    ///
    r_Sinterval& closure_of          ( const r_Sinterval&, const r_Sinterval& )
    throw( r_Eno_interval );
    ///
    r_Sinterval& closure_with        ( const r_Sinterval& )
    throw( r_Eno_interval );
    ///
    r_Sinterval  create_closure      ( const r_Sinterval& ) const
    throw( r_Eno_interval );
    ///
    //@}

    /// writes the state of the object to the specified stream
    void print_status( std::ostream& s = std::cout ) const;

    /// gives back the string representation
    char* get_string_representation() const;
    /**
      The string representation delivered by this method is allocated using {\tt malloc()} and
      has to be free unsing {\tt free()} in the end. It can be used to construct a {\tt r_Sinterval}
      again with a special constructor provided. The string representation is build using
      {\tt print_status()}.
    */

    //@Man: Methods for internal use only:
    //@{
    /// calculate the size of the storage space occupied
    r_Bytes get_storage_size( ) const;
    ///
    //@}

private:

    //@Man: Calculation methods for the operations:
    //@{
    ///
    r_Sinterval  calc_union       ( const r_Sinterval& a, const r_Sinterval& b ) const
    throw( r_Eno_interval );
    ///
    r_Sinterval  calc_difference  ( const r_Sinterval& a, const r_Sinterval& b ) const
    throw( r_Eno_interval );
    ///
    r_Sinterval  calc_intersection( const r_Sinterval& a, const r_Sinterval& b ) const
    throw( r_Eno_interval );
    ///
    r_Sinterval  calc_closure     ( const r_Sinterval& a, const r_Sinterval& b ) const
    throw( r_Eno_interval );
    ///
    //@}

    /// compute the class of the two operands
    int          classify( const r_Sinterval& a, const r_Sinterval& b ) const;

    //@Man: Attributes storing the bounds:
    //@{
    ///
    r_Range lower_bound;
    ///
    r_Range upper_bound;
    ///
    //@}

    //@Man: Attributes specifying wheter the lower/upper bound is fixed or not:
    //@{
    ///
    bool     low_fixed;
    ///
    bool     high_fixed;
    ///
    //@}
};




//@ManMemo: Module: {\bf raslib}
/**
  Output stream operator for objects of type {\tt const r_Sinterval}.
*/
extern std::ostream& operator<<( std::ostream& s, const r_Sinterval& d );

#include "raslib/sinterval.icc"

#endif
