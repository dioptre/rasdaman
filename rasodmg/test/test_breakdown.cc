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
 * SOURCE: test_breakdown.cc
 *
 * MODULE: rasodmg
 *
 * PURPOSE: Simulates a connection breakdown with an open database and a
 *          unfinished transaction.
 *
 * COMMENTS:
 *      None
*/

#ifdef EARLY_TEMPLATE
#define __EXECUTABLE__
#include "raslib/template_inst.hh"
#endif

#include <iostream>
#include <string.h>
#include <stdlib.h>     // for exit()

#include "rasodmg/ref.hh"
#include "rasodmg/transaction.hh"
#include "rasodmg/database.hh"
#include "rasodmg/set.hh"
#include "rasodmg/marray.hh"

int main( int ac, char** av )
{
    char servername[255];
    char baseName[255];
    char collName[255];

    if( ac>1 )
        strcpy( servername, av[1] );
    else
        strcpy( servername, "hpwibas5" );

    if( ac > 2 )
        strcpy( baseName, av[2] );
    else
        strcpy( baseName, "RasDaBase" );

    if( ac > 3 )
        strcpy( collName, av[3] );
    else
        strcpy( collName, "Images" );

    cout << endl << endl;
    cout << "ODMG conformant insertion of Marrays" << endl;
    cout << "====================================" << endl << endl;

    r_Database db;
    r_Transaction ta;
    r_Ref< r_Set< r_Ref< r_Marray<int> > > > image_set;
    r_Ref< r_Marray<int> >                   image;
    r_Minterval                              domain;

    domain = r_Minterval(2) << r_Sinterval( 0, 10 ) << r_Sinterval( 0, 10 );

    db.set_servername( servername );

    cout << "Opening Database on " << servername << "... ";
    cout.flush();
    db.open( baseName );
    cout << "OK" << endl;

    cout << "Starting Transaction ... ";
    cout.flush();
    ta.begin();
    cout << "OK" << endl;

    cout << "Creating a persistent set with two images ...";
    cout.flush();

    // create the set
    image_set = new( &db ) r_Set< r_Ref< r_Marray<int> > >;

    // create a name for the set
    db.set_object_name( *image_set, collName );

    // create first image
    image = new( &db ) r_Marray<int>( domain, 0 );

    // put in into the persistent list
    image_set->insert_element( image );

    //
    // *** POW! CONNECTION BREAKDOWN! ***
    //
    cout << endl << "*** POW! CONNECTION BREAKDOWN! ***" << endl;
    exit( 0 );
}
