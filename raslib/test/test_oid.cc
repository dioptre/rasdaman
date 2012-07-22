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
 * SOURCE: test_oid.cc
 *
 * MODULE: raslib
 *
 * PURPOSE:
 *
 * COMMENTS:
 *
 ************************************************************/

#include <iostream>
#include <iomanip>
// #include <limits.h>

#include "raslib/oid.hh"
#include "raslib/rminit.hh"
RMINITGLOBALS('C')

using namespace std;

int main()
{
    cout << endl << endl;
    cout << "OId Examples" << endl;
    cout << "============" << endl << endl;

    cout << "Create oid1" << endl;
    r_OId oid1( "testSystem", "testBase", 99 );
    cout << "  oid1........: " << oid1 << endl;
    cout << "   system name: " << oid1.get_system_name() << endl;
    cout << "   base   name: " << oid1.get_base_name()   << endl;
    cout << "   local oid..: " << oid1.get_local_oid()   << endl << endl;

    cout << "Create oid2 with string representation of oid1" << endl;
    r_OId oid2( oid1.get_string_representation() );
    cout << "  oid2........: " << oid2 << endl;
    cout << "   system name: " << oid2.get_system_name() << endl;
    cout << "   base   name: " << oid2.get_base_name()   << endl;
    cout << "   local oid..: " << oid2.get_local_oid()   << endl << endl;

    cout << "Assign oid1 to oid2" << endl;
    oid2 = oid1;
    cout << "  oid2........: " << oid2 << endl;
    cout << "   system name: " << oid2.get_system_name() << endl;
    cout << "   base   name: " << oid2.get_base_name()   << endl;
    cout << "   local oid..: " << oid2.get_local_oid()   << endl << endl;

    cout << "Assign temporary oid to oid2" << endl;
    oid2 = r_OId( "testSystem|testBase|100" );
    cout << "  oid2........: " << oid2 << endl;
    cout << "   system name: " << oid2.get_system_name() << endl;
    cout << "   base   name: " << oid2.get_base_name()   << endl;
    cout << "   local oid..: " << oid2.get_local_oid()   << endl << endl;

    cout << "Assign oid1 to oid1" << endl;
    oid1 = oid1;
    cout << "  oid1........: " << oid1 << endl;
    cout << "   system name: " << oid1.get_system_name() << endl;
    cout << "   base   name: " << oid1.get_base_name()   << endl;
    cout << "   local oid..: " << oid1.get_local_oid()   << endl << endl;

    cout << "Create oid3 as a copy of oid1 with the copy constructor" << endl;
    r_OId oid3( oid1 );
    cout << "  oid3........: " << oid3 << endl;
    cout << "   system name: " << oid3.get_system_name() << endl;
    cout << "   base   name: " << oid3.get_base_name()   << endl;
    cout << "   local oid..: " << oid3.get_local_oid()   << endl << endl;

    cout << "Create oid4 with string testSystem|testBase|100" << endl;
    r_OId oid4( "testSystem|testBase|100" );
    cout << "  oid4........: " << oid4 << endl;
    cout << "   system name: " << oid4.get_system_name() << endl;
    cout << "   base   name: " << oid4.get_base_name()   << endl;
    cout << "   local oid..: " << oid4.get_local_oid()   << endl << endl;

    cout << "Create oid5 with string |testBase|100" << endl;
    r_OId oid5( "|testBase|100" );
    cout << "  oid5........: " << oid5 << endl;
    cout << "   system name: " << oid5.get_system_name() << endl;
    cout << "   base   name: " << oid5.get_base_name()   << endl;
    cout << "   local oid..: " << oid5.get_local_oid()   << endl << endl;

    cout << "Create oid6 with string ||100" << endl;
    r_OId oid6( "||100" );
    cout << "  oid6........: " << oid6 << endl;
    cout << "   system name: " << oid6.get_system_name() << endl;
    cout << "   base   name: " << oid6.get_base_name()   << endl;
    cout << "   local oid..: " << oid6.get_local_oid()   << endl << endl;

    cout << "Create oid7 with string |||" << endl;
    r_OId oid7( "|||" );
    cout << "  oid7........: " << oid7 << endl;
    cout << "   system name: " << oid7.get_system_name() << endl;
    cout << "   base   name: " << oid7.get_base_name()   << endl;
    cout << "   local oid..: " << oid7.get_local_oid()   << endl << endl;

    cout << "Create oid8 with an empty string" << endl;
    r_OId oid8( "" );
    cout << "  oid8........: " << oid8 << endl;
    cout << "   system name: " << oid8.get_system_name() << endl;
    cout << "   base   name: " << oid8.get_base_name()   << endl;
    cout << "   local oid..: " << oid8.get_local_oid()   << endl << endl;

    cout << "Create oid9 with just a local oid" << endl;
    r_OId oid9( 0, 0, 100 );
    cout << "  oid8........: " << oid9 << endl;
    cout << "   system name: " << oid9.get_system_name() << endl;
    cout << "   base   name: " << oid9.get_base_name()   << endl;
    cout << "   local oid..: " << oid9.get_local_oid()   << endl << endl;

    cout << "Create oid10 with copy constructor from oid1" << endl;
    r_OId oid10( oid1 );
    cout << "  oid10.......: " << oid10 << endl;
    cout << "   system name: " << oid10.get_system_name() << endl;
    cout << "   base   name: " << oid10.get_base_name()   << endl;
    cout << "   local oid..: " << oid10.get_local_oid()   << endl << endl;

    cout << "Compare r_OId( \"testSystem1|testBase|99\") < r_OId( \"testSystem2|testBase|99\")" << endl;
    cout << (r_OId( "testSystem1|testBase|99") < r_OId( "testSystem2|testBase|99")) << endl << endl;

    cout << "Compare r_OId( \"testSystem|testBase1|99\") < r_OId( \"testSystem|testBase2|99\")" << endl;
    cout << (r_OId( "testSystem|testBase1|99") < r_OId( "testSystem|testBase2|99")) << endl << endl;

    cout << "Compare r_OId( \"testSystem|testBase|99\") < r_OId( \"testSystem|testBase|100\")" << endl;
    cout << (r_OId( "testSystem1|testBase|99") < r_OId( "testSystem2|testBase|100")) << endl << endl;

    cout << "Compare r_OId( \"testSystem1|testBase|99\") > r_OId( \"testSystem2|testBase|99\")" << endl;
    cout << (r_OId( "testSystem1|testBase|99") > r_OId( "testSystem2|testBase|99")) << endl << endl;

    cout << "Compare r_OId( \"testSystem|testBase1|99\") > r_OId( \"testSystem|testBase2|99\")" << endl;
    cout << (r_OId( "testSystem|testBase1|99") > r_OId( "testSystem|testBase2|99")) << endl << endl;

    cout << "Compare r_OId( \"testSystem|testBase|99\") > r_OId( \"testSystem|testBase|100\")" << endl;
    cout << (r_OId( "testSystem1|testBase|99") > r_OId( "testSystem2|testBase|100")) << endl << endl;

    cout << "Create oid11" << endl;
    // cout << "Double limit: " << DBL_MAX << "= 0x" << hex << DBL_MAX << dec << endl;
    double maxDouble = 0xffffffffffff;
    cout << "48bit       : " << setprecision(30) << maxDouble << endl;
    cout << "48bit - 1   : " << setprecision(30) << maxDouble-1 << endl;
    cout << "48bit + 1   : " << setprecision(30) << maxDouble+1 << endl;
    r_OId oid11( "testSystem", "testBase", maxDouble );
    cout << "  oid11.......: " << oid11 << endl;
    cout << "   system name: " << oid11.get_system_name() << endl;
    cout << "   base   name: " << oid11.get_base_name()   << endl;
    cout << "   local oid..: " << oid11.get_local_oid()   << endl << endl;

    cout << endl;

    cout << "Create oid12 with string ''" << endl;
    r_OId oid12( "" );
    cout << "  oid12.......: " << oid12 << endl;
    cout << "   system name: " << oid12.get_system_name() << endl;
    cout << "   base   name: " << oid12.get_base_name()   << endl;
    cout << "   local oid..: " << oid12.get_local_oid()   << endl << endl;

    return 0;
}

