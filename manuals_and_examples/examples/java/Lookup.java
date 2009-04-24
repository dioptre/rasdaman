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
 * PURPOSE: Example Java program for looking up a collection
 *
 *
 ************************************************************/

import rasj.*;
import rasj.odmg.*;
import org.odmg.*;
import java.util.*;

/**
 * Example Java program for looking up a collection
 *
 * set the server name with -server, the database name with -database, the collection name with -collection,
 * the port number with -port, the user login with -user, the password with -passwd
 */
public class Lookup
{
  public static void main(String[] args)
  {
    String server = "localhost";
    String base = "RASBASE";
    String coll = "rockies";
    String port = "7001";
    String user = "rasguest";
    String passwd = "rasguest";

    for (int i=args.length-1; i>=0; i--)
    {
      //System.out.println(args[i]);
      if (args[i].equals("-server"))
        server = args[i+1];
      if (args[i].equals("-database"))
        base = args[i+1];
      if (args[i].equals("-collection"))
        coll = args[i+1];
      if (args[i].equals("-port"))
        port = args[i+1];
      if (args[i].equals("-user"))
        user = args[i+1];
      if (args[i].equals("-passwd"))
        passwd = args[i+1];
    }
    //System.out.println(server+base+coll+port+user+passwd);

    DBag resultBag = null;
    Object result = null;
    Transaction myTa = null;
    Database myDb = null;
    OQLQuery myQu = null;

    try
    {
      Implementation myApp = new RasImplementation("http://"+server+":"+port);
      ((RasImplementation)myApp).setUserIdentification(user, passwd);
      myDb = myApp.newDatabase();

      System.out.println("Opening database ...");
      myDb.open(base, Database.OPEN_READ_ONLY);

      System.out.println("Starting transaction ...");
      myTa = myApp.newTransaction();
      myTa.begin();

      System.out.println("Retrieving results ...");
      myQu = myApp.newOQLQuery();
      myQu.create("select img from "+coll+" as img");
      resultBag = (DBag)myQu.execute();
      if (resultBag != null)
      {
	Iterator iter = resultBag.iterator();
	while (iter.hasNext())
	{
          result = iter.next();
          System.out.println(result);
	}
          System.out.println("All results");
      }

      System.out.println( "Committing transaction ..." );
      myTa.commit();

      System.out.println( "Closing database ..." );
      myDb.close();

    }
    catch (org.odmg.ODMGException e)
    {
      System.out.println("An exception has occurred: " + e.getMessage());
      System.out.println("Try to abort the transaction ...");
      if(myTa != null) myTa.abort();

      try
      {
        System.out.println("Try to close the database ...");
	if(myDb != null) myDb.close();
      }
      catch ( org.odmg.ODMGException exp )
      {
        System.err.println("Could not close the database: " + exp.getMessage());
      }
    }
    System.out.println( "Done." );
  }
}
