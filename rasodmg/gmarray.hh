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
 * INCLUDE: gmarray.hh
 *
 * MODULE:  rasodmg
 * CLASS:   r_GMarray
 *
 * COMMENTS:
 *      None
*/

#ifndef _D_GMARRAY_
#define _D_GMARRAY_

#include "rasodmg/object.hh"

#include "raslib/error.hh"
#include "raslib/minterval.hh"
#include "raslib/point.hh"
#include "raslib/type.hh"
#include "raslib/mddtypes.hh"

#include <iostream>

// forward declarations
class r_Storage_Layout;


//@ManMemo: Module: {\bf rasodmg}

/*@Doc:

  The class represents a generic MDD in the sense that it
  is independent of the cell base type. The only information
  available is the length in bytes of the base type.
  More specific MDDs including base type information for more
  type safety are represented by the template subclass \Ref{r_Marray}.
  Class \Ref{r_Marray} provides a constructor to convert to the base
  type specific class.
*/

/**
  * \ingroup Rasodmgs
  */
class r_GMarray : public r_Object
{
public:
    /// default constructor (no memory is allocated!)
    r_GMarray() throw(r_Error);

    /// constructor for uninitialized MDD objects
    r_GMarray(const r_Minterval& init_domain, r_Bytes type_length, r_Storage_Layout* stl = 0) throw (r_Error);
    /**
      If a storage layout pointer is provided, the object refered to is
      taken and memory control moves to the \Ref{r_GMarray} class.
      The user has to take care, that each creation of \Ref{r_GMarray}
      objects get a new storage layout object.
      r_Error is throw if the storage layout does not fit the type length or the dimension of the init domain and when the dimension of the domain is 0 (uninitialised).
    */

    /// copy constructor
    r_GMarray(const r_GMarray&) throw(r_Error);

    /// constructor which doesn't copy the data
    r_GMarray(r_GMarray&) throw(r_Error);

    /// destructor
    virtual ~r_GMarray();

    /// it is called when an object leaves transient memory (internal use only)
    virtual void r_deactivate();

    /// assignment: cleanup + copy
    const r_GMarray& operator= (const r_GMarray&);

    /// subscript operator for read access of a cell
    const char* operator[](const r_Point&) const
    throw(r_Edim_mismatch, r_Eindex_violation);

    /// Returns a r_GMarray that is the intersection of the current domain with the specified interval
    r_GMarray* intersect(r_Minterval where) const;

    //@Man: Read methods
    //@{
    ///

    /// gets a pointer to the storage layout object
    const r_Storage_Layout* get_storage_layout() const;
    /// getting the spatial domain
    inline const r_Minterval& spatial_domain() const;
    /// get the internal representation of the array
    inline char*         get_array();
    /// get the internal representation of the array for reading
    inline const char*   get_array() const;
    /// get size of internal array representation in byets
    inline r_Bytes get_array_size() const;
    /// get length of cell type in bytes
    inline r_Bytes get_type_length() const;
    /// get current data format
    inline r_Data_Format get_current_format() const;

    /// get base type schema
    const r_Base_Type* get_base_type_schema();

    ///
    //@}

    //@Man: Write methods
    //@{
    ///
    /// sets the storage layout object and checks compatibility with the domain
    void set_storage_layout(r_Storage_Layout *) throw (r_Error);
    /// set spatial domain
    inline void  set_spatial_domain(const r_Minterval& domain);
    /// set the internal representation of the array
    inline void  set_array(char*);
    /// set size of internal memory representation in bytes
    inline void  set_array_size(r_Bytes);
    /// set length of cell type in bytes
    inline void  set_type_length(r_Bytes);
    /// set current data format
    inline void  set_current_format(r_Data_Format);

    ///
    //@}

    //@Man: Methods for database communication (internal use only)
    //@{
    ///

    /// inserts an object into the database
    virtual void insert_obj_into_db();
    /// insert myself into a specific collection in the database
    void insert_obj_into_db(const char* collName);

    ///
    //@}

    /// writes the state of the object to the specified stream
    virtual void print_status(std::ostream& s = std::cout) const;

    /// writes the state of the object to the specified stream
    void print_status(std::ostream& s, int hexoutput) const;

protected:
    /// spatial domain
    r_Minterval domain;

    /// pointer to the internal array representation
    char* data;

    /// size of internal array representation in bytes
    r_Bytes data_size;

    /// length of the cell base type in bytes
    r_Bytes type_length;

    /// store current data format
    r_Data_Format current_format;

    /// pointer to storage layout object
    r_Storage_Layout* storage_layout;
};

#include "rasodmg/gmarray.icc"

#endif
