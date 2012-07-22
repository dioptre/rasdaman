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
 * SOURCE: test_sinterval.cc
 *
 * MODULE: rasodmg
 *
 * COMMENTS:
 *      None
*/

#include <iostream>

#ifdef EARLY_TEMPLATE
#define __EXECUTABLE__
#include "raslib/template_inst.hh"
#endif

#include <iostream>
#include "rasodmg/oqlquery.hh"

int main()
{
    cout << endl << endl;
    cout << "r_OQL_Query Examples" << endl;
    cout << "====================" << endl << endl;


    // 1. Example

    cout << "1. Initializing with two string arguments." << endl;
    r_OQL_Query q1("select d from $1 where d.id = $2");

    try
    {
        q1 << "Images d" << "<id>";
    }
    catch( r_Error& err )
    {
        cout << err.what() << endl;
    }

    cout << "Parameterized query string : " << q1.get_parameterized_query() << endl;
    cout << "Query string               : " << q1.get_query() << endl << endl;


    // 2. Example

    cout << "2. Initializing with too many arguments." << endl;
    r_OQL_Query q2("select d from $1 where d.id = $2");

    try
    {
        q2 << "Images d" << "<id>" << "too many";
    }
    catch( r_Error& err )
    {
        cout << err.what() << endl;
    }

    cout << "Parameterized query string : " << q2.get_parameterized_query() << endl;
    cout << "Query string               : " << q2.get_query() << endl << endl;


    // 3. Example

    cout << "3. Initializing with different arguments." << endl;
    r_OQL_Query q3("select $1 from $2 $3 where d.id1 = $4 and d.id2 = $5");

    try
    {
        q3 << (unsigned char)'d' << "Images" << (unsigned char)'d' << (r_Long)100l << (r_Long)20l;
    }
    catch( r_Error& err )
    {
        cout << err.what() << endl;
    }

    cout << "Parameterized query string : " << q3.get_parameterized_query() << endl;
    cout << "Query string               : " << q3.get_query() << endl << endl;

    // 4. Example

    cout << "4. Initializing with different arguments." << endl;
    r_OQL_Query q4("select d$1 from $2 as d where some( d$3 > $4 )");

    try
    {
        q4 << ( r_Minterval(2) << r_Sinterval((r_Long)100l,(r_Long)199l) << r_Sinterval((r_Long)200l,(r_Long)299l) )
           << "Images"
           << ( r_Minterval(2) << r_Sinterval((r_Long)300l,(r_Long)399l) << r_Sinterval((r_Long)400l,(r_Long)499l) )
           << (r_Long)127l;
    }
    catch( r_Error& err )
    {
        cout << err.what() << endl;
    }

    cout << "Parameterized query string : " << q4.get_parameterized_query() << endl;
    cout << "Query string               : " << q4.get_query() << endl << endl;

    // 5. Example

    cout << "4. Initializing with different arguments." << endl;
    r_OQL_Query q5("update upd1 as a set a assign $1 *$2*3");

    try
    {
        r_GMarray mdd;

        cout << "Parameterized query string : " << q5.get_parameterized_query() << endl;
        cout << "Query string               : " << q5.get_query() << endl << endl;
        q5 << mdd;
        cout << "Parameterized query string : " << q5.get_parameterized_query() << endl;
        cout << "Query string               : " << q5.get_query() << endl << endl;
        q5 << mdd;
        cout << "Parameterized query string : " << q5.get_parameterized_query() << endl;
        cout << "Query string               : " << q5.get_query() << endl << endl;
    }
    catch( r_Error& err )
    {
        cout << err.what() << endl;
    }

    cout << "Parameterized query string : " << q5.get_parameterized_query() << endl;
    cout << "Query string               : " << q5.get_query() << endl << endl;
}










