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
#include <malloc.h>


#ifdef EARLY_TEMPLATE
#define __EXECUTABLE__
#endif

#include "raslib/template_inst.hh"
#include "raslib/minterval.hh"
#include "raslib/sinterval.hh"

#include "raslib/rminit.hh"

using namespace std;

int main()
{
    cout << endl << endl;
    cout << "Minterval Examples" << endl;
    cout << "===================" << endl << endl;

    cout << "initializing interval [1:2,3:3] : " << endl;
    r_Minterval m1 = r_Minterval(2);
    m1 << r_Sinterval(1,2) << 3;
    cout << m1 << endl << endl;

    cout << "copy the interval with copy constructor" << endl;
    r_Minterval copy( m1 );
    cout << copy << endl << endl;

    cout << "test for equality: ";
    if( copy == m1 )
        cout << "OK" << endl << endl;
    else
        cout << "FAILED" << endl << endl;

    cout << "union of [1:4,3:6] and [3:6,1:4] :" << endl;
    r_Minterval m2 = r_Minterval(2) << r_Sinterval(1,4) << r_Sinterval(3,6);
    r_Minterval m3 = r_Minterval(2) << r_Sinterval(3,6) << r_Sinterval(1,4);
    try
    {
        m2 += m3;
    }
    catch( r_Error& ex1 )
    {
        cerr << ex1.what() << endl;
    }
    cout << m2 << endl << endl;

    cout << "union of [1:4,3:6] and [3:6,8:9] : " << endl;
    r_Minterval m4 = r_Minterval(2) << r_Sinterval(1,4) << r_Sinterval(3,6);
    r_Minterval m5 = r_Minterval(2) << r_Sinterval(3,6) << r_Sinterval(8,9);
    try
    {
        m4 += m5;
    }
    catch( r_Error& ex2 )
    {
        cerr << ex2.what() << endl;
    }
    cout << m4 << endl << endl;

    cout << "closure of [1:4,3:6] and [7:9,1:2] : " << endl;
    r_Minterval m6 = r_Minterval(2) << r_Sinterval(1,4) << r_Sinterval(3,6);
    r_Minterval m7 = r_Minterval(2) << r_Sinterval(7,9) << r_Sinterval(1,2);
    try
    {
        m6.closure_with( m7 );
    }
    catch( r_Error& ex2 )
    {
        cerr << ex2.what() << endl;
    }
    cout << m6 << endl << endl;

    cout << "intersection of [7:9,1:2] and [ " << m6[0].low() << ":" << m6[0].high() << "," << m6[1].low() << ":" << m6[1].high() << "]" << endl;
    cout << m7.intersects_with( m6 );
    cout << endl;

    cout << "initializing domain [1:4,*:6,7:*]" << endl;
    r_Minterval m8 = r_Minterval(3) << r_Sinterval(1,4) << r_Sinterval('*',6) << r_Sinterval(7,'*');
    char* stringRepresentation = m8.get_string_representation();
    cout << "string representation " << stringRepresentation << " should be " << m8 << endl;

    r_Minterval m9 = r_Minterval( stringRepresentation );
    cout << "recreation of the string delivers " << m9 << endl;
    free( stringRepresentation );

    return 0;
}
