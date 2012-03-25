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

    private int numQueries=5;

    @Before
	public void setUp() throws Exception{

	try{
	    numQueries=getNumberOfServers()*2;
	}catch(IOException e){

	    System.out.println("Failed to obtain the number of available rasdaman servers. Using 10.");
	    numQueries=10*2;
	}

	System.out.println("Testing system overload with "+numQueries+" concurrent queries.");

    }
    private int getNumberOfServers() throws IOException{
	Pattern line=Pattern.compile(".*N[1-9].*UP.*");
	String env[]={"RASLOGIN=rasadmin:d293a15562d3e70b6fdc5ee452eaed40"};
	Process proc=Runtime.getRuntime().exec("rascontrol -x list srv",env);
	Scanner scn = new Scanner(proc.getInputStream());
	int numServers=0;

	while(scn.hasNextLine()){

	    String tmp=scn.nextLine();

	    if(line.matcher(tmp).find())
		numServers++;
	}
	scn.close();

	return numServers;
    }

    @Test
	public void testSystemOverloaded() throws Exception{

	RasdamanQuery queries[]=new RasdamanQuery[numQueries];

	for(int i=0;i<queries.length;i++){

	    queries[i]=new RasdamanQuery();
	    (new Thread(queries[i])).start();
	}

	boolean finished=false;

	while(!finished){
	    finished=true;
	    for(int i=0;i<queries.length;i++){

		if(!queries[i].isDone())
		    finished=false;
	    }
	}
	
	for(int i=0;i<queries.length;i++)
	    try{
		throw queries[i].resultingException();
	    }catch(NullPointerException e){
		//The query went ok
	    }
    }
}
