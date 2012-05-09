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

import java.util.Scanner;
import java.io.IOException;
import java.util.regex.Pattern;
import java.util.regex.Matcher;

/**
   Utility methods to preform rasdaman testing through java

  @author Ernesto Rodriguez <ernesto4160@gmail.com>
*/

public class SimultaneousConnectionsTestUtil{

    //The default login credentials in case RASLOGIN is not defined
    //user: rasadmin, password: rasadmin
    private final String ENV[] = {"RASLOGIN=rasadmin:d293a15562d3e70b6fdc5ee452eaed40"};

    /**
     * Obtain the number of available Rasdaman servers through rascontrol.
     * 
     * @return The number of servers (if found)
     * @throws IOException If number of servers cannot be obtained
     */

    public int getNumberOfServers() throws IOException{

	int numServers = 0;
	Pattern line = Pattern.compile(".*UP.*", Pattern.CASE_INSENSITIVE);
	Pattern loginError = Pattern.compile(".*error.*", Pattern.CASE_INSENSITIVE);	
	Process proc = Runtime.getRuntime().exec("rascontrol -t");
	Scanner scn = new Scanner(proc.getInputStream());
	boolean loginSuccessful;
	String messages = "";

	if (scn.hasNextLine() && !(loginError.matcher(scn.nextLine()).find()))
	    loginSuccessful = true;
	else {
	    loginSuccessful = false;
	    messages += "Could not log in to rascontrol. Please consider setting the envoiernmental variable RASLOGIN properly. Trying with user: rasadmin and password: rasadmin.\n\n";
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
	    messages += "Failed to obtain the number of available rasdaman servers. \n\n";
	    System.out.println(messages);
	    throw new IOException();
	} else {
	    messages += "The number of available Rasdaman servers obtained is "+numServers+
		" if this number is incorrect please check the RASLOGIN envoirnmental variable.\n\n";
	    System.out.println(messages);
	    return numServers;
	}
    }
}