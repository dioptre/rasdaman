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
package rasj.odmg;

import org.odmg.*;
import rasj.*;
import rasj.clientcommhttp.*;
import rasj.global.*;

import java.util.*;
import java.io.*;

/**
 * This class implements the ODMG OQLQuery interface.
 *
 * @version 1.0  (07-Apr-2000)
 *
 * @author Peter Zoller
 */
public class RasOQLQuery implements OQLQuery, RasCommDefs
{
    /**
     * This variable holds a reference to the RasImplementation object which created
     * this RasOQLQuery object
     */
     
    //private RasODMGImplementation rasImplementation=null;
    private RasImplementationInterface rasImplementation=null;

    /**
     * Stores the query string.
     */
    private String queryString = "";

    /**
     * Stores the MDD parameter list
     */
    private LinkedList qParams = null;

    /**
     * Stores the number of MDD parameters
     */
    private int numberOfParams = 0;

    /**
     * Constructor
     */
    public RasOQLQuery(RasImplementationInterface imp)//RasODMGImplementation imp)
      {
        Debug.enterVerbose( "RasOQLQuery.constructor start." );
        rasImplementation=imp;
        Debug.leaveVerbose( "RasOQLQuery.constructor done." );
      }

    /**
     * Defines the query to be executed.
     *
     * @param query The OQL query string.
     */
    public void create(String query) throws QueryInvalidException
      {
        Debug.talkVerbose( "RasOQLQuery.create query=" + query );
	queryString = query;
      }

    /**
     * Binds a parameter to the query.
     */
    public void bind(Object parameter)
	throws QueryParameterCountInvalidException, QueryParameterTypeInvalidException
      {
        Debug.enterVerbose( "RasOQLQuery.bind start." );

	// add the parameter to the set
	if(qParams == null) qParams = new LinkedList();
	qParams.add(parameter);
	numberOfParams++;

        Debug.enterVerbose( "RasOQLQuery.bind done." );
      }

