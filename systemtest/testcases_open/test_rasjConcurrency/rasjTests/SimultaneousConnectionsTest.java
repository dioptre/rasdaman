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


/**
   Test for proper handling of simultaneous requests.
  @author Ernesto Rodriguez <ernesto4160@gmail.com>
*/

import org.junit.Test;
import rasj.RasQueryExecutionFailedException;
import org.odmg.ODMGException;

public class SimultaneousConnectionsTest{

    public static final String RASDAMAN_URL = "http://localhost:7001";
    public static final String QUERY= "select csv(c[0:200,0:200]) from NIR AS c";
    public static final String RASDAMAN_DATABASE = "RASBASE";
    private final int numThreads=4;

    @Test
	public void testQueryRasdaman() throws Exception{

	RasjQuery[] queries=new RasjQuery[numThreads];

	System.out.println("Starting "+numThreads+" simultaneous requests through rasj.");
	System.out.println("Rasdaman Url: "+RASDAMAN_URL);
	System.out.println("Rasdaman Database: "+RASDAMAN_DATABASE);
	for(int i=0;i<queries.length;i++){

	    queries[i]=new RasjQuery();
	    (new Thread(queries[i])).start();
	}

	boolean done=false;

	while(!done){

	    done=true;
	    for(int i=0;i<queries.length;i++)		
		if(!queries[i].isDone())
		    done=false;
	}

	int succ=0;
	for(int i=0;i<queries.length;i++){

	    try{
		
		throw queries[i].getResultingException();
	    }catch(RasQueryExecutionFailedException e){
		e.printStackTrace();
		System.out.println("A thread failed to query the database.");
		System.out.println("Only "+succ+" threads completed successfully.");
		throw e;
	    }catch(ODMGException e){
		e.printStackTrace();
		throw e;
	    }catch(NullPointerException e){
		succ++;
		//Everything went fine.
	    }
	}
	
	System.out.println("All "+numThreads+" simultaneous requests completed successfully.");
    }
}