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
 * exception testing (was: Systemtest_rasj).
 *
 *
 * COMMENTS:
 *
 * </pre>
 *********************************************************** */

import java.io.*;

import rasj.*;
import rasj.global.*;
import org.odmg.*;


// auxiliary classes for exception testing
class TestRasResultIsNoCellException extends RasResultIsNoCellException
  {
    TestRasResultIsNoCellException()
      {
	super();
      }
  }

class TestRasResultIsNoIntervalException extends RasResultIsNoIntervalException
  {
    TestRasResultIsNoIntervalException()
      {
	super();
      }
  }

class TestRasClientInternalException extends RasClientInternalException
  {
    TestRasClientInternalException(String msg)
      {
	super("TestException","TestExceptions",msg);
      }
  }

/**
 * class for testing rasj interface
 * @version $$
 */
public class TestException
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

    // main program
    public static void main(String[] args) throws Exception
      {
	String server = DEFAULT_HOST;
	int port      = DEFAULT_PORT;
        String base   = DEFAULT_BASE;
        String user   = DEFAULT_USER;
        String passwd = DEFAULT_PASSWD;
	boolean wrongUsage = false;

	// get args
	if(args.length == 0)
	    wrongUsage = true;
	for(int i = 0; i < args.length; i+=2)
          {
            if (args[i].equals("--server"))
                server = args[i+1];
            else if (args[i].equals("--port"))
              {
                try
                  {
                    port = Integer.parseInt(args[i+1]);
                  }
                catch(Exception e)
                  {
                    wrongUsage = true;
                  }
              }
            else if (args[i].equals("--database"))
                base = args[i+1];
            else if (args[i].equals("--user"))
                user = args[i+1];
            else if (args[i].equals("--passwd"))
                passwd = args[i+1];
          }

	if(wrongUsage)
	  {
            System.err.println("Usage: TestException [--server s] [--port p] [--database d]");
            System.out.println( "defaults: server=" + DEFAULT_HOST + ", port=" + DEFAULT_PORT + ", database=" + DEFAULT_BASE
+ ", user=" + DEFAULT_USER + ", passwd=" + DEFAULT_PASSWD );
            System.exit(-1);
	  }



        System.out.println( "rasdaman system test v5.1revC: testing exceptions." );
        System.out.println( PREFIX_PROGRAM + "system test started, using server " + server + ", port " + port + ", database " + base + ", user=" + user + ", passwd=" + passwd );

	// Implementation myApp = new RasImplementation("http://" + server + ":" + port);
        RasImplementation myApp = new RasImplementation( "http://" + server + ":" + port );
        myApp.setUserIdentification(user, passwd);

        // -- START test cases -------------------------------------------------
	testExceptions( myApp, base );
        // -- END test cases ---------------------------------------------------

        System.out.println( PREFIX_PROGRAM + "system test done." );
        return;
      } // main()

    /**
    * test exceptions
    * any eventual exception that is not caught here is an error, and will cause an abort
    **/
    static void testExceptions( RasImplementation imp, String database ) throws Exception
      {
        boolean resultIsOk = false;
        Database myDb = null;
        Transaction myTa = null;
        OQLQuery myQu = null;

        System.out.println( PREFIX_TESTSET + "testing exceptions started." );

        System.out.print( PREFIX_TESTCASE + "provoking RasConnectionFailedException..." );
        RasImplementation errorApp;
        String logMsg;
        try
          {
            errorApp = new RasImplementation("wrongserver:8080");
            myDb = errorApp.newDatabase();
            myDb.open( database, Database.OPEN_READ_WRITE );
            System.out.println( ERROR_TAG + "does not catch this." );
            myDb.close();
          }
        catch(RasConnectionFailedException e)
          {
            System.out.println( "OK, recognized: " + e.getMessage() );
          }

        System.out.print( PREFIX_TESTCASE + "provoking RasDimensionMismatchException..." );
        try
          {
            RasMInterval test1;
            RasMInterval test2 = new RasMInterval("[1:20,1:20]");
            RasMInterval test3 = new RasMInterval("[1:4,1:9,1:6]");
            test1 = test2.intersectionWith(test3);
            System.out.println( ERROR_TAG + "does not catch this." );
          }
        catch(RasDimensionMismatchException e)
          {
            System.out.println( "OK, recognized: " + e.getMessage() );
          }

        System.out.print( PREFIX_TESTCASE + "provoking RasIndexOutOfBoundsException..." );
        try
          {
            RasGMArray myMDD = new RasGMArray(new RasMInterval("[1:5,1:5]"),1);
            byte[] retVal = myMDD.getCell(new RasPoint(4,7));
            System.out.println( ERROR_TAG + "does not catch this." );
          }
        catch(RasIndexOutOfBoundsException e)
          {
            System.out.println( "OK, recognized: " + e.getMessage() );
          }

        System.out.print( PREFIX_TESTCASE + "provoking RasClientInternalException (via subclassed e.)..." );
        try
          {
            throw new TestRasClientInternalException("This is a test error");
          }
        catch(RasClientInternalException e)
          {
            System.out.println( "OK, recognized: " + e.getMessage() );
          }

        System.out.print( PREFIX_TESTCASE + "provoking RasResultIsNoCellException (via subclassed e.)..." );
        try
          {
            throw new TestRasResultIsNoCellException();
          }
        catch(RasResultIsNoCellException e)
          {
            System.out.println( "OK, recognized: " + e.getMessage() );
          }

        System.out.print( PREFIX_TESTCASE + "provoking RasResultIsNoIntervalException (via subclassed e.)..." );
        try
          {
            throw new TestRasResultIsNoIntervalException();
          }
        catch(RasResultIsNoIntervalException e)
          {
            System.out.println( "OK, recognized: " + e.getMessage() );
          }

        System.out.print( PREFIX_TESTCASE + "provoking RasStreamInputOverflowException..." );
        try
          {
            RasMInterval int1 = new RasMInterval(2);
            int1.stream(new RasSInterval("1:10"));
            int1.stream(new RasSInterval("1:5"));
            int1.stream(new RasSInterval("1:20"));
            System.out.println( ERROR_TAG + "does not catch this." );
          }
        catch(RasStreamInputOverflowException e)
          {
            System.out.println( "OK, recognized: " + e.getMessage() );
          }

        System.out.print( PREFIX_TESTCASE + "provoking RasTypeInvalidException..." );
        try
          {
            RasPrimitiveType myType = new RasPrimitiveType("myPrivateType",RasGlobalDefs.RAS_BOOLEAN);
            myType.getFloat(new Object());
            System.out.println( ERROR_TAG + "does not catch this." );
          }
        catch(RasTypeInvalidException e)
          {
            System.out.println( "OK, recognized: " + e.getMessage() );
          }

        System.out.print( PREFIX_TESTCASE + "provoking RasTypeNotSupportedException..." );
        try
          {
            throw new RasTypeNotSupportedException("test");
          }
        catch(RasTypeNotSupportedException e)
          {
            System.out.println( "OK, recognized: " + e.getMessage() );
          }

        System.out.print( PREFIX_TESTCASE + "provoking RasIllegalULongValueException (faked)..." );
        try
          {
            throw new RasIllegalULongValueException( 42 );
          }
        catch(RasIllegalULongValueException e)
          {
            System.out.println( "OK, recognized: " + e.getMessage() );
          }

        System.out.print( PREFIX_TESTCASE + "provoking RasIllegalUShortValueException (faked)..." );
        try
          {
            throw new RasIllegalUShortValueException( 42 );
          }
        catch(RasIllegalUShortValueException e)
          {
            System.out.println( "OK, recognized: " + e.getMessage() );
          }

        System.out.print( PREFIX_TESTCASE + "provoking RasInvalidNameException (illegal chars in obj type name)..." );
        try
          {
            RasGMArray myMDD = new RasGMArray(new RasMInterval("[1:400,1:400]"),1);
            myMDD.setObjectTypeName("_!_$_%_&_ _");
            System.out.println( ERROR_TAG + "does not catch this." );
          }
        catch(RasInvalidNameException e)
          {
            System.out.println( "OK, recognized: " + e.getMessage() );
          }

        // FIXME: PB9
        System.out.print( PREFIX_TESTCASE + "provoking RasInvalidNameException (name too long)..." );
        try
          {
            RasGMArray myMDD = new RasGMArray(new RasMInterval("[1:400,1:400]"),1);
            myMDD.setObjectTypeName("loooongname__1234567890_1234567890_1234567890_1234567890_1234567890_1234567890_1234567890_1234567890_1234567890_1234567890_1234567890_1234567890_1234567890_1234567890_1234567890_1234567890_1234567890_1234567890_1234567890_1234567890_" );
            System.out.println( ERROR_TAG + "does not catch this." );
          }
        catch(RasInvalidNameException e)
          {
            System.out.println( "OK, recognized: " + e.getMessage() );
          }

        System.out.print( PREFIX_TESTCASE + "provoking RasQueryExecutionFailedException..." );
        try
          {
            // Database myDb2 = imp.newDatabase();
            // myDb2.open( database, Database.OPEN_READ_ONLY );
            // Transaction myTa2 = imp.newTransaction();
            // myTa2.begin();

            // OQLQuery myQu2 = imp.newOQLQuery();
            // myQu2.create( "select r r from RAS_COLLECTIONNAMES as r" );
            // DBag result = (DBag) myQu2.execute();

            // myTa2.abort();
            // myDb2.close();
            System.out.print( "this is meant for a severe internal problem which I cannot provoke; will fake..." );
            throw new RasQueryExecutionFailedException( 1, 2, 3, "(fake)" );
          }
        catch(RasQueryExecutionFailedException e)
          {
            System.out.println( "OK, recognized: " + e.getMessage() );
          }

	// protected exception in rasj, not accessible
        // System.out.print( PREFIX_TESTCASE + "provoking RasTypeUnknownException..." );
        // try
          // {
            // // RasGMArray myMDD = new RasGMArray(new RasMInterval("[1:400,1:400]"),1);
            // // myMDD.setObjectTypeName( "charrrrr" );
            // System.out.print( "superseded by RasTypeNotSupportedException - cannot catch it; will fake..." );
            // throw new RasTypeUnknownException("test");
          // }
        // catch(RasTypeUnknownException e)
          // {
            // System.out.println( "OK, recognized: " + e.getMessage() );
          // }

        System.out.println( PREFIX_TESTSET + "testing exceptions done." );
        return;
      } // testExceptions()

  } // TestException
