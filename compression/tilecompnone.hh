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
 * INCLUDE: tilecompnone.hh
 *
 * MODULE:  compression
 * CLASS:   r_Tile_Comp_None
 *
 * COMMENTS:
 *
*/

/**
*   @file tilecompnone.hh
*
*   @ingroup Compression
*/

#ifndef _R_TILE_COMP_NONE_HH_
#define _R_TILE_COMP_NONE_HH_

#include "compression/tilecompression.hh"


//@ManMemo: Module {\bf compression}

/*@Doc:
  Tile compression for no compression.
*/

class r_Tile_Comp_None : public r_Tile_Compression
{
public:
    /// constructor
    r_Tile_Comp_None( const r_Minterval &dom, const r_Base_Type *type );
    /// copy constructor
    r_Tile_Comp_None( const r_Tile_Comp_None &src );
    /// destructor
    ~r_Tile_Comp_None( void );
    /// compress function
    virtual void *compress( const void *data, r_ULong &size, const char *options=NULL );
    /// decompress function
    virtual void *decompress( const void *data, r_ULong size, const char *options=NULL );
    /// identification
    const char *get_name( void ) const;
    r_Data_Format get_data_format( void ) const;
    /// create a copy
    r_Tile_Compression *clone( void ) const;
};

#endif
