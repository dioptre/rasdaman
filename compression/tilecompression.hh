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
 * INCLUDE: tilecompression.hh
 *
 * MODULE:  compression
 * CLASS:   r_Tile_Compression
 *
 * COMMENTS:
 *
*/

/**
*	@file tilecompression.hh
*
*	@ingroup Compression
*/

#ifndef _R_TILE_COMPRESSION_HH_
#define _R_TILE_COMPRESSION_HH_


#include <stdio.h>

#include "raslib/error.hh"
#include "raslib/minterval.hh"
#include "raslib/mddtypes.hh"
#include "raslib/odmgtypes.hh"
#include "raslib/storageman.hh"


class r_Base_Type;
class r_Parse_Params;
class RMTimer;


//@ManMemo: Module: {\bf compression}

/*@Doc:
  Abstract base class for compression / decompression functionality
  of tiles, to be executed on client or server. r_Tile_Compression is
  used by both compressing and uncompressing classes.
 */

class r_Tile_Compression
{
  public:
  /// enum used as return for check_data_format
  enum Support_Format
       { INVALID=0, COMPRESSION, CONVERSION };

  /// default constructor, fixes size and layout of tile
  r_Tile_Compression( const r_Minterval &dom, const r_Base_Type *type );
  /// copy constructor
  r_Tile_Compression( const r_Tile_Compression &src );
  /// destructor
  virtual ~r_Tile_Compression( void );

  /// check support for fmt
  static Support_Format check_data_format( r_Data_Format fmt );
  /**
     check whether there is support for the given data format; returns COMPRESSION
     for a real compression format, CONVERSION for a data exchange format, INVALID othe  */

  /// create a compression class from the data format
  static r_Tile_Compression *create( r_Data_Format fmt, const r_Minterval &dom, const r_Base_Type *type ) throw(r_Error);

  /// create a compression class from the name of the compression algorithm
  static r_Tile_Compression *create( const char *name, const r_Minterval &dom, const r_Base_Type *type ) throw(r_Error);

  /// get a data format for a compression name
  static r_Data_Format get_format_from_name( const char *name ) throw(r_Error);

  /// read name and data format of available tile compression formats
  static const char *get_format_info( unsigned int number, r_Data_Format &fmt );

  /// compress
  virtual void *compress( const void *data, r_ULong &size, const char *options = NULL ) = 0;
  /**
     compresses data, passes options to the underlying compression scheme
     and returns a pointer to the compressed data and its size. Deallocate
     the pointer with tilecomp_free() !
  */

  /// decompress
  virtual void *decompress( const void *data, r_ULong size, const char *options = NULL ) = 0;
  /**
     decompresses the compressed data of size bytes, passing options to the
     underlying decompression scheme and returns a pointer to the
     decompressed data. Deallocate the pointer with tilecomp_free() !
  */

  /// data format after decompression
  virtual r_Data_Format get_decomp_format( void ) const;
  /**
     data format after decompression. This allows easier integration of
     data exchange formats with the compression mechanism: real compression
     engines return r_Array, DEF containers return their DEF's data format.
     This function need only be overloaded if the decompressed format differs
     from r_Array.
  */

  /// check whether compression and decompression automatically do the
  /// required endianness conversions (this is usually only true for
  /// complex approaches like wavelets)
  virtual bool converts_endianness( void ) const;

  /// returns name of compression scheme
  virtual const char *get_name( void ) const = 0;
  /// returns compression scheme identifier
  virtual r_Data_Format get_data_format( void ) const = 0;
  /// make a copy of this compression scheme
  virtual r_Tile_Compression *clone( void ) const = 0;

  /// returns the size of one cell
  r_Bytes get_type_size( void ) const;
  /// returns the size of the uncompressed data
  r_ULong get_tile_size( void ) const;
  /// returns tile domain
  const r_Minterval &get_domain( void ) const;
  /// returns tile type
  const r_Base_Type *get_base_type( void ) const;
  /// resume timer (COMPBENCHMARK only)
  void resume_timer( void );
  /// pause timer (COMPBENCHMARK only)
  void pause_timer( void );

  /// set the storage handler
  virtual void set_storage_handler( const r_Storage_Man &newStore );
  /**
     set the storage handler to use for memory (de)allocation of the
     pointers returned from (de)compress. By default malloc/free are
     used. 
  */

  //@Man: internal<--> external data representations
  /**
     Since the tile compression class must run on client and server, the
     data format must be well-specified on both. The following functions
     take care of endianness and alignment.
  */
  /// write a short to dest
  static void write_short( void *dest, r_Short val );
  /// write a long to dest
  static void write_long( void *dest, r_Long val );
  /// read a short from dest
  static void read_short( const void *src, r_Short &val );
  /// write a short to dest
  static void read_long( const void *src, r_Long &val );

  /// structure for storing information about all data formats
  typedef struct tile_comp_format_s {
    r_Data_Format format;
    const char *name;
  } tile_comp_format_t;



  protected:

  /// get information about a base type's atomic types
  static unsigned int get_atom_info( const r_Base_Type* baseType, unsigned int* sizes=NULL,
				     unsigned int *idxptr=NULL );
  /**
     return number of atomic types in base type, optionally return the
     individual sizes in sizes. idxptr must be NULL (or missing) when calling.
  */

  /// instantiate timer for benchmarking with a given level
  void instantiate_timer( const char *func, int level=0 );

  r_Storage_Man mystore;

  r_Minterval mydomain;
  r_Base_Type *mytype;

  r_Parse_Params *compParams;
  r_Parse_Params *decompParams;

  RMTimer *myTimer;

  static const tile_comp_format_t all_formats[];
};


#endif
