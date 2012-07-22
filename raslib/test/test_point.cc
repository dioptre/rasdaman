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
 * SOURCE: test_point.cc
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

#include "raslib/point.hh"

#include "raslib/rminit.hh"
RMINITGLOBALS('C')

using namespace std;

int main()
{
    cout << endl << endl;
    cout << "Point Examples" << endl;
    cout << "===============" << endl << endl;

    cout << "initialize 1D-point: " << flush;
    r_Point d1(1);
    cout << d1 << endl;

    cout <<  "set value to 9 : " << flush;
    d1[0] = 9;
    cout << d1 << endl;

    r_Point a(3);
    a << 1 << 2 << 3;
    cout << "stream initializing point (1,2,3) : " << a << endl << endl;

    try
    {
        cout << "read out of range : " << a[3] << endl;
    }
    catch( r_Eindex_violation& ex )
    {
        cout << ex.what();
    }
    cout << endl << endl;

    r_Point b = r_Point(3) << 1 << 2 << 3;
    cout << "assignment point (1,2,3) : " << b << endl << endl;

    cout << "temp. obj. (1,2,3), access second coord. : " << ( r_Point(3) << 1 << 2 << 3 )[1] << endl << endl;

    a[1] = 4;
    cout << "assignment a[1]=4 to a(1,2,3) : " << a << endl << endl;

    r_Point c = b;
    cout << "equal operator test: " << flush;
    if( c == b )
        cout << "OK" << endl;
    else
        cout << "FALSE" << endl;

    char* stringRep = b.get_string_representation();
    cout << endl << "String representation of point " << b << ": " << stringRep << endl;
    cout << "Result of string constructor: " << r_Point( stringRep ) << endl;
    free( stringRep );

    cout << "Result of r_Point(\"[ 0, 5, 3]\"): " << r_Point("[ 0, 5, 3]") << endl;
}
