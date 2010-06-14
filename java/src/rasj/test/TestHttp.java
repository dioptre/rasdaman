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
 * <pre>
 *
 * PURPOSE:
 *
 *
 * COMMENTS:
 * - not part of the standard systemtest, because it does not use std
 *   interface but copies internal ras/clientcomm code
 * </pre>
 *********************************************************** */

import rasj.*;
import rasj.odmg.*;
import org.odmg.*;
import rasj.global.*;

import java.io.*;
import java.util.*;

import java.lang.*;
import java.net.*;
import rasj.clientcommhttp.*;

/**
 * This class handles a HTTP-request to the RasDaMan server.
 * The specified RasDaMan server is contacted, the specified command is sent to
 * the server, and the result of the query is retrieved and stored in a byte array.
 * The specification of the communication protocol is given below.
 * <P>
 *
 * @version $Revision: 1.1 $
 */

public class TestHttp implements RasCommDefs, RasGlobalDefs
{

  static final String rcsid = "@(#)Package rasj.test, class TestHttp: $Header: /home/rasdev/CVS-repository/rasdaman/java/rasj/test/TestHttp.java,v 1.1 2003/12/19 16:05:43 rasdev Exp $";


/**
 * The type of this client
 */
 private String client = "RASCLIENT";

/**
 * The result type ( MDD Collection, Skalar Collection, Error, Integer ). This field is set
 * automatically when a query has been executed, so there's no setResultType method.
 */
 private static byte resultType = 0;

/**
 * The result of the query
**/
 private static Object result = null;


/**
 * This method sends a query to the RasDaMan Server and retrieves the results.
 *
 * @param con server connection
 * @param parameters the parameters for the request as name/value pairs (for example "clientID=4354351&queryString=select img from test")
 */
 public static void execute( String serverURL, String parameters )
      throws RasQueryExecutionFailedException, RasConnectionFailedException
      {
        System.out.println( "RasHttpRequest.execute: start. parameters=" + parameters );

        BenchmarkTimer httpTimer = new BenchmarkTimer("httpRequest direct");
        BenchmarkTimer rcvTimer = new BenchmarkTimer("receive direct");

        try
	  {
            URL url = new URL( serverURL );

            System.out.println( "RasHttpRequest.execute: sending to " + url + " POST request=" + parameters );

            httpTimer.startTimer();

            BenchmarkTimer sendTimer = new BenchmarkTimer("send direct");
            sendTimer.startTimer();

            // Send the query
            HttpURLConnection con = (HttpURLConnection) url.openConnection();
            con.setRequestProperty("Content-type","application/octet-stream");
            con.setRequestProperty("User-Agent","RasDaMan Java Client");
            con.setRequestProperty("Version","1.0");
            con.setRequestMethod("POST");
            con.setDoInput(true);
            con.setDoOutput(true);
            con.setUseCaches(false);

            OutputStreamWriter out = new OutputStreamWriter(con.getOutputStream(),"8859_1");
	    out.write(parameters,0,parameters.length());
            out.flush();
            out.close();

            sendTimer.stopTimer();
            sendTimer.print();

            rcvTimer.startTimer();

            // Read response
            BenchmarkTimer getInputTimer = new BenchmarkTimer("getInputStream direct");
            getInputTimer.startTimer();
            DataInputStream in = new DataInputStream(con.getInputStream());
            System.out.println("RasHttpRequest.execute: received " + in );
            getInputTimer.stopTimer();
            getInputTimer.print();

            // read binData
            // binData = new byte[dataSize];
            // readBytes = 0;
            // readBytesTmp = 0;
            // while( (readBytesTmp != -1) && (readBytes < dataSize) )
              // {
                // readBytesTmp = in.read(binData,readBytes,dataSize-readBytes);
                // readBytes += readBytesTmp;
                // System.out.println("Read " + readBytesTmp +" Bytes:" + binData);
              // }
            // in.close();
            // con.disconnect();

           /* variables for later use */
            byte[] b1 = new byte[1];
            byte[] b4 = new byte[4];
            byte endianess = 0;
            String collType = null;
            int numberOfResults = 0;
            int dataSize = 0;
            byte[] binData = null;
            int readBytes = 0;
            int readBytesTmp = 0;
	    DBag resultBag;
	    RasGMArray res = null;

            in.read(b1);

            resultType = b1[0];
            Debug.talkVerbose("RasHttpRequest.execute: resultType=" + resultType );
            switch( resultType )
	       {
               case RESPONSE_OK:
	       case RESPONSE_OK_NEGATIVE:
		    //Nothing todo
		    break;

	       // +++++++++++++++++++++++++++++++++++++++++++++++++
	       case RESPONSE_MDDS:
                    Debug.talkVerbose("RasHttpRequest.execute: result type is MDD." );
                    // read Endianess
                    while(in.read(b1) == 0)
                        ;
                    endianess = b1[0];

                    // read Collection Type
                    collType = RasUtils.readString(in);
                    Debug.talkVerbose("RasHttpRequest.execute: colltype=" + collType);

                    // read NumberOfResults
                    while(in.available() < 4)
                        ;
                    in.read(b4);
                    numberOfResults = RasUtils.ubytesToInt(b4,endianess);
                    Debug.talkVerbose("RasHttpRequest.execute: number of results: " + numberOfResults);

		    // Initialize return-set and parameters
		    resultBag = new RasBag();
		    String mddBaseType = null;
		    String domain = null;
                    String oid = "";
                    RasOID roid = null;

                    // do this for each result
                    for(int x = 0; x < numberOfResults; x++)
			{
                        Debug.talkVerbose("RasHttpRequest.execute: handling result #" + (x+1) );
			//read mddBaseType
                        mddBaseType = RasUtils.readString(in);

                        // read spatialDomain
                        domain = RasUtils.readString(in);

                        // read OID
                        oid = RasUtils.readString(in);
                        //System.err.println("OID is " + oid);
                        roid = new RasOID(oid);

                        // read size of binData
                        while(in.available() < 4)
                            ;
                        in.read(b4);

                        dataSize = RasUtils.ubytesToInt(b4,endianess);

                        Debug.talkVerbose("RasHttpRequest.execute: mddBaseType is " + mddBaseType + ", spatialDomain=" + domain + ", size of BinData=" + dataSize );

                        // read binData
                        binData = new byte[dataSize];
                        readBytes = 0;
			readBytesTmp = 0;

                        while( (readBytesTmp != -1) && (readBytes < dataSize) )
		             {
                             readBytesTmp = in.read(binData,readBytes,dataSize-readBytes);
                             readBytes += readBytesTmp;
			     }

                        Debug.talkVerbose("RasHttpRequest.execute: read " + readBytes + " bytes.");

                        RasType rType = RasType.getAnyType(mddBaseType);
                        //System.out.println(rType);
			RasBaseType rb = null;

			if(rType.getClass().getName().equals("rasj.RasMArrayType"))
			    {
				RasMArrayType tmp = (RasMArrayType)rType;
				rb = tmp.getBaseType();
			    }
			else
                            {
                                Debug.talkCritical("RasHttpRequest.execute: exception: element of MDD Collection is no MArray" );
                                throw new RasClientInternalException("RasHttpRequest","execute()","element of MDD Collection is no MArray");
                            }

		        if(rb.isBaseType())
			    {
			        if(rb.isStructType())
				    {
                                        // It is a structType
				        //System.err.println("It is a structType");
                                        RasStructureType sType = (RasStructureType)rb;
                                        //System.out.println(sType);
                                        res = new RasGMArray(new RasMInterval(domain), 0);
                                        res.setTypeLength(rb.getSize());
                                        res.setArraySize(dataSize);
                                        res.setArray(binData);
					//insert into result set
					resultBag.add(res);
				        break;

				    } else
				    {
				        // It is a primitiveType
				        RasPrimitiveType pType = (RasPrimitiveType)rb;

					//System.err.println("It's a primitive type: " + pType);
                                        switch(pType.getTypeID())
					    {
					    case RAS_BOOLEAN:
					    case RAS_BYTE:
					    case RAS_CHAR:
					        //System.err.println("It's a byte array!");
                                                res = new RasMArrayByte(new RasMInterval(domain));
					        break;
                                            case RAS_SHORT:
                                                //System.err.println("It's a short array!");
                                                res = new RasMArrayShort(new RasMInterval(domain));
                                                break;

                                            case RAS_USHORT:
                                                //System.err.println("It's a ushort array!");
                                                byte[] tmData = new byte[dataSize*2];
                                                for(int i=0;i<dataSize*2;)
                                                {
                                                  tmData[i] = 0;
                                                  tmData[i+1] = 0;
                                                  tmData[i+2] = binData[i/2];
                                                  tmData[i+3] = binData[i/2+1];
                                                  i = i+SIZE_OF_INTEGER;
                                                }
                                                binData = tmData;
                                                res = new RasMArrayInteger(new RasMInterval(domain));
                                                break;

                                            case RAS_INT:
                                            case RAS_LONG:
                                                //System.err.println("It's a integer array!");
                                                res = new RasMArrayInteger(new RasMInterval(domain));
                                                break;
                                            case RAS_ULONG:
                                                //System.err.println("It's a ulong array!");
                                                byte[] tmpData = new byte[dataSize*2];
                                                for(int i=0;i<dataSize*2;)
                                                {
                                                  tmpData[i] = 0;
                                                  tmpData[i+1] = 0;
                                                  tmpData[i+2] = 0;
                                                  tmpData[i+3] = 0;
                                                  tmpData[i+4] = binData[i/2];
                                                  tmpData[i+5] = binData[i/2+1];
                                                  tmpData[i+6] = binData[i/2+2];
                                                  tmpData[i+7] = binData[i/2+3];
                                                  i = i+SIZE_OF_LONG;
                                                }
                                                binData = tmpData;
                                                res = new RasMArrayLong(new RasMInterval(domain));
                                                break;
                                            case RAS_FLOAT:
                                                //System.err.println("It's a float array!");
                                                res = new RasMArrayFloat(new RasMInterval(domain));
                                                break;
                                            case RAS_DOUBLE:
                                                //System.err.println("It's a double array!");
                                                res = new RasMArrayDouble(new RasMInterval(domain));
                                                break;
					    default:
                                                //System.err.println("It's a GMArray!");
                                                res = new RasGMArray(new RasMInterval(domain), pType.getSize());
                                                //throw new RasTypeNotSupportedException(pType.getName());
                                            }
                                            // set array data
                                            res.setArray(binData);
                                            // set oid
                                            res.setOID(roid);
                                            //insert into result set
                                            resultBag.add(res);
				    }

			    }
		        else throw new RasClientInternalException("RasHttpRequest","execute()","Type of MDD is no Base Type");
			}

		    result = resultBag;

                    // close stream
                    in.close();

                    break;

	       // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    	       case RESPONSE_SKALARS:
		 //System.err.println("Skalar = 2");
                    // read Endianess
                    while(in.read(b1) == 0);
                    endianess = b1[0];

                    // read Collection Type
                    collType = RasUtils.readString(in);
                    RasType rt = new RasType();
                    try
                    {
                      rt = rt.getAnyType(collType);
                      //System.err.println("Colltype is " + rt);
                    }
                    catch(Exception e)
                    {
                      throw new RasTypeNotSupportedException(rt + " as RasCollectionType");
                    }
                    if(rt.getTypeID()!=RasGlobalDefs.RAS_COLLECTION)
                      throw new RasTypeNotSupportedException(rt + " as RasCollectionType");

                    // read NumberOfResults
                    while(in.available() < 4);
                    in.read(b4);
                    numberOfResults = RasUtils.ubytesToInt(b4,endianess);
                    //System.err.println("Number of results: " + numberOfResults);

		    // Initailize return-list
                    resultBag = new RasBag();

                    // do this for each result
                    for(int x = 0; x < numberOfResults; x++)
                    {
                        // read elementType
                        String elementType = RasUtils.readString(in);
                        RasType et = new RasType();
                        et = ((RasCollectionType)rt).getElementType();
                        //System.err.println("ElementType is " + et);

                        // read size of binData
                        while(in.available() < 4);
                        in.read(b4);
                        dataSize = RasUtils.ubytesToInt(b4,endianess);
                        //System.err.print("Size of BinData: ");
                        //System.err.println(dataSize);

                        // read binData
                        binData = new byte[dataSize];
                        readBytes = 0;
			readBytesTmp = 0;
                        while( (readBytesTmp != -1) && (readBytes < dataSize) )
                        {
                             readBytesTmp = in.read(binData,readBytes,dataSize-readBytes);
                             readBytes += readBytesTmp;
			     /*
                             System.err.println("Read " + readBytesTmp +" Bytes ("
                                                + readBytes + " Bytes overall)");
			     */
                        }

                        ByteArrayInputStream bis = new ByteArrayInputStream(binData);
                        DataInputStream dis = new DataInputStream(bis);
                        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                        switch(et.getTypeID())
                        {
                          case RasGlobalDefs.RAS_MINTERVAL:
                            resultBag.add(new RasMInterval(new String(binData)));
                            break;
                          case RasGlobalDefs.RAS_SINTERVAL:
                            resultBag.add(new RasSInterval(new String(binData)));
                            break;
                          case RasGlobalDefs.RAS_POINT:
                            resultBag.add(new RasPoint(new String(binData)));
                            break;
                          case RasGlobalDefs.RAS_OID:
                            resultBag.add(new RasOID(new String(binData)));
                            break;
                          case RAS_BOOLEAN:
                          case RAS_BYTE:
                          case RAS_CHAR:
                            byte b = binData[0];
                            resultBag.add(new Byte(b));
                            break;
                          case RAS_DOUBLE:
                            double d = dis.readDouble();
                            resultBag.add(new Double(d));
                            break;
                          case RAS_FLOAT:
                            float f = dis.readFloat();
                            resultBag.add(new Float(f));
                            break;
                          case RAS_ULONG:
                            byte[] bu = new byte[8];
                            bu[0] = 0;
                            bu[1] = 0;
                            bu[2] = 0;
                            bu[3] = 0;
                            bu[4] = dis.readByte();
                            bu[5] = dis.readByte();
                            bu[6] = dis.readByte();
                            bu[7] = dis.readByte();
                            ByteArrayInputStream bis2 = new ByteArrayInputStream(bu);
                            DataInputStream dis2 = new DataInputStream(bis2);
                            long ul = dis2.readLong();
                            resultBag.add(new Long(ul));
                            break;
                          case RAS_LONG:
                          case RAS_INT:
                            int i = dis.readInt();
                            resultBag.add(new Integer(i));
                            break;
                          case RAS_USHORT:
                            int j = dis.readUnsignedShort();
                            resultBag.add(new Integer(j));
                            break;
                          case RAS_SHORT:
                            short s = dis.readShort();
                            resultBag.add(new Short(s));
                            break;
                          default:
                              throw new RasTypeNotSupportedException(et + " as ElementType ");
                        }
                    }
                    result = resultBag;

                    // close stream
                    in.close();
                    break;

	       //++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	       case RESPONSE_ERROR:
                    Debug.talkCritical("RasHttpRequest.execute: execution failed. Error = 0");

                    // read Endianess
                    while(in.read(b1) == 0)
                        ;
                    endianess = b1[0];

                    // read Error Number
                    while(in.available() < 4)
                        ;
                    in.read(b4);
                    int errNo = RasUtils.ubytesToInt(b4,endianess);

                    // read Line Number
                    while(in.available() < 4)
                        ;
                    in.read(b4);
                    int lineNo = RasUtils.ubytesToInt(b4,endianess);

                    // read Column Number
                    while(in.available() < 4)
                        ;
                    in.read(b4);
                    int colNo = RasUtils.ubytesToInt(b4,endianess);

                    // read token
                    String token = RasUtils.readString(in);

                    Debug.talkCritical("RasHttpRequest.execute: Errno=" + errNo + ", lineNo=" + lineNo + ", colNo=" + colNo + ", Token=" + token);

                    throw new RasQueryExecutionFailedException(errNo,lineNo,colNo,token);
 	       //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	       case RESPONSE_INT:
                    // read Integer Value
		    //System.err.println("Now reading integer value...");
                    while(in.available() < 4);
                    in.read(b4);
                    result = new Integer(RasUtils.ubytesToInt(b4,endianess));
                    //System.err.println("Int Value is : " + result.getInt());
		    break;

              //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
              case RESPONSE_OID:
                    // read Values
                    String sys = RasUtils.readString(in);
                    String base = RasUtils.readString(in);
                    double d = in.readDouble();
                    //System.out.println(sys+base+"localOID als double = " + d);
                    resultBag = new RasBag();
                    resultBag.add(new RasOID(sys, base, d));
                    result = resultBag;
                    // close stream
                    in.close();
		    break;
  	       default:
                    Debug.talkCritical( "RasHttpRequest.execute: illegal response type: " + resultType );
                    break;

	       }

            con.disconnect();				// close connection to server -- PB 2003-jun-15
	  }
        catch( MalformedURLException e )
	   {
                 Debug.leaveCritical( "RasHttpRequest.execute: leave. malformed URL: " + e.getMessage() );
		 throw new RasConnectionFailedException(MANAGER_CONN_FAILED, serverURL );
	   }
        catch( IOException e )
	   {
                 Debug.leaveCritical( "RasHttpRequest.execute: leave. IO exception: " + e.getMessage() );
                 throw new RasClientInternalException("RasHttpRequest","execute()",e.getMessage());
	   }
        catch( RasResultIsNoIntervalException e )
	   {
                 Debug.leaveCritical( "RasHttpRequest.execute: leave. result no interval: " + e.getMessage() );
                 throw new RasClientInternalException("RasHttpRequest","execute()",e.getMessage());
	   }

        rcvTimer.stopTimer();
        rcvTimer.print();

        httpTimer.stopTimer();
        httpTimer.print();

        System.out.println( "RasHttpRequest.execute: leave. resultType=" + resultType );
	} // execute()

