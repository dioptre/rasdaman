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
 * test socket communication.
 *
 *
 * COMMENTS:
 *
 * </pre>
 *********************************************************** */
 
import java.io.*;
import java.net.*;
import java.lang.*;
import java.util.*;

public class Client
{ 
	private static final String DEFAULT_SERVER = "localhost";
	private static final int    DEFAULT_PORT   = 7001;

	private static String server = DEFAULT_SERVER;
	private static int    port   = DEFAULT_PORT;

	private static void communicate()
	{
                System.out.println( "server=" + server + ", port=" + port );

		try
		{
			Socket socket = new Socket( server, port );

 			PrintStream ps = new PrintStream(socket.getOutputStream());
			ps.print("Hello " + server + " at port " + port + " !\n" );
			ps.flush();
	
			BufferedReader ds = new BufferedReader(new InputStreamReader(socket.getInputStream()));
			System.out.println( "server delivered: '" + ds.readLine() );

			ps.close();
			ds.close();		
			socket.close();
			System.out.println( "socket closed." );
		}
		catch(UnknownHostException e2)
		{
			System.out.println( "host unknown: " + e2.getMessage() );
		}
		catch(IOException e1)
		{
			System.out.println( "io exception: " + e1.getMessage() );
		}
	}

	public static void main( String args[] )
	{
                System.out.println( "Socket test started." );

		if (args.length != 0 && args.length != 2)
                	System.out.println( "usage: java Client [server port]" );
		else if (args.length == 2)
		{
			server = args[0];
			try
			{
				port = Integer.parseInt( args[1] );
			}
			catch(NumberFormatException e)
			{
                		System.err.println( "Error: illegal port parameter: " + args[1] );
				return;
			}
		}

                communicate();

                System.out.println( "Socket test done." );
	}
}


