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
 * INCLUDE: marray.hh
 *
 * MODULE:  rasodmg
 * CLASS:   r_Marray
 *
 * COMMENTS:
 *      None
*/

#ifndef _D_MARRAY_
#define _D_MARRAY_

#include "rasodmg/gmarray.hh"

#include <iostream>

using namespace std;


//@ManMemo: Module: {\bf rasodmg}

/*@Doc:

 The central class of the library for representing an MDD
 object is named r_Marray. Through overloading operators,
 the handling of an MDD object is similar to the usage of
 a normal C or C++ array from the programmers point of view.

*/

/**
  * \ingroup Rasodmgs
  */
template<class T>
class r_Marray : public r_GMarray
{
public:
    /// function type for initialization function
    typedef T (*r_InitFunction)(const r_Point&);

    /// default constructor (no memory is allocated!)
    r_Marray() throw(r_Error);

    /// constructor for uninitialized MDD objects
    r_Marray( const r_Minterval&, r_Storage_Layout* stl = 0 ) throw(r_Error);
    /**
      If a storage layout pointer is provided, the object refered to is
      taken and memory control moves to the \Ref{r_Marray} class.
      The user has to take care, that each creation of \Ref{r_Marray}
      objects get a new storage layout object.
    */

    /// constructor for constant MDD objects
    r_Marray( const r_Minterval&, const T&, r_Storage_Layout* stl = 0 ) throw(r_Error);
    /**
      If a storage layout pointer is provided, the object refered to is
      taken and memory control moves to the \Ref{r_Marray} class.
      The user has to take care, that each creation of \Ref{r_Marray}
      objects get a new storage layout object.
    */

    /// constructor with initializing function
    r_Marray( const r_Minterval&, r_InitFunction, r_Storage_Layout* stl = 0 ) throw(r_Error);
    /**
      If a storage layout pointer is provided, the object refered to is
      taken and memory control moves to the \Ref{r_Marray} class.
      The user has to take care, that each creation of \Ref{r_Marray}
      objects get a new storage layout object.
    */

    /// copy constructor
    r_Marray( const r_Marray<T>& ) throw(r_Error);

    /// constructor getting an object of type r_GMarray
    r_Marray( r_GMarray& ) throw(r_Error);
    /*
      This constructor is used for converting general \Ref{r_GMarray} objects
      to cell type safe \Ref{r_Marray} objects. Care has to be taken because
      the memory of the \Ref{r_GMarray} can not be used anymore; it is passed
      to the \Ref{r_Marray<T>} object.
    */

    /// destructor
    virtual ~r_Marray();

    /// assignment: cleanup + copy
    const r_Marray& operator= ( const r_Marray& );

    /// subscript operator for projection in the 1st dimension
    r_Marray<T> operator[]( long ) const
    throw(r_Eindex_violation);

    /// subscript operator for restriction/extension combination
    r_Marray<T> operator[]( const r_Minterval& ) const
    throw( r_Edim_mismatch );

    /// subscript operator for read access of a cell
    const T& operator[]( const r_Point& ) const
    throw(r_Edim_mismatch, r_Eindex_violation);

    /// subscript operator for write access of a cell
    T& operator[]( const r_Point& )
    throw(r_Edim_mismatch, r_Eindex_violation);

    /// cast operator for converting to base type for cell access
    operator T()
    throw( r_Eno_cell );

    /// writes the state of the object to the specified stream
    virtual void print_status( std::ostream& s = cout ) const;
};

#include "rasodmg/marray.icc"

#ifdef EARLY_TEMPLATE
#ifdef __EXECUTABLE__
#ifdef __VISUALC__
#include "rasodmg/marray.cpp"
#else
#include "rasodmg/marray.cc"
#endif
#endif
#endif

#endif
