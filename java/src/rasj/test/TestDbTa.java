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
*/
/** ***********************************************************
 * <pre>
 *
 * PURPOSE:
 * test rasj ODMG-level interface (database, TA, and basic query logic).
 *  TO BE DONE!
 *
 * PREREQUISITES:
 * - Must have a database with at least one HTTP server running.
 * - database must know basic types, in particular: GreySet.
 * - login must allow read/write access to database
 *
 *
 * COMMENTS:
 * - testing philosophy is to run all tests and report. no exit code / exception
 *   is used for error indication; look into the output for ERROR_TAG instead.
 * - unexpected exceptions are not caught, hence will make gmake fail and show the error
 * - every test that fails must print a line containing ERROR_TAG
 * </pre>
 *********************************************************** */

import java.lang.Integer;
import org.odmg.*;
import rasj.*;
import rasj.odmg.*;

public class TestDbTa
  {
    /**
    * prefixes for test output
    **/
    static final String PREFIX_PROGRAM  = "+++ +++ +++ ";
    static final String PREFIX_TESTSET  = "+++ +++ ";
    static final String PREFIX_TESTCASE = "+++ ";

    /**
    * std error tag printed if a test fails
    **/
    static final String ERROR_TAG = "ERROR: ";

    /**
    * default values (override with cmd line option)
    **/
    static final String DEFAULT_HOST   = "localhost";
    static final int    DEFAULT_PORT   = 7001;
    static final String DEFAULT_BASE   = "RASBASE";
    static final String DEFAULT_USER   = "rasguest";
    static final String DEFAULT_PASSWD = "rasguest";

    /**
    * global ODMG implementation object for use by all methods
    **/
    static RasImplementation myApp = null;

    /**
    * main program for testing
    * on error, an exception is thrown (java main() knows no exit status)
    **/
    public static void main(String argv[]) throws Exception
      {
        String serv   = DEFAULT_HOST;
        int port      = DEFAULT_PORT;
        String base   = DEFAULT_BASE;
        String user   = DEFAULT_USER;
        String passwd = DEFAULT_PASSWD;
        boolean wrongUsage = false;                 // error in cmd line params?

        for (int i=argv.length-1; i>=0; i--)
          {
            if (argv[i].equals("--server"))
                serv = argv[i+1];
            else if (argv[i].equals("--port"))
              {
                try
                  {
                    port = Integer.parseInt(argv[i+1]);
                  }
                catch(Exception e)
                  {
                    wrongUsage = true;
                  }
              }
            else if (argv[i].equals("--database"))
                base = argv[i+1];
            else if (argv[i].equals("--user"))
                user = argv[i+1];
            else if (argv[i].equals("--passwd"))
                passwd = argv[i+1];
          }

        if (wrongUsage)
          {
            System.out.println( "Usage: TestDbTa [--server s] [--port p] [--database d] [--user u] [--passwd p]" );
            System.out.println( "defaults: server=" + DEFAULT_HOST + ", port=" + DEFAULT_PORT + ", database=" + DEFAULT_BASE + ", user=" + DEFAULT_USER + ", passwd=" + DEFAULT_PASSWD );
            System.exit(-1);
          }

        System.out.println( "rasdaman system test v5.1revC: testing class ODMG logic." );
        System.out.println( PREFIX_PROGRAM + "system test started, using server " + serv + ", port " + port + ", database " + base + ", user=" + user + ", passwd=" + passwd );

        myApp = new RasImplementation( "http://" + serv + ":" + port );
        myApp.setUserIdentification(user, passwd);

        // -- START test cases -------------------------------------------------
        testDatabase( myApp, base );
        testTransaction( myApp, base );
        testAccessMode( myApp, base );
        // -- END test cases ---------------------------------------------------

        System.out.println( PREFIX_PROGRAM + "system test done." );
        return;
      } // main()

    /**
    * test database open/close
    * any eventual exception that is not caught here is an error, and will cause an abort
    **/
    static void testDatabase( RasImplementation myApp, String database )
        // throws RasDimensionMismatchException, RasIndexOutOfBoundsException
      {
        Database myDb = null;
        System.out.println( PREFIX_TESTSET + "testing database open/close started." );

        System.out.print( PREFIX_TESTCASE + "open db, null name..." );
        try
          {
            myDb = myApp.newDatabase();
            myDb.open( null, Database.OPEN_READ_ONLY );
            System.out.println( ERROR_TAG + "does not catch this." );
          }
        catch(Exception e)
          {
            System.out.println( "OK, recognized: " + e.getMessage() );
          }

        // FIXME: PB6: inappropriate error message
        System.out.print( PREFIX_TESTCASE + "open db, empty name..." );
        try
          {
            myDb = myApp.newDatabase();
            myDb.open( "", Database.OPEN_READ_ONLY );
            System.out.println( ERROR_TAG + "does not catch this." );
          }
        catch(Exception e)
          {
            System.out.println( "OK, recognized: " + e.getMessage() );
          }

        System.out.print( PREFIX_TESTCASE + "open db, nonexisting db..." );
        try
          {
            myDb = myApp.newDatabase();
            myDb.open( "NO_SUCH_DATABASE", Database.OPEN_READ_ONLY );
            System.out.println( ERROR_TAG + "does not catch this." );
          }
        catch(Exception e)
          {
            System.out.println( "OK, recognized: " + e.getMessage() );
          }

        // FIXME: PB6: inappropriate error message
        // System.out.print( PREFIX_TESTCASE + "open db, negative accessMode value..." );
        // try
          // {
            // myDb = myApp.newDatabase();
            // myDb.open( database, -1 );
            // System.out.println( ERROR_TAG + "does not catch this." );
          // }
        // catch(Exception e)
          // {
            // System.out.println( "OK, recognized: " + e.getMessage() );
          // }

        // FIXME: PB6: inappropriate error message
        // System.out.print( PREFIX_TESTCASE + "open db, illegal accessMode..." );
        // try
          // {
            // myDb = myApp.newDatabase();
            // myDb.open( "NO_SUCH_DATABASE", 100 );
            // System.out.println( ERROR_TAG + "does not catch this." );
          // }
        // catch(Exception e)
          // {
            // System.out.println( "OK, recognized: " + e.getMessage() );
          // }

        System.out.print( PREFIX_TESTCASE + "open db, good call..." );
        try
          {
            myDb = myApp.newDatabase();
            myDb.open( database, Database.OPEN_READ_ONLY );
            Transaction myTa = myApp.newTransaction();
            myTa.begin();
            myTa.abort();
            System.out.println( "OK, tested through beginTA." );
          }
        catch(Exception e)
          {
            System.out.println( ERROR_TAG + "good call does not open db: " + e.getMessage() );
          }

        System.out.print( PREFIX_TESTCASE + "close previously opened db..." );
        try
          {
            myDb.close();
            System.out.println( "OK." );
          }
        catch (Exception e)
          {
            System.out.println( ERROR_TAG + e.getMessage() );
          }

        System.out.print( PREFIX_TESTCASE + "can we open a transaction after closing db..." );
        try
          {
            Transaction myTa = myApp.newTransaction();
            myTa.begin();
            System.out.println( ERROR_TAG + "unfortunately yes." );
          }
        catch(Exception e)
          {
            System.out.println( "OK, we can't." );
          }

        System.out.print( PREFIX_TESTCASE + "close db already closed..." );
        try
          {
            myDb.close();
            System.out.println( ERROR_TAG + "repeated close possible, alas." );
          }
        catch (Exception e)
          {
            System.out.println( "OK." );
          }

        System.out.println( PREFIX_TESTSET + "testing database open/close done.\n" );
        return;
      } // testDatabase()

    /**
    * test transaction open/close
    * any eventual exception that is not caught here is an error, and will cause an abort
    **/
    static void testTransaction( RasImplementation myApp, String database ) throws ODMGException
      {
        Database myDb = null;
        Transaction myTa = null;
        System.out.println( PREFIX_TESTSET + "testing transaction start." );

        System.out.print( PREFIX_TESTCASE + "isOpen on open DB, closed TA..." );
        try
          {
            myDb = myApp.newDatabase();
            myDb.open( database, Database.OPEN_READ_ONLY );
            myTa = myApp.newTransaction();
            if ( myTa.isOpen() )
                System.out.println( ERROR_TAG + "TA open on closed DB." );
            else
                System.out.println( "OK, is not open." );
          }
        catch (Exception e)
          {
            System.out.println( "OK: " + e.getMessage() );
          }

        System.out.print( PREFIX_TESTCASE + "isOpen on open DB/TA..." );
        try
          {
            myTa.begin();
            if ( myTa.isOpen() )
                System.out.println( "OK, is open." );
            else
                System.out.println( ERROR_TAG + "TA not open after opening." );
          }
        catch (Exception e)
          {
            System.out.println( ERROR_TAG + e.getMessage() );
          }

        System.out.print( PREFIX_TESTCASE + "sending query to check open TA..." );
        try
          {
            OQLQuery myQu = myApp.newOQLQuery();
            myQu.create( "select r from RAS_COLLECTIONNAMES as r" );
            DBag result = (DBag) myQu.execute();
            if ( result == null )
                System.out.println( ERROR_TAG + "standard query failed." );
            else
                System.out.println( "OK, query done." );
          }
        catch (Exception e)
          {
            System.out.println( ERROR_TAG + e.getMessage() );
          }

        System.out.print( PREFIX_TESTCASE + "open TA on an already open TA..." );
        try
          {
            myTa.begin();
            if ( myTa.isOpen() )
                System.out.println( "OK, should remain open." );
            else
                System.out.println( ERROR_TAG + "TA not open after 2nd opening." );
          }
        catch (Exception e)
          {
            System.out.println( ERROR_TAG + e.getMessage() );
          }

        System.out.print( PREFIX_TESTCASE + "abort TA..." );
        try
          {
            myTa.abort();
            if ( myTa.isOpen() )
                System.out.println( ERROR_TAG + "TA open after abort." );
            else
                System.out.println( "OK, is not open." );
          }
        catch (Exception e)
          {
            System.out.println( ERROR_TAG + e.getMessage() );
          }

        System.out.print( PREFIX_TESTCASE + "query on aborted TA..." );
        try
          {
            OQLQuery myQu = myApp.newOQLQuery();
            myQu.create( "select r from RAS_COLLECTIONNAMES as r" );
            DBag result = (DBag) myQu.execute();
            if ( result == null )
                System.out.println( "OK, standard query failed." );
            else
                System.out.println( ERROR_TAG + "query got through." );
          }
        catch (Exception e)
          {
            System.out.println( "OK, failed: " + e.getMessage() );
          }

        System.out.print( PREFIX_TESTCASE + "abort TA on aborted TA..." );
        try
          {
            myTa.abort();
            if ( myTa.isOpen() )
                System.out.println( ERROR_TAG + "TA open after abort." );
            else
                System.out.println( "OK, is not open." );
          }
        catch (Exception e)
          {
            System.out.println( "OK, noticed: " + e.getMessage() );
          }

        System.out.print( PREFIX_TESTCASE + "open new TA, send query..." );
        try
          {
            myTa.begin();
            if ( ! myTa.isOpen() )
                System.out.println( ERROR_TAG + "TA not open." );
            else
              {
                OQLQuery myQu = myApp.newOQLQuery();
                myQu.create( "select r from RAS_COLLECTIONNAMES as r" );
                DBag result = (DBag) myQu.execute();
                if ( result == null )
                    System.out.println( ERROR_TAG + "standard query failed." );
                else
                    System.out.println( "OK." );
              }
          }
        catch (Exception e)
          {
            System.out.println( ERROR_TAG + e.getMessage() );
          }

        System.out.print( PREFIX_TESTCASE + "commit TA..." );
        try
          {
            myTa.commit();
            if ( myTa.isOpen() )
                System.out.println( "OK, closed now. " );
            else
                System.out.println( ERROR_TAG + "TA still open. " );
            OQLQuery myQu = myApp.newOQLQuery();
            myQu.create( "select r from RAS_COLLECTIONNAMES as r" );
            DBag result = (DBag) myQu.execute();
            if ( result == null )
                System.out.println( "OK, query failed." );
            else
                System.out.println( ERROR_TAG + "query got through." );
          }
        catch (TransactionNotInProgressException e)
          {
            System.out.println( "...OK, query failed: " + e.getMessage() );
          }
        catch (Exception e)
          {
            System.out.println( ERROR_TAG + e.getMessage() );
          }

        System.out.println( PREFIX_TESTSET + "testing transaction done.\n" );
        return;
      } // testTransaction()

    /**
    * test access modes
    * any eventual exception that is not caught here is an error, and will cause an abort
    **/
    static void testAccessMode( RasImplementation myApp, String database )
        // throws RasDimensionMismatchException, RasIndexOutOfBoundsException
      {
        Database myDb = null;
        Transaction myTa = null;
        System.out.println( PREFIX_TESTSET + "testing accessMode start." );

        System.out.print( PREFIX_TESTCASE + "open readonly, read query..." );
        try
          {
            myDb = myApp.newDatabase();
            myDb.open( database, Database.OPEN_READ_ONLY );
            myTa = myApp.newTransaction();
            myTa.begin();

            OQLQuery myQu = myApp.newOQLQuery();
            myQu.create( "select r from RAS_COLLECTIONNAMES as r" );
            DBag result = (DBag) myQu.execute();
            if ( result == null )
                System.out.println( ERROR_TAG + "query failed." );
            else
                System.out.println( "OK, query got through." );
          }
        catch (Exception e)
          {
            System.out.println( ERROR_TAG + e.getMessage() );
          }

        System.out.print( PREFIX_TESTCASE + "open readonly, update query (create collection)..." );
        try
          {
            myDb = myApp.newDatabase();
            myDb.open( database, Database.OPEN_READ_ONLY );
            myTa = myApp.newTransaction();
            myTa.begin();

            OQLQuery myQu = myApp.newOQLQuery();
            myQu.create( "create collection TestDbTa_collection GreySet" );
            DBag result = (DBag) myQu.execute();
          }
        catch (DatabaseIsReadOnlyException e)
          {
            System.out.println( "OK, update failed." );
          }
        catch (Exception e)
          {
            System.out.println( ERROR_TAG + e.getMessage() );
          }

        System.out.print( PREFIX_TESTCASE + "commit TA, close DB..." );
        try
          {
            myTa.commit();
            myDb.close();
            System.out.println( "OK." );
          }
        catch (Exception e)
          {
            System.out.println( ERROR_TAG + e.getMessage() );
          }

        System.out.print( PREFIX_TESTCASE + "open DB readwrite, open TA, read query..." );
        try
          {
            myDb.open( database, Database.OPEN_READ_WRITE );
            myTa = myApp.newTransaction();
            myTa.begin();

            OQLQuery myQu = myApp.newOQLQuery();
            myQu.create( "select r from RAS_COLLECTIONNAMES as r" );
            DBag result = (DBag) myQu.execute();
            if ( result == null )
                System.out.println( ERROR_TAG + "query failed." );
            else
                System.out.println( "OK, query got through." );
          }
        catch (Exception e)
          {
            System.out.println( ERROR_TAG + e.getMessage() );
          }

        System.out.print( PREFIX_TESTCASE + "update query (create & drop collection)..." );
        try
          {
            OQLQuery myQu = myApp.newOQLQuery();
            myQu.create( "create collection TestDbTa_collection GreySet" );
            DBag result = (DBag) myQu.execute();

            // we want to see that a commit really preserves changes:
            System.out.print( "re-opening TA..." );
            myTa.commit();
            myTa.begin();

            myQu.create( "drop collection TestDbTa_collection" );
            result = (DBag) myQu.execute();
            System.out.println( "OK, query got through." );
          }
        catch (Exception e)
          {
            System.out.println( ERROR_TAG + e.getMessage() );
          }

        System.out.print( PREFIX_TESTCASE + "commit TA, close DB..." );
        try
          {
            myTa.commit();
            myDb.close();
            System.out.println( "OK." );
          }
        catch (Exception e)
          {
            System.out.println( ERROR_TAG + e.getMessage() );
          }

        System.out.println( PREFIX_TESTSET + "testing accessMode done.\n" );
        return;
      } // testAccessMode()

  } // TestDbTa
