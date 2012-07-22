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

/*************************************************************
 *
 * SOURCE: test_sinterval.cc
 *
 * MODULE: raslib
 *
 * PURPOSE:
 *
 * COMMENTS:
 *
 ************************************************************/

#include <iostream>
#include <stdlib.h>

#include "raslib/sinterval.hh"

#include "raslib/rminit.hh"
RMINITGLOBALS('C')

using namespace std;

int main()
{
    r_Sinterval interval1, interval2;

    char Buffer[256];

    cout << "lower bound ? ";
    cin >> Buffer;
    cout << endl;

    if( Buffer[0] == '*' )
        interval1.set_low('*');
    else
        interval1.set_low( atoi( Buffer ) );

    cout << "upper bound ? ";
    cin  >> Buffer;
    cout << endl;

    if( Buffer[0] == '*' )
        interval1.set_high('*');
    else
        interval1.set_high( atoi( Buffer ) );

    cout << "interval1 = [";
    interval1.print_status( cout );
    cout << "]" << endl << endl;

    cout << "lower bound ? ";
    cin  >> Buffer;
    cout << endl;

    if( Buffer[0] == '*' )
        interval2.set_low('*');
    else
        interval2.set_low( atoi( Buffer ) );

    cout << "upper bound ? ";
    cin  >> Buffer;
    cout << endl;

    if( Buffer[0] == '*' )
        interval2.set_high('*');
    else
        interval2.set_high( atoi( Buffer ) );

    cout << "interval1 = [";
    interval2.print_status( cout );
    cout << "]" << endl << endl;

    r_Sinterval result;

    try
    {
        result.union_of( interval1, interval2 );

        cout << "union = [";
        result.print_status( cout );
        cout << "]" << endl;
    }
    catch( r_Eno_interval error )
    {
        cerr << "union " << error.what() << endl;
    }

    try
    {
        result.difference_of( interval1, interval2 );

        cout << "difference = [";
        result.print_status( cout );
        cout << "]" << endl;
    }
    catch( r_Eno_interval error )
    {
        cerr << "difference " << error.what() << endl;
    }

    try
    {
        result.intersection_of( interval1, interval2 );

        cout << "intersection = [";
        result.print_status( cout );
        cout << "]" << endl;
    }
    catch( r_Eno_interval error )
    {
        cerr << "intersection " << error.what() << endl;
    }

    try
    {
        result.closure_of( interval1, interval2 );

        cout << "closure = [";
        result.print_status( cout );
        cout << "]" << endl;
    }
    catch( r_Eno_interval error )
    {
        cerr << "closure " << error.what() << endl;
    }

    if( interval1.intersects_with( interval2 ) )
        cerr << "The intervals intersect." << endl;
    else
        cerr << "The intervals do not intersect." << endl;

    cerr << endl;

    r_Sinterval b( 4, 10 );
    char* stringRep = b.get_string_representation();
    cout << endl << "String representation of interval " << b << ": " << stringRep << endl;
    cout << "Result of string constructor: " << r_Sinterval( stringRep ) << endl;
    free( stringRep );

    cout << "Result of r_Sinterval(\" 10 : 100\"): " << r_Sinterval(" 10 : 100") << endl;
    cout << "Result of r_Sinterval(\"  * : 100\"): " << r_Sinterval("  * : 100") << endl;
    cout << "Result of r_Sinterval(\" 10 : *  \"): " << r_Sinterval(" 10 : *  ") << endl;
    cout << "Result of r_Sinterval(\"  * : *  \"): " << r_Sinterval("  * : *  ") << endl;

    return 0;
}


