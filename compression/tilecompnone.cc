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
 * INCLUDE: tilecompnone.cc
 *
 * MODULE:  compression
 * CLASS:   r_Tile_Comp_None
 *
 * COMMENTS:
 *
*/

#include <string.h>

#include "raslib/rminit.hh"
#include "raslib/rmdebug.hh"
#include "compression/tilecompnone.hh"


r_Tile_Comp_None::r_Tile_Comp_None( const r_Minterval &dom, const r_Base_Type *type ) : r_Tile_Compression(dom, type)
{
    RMDBGONCE( 2, RMDebug::module_compression, "r_Tile_Comp_None", "r_Tile_Comp_None(dom,type)" );
}


r_Tile_Comp_None::r_Tile_Comp_None( const r_Tile_Comp_None &src) : r_Tile_Compression(src)
{
    RMDBGONCE( 2, RMDebug::module_compression, "r_Tile_Comp_None", "r_Tile_Comp_None(src)" );
}


r_Tile_Comp_None::~r_Tile_Comp_None( void )
{
    RMDBGONCE( 2, RMDebug::module_compression, "r_Tile_Comp_None", "~r_Tile_Comp_None()" );
}


void *r_Tile_Comp_None::compress( const void *src, r_ULong &size, const char *options )
{
    void *result;

    RMDBGONCE( 2, RMDebug::module_compression, "r_Tile_Comp_None", "compress()" );

    size = get_tile_size();
    if ((result = mystore.storage_alloc(size)) != NULL)
        memcpy(result, src, get_tile_size());
    else
        RMInit::logOut << "r_Tile_Comp_None::compress(): unable to allocate memory" << endl;

    return result;
}


void *r_Tile_Comp_None::decompress( const void *src, r_ULong size, const char *options )
{
    void *result;

    RMDBGONCE( 2, RMDebug::module_compression, "r_Tile_Comp_None", "decompress()" );

    if ((result = mystore.storage_alloc(get_tile_size())) != NULL)
        memcpy(result, src, get_tile_size());
    else
        RMInit::logOut << "r_Tile_Comp_None::decompress(): unable to allocate memory" << endl;

    return result;
}


const char *r_Tile_Comp_None::get_name( void ) const
{
    return format_name_array;
}


r_Data_Format r_Tile_Comp_None::get_data_format( void ) const
{
    return r_Array;
}


r_Tile_Compression *r_Tile_Comp_None::clone( void ) const
{
    return new r_Tile_Comp_None(*this);
}
