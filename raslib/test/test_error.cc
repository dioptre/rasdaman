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
 * SOURCE: test_error.cc
 *
 * MODULE: raslib
 *
 * PURPOSE:
 *
 * Test program to test class r_Error and its subclasses.
 *
 *
 * COMMENTS:
 *
 ************************************************************/

#include <iostream>
#include <malloc.h>
#include "raslib/error.hh"

#include "raslib/rminit.hh"

RMINITGLOBALS('C')

using namespace std;

int main()
{
    cout << endl << "0. reading from errortxts table ----------------------------------" << endl;
    r_Error::initTextTable();
    try
    {
        throw r_Error( 10000 );
    }
    catch( r_Error &err )
    {
        cout << err.what() << endl;
    }

    cout << endl << "1. Throwing r_Error()  -------------------------------------------" << endl;
    try
    {
        throw r_Error();
    }
    catch( r_Error &err )
    {
        cout << err.what() << endl;
    }

    cout << endl << "2. Throwing r_Error( r_Error::r_Error_DatabaseUnknown )  ---------" << endl;
    try
    {
        throw r_Error( r_Error::r_Error_DatabaseUnknown );
    }
    catch( r_Error &err )
    {
        cout << err.what() << endl;
    }

    cout << endl << "3. Throwing r_Eno_interval()  ------------------------------------" << endl;
    try
    {
        throw r_Eno_interval();
    }
    catch( r_Error &err )
    {
        cout << err.what() << endl;
    }

    cout << endl << "4. Throwing r_Eindex_violation( 10, 20, 25 )  --------------------" << endl;
    try
    {
        throw r_Eindex_violation( 10, 20, 25 );
    }
    catch( r_Error &err )
    {
        cout << err.what() << endl;
    }


    cout << endl << "5. Throwing r_Edim_mismatch( 2, 3 )  -----------------------------" << endl;
    try
    {
        throw r_Edim_mismatch( 2, 3 );
    }
    catch( r_Error &err )
    {
        cout << err.what() << endl;
    }


    cout << endl << "6. Throwing r_Einit_overflow()  ----------------------------------" << endl;
    try
    {
        throw r_Einit_overflow();
    }
    catch( r_Error &err )
    {
        cout << err.what() << endl;
    }


    cout << endl << "7. Throwing r_Eno_cell()  ----------------------------------------" << endl;
    try
    {
        throw r_Eno_cell();
    }
    catch( r_Error &err )
    {
        cout << err.what() << endl;
    }

    cout << endl << "8. Throwing r_Equery_execution_failed( 99, 5, 7, 'SELECT' )  -----" << endl;
    try
    {
        throw r_Equery_execution_failed( 99, 5, 7, "SELECT" );
    }
    catch( r_Error &err )
    {
        cout << err.what() << endl;
    }

    cout << endl << "8. Testing r_Ebase_dbms ------------------------------------------" << endl;
    cout << endl << "8.1 Throwing (4711, 'This is a test') ----------------------------" << endl;
    try
    {
        throw r_Ebase_dbms( 4711, "This is a test" );
    }
    catch( r_Error &err )
    {
        cout << "Output of what() catching an r_Error &err" << endl;
        cout << err.what() << endl;
    }
    try
    {
        throw r_Ebase_dbms( 4711, "This is a test" );
    }
    catch( r_Ebase_dbms &err )
    {
        cout << "Output of what() catching an r_Ebase_dbms &err" << endl;
        cout << err.what() << endl;
    }

    cout << endl << "8.2 Testing serialisation of r_Ebase_dbms ------------------------" << endl;
    char* serialErr;
    try
    {
        throw r_Ebase_dbms( 4711, "This is a test" );
    }
    catch( r_Error &err )
    {
        serialErr = err.serialiseError();
        cout << "serialised form: " << serialErr << endl;
    }
    try
    {
        cout << "Throwing error constructed from serialised form." << endl;
        r_Error* testErr = r_Error::getAnyError(serialErr);
        // for some strange reason a simple throw(*testErr) does not work here. It does not work
        // if an r_Ebase_dbms is caught (core dump), it works if an r_Error is caught. Hmm, makes
        // some sense since *r_Error is not polymorphic anymore. But it should not core dump.
        // Well strange, but like this it works and this will be done only once in clientcomm.cc.
        if(testErr->get_errorno () == 206)
        {
            r_Ebase_dbms correctErr(*(r_Ebase_dbms*)testErr);
            delete testErr;
            throw correctErr;
        }
        else
            cout << "Unexpected error read from serialised representation." << endl;
    }
    catch( r_Ebase_dbms &err )
    {
        cout << "Output of what() catching an r_Ebase_dbms &err" << endl;
        cout << err.what() << endl;
    }
    try
    {
        cout << "Throwing error constructed from serialised form." << endl;
        r_Error* testErr = r_Error::getAnyError(serialErr);
        if(testErr->get_errorno () == 206)
        {
            r_Ebase_dbms correctErr(*(r_Ebase_dbms*)testErr);
            delete testErr;
            throw correctErr;
        }
        else
            cout << "Unexpected error read from serialised representation." << endl;
    }
    catch( r_Error &err )
    {
        cout << "Output of what() catching an r_Error &err" << endl;
        cout << err.what() << endl;
    }
    cout << "Done" << endl;
    free(serialErr);

    cout << endl;

    cout << "freeing error text table...";
    r_Error::freeTextTable();
    cout << "ok." << endl;

    cout << endl << "------------------------------------------------------------------" << endl;

    return 0;
}
