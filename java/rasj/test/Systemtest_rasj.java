package rasj.test;

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
 *
 *
 *
 * COMMENTS:
 *
 * </pre>
 *********************************************************** */

import java.io.*;

import rasj.*;
import rasj.global.*;
import org.odmg.*;


// auxiliary classes for exception testing
class TestRasResultIsNoCellException extends RasResultIsNoCellException
{
    TestRasResultIsNoCellException()
    {
	super();
    }
}

class TestRasResultIsNoIntervalException extends RasResultIsNoIntervalException
{
    TestRasResultIsNoIntervalException()
    {
	super();
    }
}

class TestRasClientInternalException extends RasClientInternalException
{
    TestRasClientInternalException(String msg)
    {
	super("Systemtest_rasj","TestExceptions",msg);
    }
}

// main program
/**
 * class for testing rasj interface
 * @version $$
 */
public class Systemtest_rasj
{

public static void main(String[] args)
    {
	String server = null;
	String port = "7001";
	boolean wrongUsage = false;

	// get args
	if(args.length == 0)
	    wrongUsage = true;
	for(int i = 0; i < args.length; i+=2)
	    {
		if (args[i].equals("-server"))
		    server = args[i+1];
		else if (args[i].equals("-port"))
		    port = args[i+1];
		else
		    wrongUsage = true;
	    }

	if(wrongUsage)
	    {
		System.err.println("Usage: Systemtest_rasj -s <servername> [ -p <port> ]");
		System.exit(-1);
	    }

	// start
	Implementation myApp = new RasImplementation("http://" + server + ":" + port);

	testExceptions(myApp);

        testMArrays marrayTest = new testMArrays(server);

        testIntervals intervalTest = new testIntervals();

    }

    // exception testing
    static void testExceptions(Implementation imp)
    {

	boolean resultIsOk = false;
	Database myDb = null;
	Transaction myTa = null;
	OQLQuery myQu = null;

	System.err.println("\n### Testing exceptions:");

	// RasConnectionFailedException
	Implementation errorApp;
	String logMsg;
	try {
	    errorApp = new RasImplementation("wrongserver:8080");
	    myDb = errorApp.newDatabase();
	    logMsg = "Exception was not thrown";
	    myDb.open("RASBASE",Database.OPEN_READ_WRITE);
	}
	catch(RasConnectionFailedException e1) {
	    resultIsOk = true;
	    logMsg = e1.getMessage();
	}
	catch(ODMGException e2) {
	    logMsg = e2.getMessage();
	}
	log("RasConnectionFailedException",resultIsOk,logMsg);

	// RasDimensionMismatchException
	resultIsOk = false;
	logMsg = "Exception was not thrown";
	try {
	    RasMInterval test1;
	    RasMInterval test2 = new RasMInterval("[1:20,1:20]");
	    RasMInterval test3 = new RasMInterval("[1:4,1:9,1:6]");

	    test1 = test2.intersectionWith(test3);
	}
	catch(RasDimensionMismatchException e1) {
	    resultIsOk = true;
	    logMsg = e1.getMessage();
	}
	catch(Exception e2) {
	    logMsg = e2.getClass().getName() + ", " + e2.getMessage();
	}
	log("RasDimensionMismatchException",resultIsOk,logMsg);

	// RasIndexOutOfBoundsException
	resultIsOk = false;
	logMsg = "Exception was not thrown";
	try {
	    RasGMArray myMDD = new RasGMArray(new RasMInterval("[1:5,1:5]"),1);
	    byte[] retVal = myMDD.getCell(new RasPoint(4,7));
	}
	catch(RasIndexOutOfBoundsException e1) {
	    resultIsOk = true;
	    logMsg = e1.getMessage();
	}
	catch(RasResultIsNoIntervalException e2) {
	    //this cannot occur
	}
	catch(RasDimensionMismatchException e3) {
	    // this cannot occur
	}
	log("RasIndexOutOfBoundsException",resultIsOk,logMsg);

	// RasClientInternalException
	resultIsOk = false;
	logMsg = "Exception was not thrown";
	try {
	    throw new TestRasClientInternalException("This is a test error");
	}
	catch(RasClientInternalException e1) {
	    resultIsOk = true;
	    logMsg = e1.getMessage();
	}
	log("RasClientInternalException",resultIsOk,logMsg);

	// RasResultIsNoCellException
	resultIsOk = false;
	logMsg = "Exception was not thrown";
	try {
	    throw new TestRasResultIsNoCellException();
	}
	catch(RasResultIsNoCellException e1) {
	    resultIsOk = true;
	    logMsg = e1.getMessage();
	}
	log("RasResultIsNoCellException",resultIsOk,logMsg);

	// RasResultIsNoIntervalException
	resultIsOk = false;
	logMsg = "Exception was not thrown";
	try {
	    throw new TestRasResultIsNoIntervalException();
	}
	catch(RasResultIsNoIntervalException e1) {
	    resultIsOk = true;
	    logMsg = e1.getMessage();
	}
	log("RasResultIsNoIntervalException",resultIsOk,logMsg);

	// RasStreamInputOverflowException
	resultIsOk = false;
	logMsg = "Exception was not thrown";
	try {
	    RasMInterval int1 = new RasMInterval(2);
	    int1.stream(new RasSInterval("1:10"));
	    int1.stream(new RasSInterval("1:5"));
	    int1.stream(new RasSInterval("1:20"));
	}
	catch(RasResultIsNoIntervalException e1) {
	    //
	}
	catch(RasStreamInputOverflowException e3) {
	    resultIsOk = true;
	    logMsg = e3.getMessage();
	}
	log("RasStreamInputOverflowException",resultIsOk,logMsg);


	// RasTypeInvalidException
	resultIsOk = false;
	logMsg = "Exception was not thrown";
	try {
	    RasPrimitiveType myType = new RasPrimitiveType("myType",RasGlobalDefs.RAS_BOOLEAN);
	    myType.getFloat(new Object());
	}
	catch(RasTypeInvalidException e3) {
	    resultIsOk = true;
	    logMsg = e3.getMessage();
	}
	log("RasTypeInvalidException",resultIsOk,logMsg);

	// RasTypeNotSupportedException
	resultIsOk = false;
	logMsg = "Exception was not thrown";
	try {
	    throw new RasTypeNotSupportedException("RAS_TEST");
	}
	catch(RasTypeNotSupportedException e3) {
	    resultIsOk = true;
	    logMsg = e3.getMessage();
	}
	log("RasTypeNotSupportedException",resultIsOk,logMsg);
    }





    static void log(String testObject, boolean resultOK, String message)
    {
	if(resultOK)
	    System.err.println(testObject + " ... OK\n     (" + message + ")");
	else
	    {
		System.err.println(testObject + " ... FAILED!\n     (" + message + ")");
		System.err.println("Continue? [y/n]");
		BufferedReader br = new BufferedReader( new InputStreamReader(System.in));
		try {
		String answer = br.readLine();
		while(answer != "")
		    {
		    if(answer.equals("n"))
			System.exit(1);
		    else if(!answer.equals("y"))
			{
			    System.err.println("Please answer y or n: ");
			    answer = br.readLine();
			}
		    }
		}
		catch(IOException e1) {
		    //
		}

	    }
    }


}
