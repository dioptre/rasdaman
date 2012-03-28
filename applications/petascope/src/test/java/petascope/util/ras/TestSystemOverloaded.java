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
 * Copyright 2003 - 2010 Peter Baumann / rasdaman GmbH.
 *
 * For more information please see <http://www.rasdaman.org>
 * or contact Peter Baumann via <baumann@rasdaman.com>.
 */

package petascope.util.ras;

import org.junit.Assert;
import org.junit.Ignore;
import org.junit.Test;
import petascope.BaseTestCase;
import petascope.exceptions.RasdamanException;
import org.junit.After;
import org.junit.Before;
import java.util.Scanner;
import java.util.regex.Pattern;
import java.util.regex.Matcher;
import java.io.IOException;

/**
   Test class for quering an overloaded Rasdaman server
  @author Ernesto Rodriguez <ernesto4160@gmail.com>
*/

public class TestSystemOverloaded extends BaseTestCase{

    private int numQueries = 5;

    //The default number of servers assumed if the test is unable to determine it at runtime
    private final int DEFAULT_NUM_SERVERS=10;

    //The default login credentials in case RASLOGIN is not defined
    //user: rasadmin, password: rasadmin
    private final String ENV[] = {"RASLOGIN=rasadmin:d293a15562d3e70b6fdc5ee452eaed40"};

    @Before
    public void setUp() throws Exception {

	try {
	    numQueries = getNumberOfServers() * 2;
	} catch(IOException e) {

	    System.out.println("Failed to obtain the number of available rasdaman servers. Assuming " + DEFAULT_NUM_SERVERS + ".");
	    numQueries = DEFAULT_NUM_SERVERS * 2;
	}

	System.out.println("Testing system overload with " + numQueries + " concurrent queries.");

    }
    private int getNumberOfServers() throws IOException {

	int numServers = 0;
	Pattern line = Pattern.compile(".*UP.*", Pattern.CASE_INSENSITIVE);
	Pattern loginError = Pattern.compile(".*error.*", Pattern.CASE_INSENSITIVE);
	
	Process proc = Runtime.getRuntime().exec("rascontrol -t");
	Scanner scn = new Scanner(proc.getInputStream());
	boolean loginSuccessful;

	if (scn.hasNextLine() && !(loginError.matcher(scn.nextLine()).find()))
	    loginSuccessful = true;
	else {
	    loginSuccessful = false;
	    System.out.println("Could not log in to rascontrol. Please consider setting the envoiernmental variable RASLOGIN properly. Trying with user: rasadmin and password: rasadmin.");
	}

	scn.close();

	if (loginSuccessful)
	    proc = Runtime.getRuntime().exec("rascontrol -x list srv");
	else
	    proc = Runtime.getRuntime().exec("rascontrol -x list srv", ENV);

	scn = new Scanner(proc.getInputStream());	

	while(scn.hasNextLine()){

	    String tmp = scn.nextLine();

	    if(line.matcher(tmp).find())
		numServers++;
	}
	scn.close();

	if (numServers <= 0) {
	    System.out.println("Failed to obtain the number of available rasdaman servers. Assuming " + DEFAULT_NUM_SERVERS + ".");
	    return DEFAULT_NUM_SERVERS;
	} else	    
	    return numServers;
    }
    
    /**
      Test overloading a rasdaman server with queries. The queries should be properly queued until more rasdaman servers
      become available.
      @return void No error occurred, queries to overloaded servers were properly handled.
      @throws Exception, if one or more queries failed to execute, failure since queries couldn't be processed due to system overload
    */
    @Test
    public void testSystemOverloaded() throws Exception {

	RasdamanQuery queries[] = new RasdamanQuery[numQueries];

	for(int i = 0; i < queries.length; i++){

	    queries[i]=new RasdamanQuery();
	    (new Thread(queries[i])).start();
	}

	boolean finished = false;

	while(!finished){
	    finished = true;
	    for(int i = 0; i < queries.length; i++) {

		if(!queries[i].isDone())
		    finished=false;
	    }
	}
	
	for(int i = 0; i < queries.length; i++)
	    try{
		throw queries[i].resultingException();
	    }catch(NullPointerException e){
		//The query completed successfully
	    }
    }
}
