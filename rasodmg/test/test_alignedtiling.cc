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
 * SOURCE: test_alignedtiling.cc
 *
 * MODULE: rasodmg
 *
 * COMMENTS:
 *      None
*/

#include <iostream>
#include <string.h>

#ifdef __VISUALC__
#define __EXECUTABLE__
#endif
// #include "clientcomm/clientcomm.hh"
#include "rasodmg/ref.hh"
#include "rasodmg/transaction.hh"
#include "rasodmg/database.hh"
#include "rasodmg/set.hh"
#include "rasodmg/marray.hh"
#include "rasodmg/odmgtypes.hh"
#include "raslib/minterval.hh"
#include "rasodmg/alignedtiling.hh"
#ifdef __VISUALC__
#undef  __EXECUTABLE__
#endif


int checkArguments( int argc, char** argv, const char* searchText, int& optionValueIndex )
{
    int found = 0;
    int i=1;

    while( !found && i<argc )
        found = !strcmp( searchText, argv[i++] );

    if( found && i<argc && !strchr(argv[i],'-') )
        optionValueIndex = i;
    else
        optionValueIndex = 0;

    return found;
}


int main( int argc, char** argv )
{

    int  optionValueIndex;
    if( argc < 5 || checkArguments( argc, argv, "-h", optionValueIndex ) )
    {
        cout << "Usage:   test_alignedtiling tile_config tile_size  cell_size domain" << endl << endl;
        cout << "Options: -h  ... this help" << endl;
        cout << endl;
        return 0;
    }

    r_Minterval* tile_config = 0;
    r_Minterval* domain = 0;

    try
    {
        tile_config = new r_Minterval( argv[1] );
        domain = new r_Minterval( argv[4] );
    }
    catch (...)
    {
        return -1;
    }

    unsigned cell_size = strtoul( argv[3], (char **)NULL, 10 );
    unsigned long tile_size = strtoul( argv[2],  (char **)NULL, 10 );

    cout << "Tile Config " << *tile_config << endl;
    r_Aligned_Tiling storeOptions( *tile_config, tile_size );

    cout << endl;

    cout << "Tiling Options : ts - " << storeOptions.get_tile_size( )
         << ", tc - " << storeOptions.get_tile_config( ) << endl;

    cout << "Object domain : " << *domain << ", cell size " << cell_size << endl;


    r_Aligned_Tiling newSL(storeOptions);

    cout << "Testing copy constructor. Newly constructed object..." << endl;
    cout << "Tiling Options : ts - " << storeOptions.get_tile_size( )
         << ", tc - " << storeOptions.get_tile_config( ) << endl;

    cout << "Object domain : " << *domain << ", cell size " << cell_size << endl;

    r_Minterval result_tile =
        storeOptions.compute_tile_domain(cell_size, *domain);

    cout << "Tiling Options resulting tile :" << result_tile << endl;


    delete domain;
    delete tile_config;

    return 0;
}