    /**
     * Execute the OQL query.
     */
    public Object execute() throws QueryException
      {
        Debug.enterVerbose( "RasOQLQuery.execute start." );

        BenchmarkTimer rasjQueryTimer = new BenchmarkTimer("rasjQuery");
        rasjQueryTimer.startTimer();

	String mddData = null;
	String params = null;
	String dummy = null;
	//RasHttpRequest request;
	Object item;
	StringBuffer buff;
	int offset;
	int buffIndex;
        Object result = null;

	try
          {
	    String errorMsg = "Could not execute OQL-Query: ";
	    // test if database is open
	    if(rasImplementation.dbIsOpen() == 0)
              {
                Debug.leaveVerbose( "RasQOLQuery.execute done. database not open." );
		throw new DatabaseClosedException(errorMsg + "database not open");
              }

	    // test if we have an open transaction
	    params = "ClientID=" + rasImplementation.getClientID() + "&Command=" + RasODMGGlobal.commIsOpenTA;
	     
	    /*		
	    request = new RasHttpRequest();

	    request.execute(rasImplementation.getRasServer(),params);//RasODMGGlobal.getRasServer(),params);
	    if(request.getResultType() == 98)
		throw new TransactionNotInProgressException(errorMsg + "no open transaction");            	     
	    */	
	    if(rasImplementation.isOpenTA()==false)
              {
                Debug.leaveVerbose( "RasQOLQuery.execute done. Error: no open transaction." );
	        throw new TransactionNotInProgressException(errorMsg + "no open transaction");
              }

            // test for correct number of query parameters
            StringTokenizer strTok = new StringTokenizer(queryString);
            String token = "";
            int counter = 0;
            while(strTok.hasMoreTokens())
              {
                token = strTok.nextToken();
                if(token.charAt(0) == '$')
                  {
                    try
                      {
                        if(Integer.parseInt(token.substring(1, 2)) > counter)
                        counter++;
                      }
                    catch(NumberFormatException e)
                      {
                        // should not happen!
                        Debug.leaveVerbose( "RasOQLQuery.execute done. number format exception in query parsing." );
                        throw new QueryParameterCountInvalidException("There are was a NumberFormatException while parsing the query.");
                      }
                  }
              }
            if(counter != numberOfParams)
              {
                Debug.leaveVerbose( "RasOQLQuery.execute done. number of parameters does not match query." );
		throw new QueryParameterCountInvalidException( counter + " variable(s) in the query string vs. " + numberOfParams + " parameter(s) bound to the query.");
              }

	    // process the parameters
	    dummy = queryString.trim();
	    queryString = dummy;
	    if(numberOfParams > 0)
              {
                counter = 0;
                ListIterator iter = qParams.listIterator(0);
                while (iter.hasNext())
                  {
                    counter++;
                    item = iter.next();
                    if(item instanceof RasGMArray)
                      {
                        // we have a MDD parameter => substitute the $x occurance in
                        // the query string with #MDDx# and get the transfer encoding of
                        // the MDDs. The format is specified in the file
                        // clientcommhttp/RasHttprequest.java.
                        RasGMArray mdd = (RasGMArray)item;
                        if(mddData == null)
                            mddData = utils.getTransferEncoding(mdd);
                        else
                            mddData = mddData + utils.getTransferEncoding(mdd);
                            queryString = utils.substitute(queryString,"$"+counter,"#MDD"+counter+"#");
                      }
                    else
                      {
                        // no MDD parameter => substitute each occurence of the
                        // corresponding $ parameter in the query string with the
                        // value of this parameter
                        queryString = utils.substitute(queryString,"$"+counter,item.toString());
                        iter.remove();
                        numberOfParams--;
                      }
                  }
              }
             
            // what kind of query do we have?
            // FIXME: this way you don't see it keyword is in comment, and you miss mixed case!! -- PB 2003-jun-15
            if((queryString.indexOf("select") != -1) || (queryString.indexOf("SELECT") != -1))
              {
                //select query
                params = "Command=" + RasODMGGlobal.commQueryExec + "&ClientID=" +
                         rasImplementation.getClientID() + "&QueryString=" + queryString;
              }
            else
              {
                // update query
                params = "Command=" + RasODMGGlobal.commUpdateQueryExec + "&ClientID=" +
                    rasImplementation.getClientID() + "&QueryString=" + queryString +
                    "&Endianess=" + BIG_ENDIAN + "&NumberOfQueryParameters=" +
                    numberOfParams;
                if(numberOfParams > 0)
                    params = params + "&BinDataSize=" + mddData.length() + "&BinData=" + mddData;
              }
            
            //request.execute(rasImplementation.getRasServer(),params);//RasODMGGlobal.getRasServer(),params);
            //return request.getResult();
            result = rasImplementation.queryRequest(params);
          }
        catch(RasQueryExecutionFailedException e) 
          {
            Debug.leaveVerbose( "RasOQLQuery.execute done. query execution failed: " + e.getMessage() );
            throw new QueryException(e.getMessage());
          }
        catch(IOException e2) 
          {
            Debug.leaveVerbose( "RasOQLQuery.execute done. error while generating transfer encoding: " + e2.getMessage() );
            throw new QueryException("Error while generating transfer encoding:\n" + e2.getMessage());
          }

        rasjQueryTimer.stopTimer();
        rasjQueryTimer.print();

        Debug.leaveVerbose( "RasOQLQuery.execute done. result=" + result );
        return result;
      } // execute()

} // RasOQLQuery

/*
 * Utilities
 */

