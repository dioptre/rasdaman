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
 * INCLUDE: flatbasetype.hh
 *
 * MODULE:  raslib
 * CLASS:   r_Flat_Base_Type
 *
 * COMMENTS:
 *
*/

#ifndef _FLAT_BASE_TYPE_HH_
#define _FLAT_BASE_TYPE_HH_

#include <iostream>

#include "raslib/error.hh"

class r_Base_Type;
class r_Primitive_Type;
class r_Structure_Type;

//@ManMemo: Module {\bf raslib}


/*@Doc:
  This class can be used to get a more convenient view on a structured
  base type. It eliminates all hierarchies and gives you the primitive
  types only which can be iterated over with a normal linear loop. Used
  in e.g. the compression module. Note that this is not a regular member
  of the r_Type hierarchy!
*/

class r_Flat_Base_Type
{
  public:
  /// default constructor, shouldn't be used
  r_Flat_Base_Type( void );
  /// constructor receiving the (hierarchical) base type
  r_Flat_Base_Type( const r_Base_Type *type );
  /// copy constructor
  r_Flat_Base_Type( const r_Flat_Base_Type &src );
  /// destructor
  ~r_Flat_Base_Type( void );

  /// return number of primitive types
  unsigned int get_num_types( void ) const;
  /// return pointer to primitive type num
  /// index violation is thrown if higher index is requested than available
  const r_Primitive_Type *type( unsigned int num ) const throw (r_Eindex_violation);
  /// operator returns pointer to primitive type num or NULL if invalid
  /// index violation is thrown if higher index is requested than available
  const r_Primitive_Type *operator[]( unsigned int num ) const throw (r_Eindex_violation);
  /// return offset of primitive type num
  /// index violation is thrown if higher index is requested than available
  unsigned int offset( unsigned int num ) const throw (r_Eindex_violation);
  /// return size of entire type
  r_Bytes size( void ) const;
  /// assignment of another flat type
  r_Flat_Base_Type &operator=( const r_Flat_Base_Type &src );
  /// assignment of a base type
  r_Flat_Base_Type &operator=( const r_Base_Type *type );
  /// equality
  bool operator==( const r_Flat_Base_Type &src ) const;
  /// print status to a stream
  void print_status( std::ostream &str ) const;


  protected:
  /// shared init code
  void init_shared( void );
  /// process a base type
  void process_type( const r_Base_Type *type );
  /// copy another flat type
  void copy_flat_type( const r_Flat_Base_Type &type );
  /// free type-specific data (destructor, assignment)
  void free_type_data( void );

  /// parse a structure type and return number of primitive types contained therein
  unsigned int parse_structure_type( const r_Structure_Type *type, unsigned int number,
				     unsigned int offset );
  /// parse a primitive type
  void parse_primitive_type( r_Primitive_Type *type, unsigned int number,
			     unsigned int offset );

  /// the number of primitive types
  unsigned int numPrimTypes;
  /// the total size of the entire type
  r_Bytes typeSize;
  /// the primitive types
  r_Primitive_Type **primTypes;
  /// the corresponding offsets
  unsigned int *offsets;
};


//@ManMemo: Module {\bf raslib}

//@Doc: write the status of a flat base type to a stream
extern std::ostream &operator<<( std::ostream &str, const r_Flat_Base_Type &type );

#endif
