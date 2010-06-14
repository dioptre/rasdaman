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
package rasj.test;

import rasj.*;
import rasj.global.*;
import org.odmg.*;

import java.util.*;
import java.io.*;


public class MassLoadTest implements RasGlobalDefs
{

    public static void main(String[] args)
    {

	String hostname = null;
	String port = null;
	Database mydb = null;
	Transaction myta = null;
	RasGMArray myMDD = null;
	Implementation myApp = null;
	OQLQuery myQuery = null;
	String collectionName = "testCollection";
	int tileSize = 1000;
	int overallSize = 200000;
	int commitInterval = 50;


	/*******************************************
	 *                                         *
	 * Usage: MassLoadTest hostname [port]     *
	 *                                         *
	 *******************************************/

	// we need the hostname
	if(args.length>0 && args[0] == null)
	    {
		System.out.println("Please provide the hostname as a parameter when calling this test-program.");
		System.exit(-1);
	    }
        else
	    hostname = args[0];

	// is there a port specified?
	if(args.length>1 && args[1] != null)
	    port = args[1];         
        else
	    port = "8080";

	try {
	    myApp = new RasImplementation("http://" + hostname + ":" + port);
	    mydb = myApp.newDatabase();
	    System.err.println("Open Database ...");
	    mydb.open("RASBASE",Database.OPEN_READ_WRITE);
	    System.err.println("     ok.");
	    myta = myApp.newTransaction();
	    System.err.println("Begin transaction ...");
	    myta.begin();
	    System.err.println("     ok.");

	    //create test collection
	    System.err.println("Create a test collection ...");
	    myQuery = myApp.newOQLQuery();
	    myQuery.create("create collection $1 GreySet");
	    //myQuery.create("drop collection $1");

	    myQuery.bind(collectionName);
	    myQuery.execute();
	    System.err.println("     ok.");
	    myta.commit();

	    // init insertion tile
	    System.err.println("create test arrays ...");
	    myMDD = new RasGMArray(new RasMInterval("[0:" + (tileSize-1) + ",0:" + (tileSize-1) + "]"),1);
	    int cells = tileSize*tileSize;
	    byte[] mydata = new byte[cells];
	    for(int i = 0; i < cells; i++)
		mydata[i]=0;
            myMDD.setArray(mydata);
	    myMDD.setObjectTypeName("GreyImage");

	    // now insert tiles
	    boolean first = true;
	    myta.begin();
	    int dimX = 0;
	    int dimY = 0;
	    String domain = null;
	    int finished = 1;

	    int counter = (int)Math.abs(overallSize/tileSize);
	    for(int x = 0; x < counter; x++)
		{
		    dimX += tileSize;
		    dimY = 0;
		    for(int y = 0; y < counter; y++)
			{
			    dimY += tileSize;

			    // init mdd
			    RasGMArray myMDD2 = new RasGMArray(new RasMInterval("[" + String.valueOf(dimX-tileSize) + ":" + String.valueOf(dimX-1) + "," +
				String.valueOf(dimY-tileSize) + ":" + String.valueOf(dimY-1) + "]"),1);
			    cells = tileSize*tileSize;
			    byte[] mydata2 = new byte[cells];
			    for(int i = 0; i < cells; i++)
				mydata2[i]=0;
			    myMDD2.setArray(mydata2);
			    myMDD2.setObjectTypeName("GreyImage");

			    // prepare query
			    myQuery = myApp.newOQLQuery();
			    if(first)
				{
				    myQuery.create("insert into $1 values $2");
				    first = false;
				}
			    else
				myQuery.create("update $1 AS img SET img ASSIGN $2");
			    myQuery.bind(collectionName);
			    myQuery.bind(myMDD2);

			    // execute query
			    System.err.println("Inserting tile " + myMDD2.spatialDomain());
			    myQuery.execute();
			    finished += 1;

			    // commit?
			    if(finished == commitInterval)
				{
				    myta.commit();
				    System.err.println("... committed, begin new transaction ...");
				    myta = myApp.newTransaction();
				    myta.begin();
				    finished = 1;
				}

			}
		    // now write the rest of y if there is something left:
		    if(dimY < overallSize-1)
			{
			    RasGMArray myMDD3 = new RasGMArray(new RasMInterval("[" + String.valueOf(dimX-tileSize) + ":" + String.valueOf(dimX-1) + "," +
										String.valueOf(dimY) + ":" + String.valueOf(overallSize-1) + "]"),1);
			    cells = tileSize*(overallSize-dimY);
			    byte[] mydata3 = new byte[cells];
			    for(int i = 0; i < cells; i++)
				mydata3[i]=0;
			    myMDD3.setArray(mydata3);
			    myMDD3.setObjectTypeName("GreyImage");

			    // prepare query
			    myQuery = myApp.newOQLQuery();
			    if(first)
				{
				    myQuery.create("insert into $1 values $2");
				    first = false;
				}
			    else
				myQuery.create("update $1 AS img SET img ASSIGN $2");
			    myQuery.bind(collectionName);
			    myQuery.bind(myMDD3);

			    // execute query
			    System.err.println("Inserting tile " + myMDD3.spatialDomain());
			    myQuery.execute();
			    finished += 1;

			    // commit?
			    if(finished == commitInterval)
				{
				    myta.commit();
				    System.err.println("... committed, begin new transaction ...");
				    myta = myApp.newTransaction();
				    myta.begin();
				    finished = 1;
				}
			}
		}

	    // now write the rest of x if there is something left
	    dimY = 0;
	    if(dimX < overallSize-1)
		{
		    for(int y = 0; y < counter; y++)
			{
			    dimY += tileSize;

			    // init mdd
			    RasGMArray myMDD4 = new RasGMArray(new RasMInterval("[" + String.valueOf(dimX) + ":" + String.valueOf(overallSize-1) + "," +
										String.valueOf(dimY-tileSize) + ":" + String.valueOf(dimY-1) + "]"),1);
			    cells = (overallSize-dimX)*tileSize;
			    byte[] mydata4 = new byte[cells];
			    for(int i = 0; i < cells; i++)
				mydata4[i]=0;
			    myMDD4.setArray(mydata4);
			    myMDD4.setObjectTypeName("GreyImage");

			    // prepare query
			    myQuery = myApp.newOQLQuery();
			    if(first)
				{
				    myQuery.create("insert into $1 values $2");
				    first = false;
				}
			    else
				myQuery.create("update $1 AS img SET img ASSIGN $2");
			    myQuery.bind(collectionName);
			    myQuery.bind(myMDD4);

			    // execute query
			    System.err.println("Inserting tile " + myMDD4.spatialDomain());
			    myQuery.execute();
			    finished += 1;

			    // commit?
			    if(finished == commitInterval)
				{
				    myta.commit();
				    System.err.println("... committed, begin new transaction ...");
				    myta = myApp.newTransaction();
				    myta.begin();
				    finished = 1;
				}
			}

		    // now write the rest of y if available
		    if(dimY < overallSize-1)
			{
			    RasGMArray myMDD5 = new RasGMArray(new RasMInterval("[" + String.valueOf(dimX) + ":" + String.valueOf(overallSize-1) + "," +
										String.valueOf(dimY) + ":" + String.valueOf(overallSize-1) + "]"),1);
			    cells = (overallSize-dimX)*(overallSize-dimY);
			    byte[] mydata5 = new byte[cells];
			    for(int i = 0; i < cells; i++)
				mydata5[i]=0;
			    myMDD5.setArray(mydata5);
			    myMDD5.setObjectTypeName("GreyImage");

			    // prepare query
			    myQuery = myApp.newOQLQuery();
			    if(first)
				{
				    myQuery.create("insert into $1 values $2");
				    first = false;
				}
			    else
				myQuery.create("update $1 AS img SET img ASSIGN $2");
			    myQuery.bind(collectionName);
			    myQuery.bind(myMDD5);

			    // execute query
			    System.err.println("Inserting tile " + myMDD5.spatialDomain());
			    myQuery.execute();
			    finished += 1;
			}
		}

	    // READY!			
	    myta.commit();
	    System.err.println("FINISHED!");


	    // drop test collection
	    myta = myApp.newTransaction();
	    myta.begin();
	    System.err.println("Drop test collection ...");
	    myQuery = myApp.newOQLQuery();
	    myQuery.create("drop collection $1");
	    myQuery.bind(collectionName);
	    myQuery.execute();
	    System.err.println("     ok.");
	    myta.commit();
	    myta = myApp.newTransaction();
	    myta.commit();



	}

	catch ( Exception e ) {
	    System.out.println("ERROR: ");
	    System.out.println(e.getMessage());

	    if(myta.isOpen())
		{
		    System.err.println("Abort transaction ...");
		    myta.abort();
		}
	    System.err.println("Close database ...");
	    try {
		mydb.close();
		System.err.println("     ok.");
	    }
	    catch ( Exception e2 ) {
	    }
	    System.err.println("Exiting...");
	    System.exit(-1);
	}
    }

}



