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
 * PURPOSE: Example Java program for reading a query out of a file
 *          and send it to the server and write the result into a log file
 *
 *
 ************************************************************/

import rasj.*;
import rasj.odmg.*;
import org.odmg.*;
import java.util.*;
import java.io.*;

/**
 * Example Java program for reading a query out of a file
 * and send it to the server and write the result into a log file
 *
 * set the server name with -server, the database name with -database, the collection name with -collection,
 * set port number with -port, the user login with -user, the password with -passwd,
 * the path to log file with -logfile, path to query file with -queryfile
 */
public class Query
{

  public static void main( String[] args )
  {
    String server = "localhost";
    String port = "7001";
    String base = "RASBASE";
    String coll = "rockies";
    String user = "rasguest";
    String passwd = "rasguest";
    String file = "";
    String query = "";
    String log = "";
    String ret = "";

    for (int i=args.length-1; i>=0; i--)
    {
      //System.out.println(args[i]);
      if (args[i].equals("-server"))
        server = args[i+1];
      if (args[i].equals("-port"))
        port = args[i+1];
      if (args[i].equals("-database"))
        base = args[i+1];
      if (args[i].equals("-collection"))
        coll = args[i+1];
      if (args[i].equals("-logfile"))
        log = args[i+1];
      if (args[i].equals("-user"))
        user = args[i+1];
      if (args[i].equals("-passwd"))
        passwd = args[i+1];
      if (args[i].equals("-queryfile"))
      {
        try
        {
          file = args[i+1];
          //System.out.println(file);
          FileReader fr = new FileReader(file);
          BufferedReader in = new BufferedReader(fr);
          String s;

          while((s = in.readLine()) != null)
            {
              String t = s.trim();
              while(t.regionMatches(0, "--", 0, 2))
              {
                s = in.readLine();
                t = s.trim();
              }
              query = query +" "+ s;
            }

        }
        catch(FileNotFoundException e)
        {
          ret = ret+" Query-file not found!"+e.getMessage();
          System.err.println(ret);
        }
        catch(IOException e)
        {
          ret = ret+" Could not read Query-file!"+e.getMessage();
          System.err.println(ret);
        }

      }

    }


      Transaction myTa = null;
      Database myDb = null;
      int accessMode = Database.OPEN_READ_ONLY;

      try
      {
        Implementation myApp = new RasImplementation("http://"+server+":"+port);
        ((RasImplementation)myApp).setUserIdentification(user, passwd);
        OQLQuery myQu = myApp.newOQLQuery();

        if(query.equals(""))
          query="select avg_cells(a) from "+ coll+" as a";
          //query="create collection UpdateULong1 ULongSet";
          //query="insert into UpdateULong1 values $1";

        myQu.create(query);

        StringTokenizer strTok = new StringTokenizer(query, "$");
        RasMInterval domain;
        RasGMArray mddConst;
        if(strTok.hasMoreTokens())
          strTok.nextToken();

        while(strTok.hasMoreTokens())
        {
          strTok.nextToken();
          domain = new RasMInterval("[0:10,0:10]");
          RasStorageLayout stl = new RasStorageLayout();
          stl.setTileSize(100);
          mddConst = new RasGMArray(domain, 4, stl);
          mddConst.setObjectTypeName("ULongImage");
          //System.out.println(mddConst);
          myQu.bind(mddConst);
        }

        //check if is an update Query
	if((query.indexOf("select") == -1) &&
 	   (query.indexOf("SELECT") == -1))
	  accessMode = Database.OPEN_READ_WRITE;

        myDb = myApp.newDatabase();
        //System.out.println( "Opening database ..." );
        myDb.open( base, accessMode );

        //System.out.println( "Starting transaction ..." );
        myTa = myApp.newTransaction();
        myTa.begin();

        //System.out.println( "Executing query..." );
        DBag result = (DBag) myQu.execute();

        //System.out.println( "Committing transaction ..." );
        myTa.commit();

        //System.out.println( "Closing database ..." );
        myDb.close();

	//System.out.println( " Done." );
      }
      catch(RasException e)
      {
        ret = ret+"RasException: "+e.getMessage();
        System.err.println(ret);
      }
      catch (org.odmg.ODMGException e)
      {
        ret = ret + query + "\n Failed:\n " + e.getMessage() + "\n";
        System.err.println(ret);
	//System.err.println("Try to abort the transaction ...");
        if(myTa != null) myTa.abort();

	try
        {
	    //System.err.println("Try to close the database ...");
	  if(myDb != null) myDb.close();
        }
	catch ( org.odmg.ODMGException exp )
	{
          ret = ret+" Could not close the database: " + exp.getMessage();
	  System.err.println(ret);
	}

      }
      if(!log.equals(""))
      {
        try
        {
          FileOutputStream fos = new FileOutputStream(log, true);
          PrintWriter pw = new PrintWriter(fos);
          if(ret.equals(""))
            ret = "OK.";
          pw.write(ret);
          pw.close();
        }
        catch(FileNotFoundException e)
        {
          ret = ret+" Log-file not found!"+e.getMessage();
          System.out.println(ret);
        }
        catch(IOException e)
        {
          ret = ret+" Could not write Log-file!"+e.getMessage();
          System.out.println(ret);
        }
      }

  }

}
