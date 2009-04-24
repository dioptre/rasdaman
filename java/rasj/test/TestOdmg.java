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
/*************************************************************
 *
 *
 * PURPOSE:
 * test rasj with some queries
 * @param --server s   - use server s (default: localhost)
 * @param --port p     - use server port p (default: 7001)
 * @param --database d - use database d (default: RASBASE)
 * @param --user u     - log in as user u (default: rasguest)
 * @param --passwd p   - log in with password p (default: rasguest)
 *
 * PREREQUISITES:
 * - needs an HTTP server
 * - needs write access to database
 * - database must know type GreySet
 *
 *
 * COMMENTS:
 * - no parameter line error handling
 * was formerly in rasj/odmg/test, this has been merged
 ************************************************************/

import rasj.*;
import rasj.global.*;
import org.odmg.*;
import rasj.odmg.*;

import java.util.*;
import java.io.*;

public class TestOdmg implements RasGlobalDefs
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

    public static void main(String[] args) throws Exception
      {

        String serv   = DEFAULT_HOST;
        int port      = DEFAULT_PORT;
        String base   = DEFAULT_BASE;
        String user   = DEFAULT_USER;
        String passwd = DEFAULT_PASSWD;
        boolean wrongUsage = false;                 // error in cmd line params?
        RasImplementation myImp = null;

        for (int i=args.length-1; i>=0; i--)
          {
            if (args[i].equals("--server"))
                serv = args[i+1];
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

        if (wrongUsage)
          {
            System.out.println( "Usage: ODMGtest [--server s] [--port p] [--database d] [--user u] [--passwd p]" );
            System.out.println( "defaults: server=" + DEFAULT_HOST + ", port=" + DEFAULT_PORT + ", database=" + DEFAULT_BASE
+ ", user=" + DEFAULT_USER + ", passwd=" + DEFAULT_PASSWD );
            System.exit(-1);
          }

        System.out.println( "rasdaman system test v5.1revC: testing ODMG queries." );
        System.out.println( PREFIX_PROGRAM + "system test started, using server " + serv + ", port " + port + ", database " + base + ", user=" + user + ", passwd=" + passwd );

	myImp = new RasImplementation("http://" + serv + ":" + port);

        // START tests ----------------------------------------------
        testQueries( myImp, base );
        // END tests ------------------------------------------------

        System.out.println( PREFIX_PROGRAM + "system test done." );
        return;
      } // main()

    /**
    * test database queries
    * any eventual exception that is not caught here is an error, and will cause an abort
    **/
    static void testQueries( RasImplementation imp, String database ) throws Exception
      {
        System.out.println( PREFIX_TESTSET + "testing queries started." );

	// objectTest(); // - just to test setObjectName() which now verifies that the name is a valid identifier
	
	Database mydb = null;
	Transaction myta = null;
	RasGMArray myMDD = null;
	OQLQuery myQuery = null;


	/*********************
	 * test update query *
	 *********************/
	// create some test data
	try
          {
	    System.out.println("Start creating test arrays ...");
	    myMDD = new RasGMArray(new RasMInterval("[1:400,1:400]"),1);
	    byte[] mydata = new byte[160000];
	    for(int y=0; y<400; y++)
		{
                 for(int x=0; x<400; x++)
		     {
			 if((x>99 && x<151) || (x>299 && x<351))
			     mydata[y*399+x]=100;
			 else
			     mydata[y*399+x]=0;
		     }
		}
            myMDD.setArray(mydata);
	    myMDD.setObjectTypeName("GreyImage");

	    RasStorageLayout myLayout = new RasStorageLayout();
	    //myLayout.setTileSize(640000);
	    myLayout.setTileDomain("[1:100,1:70]");
	    myMDD.setStorageLayout(myLayout);

	    System.out.println("Created Test Data:");
	    System.out.println(myMDD);
	}

	catch ( Exception e )
          {
	    System.out.println("ERROR: ");
	    System.out.println(e.getMessage());
	    System.exit(-1);
	}

	// now access the database and start a transaction
	try {
	    mydb = imp.newDatabase();
	    System.out.println("Open Database ...");
	    mydb.open( database, Database.OPEN_READ_WRITE );
	    System.out.println("     ok.");
	    myta = imp.newTransaction();
	    System.out.println("Begin transaction ...");
	    myta.begin();
	    System.out.println("     ok.");
	}

	catch ( Exception e ) {
	    System.out.println("ERROR: ");
	    System.out.println(e.getMessage());

	    if(myta.isOpen())
		{
		    System.out.println("Abort transaction ...");
		    myta.abort();
		}
	    System.out.println("Close database ...");
	    try {
		mydb.close();
		System.out.println("     ok.");
	    }
	    catch ( Exception e2 ) {
	    }
	    System.out.println("Exiting...");
	    System.exit(-1);
	}

	// create the test collection
	try {
	    System.out.println("Create a new test collection ...");
	    myQuery = imp.newOQLQuery();
	    myQuery.create("create collection testCollection GreySet");
	    //myQuery.create("create collection $1 GreySet");
	    //myQuery.bind("testCollection");
	    myQuery.execute();
	    myta.commit();
	    myta.begin();
	    System.out.println("     ok.");
	}

	catch ( Exception e) {
	    System.out.println("ERROR: ");
	    System.out.println(e.getMessage());
	    System.out.println("Try to remove the test collection ...");
	    try {
	    myQuery.create("drop collection testCollection");
	    myQuery.execute();
	    myta.commit();
	    }
	    catch ( Exception e2 ) {
	    }

	    if(myta.isOpen())
		{
		    System.out.println("Abort transaction ...");
		    myta.abort();
		}
	    System.out.println("Close database ...");
	    try {
		mydb.close();
		System.out.println("     ok.");
	    }
	    catch ( Exception e3 ) {
	    }

	    System.out.println("Exiting...");
	    System.exit(-1);
	}

	// testquery with empty result
	/*
	try {
	    System.out.println("Define a testquery with an empty result... ");
	    myQuery.create("select a from ImgRGBA as a where oid(a) <= 0");
	    System.out.println("Send the query ...");
	    DSet myResult = (DSet) myQuery.execute();

	    Iterator iter = myResult.iterator();
	    System.out.println("Number of results: " + myResult.size());


	    while(iter.hasNext())
		{
		    System.out.println("Ergebnis:");
		    System.out.println(iter.next());
		}

	    System.out.println("     ok.");
	    System.out.println("Commit transaction ...");
	    System.out.println("     ok.");
	}

	catch ( Exception e ) {
	    System.out.println("ERROR: ");
	    System.out.println(e.getMessage());

	    if(myta.isOpen())
		{
		    System.out.println("Abort transaction ...");
		    myta.abort();
		}
	    System.out.println("Close database ...");
	    try {
		mydb.close();
		System.out.println("     ok.");
	    }
	    catch ( Exception e2 ) {
	    }
	    System.out.println("Exiting...");
	    System.exit(-1);
	}
	*/

	// now insert MDDS and commit
	try {
	    System.out.println("Define the update query and bind the parameters ...");
	    myQuery.create("insert into testCollection VALUES $1");
	    //myQuery.create("insert into testCollection VALUES $1");
	    myQuery.bind(myMDD);
	    System.out.println("     ok.");
	    System.out.println("Send the query ...");
	    myQuery.execute();
	    System.out.println("     ok.");
	    System.out.println("Commit transaction ...");
	    myta.commit();
	    System.out.println("     ok.");
	}

	catch ( Exception e ) {
	    System.out.println("ERROR: ");
	    System.out.println(e.getMessage());

	    if(myta.isOpen())
		{
		    System.out.println("Abort transaction ...");
		    myta.abort();
		}
	    System.out.println("Close database ...");
	    try {
		mydb.close();
		System.out.println("     ok.");
	    }
	    catch ( Exception e2 ) {
	    }
	    System.out.println("Exiting...");
	    System.exit(-1);
	}

	// start new transaction and todo: read back the testimage
	try {
	    System.out.println("Start new transaction ...");
	    myta.begin();
	    myQuery = imp.newOQLQuery();
	    myQuery.create("select testCollection from testCollection");
	    //myQuery.create("create collection $1 GreySet");
	    //myQuery.bind("testCollection");
	    myQuery.execute();
	    myta.commit();
	    System.out.println("     ok.");
	    myta.begin();

	}

	catch ( Exception e ) {
	    System.out.println("ERROR: ");
	    System.out.println(e.getMessage());

	    if(myta.isOpen())
		{
		    System.out.println("Abort transaction ...");
		    myta.abort();
		}
	    System.out.println("Close database ...");
	    try {
		mydb.close();
		System.out.println("     ok.");
	    }
	    catch ( Exception e2 ) {
	    }
	    System.out.println("Exiting...");
	    System.exit(-1);
	}

	// todo: compare to the original image

	// drop test collection
	try {
	    System.out.println("Drop test collection ...");
	    myQuery = imp.newOQLQuery();
	    myQuery.create("drop collection testCollection");
	    myQuery.execute();
	    myta.commit();
	    mydb.close();
	    System.out.println("     ok.");
	}

	catch ( Exception e) {
	    System.out.println("ERROR: ");
	    System.out.println(e.getMessage());

	    if(myta.isOpen())
		{
		    System.out.println("Abort transaction ...");
		    myta.abort();
		}
	    System.out.println("Close database ...");
	    try {
		mydb.close();
		System.out.println("     ok.");
	    }
	    catch ( Exception e2 ) {
	    }
	    System.out.println("Exiting...");
	    System.exit(-1);
	}

        System.out.println( PREFIX_TESTSET + "testing queries done.\n" );
        return;
      } // testQueries()

   public static void objectTest()
     {
       System.out.println("Set object name test - in");
       
       RasObject dummy = new RasObject();
       
       dummy.setObjectName("goodName");
       
       System.out.println("Set name=" + dummy.getObjectName());
       
       dummy.setObjectName("bad-Name");
       
       System.out.println("Set name=" + dummy.getObjectName());
       
       System.out.println("Set object name test - out");
       
      }

} // TestOdmg

