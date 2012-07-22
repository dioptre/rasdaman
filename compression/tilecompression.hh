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
*   @file tilecompression.hh
*
*   @ingroup Compression
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
    enum Support_Format
    { INVALID=0, COMPRESSION, CONVERSION };

    r_Tile_Compression( const r_Minterval &dom, const r_Base_Type *type );
    r_Tile_Compression( const r_Tile_Compression &src );
    virtual ~r_Tile_Compression( void );

    static Support_Format check_data_format( r_Data_Format fmt );
    static r_Tile_Compression *create( r_Data_Format fmt, const r_Minterval &dom, const r_Base_Type *type ) throw(r_Error);
    static r_Tile_Compression *create( const char *name, const r_Minterval &dom, const r_Base_Type *type ) throw(r_Error);
    static r_Data_Format get_format_from_name( const char *name ) throw(r_Error);
    static const char *get_format_info( unsigned int number, r_Data_Format &fmt );
    virtual void *compress( const void *data, r_ULong &size, const char *options = NULL ) = 0;
    virtual void *decompress( const void *data, r_ULong size, const char *options = NULL ) = 0;
    virtual r_Data_Format get_decomp_format( void ) const;
    virtual bool converts_endianness( void ) const;
    virtual const char *get_name( void ) const = 0;
    virtual r_Data_Format get_data_format( void ) const = 0;
    virtual r_Tile_Compression *clone( void ) const = 0;
    r_Bytes get_type_size( void ) const;
    r_ULong get_tile_size( void ) const;
    const r_Minterval &get_domain( void ) const;
    const r_Base_Type *get_base_type( void ) const;
    void resume_timer( void );
    void pause_timer( void );
    virtual void set_storage_handler( const r_Storage_Man &newStore );
    static void write_short( void *dest, r_Short val );
    static void write_long( void *dest, r_Long val );
    static void read_short( const void *src, r_Short &val );
    static void read_long( const void *src, r_Long &val );
    typedef struct tile_comp_format_s
    {
        r_Data_Format format;
        const char *name;
    } tile_comp_format_t;

protected:
    static unsigned int get_atom_info( const r_Base_Type* baseType, unsigned int* sizes=NULL,
                                       unsigned int *idxptr=NULL );
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