    public static void main( String[] args )
      {
        String server = "localhost";
        String port = "7001";
        String base = "RASBASE";
        String user = "rasguest";
        String passwd = "rasguest";
        String query = "select r from RAS_COLLECTIONNAMES as r";
        int count = 1;
 
        System.out.println( "Query test started." );
 
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
 
        try
          {
            RasImplementation myApp = new RasImplementation("http://"+server+":"+port);
            myApp.setUserIdentification(user, passwd);
 
            System.out.println( "opening database..." );
            Database myDb = myApp.newDatabase();
            myDb.open( base, Database.OPEN_READ_ONLY );
 
            System.out.println( "starting transaction..." );
            Transaction myTa = myApp.newTransaction();
            myTa.begin();

            String parameters = "Command=8&ClientID=1&QueryString=" + query;
            String serverUrl = "http://" + server + ":" + 7102; // port;
 
            for (int i = 0; i < count; i++)
              {
                System.out.println( "sending query #" + i + "..." );
                execute( serverUrl, parameters );
              }
 
            System.out.println( "closing transaction..." );
            myTa.abort();
 
            System.out.println( "closing database..." );
            myDb.close();
            System.out.println( "all done." );
 
          }
        catch(Exception e)
          {
            System.err.println( e.getMessage() );
          }
 
        System.out.println( "Query test done." );
 
      } // main()

} // TestHttp