abstract class utils
  {
    static String substitute(final String sourceString, final String oldString, final String newString)
      {
        Debug.enterVerbose( "utils.substitute start." );

        StringBuffer buff = new StringBuffer(sourceString);
        int offset = 0;
        int buffIndex = sourceString.indexOf(oldString,offset);
        while(buffIndex != -1)
          {
            buff.replace(buffIndex,buffIndex+oldString.length(),newString);
            offset = buffIndex+1;
            buffIndex = buff.toString().indexOf(oldString,offset);
          }

        String result = buff.toString();
        Debug.leaveVerbose( "utils.substitute done. result=" + result );
        return result;
      }

    /** returns a byte array representing the GMArray. This byte array is used for uploading query
     * parameters to the server.
     * The exact format of the byte array is described in the documentation of class
     * @see rasj.clientcommhttp.RasHttpRequest.
     */
    static String getTransferEncoding(RasGMArray mdd) throws IOException
      {
        Debug.enterVerbose( "utils.getTransferEncoding start." );

        String tileDomain = null;
        RasMInterval domain = mdd.spatialDomain();
        long typeLength = mdd.getTypeLength();

        ByteArrayOutputStream outStream = new ByteArrayOutputStream();
        DataOutputStream dStream = new DataOutputStream(outStream);

        // get tilingDomain from the storageLayout object
        if(mdd.getStorageLayout().getSpatialDomain() == null)
          {
            tileDomain = getTilingDomain(domain,typeLength,mdd.getStorageLayout());
          }
        else
            tileDomain = mdd.getStorageLayout().getSpatialDomain().toString();


        // write object type
        //dStream.writeInt((int)getObjectType());
        dStream.writeInt(1);
        dStream.writeBytes(mdd.getObjectTypeName()+String.valueOf('\0'));
        dStream.writeBytes(mdd.getTypeStructure()+String.valueOf('\0'));
        dStream.writeInt((int)typeLength);
        dStream.writeBytes(domain+String.valueOf('\0'));
        dStream.writeBytes(tileDomain+String.valueOf('\0'));
        dStream.writeBytes(mdd.getOID().toString()+String.valueOf('\0'));

        // if we have an MArray of type ulong: test that each cell value does not
        // exceed 2^32, because the server does only store 4 byte ushorts, and
        // convert the java-8-byte-long-array to a java-byte-array containing
        // only the 4 least bytes of each long value.
        if(mdd instanceof RasMArrayLong) 
          {
            int arraySize = (int)mdd.getArraySize();
            byte[] longArray = mdd.getArray();
            // we skip the first 4 bytes of each long => arraySize / 2
            dStream.writeInt(arraySize / 2);
            for(int i=0; i<arraySize; i+=RasGlobalDefs.SIZE_OF_LONG)
              {
                for(int j=0; j<4; j++)
                  {
                    // read the first 4 bytes of the former long value and check
                    // if they are 0. If not, the value is too large to fit into
                    // a 4 byte ulong => throw exception
                    if(longArray[i+j] != 0)
                      {
                        DataInputStream dis = new DataInputStream(new ByteArrayInputStream(longArray));
                        dis.skipBytes(i);
                        long illegalValue = dis.readLong();
                        Debug.leaveVerbose( "utils.getTransferEncoding done. illegal long value." );
                        throw new RasIllegalULongValueException(illegalValue);
                      }
                  }
                for(int j=4; j<RasGlobalDefs.SIZE_OF_LONG; j++)
                  {
                    // now write the next 4 bytes
                    dStream.writeByte(longArray[i+j]);
                  }
              }
           }
        // if we have an MArray of type ushort: test that each cell value does not
        // exceed 2^16, because the server does only store 2 byte ushorts, and
        // convert the java-4-byte-integer-array to a java-byte-array containing
        // only the 2 least bytes of each integer value.
        else if(mdd instanceof RasMArrayInteger && mdd.getObjectTypeName().regionMatches(0, "UShort", 0, 5)) 
          {
            int arraySize = (int)mdd.getArraySize();
            byte[] intArray = mdd.getArray();
            // we skip the first 2 bytes of each integer => arraySize / 2
            dStream.writeInt(arraySize / 2);
            for(int i=0; i<arraySize; i+=RasGlobalDefs.SIZE_OF_INTEGER)
              {
                for(int j=0; j<2; j++)
                  {
                    // read the first 2 bytes of the former integer value and check
                    // if they are 0. If not, the value is too large to fit into
                    // a 2 byte ushort => throw exception
                    if(intArray[i+j] != 0)
                      {
                        DataInputStream dis = new DataInputStream(new ByteArrayInputStream(intArray));
                        dis.skipBytes(i);
                        int illegalValue = dis.readInt();
                        Debug.leaveVerbose( "utils.getTransferEncoding done. illegal short value." );
                        throw new RasIllegalUShortValueException(illegalValue);
                      }
                  }
                for(int j=2; j<RasGlobalDefs.SIZE_OF_INTEGER; j++) 
                  {
                    // now write the next 2 bytes
                    dStream.writeByte(intArray[i+j]);
                  }
              } 
          }
        // all marrays other than ulong or ushort
        else
          {
            dStream.writeInt((int)mdd.getArraySize());
            dStream.write(mdd.getArray());
          }
        dStream.flush();

        String result = outStream.toString("8859_1");
        Debug.leaveVerbose( "utils.getTransferEncoding done. result=" + result );
        return result;
      } // getTransferEncoding()

    // calculates the tiling domain based on the original MDD, the type length and the tileSize
    // of the MDD's storageLayout.
    static String getTilingDomain(final RasMInterval originalDomain, final long typeLength, final RasStorageLayout layout)
      {
        Debug.enterVerbose( "utils.getTilingDomain start." );

        long tileSize = layout.getTileSize();
        int dim = originalDomain.dimension();
        double tmp = 1.0/dim;
        int size=(int)(Math.pow((double)(tileSize / typeLength),tmp))-1;

        String retVal = "0:"+String.valueOf(size);
        for(int x=1; x<dim; x++)
            retVal = retVal + ",0:" + size;
        retVal = "[" + retVal + "]";

        Debug.leaveVerbose( "utils.getTilingDomain done. result=" + retVal );
        return retVal;
      }

  } // utils
