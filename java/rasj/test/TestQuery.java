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
 * test rasj: simple open/read query/close cycle.
 * rasj test program for executing a query against some rasdaman database.
 * Note: database is opened readonly, so send only SELECT queries.
 * 
 * @param --server s   - use server s (default: localhost)
 * @param --port p     - use server port p (default: 7001)
 * @param --database d - use database d (default: RASBASE)
 * @param --user u     - log in as user u (default: rasguest)
 * @param --passwd p   - log in with password p (default: rasguest)
 * @param --query q    - send SELECT query string q to server (default: "select r from RAS_COLLECTIONNAMES as r"")
 * @param --count c    - execute query c times (default: 1)
 *
 *
 * COMMENTS:
 * - no parameter line error handling
 ************************************************************/

import rasj.*;
import rasj.odmg.*;
import org.odmg.*;
import java.util.*;
import java.io.*;

public class TestQuery
  {

    public static void main( String[] args )
      {
        String server = "localhost";
        String port = "7001";
        String base = "RASBASE";
        String user = "rasguest";
        String passwd = "rasguest";
        String query = "select r from RAS_COLLECTIONNAMES as r";
        int count = 1;

        if (args.length == 0)
          {
            System.out.println( "usage: TestQuery [options]" );
            System.out.println( "options:" );
            System.out.println( "  --server s   - use server s (default: localhost)" );
            System.out.println( "  --port p     - use server port p (default: 7001)" );
            System.out.println( "  --database d - use database d (default: RASBASE)" );
            System.out.println( "  --user u     - log in as user u (default: rasguest)" );
            System.out.println( "  --passwd p   - log in with password p (default: rasguest)" );
            System.out.println( "  --query q    - send SELECT query string q to server (default: select r from RAS_COLLECTIONNAMES as r)" );
            System.out.println( "  --count c    - execute query c times (default: 1)" );
            return;
          }

        for (int i=args.length-1; i>=0; i--)
          {
            if (args[i].equals("--server"))
                server = args[i+1];
            if (args[i].equals("--port"))
                port = args[i+1];
            if (args[i].equals("--database"))
                base = args[i+1];
            if (args[i].equals("--user"))
                user = args[i+1];
            if (args[i].equals("--passwd"))
                passwd = args[i+1];
            if (args[i].equals("--query"))
                query = args[i+1];
            if (args[i].equals("--count"))
                count = Integer.parseInt(args[i+1]);
          }

        System.out.println( "Query test started with server=" + server + ", port=" + port + ", database=" + base + ", user=" + user + ", count=" + count + ", query=" + query );

        try
          {
            RasImplementation myApp = new RasImplementation("http://"+server+":"+port);
            myApp.setUserIdentification(user, passwd);

            System.out.println( "opening database..." );
            Database myDb = myApp.newDatabase();
            myDb.open( base, Database.OPEN_READ_WRITE );

            System.out.println( "starting transaction..." );
            Transaction myTa = myApp.newTransaction();
            myTa.begin();

            for (int i = 1; i <= count; i++)
              {

                System.out.print( "sending query #" + i + "..." );
                OQLQuery myQu = myApp.newOQLQuery();
                myQu.create(query);
                DBag result = (DBag) myQu.execute();

                System.out.println( "result is: " + result );
        
              }

            System.out.println( "closing transaction..." );
            myTa.commit();

            // System.out.println( "sending query out of TA, must produce an error..." );
            // OQLQuery myQu2 = myApp.newOQLQuery();
            // myQu2.create(query);
            // DBag result2 = (DBag) myQu2.execute();

            System.out.println( "closing database..." );
            myDb.close();

          }
        catch(Exception e)
          {
            System.err.println( e.getMessage() );
          }

        System.out.println( "Query test done." );

      } // main()

} // TestQuery
