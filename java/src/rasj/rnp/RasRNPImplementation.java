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
 * COMMENTS:
 *
 * </pre>
 *********************************************************** */
 
package rasj.rnp;

import rasj.odmg.*;
import rasj.*;
import rasj.global.*;
import rasj.clientcommhttp.*;
import org.odmg.*;
import rasj.odmg.RasOQLQuery;

import java.io.*;
import java.net.*;
import java.lang.*;
import java.util.*;

public class RasRNPImplementation extends RnpBaseClientComm implements RasImplementationInterface, RasCommDefs,RasGlobalDefs
  { 
     public static final int pmt_none         = 0;
     public static final int pmt_clientid     = 1;
     public static final int pmt_rErrorString = 2;
     public static final int pmt_dbname       = 3;
     public static final int pmt_accesmode    = 4;
     public static final int pmt_queryString  = 5;
     public static final int pmt_httpqanswer  = 6;
     public static final int pmt_oidstring    = 7;
     public static final int pmt_capability   = 8;
     public static final int pmt_transstatus  = 9;
     public static final int pmt_objecttype   =10;

     public static final int cmd_none         = 0;
     public static final int cmd_connect      = 1;
     public static final int cmd_disconnect   = 2;
     public static final int cmd_opendb       = 3;
     public static final int cmd_closedb      = 4;
     public static final int cmd_beginta      = 5;
     public static final int cmd_committa     = 6;
     public static final int cmd_abortta      = 7;
     public static final int cmd_istaopen     = 8;
     public static final int cmd_queryhttp    = 9;
     public static final int cmd_getnewoid    =10;
     
     public static final int rasServerId = 3072002;

     private String rasServer            = "";
     private String rasMgr               = "";
     private int    rasMgrPort           = 7001;
     private String userIdentification   = "rasguest:8e70a429be359b6dace8b5b2500dedb0";
     private String databaseName         = "";
     private String capability           = "dummy";
     private int    maxRetry             = 5; 	// was 120; -- PB 2003-nov-20

     private RasTransaction transaction  = null;
     private RasDatabase    database     = null;
     private RasOQLQuery    query        = null;
     
     private int accessMode              = 0;
     private boolean readWrite           = false; 
     private int dbIsOpen                = 0;
     private int taIsOpen                = 0;
     private int clientID                = 0;
     
     private String errorStatus          = "";
     
     public static boolean useTurbo      = true;	// whenever possible pack multiple requests into one call 
     
     
     
     public RasRNPImplementation(String server)
       {
         super(rasServerId);

         Debug.enterVerbose( "RasRNPImplementation.RasRNPImplementation start. server=" + server );
         try
           {
	     StringTokenizer t=new StringTokenizer (server,"/");
	     String xxx=t.nextToken();
	     rasMgr =t.nextToken("/:");
	     String portStr = t.nextToken(":");
	     rasMgrPort = Integer.parseInt(portStr);
           }
         catch(NoSuchElementException e)
           {
             Debug.talkCritical( "RasRNPImplementation.RasRNPImplementation: " + e.getMessage()  );
             Debug.leaveVerbose( "RasRNPImplementation.RasRNPImplementation done: " + e.getMessage()  );
	     throw new  RasConnectionFailedException(RasGlobalDefs.URL_FORMAT_ERROR, server);
	   }
         Debug.leaveVerbose( "RasRNPImplementation.RasRNPImplementation done." );
       } // RasRNPImplementation()

     public String getErrorStatus()
       {
         Debug.talkSparse( "RasRNPImplementation.getErrorStatus: " + errorStatus );
	 return errorStatus;
       }

     public String getRasServer()
       {
         Debug.talkSparse( "RasRNPImplementation.getRasServer: " + rasServer );
         return rasServer;
       }
     
     public int dbIsOpen()
       {
         Debug.talkSparse( "RasRNPImplementation.dbIsOpen: " + dbIsOpen );
         return dbIsOpen;	
       }
     
     public int getClientID()
       {
         Debug.talkSparse( "RasRNPImplementation.getClientID: " + clientID );
	 return clientID;
       }
         
     public int getAccessMode()
       {
         Debug.talkSparse( "RasRNPImplementation.getAccessMode: " + accessMode );
	 return accessMode;
       } 
     
     public Transaction newTransaction()
       {
	 transaction = new RasTransaction(this);
         Debug.talkSparse( "RasRNPImplementation.newTransaction." );
	 return transaction;
       }
    
     public Transaction currentTransaction()
       {
         Debug.talkSparse( "RasRNPImplementation.currentTransaction." );
	 return transaction;
       }
     
     public Database newDatabase()
       {
	 database=new RasDatabase(this);
         Debug.talkSparse( "RasRNPImplementation.newDatabase." );
	 return database;
       }
    
     public OQLQuery newOQLQuery()
       {
	 query=new RasOQLQuery(this);
         Debug.talkSparse( "RasRNPImplementation.newOQLQuery." );
	 return query;
       }
     
     public DList newDList()
       {
         Debug.talkSparse( "RasRNPImplementation.newDList." );
	 return new RasList();
       }
     
     public DBag newDBag()
       {
         Debug.talkSparse( "RasRNPImplementation.newDBag." );
	 return new RasBag();
       }
     
     public DSet newDSet()
       {
         Debug.talkSparse( "RasRNPImplementation.newDSet." );
	 return new RasSet();
       }
     
     public DArray newDArray()
       {
         Debug.talkCritical( "RasRNPImplementation.newDArray: not yet implemented." );
	 throw new NotImplementedException();
       }
     
     public DMap newDMap()
       {
         Debug.talkCritical( "RasRNPImplementation.newDMap: not yet implemented." );
	 throw new NotImplementedException();
       }
	    
     public Database getDatabase(Object obj)
       {
         Debug.talkCritical( "RasRNPImplementation.getDatabase: not yet implemented." );
	 throw new NotImplementedException();
       } 
      
     public String getObjectId(Object obj)
       {
         Debug.enterVerbose( "RasRNPImplementation.getObjectId start." );
	 String oid = null;
	 if (obj instanceof RasObject)
           {
             RasOID roid = ((RasObject)obj).getOID();
             oid = roid.toString();
             if (!((RasObject)obj).getOID().isValid())
	       {
	         roid = executeGetNewObjectId();
	         oid = roid.toString();
                 ((RasObject)obj).setOID(roid);
	       }
	     else
	       {
                 Debug.leaveCritical( "RasRNPImplementation.getObjectId done. not yet implemented." );
	         throw new NotImplementedException();	  
	       }
           }
         Debug.leaveVerbose( "RasRNPImplementation.getObjectId done. oid=" + oid );
	 return oid;
       }	      
 
     public void setMaxRetry(int newRetry)
       {       
         Debug.talkVerbose( "RasRNPImplementation.setMaxRetry: setting to " + newRetry );
	 maxRetry = newRetry;
       }
     
     public int getMaxRetry()
       {
         Debug.talkVerbose( "RasRNPImplementation.getMaxRetry: is " + maxRetry );
	 return maxRetry;
       }
	           	    
     public void openDB(String name, int accessMode) throws ODMGException
       {
         Debug.enterVerbose( "RasRNPImplementation.openDB start. db=" + name + ", accessMode=" + accessMode );
	 databaseName = name;	 
	 this.accessMode = accessMode;
	 readWrite = (accessMode != Database.OPEN_READ_ONLY) ? true:false;
	 //getFreeServer(false); // fake server
	 getFreeServer(true); //Allow connections to multiple servers. Changed by: ernesto4160@gmail.com

	// "turbo" doesn't work because connect delivers the client id needed for later calls
	 // if(useTurbo==false)
	 //   {
	   executeConnect();	 
	   executeOpenDB(databaseName);	 
	   executeCloseDB();	 
	   executeDisconnect();	 
	 //   }
	 // else executeTurboOpen(name);  

	 dbIsOpen = 1;
         Debug.leaveVerbose( "RasRNPImplementation.openDB done." );
       }
	 
    public void closeDB() throws ODMGException
       {	
         Debug.talkVerbose( "RasRNPImplementation.closeDB." );
	 dbIsOpen = 0;
       }
	    
    public void beginTA() 
       {
         Debug.enterVerbose( "RasRNPImplementation.beginTA start." );
	 if(useTurbo==false)
	   {	    
	     try
	       {	 
	         getFreeServer(true);
                 executeConnect();	     
	         executeOpenDB(databaseName); 	     
	         executeBeginTA();	     
	       }
	     catch(ODMGException e)
	       {
	         errorStatus = e.getMessage();
                 Debug.talkCritical( "RasRNPImplementation.beginTA: " + errorStatus );
	       }   
	   }
	 else
	   { 
	     try
	       {	 
	         getFreeServer(true);
	         executeTurboBegin(databaseName);    
	       }
	     catch(ODMGException e)  
	       {
	         errorStatus = e.getMessage();
                 Debug.talkCritical( "RasRNPImplementation.beginTA: " + errorStatus );
	       }
	   }      
        Debug.leaveVerbose( "RasRNPImplementation.beginTA done." );
      }
	        
    public boolean isOpenTA() 
      {
        Debug.enterVerbose( "RasRNPImplementation.isOpenTA start." );
        boolean result = false;
	try 
	  {
	    if(executeIsOpenTA()!=0)
                result = true;
	  }
	catch(ODMGException e)
	  {
	    errorStatus = e.getMessage();
            Debug.talkCritical( "RasRNPImplementation.isOpenTA: " + errorStatus );
	  }  
        Debug.leaveVerbose( "RasRNPImplementation.isOpenTA done. result=" + result );
	return result;
      }
     
    public void commitTA()
      {
        Debug.enterVerbose( "RasRNPImplementation.commitTA start." );
        if(useTurbo==false)
          {
            try
	      {
	        executeCommitTA();
	        executeCloseDB();	
	        executeDisconnect();	 
	      }
	    catch(ODMGException e)  
	      {
	        errorStatus = e.getMessage();
                Debug.talkCritical( "RasRNPImplementation.commitTA: " + errorStatus );
	      }
          }
	else
	  {
	    try 
	      {
	        executeTurboCommit();
	      }
	    catch(ODMGException e)  
	      {
	        errorStatus = e.getMessage();
                Debug.talkCritical( "RasRNPImplementation.commitTA: " + errorStatus );
	      }
	   }
        Debug.leaveVerbose( "RasRNPImplementation.commitTA done." );
      }
	
    public void abortTA()
      {
        Debug.enterVerbose( "RasRNPImplementation.abortTA done." );
        if (useTurbo==false)
	  {
	    try
	      { 
	       executeAbortTA();
	       executeCloseDB();	
	       executeDisconnect();
	      }
	    catch(ODMGException e)  
	      {
	        errorStatus = e.getMessage();
                Debug.talkCritical( "RasRNPImplementation.abortTA: " + errorStatus );
	      }
	  }  
	else
	  {
	    try
	      {
	        executeTurboAbort();
	      }
	    catch(ODMGException e)
	      {
	        errorStatus = e.getMessage();
                Debug.talkCritical( "RasRNPImplementation.abortTA: " + errorStatus );
	      } 
	  }     
        Debug.leaveVerbose( "RasRNPImplementation.abortTA done." );
      }
	 
    public Object queryRequest(String parameters) throws RasQueryExecutionFailedException
      {
         Debug.talkVerbose( "RasRNPImplementation.queryRequest." );
         return executeQueryRequest(parameters);
      }
		   
    private void executeTurboOpen(String name) throws ODMGException
      {	 
        Debug.enterVerbose( "RasRNPImplementation.executeTurboOpen start. name=" + name );
	clientID = 0;
	startMessage();

	//connect
	startFragment(cmd_connect);
        encoder.addParameterInt32(pmt_clientid,clientID); 
        encoder.addParameterString(pmt_capability,capability); 
        endFragment();

	//opendb
	startFragment(cmd_opendb);
        encoder.addParameterInt32(pmt_clientid,clientID);
	encoder.addParameterString(pmt_dbname,name);	
	endFragment();

	//close db
	startFragment(cmd_closedb);
        encoder.addParameterInt32(pmt_clientid,clientID);
	endFragment();

	//disconnect
	startFragment(cmd_disconnect);	 
	encoder.addParameterInt32(pmt_clientid,clientID);	
      	endFragment();

        endMessage();

	//send message and receive answer
	turboSendRequestGetAnswer();         	 

	// connect answer
	RnpFragment fragment=decoder.getFirstFragment();
	checkForError();
	decoder.getFirstParameter();
        clientID=decoder.getDataAsInteger();	 

	//opendb answer
	fragment=decoder.getNextFragment(fragment);
	checkForError();

	//closedb answer
	fragment=decoder.getNextFragment(fragment);
	checkForError();

	//disconnect answer
	decoder.getNextFragment(fragment);
        checkForError();
	clientID = 0;
        Debug.leaveVerbose( "RasRNPImplementation.executeTurboOpen done." );
      }
     
    private void executeTurboBegin(String name) throws ODMGException
       {
        Debug.enterVerbose( "RasRNPImplementation.executeTurboBegin start. name=" + name );
	clientID = 0;
	startMessage();

	//connect
	startFragment(cmd_connect);
        encoder.addParameterInt32(pmt_clientid,clientID); 
        encoder.addParameterString(pmt_capability,capability); 
        endFragment();

	//opendb
	startFragment(cmd_opendb);
        encoder.addParameterInt32(pmt_clientid,clientID);
	encoder.addParameterString(pmt_dbname,name);	
	endFragment();

	// begin ta
	startFragment(cmd_beginta);
	encoder.addParameterInt32(pmt_clientid,clientID);
	encoder.addParameterInt32(pmt_accesmode, readWrite ? 1:0);	
	endFragment();

	endMessage();	 

	//send message and receive answer
	turboSendRequestGetAnswer();         	 	 

	// connect answer
	RnpFragment fragment=decoder.getFirstFragment();
	checkForError();	 
	decoder.getFirstParameter();
        clientID=decoder.getDataAsInteger();

	//opendb answer
	fragment=decoder.getNextFragment(fragment);
	checkForError();

	// begin ta answer
        decoder.getNextFragment(fragment);
	checkForError();
        Debug.leaveVerbose( "RasRNPImplementation.executeTurboBegin done." );
      }
	 
     private void executeTurboCommit() throws ODMGException   
       {
        Debug.enterVerbose( "RasRNPImplementation.executeTurboCommit start." );
	startMessage();

	//commit
	startFragment(cmd_committa);
	encoder.addParameterInt32(pmt_clientid,clientID);
	endFragment();

	//close
	startFragment(cmd_closedb);
        encoder.addParameterInt32(pmt_clientid,clientID);
        endFragment();

	//disconnect
	startFragment(cmd_disconnect);
	encoder.addParameterInt32(pmt_clientid,clientID);	 
        endFragment();

	endMessage();	 

	//send message and receive answer
	turboSendRequestGetAnswer();     	     	 

        //commit answer
	RnpFragment fragment=decoder.getFirstFragment();
	checkForError();

	//close answer
	fragment=decoder.getNextFragment(fragment);
 	checkForError();

	//disconnect answer
        decoder.getNextFragment(fragment);
	checkForError();
	clientID = 0;
        Debug.leaveVerbose( "RasRNPImplementation.executeTurboCommit done." );
       }
     	 
    private void executeTurboAbort() throws ODMGException
      {
        Debug.enterVerbose( "RasRNPImplementation.executeTurboAbort start." );
	startMessage();

	//abort
	startFragment(cmd_abortta);
	encoder.addParameterInt32(pmt_clientid,clientID);
        endFragment();  

	//close
	startFragment(cmd_closedb);
        encoder.addParameterInt32(pmt_clientid,clientID);
	endFragment();

	//disconnect
	startFragment(cmd_disconnect);
	encoder.addParameterInt32(pmt_clientid,clientID);	 
        endFragment();

	endMessage();	 

	//send message and receive answer
	turboSendRequestGetAnswer();         	 

	//abort answer
	RnpFragment fragment=decoder.getFirstFragment();
	checkForError();

        //close answer
	fragment=decoder.getNextFragment(fragment);
	checkForError();

        //disconnect answer 	 
        decoder.getNextFragment(fragment);
 	checkForError();
	clientID = 0;
        Debug.leaveVerbose( "RasRNPImplementation.executeTurboAbort done." );
      }

    private RasOID executeGetNewObjectId()
      {
        Debug.enterVerbose( "RasRNPImplementation.executeGetNewObjectId start." );
	startRequest(cmd_getnewoid);
        encoder.addParameterInt32(pmt_clientid,clientID);         
	encoder.addParameterInt32(pmt_objecttype,1);
	sendRequestGetAnswer();
	decoder.getFirstParameter();         
	RasOID result = new RasOID(decoder.getDataAsString());
        Debug.leaveVerbose( "RasRNPImplementation.executeGetNewObjectId done." );
        return result;
      } 
     
    private int executeIsOpenTA()throws ODMGException
      {
	startRequest(cmd_istaopen);         
	encoder.addParameterInt32(pmt_clientid,clientID);
	sendRequestGetAnswer();	 
	decoder.getFirstParameter();
        int result = decoder.getDataAsInteger();
        Debug.leaveVerbose( "RasRNPImplementation.executeIsOpenTA done. result=" + result );
        return result;
      }

//######## These functions are kept only for testing purpose, we will emove them soon ####	 	 	 
     private void executeConnect() throws ODMGException
       {
	 startRequest(cmd_connect);
         encoder.addParameterInt32(pmt_clientid,clientID); 
         encoder.addParameterString(pmt_capability,capability); 
	 sendRequestGetAnswer();
         checkForError();
	 decoder.getFirstParameter();
         clientID=decoder.getDataAsInteger();
       }
	 
     private void executeDisconnect() throws ODMGException
       {
	 startRequest(cmd_disconnect);	 
	 encoder.addParameterInt32(pmt_clientid,clientID);	 
	 sendRequestGetAnswer();
         checkForError();
        }
	 	
     private void executeOpenDB(String name) throws ODMGException
       {
	 startRequest(cmd_opendb);
         encoder.addParameterInt32(pmt_clientid,clientID);
	 encoder.addParameterString(pmt_dbname,name);	 
	 sendRequestGetAnswer();
         checkForError();
       }
	 
     private void executeCloseDB() throws ODMGException
       {
	 startRequest(cmd_closedb);
         encoder.addParameterInt32(pmt_clientid,clientID);
	 sendRequestGetAnswer();
         checkForError();
       }
	 
     private void executeBeginTA() throws ODMGException
       {
	 startRequest(cmd_beginta);         
	 encoder.addParameterInt32(pmt_clientid,clientID);
	 encoder.addParameterInt32(pmt_accesmode, readWrite ? 1:0);	 	 	 
	 sendRequestGetAnswer();	
         checkForError();
       }
     
     private void executeCommitTA()throws ODMGException
       {
	 startRequest(cmd_committa);         
	 encoder.addParameterInt32(pmt_clientid,clientID);
	 sendRequestGetAnswer();
         checkForError();
       }
	 
     private void executeAbortTA()throws ODMGException
       {
	 startRequest(cmd_abortta);         
	 encoder.addParameterInt32(pmt_clientid,clientID);
	 sendRequestGetAnswer();
         checkForError();
       }

//################################################################################     
     
    public void getFreeServer(boolean realServer)throws  RasQueryExecutionFailedException, RasConnectionFailedException
      {
        Debug.enterVerbose( "RasRNPImplementation.getFreeServer start. realServer=" + realServer );
	String uniqueID = uniqueRequestID();
       
	for (int retryCount = 0; ;retryCount++)
          {
	    try
	      {
	        executeGetFreeServer(realServer,uniqueID);
	        break;
	      }
	    catch(RasConnectionFailedException e)
	      {
	        int errno = e.getErrorNo();
	// FIXME: adapt algorithm to that of http, see RasODMGImplementation.java
	        if ( (  errno==RasGlobalDefs.MANAGER_BUSY
                //   || errno==RasGlobalDefs.NO_ACTIVE_SERVERS		// if server doesn't run waiting won't help -- PB 2003-nov-20
                     || errno==RasGlobalDefs.WRITE_TRANS_IN_PROGRESS) 
		   && retryCount < maxRetry)
	          { 
		    int millisec = 50 * retryCount + 50;
		    if(millisec > 1000)
                        millisec = 1000;
                    Debug.talkVerbose( "RasRNPImplementation.getFreeServer: retry #" + retryCount + ", sleeping " + millisec + "msecs" );
		    try
                      {
                        Thread.sleep(millisec);
                      }
                    catch(InterruptedException intex)
                      {
                        // wake up
                      }
		  }
	         else
	           {
                     Debug.talkCritical( "RasRNPImplementation.getFreeServer: " + e.getMessage() );
                     Debug.leaveVerbose( "RasRNPImplementation.getFreeServer done: " + e.getMessage() );
                     throw(e);	
	           }
	      } // catch
           } // for

        Debug.leaveVerbose( "RasRNPImplementation.getFreeServer done." );
       } // getFreeServer

    private void  executeGetFreeServer(boolean realServer, String uniqueID)
        throws  RasQueryExecutionFailedException, RasConnectionFailedException
      {
        Debug.enterVerbose( "RasRNPImplementation.executeGetFreeServer start. realServer=" + realServer + ", uniqueID=" + uniqueID );
	try
	  {
            Debug.talkVerbose( "RasRNPImplementation.executeGetFreeServer: rasmgr=" + rasMgr + ", port=" + rasMgrPort );
	    Socket socket = new Socket(rasMgr,rasMgrPort);
            Debug.talkVerbose( "RasRNPImplementation.executeGetFreeServer: socket=" + socket );
 	    PrintStream ps = new PrintStream(socket.getOutputStream());
            String accessFlag = (accessMode == Database.OPEN_READ_ONLY ? "ro":"rw");
	    String body = databaseName + " RNP " + accessFlag + ' ' +  uniqueID + " \0";
	    if (realServer)
	        ps.print("POST getfreeserver RNP/1.1\r\nAccept: text/plain\r\nContent-type: text/plain\r\n"
	                    + "User-Agent: RasDaMan Java Client1.0\r\nAuthorization: ras " + userIdentification
                            + "\r\nContent length: " + body.length()+"\r\n\r\n" + body);
	    else
	        ps.print("POST getfreeserver2 RNP/1.1\r\nAccept: text/plain\r\nContent-type: text/plain\r\n"
                            + "User-Agent: RasDaMan Java Client1.0\r\nAuthorization: ras " + userIdentification
                            + "\r\nContent length: " + body.length()+"\r\n\r\n" + body);
	    ps.flush();	     

	    BufferedReader ds = new BufferedReader(new InputStreamReader(socket.getInputStream()));
	    int resultCode = getResultCode(ds);
	    String bodyLine = getBodyLine(ds);
            Debug.talkVerbose( "RasRNPImplementation.executeGetFreeServer: bodyLine: " + bodyLine );

	    ps.close();
	    ds.close();	     
            socket.close();
            Debug.talkVerbose( "RasRNPImplementation.executeGetFreeServer: socket closed: " + socket );

            if (resultCode==200)
	      {
                StringTokenizer t=new StringTokenizer(bodyLine," ");
                String host=t.nextToken();
		String port=t.nextToken(" ");
		capability=t.nextToken(" \t\r\n\0");
		rasServer="http://" + host + ":" + port;		
		setConnectionParameters(host,Integer.parseInt(port));		
              }
	    else
              {
		StringTokenizer t=new StringTokenizer(bodyLine," ");
                String errorStr = t.nextToken();
                int errorCode = Integer.parseInt(errorStr);
                if( resultCode < 1000 )
                  {
                    Debug.talkCritical( "RasRNPImplementation.executeGetFreeServer: " + errorCode );
                    Debug.leaveVerbose( "RasRNPImplementation.executeGetFreeServer: done. errorcode=" + errorCode );
                    throw new RasConnectionFailedException(errorCode,null);
                  }
		else
                  {
                    Debug.talkCritical( "RasRNPImplementation.executeGetFreeServer: " + errorCode );
                    Debug.leaveVerbose( "RasRNPImplementation.executeGetFreeServer: done. errorcode=" + errorCode );
                    throw new RasConnectionFailedException(RasGlobalDefs.REQUEST_FORMAT_ERROR," code=" + errorCode);
                  }
              }
	  }	      
	catch(MalformedURLException e)
	  {
            Debug.talkCritical( "RasRNPImplementation.executeGetFreeServer: " + e.getMessage() );
            Debug.leaveVerbose( "RasRNPImplementation.executeGetFreeServer: done. " + e.getMessage() );
	    throw new RasConnectionFailedException(RasGlobalDefs.MANAGER_CONN_FAILED,rasMgr);
	  }
        catch(IOException e)
	  {
            Debug.talkCritical( "RasRNPImplementation.executeGetFreeServer: " + e.getMessage() );
            Debug.leaveVerbose( "RasRNPImplementation.executeGetFreeServer: done. " + e.getMessage() );
	    throw new RasClientInternalException("RasODMGImplementation","getFreeServer()",e.getMessage());
	  }
        catch(NumberFormatException e)
	  {
            Debug.talkCritical( "RasRNPImplementation.executeGetFreeServer: cannot decode integer: " + e.getMessage() );
            Debug.leaveVerbose( "RasRNPImplementation.executeGetFreeServer: done. " + e.getMessage() );
	    throw new RasClientInternalException("RasODMGImplementation","getFreeServer()",e.getMessage());
	  }
        Debug.leaveVerbose( "RasRNPImplementation.executeGetFreeServer done." );
      }

    public int getResultCode(BufferedReader ds) throws IOException
      { 
        Debug.enterVerbose( "RasRNPImplementation.getResultCode: start." );

        String s = ds.readLine();
      	StringTokenizer t = new StringTokenizer(s," ");
        String http = t.nextToken();		// FIXME: never used
        String resultString = t.nextToken(" ");
        int result = 0;
        try
          {
            result = Integer.parseInt(resultString);
          }
        catch(NumberFormatException e)
          {
            Debug.talkCritical( "RasRNPImplementation.getResultCode: cannot decode integer: " + e.getMessage() );
            result = 0;				// FIXME: set to some real error code
          }

        Debug.leaveVerbose( "RasRNPImplementation.getResultCode: done. result=" + result );
        return result;
     }

    public String getBodyLine(BufferedReader ds)throws IOException
      { 
        Debug.enterVerbose( "RasRNPImplementation.getBodyLine: start." );

        // obviously we are searching for what follows an empty line:
        for(;;)
	  {
	    String s=ds.readLine();
	    if(s==null)
              {
                Debug.talkCritical( "RasRNPImplementation.getBodyLine: done. Unexpected EOF in rasmgr answer." );
                Debug.leaveVerbose( "RasRNPImplementation.getBodyLine: done, with eof exception." );
                throw new IOException("Unexpected EOF in rasmgr answer.");
              }
	    if(s.length()==0)
                break;
	  }
        String result = ds.readLine();

        Debug.leaveVerbose( "RasRNPImplementation.getBodyLine: done. result=" + result );
        return result;
      }      

    private Object executeQueryRequest(String parameters) throws RasQueryExecutionFailedException
      {
        Debug.enterVerbose( "RasRNPImplementation.executeQueryRequest: start. parameters=" + parameters );

	startRequest(cmd_queryhttp);
	encoder.addParameterInt32(pmt_clientid,clientID);
	try 
	  { 
	    encoder.addParameterOpaque(pmt_queryString,parameters.getBytes("8859_1"));
	  }
	catch(UnsupportedEncodingException e)
	  {
            Debug.talkCritical( "RasRNPImplementation.executeQueryRequest: " + e.getMessage() );
            Debug.leaveVerbose( "RasRNPImplementation.executeQueryRequest: done, " + e.getMessage() );
	    throw new RasClientInternalException("RasRNPImplementation","executeQueryRequest()",e.getMessage());
	  }	    
	sendRequestGetAnswer();
        checkForError();
        decoder.getFirstParameter();
	Object result= getResponse(decoder.getDataOpaque());

        Debug.leaveVerbose( "RasRNPImplementation.executeQueryRequest: done. result=" + result );
	return result;
      }

    private Object getResponse(byte[] opaqueAnswer) 
        throws RasQueryExecutionFailedException	 
      {
        Debug.enterVerbose( "RasRNPImplementation.getResponse: start." );

	Object result=null;	   
	DataInputStream in =new DataInputStream(new ByteArrayInputStream(opaqueAnswer));
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
        try
	  {
	    in.read(b1);
            int resultType = b1[0];
            switch( resultType )
	      {
                case RESPONSE_OK:
	        case RESPONSE_OK_NEGATIVE:
		    //Nothing todo
		    break;

	        // +++++++++++++++++++++++++++++++++++++++++++++++++
	        case RESPONSE_MDDS:
                    // read Endianess
                    while(in.read(b1) == 0);
                    endianess = b1[0];

                    // read Collection Type
                    collType = RasUtils.readString(in);

                    // read NumberOfResults
                    while(in.available() < 4);
                    in.read(b4);
                    numberOfResults = RasUtils.ubytesToInt(b4,endianess);

		    // Initialize return-set and parameters
		    resultBag = new RasBag();
		    String mddBaseType = null;
		    String domain = null;
                    String oid = "";
                    RasOID roid = null;

                    // do this for each result
                    for(int x = 0; x < numberOfResults; x++)
                      {
                        //read mddBaseType
                        mddBaseType = RasUtils.readString(in);

                        // read spatialDomain
                        domain = RasUtils.readString(in);

                        // read OID
                        oid = RasUtils.readString(in);
                        roid = new RasOID(oid);

                        // read size of binData
                        while(in.available() < 4);
                        in.read(b4);

                        dataSize = RasUtils.ubytesToInt(b4,endianess);
			
                        // read binData
                        binData = new byte[dataSize];
                        readBytes = 0;
			readBytesTmp = 0;

                        while( (readBytesTmp != -1) && (readBytes < dataSize) )
		          {
                            readBytesTmp = in.read(binData,readBytes,dataSize-readBytes);
                            readBytes += readBytesTmp;
                          }

                        RasType rType = RasType.getAnyType(mddBaseType);
			RasBaseType rb = null;

			if(rType.getClass().getName().equals("rasj.RasMArrayType"))
			  {
			     RasMArrayType tmp = (RasMArrayType)rType;
			     rb = tmp.getBaseType();
			  }
			else
			  {
                            Debug.talkCritical( "RasRNPImplementation.getResponse: collection element is no MArray." );
                            Debug.leaveVerbose( "RasRNPImplementation.getResponse: done, with exception." );
			    throw new RasClientInternalException("RasHttpRequest","execute()","element of MDD Collection is no MArray");
                          }  
		        if(rb.isBaseType())
			  {
			    if(rb.isStructType())
                              {
                                RasStructureType sType = (RasStructureType)rb;
                                res = new RasGMArray(new RasMInterval(domain), 0);
                                res.setTypeLength(rb.getSize());
                                res.setArraySize(dataSize);
                                res.setArray(binData);
				//insert into result set
				resultBag.add(res);
				break;
                              } 
                            else
                              {
                                // It is a primitiveType
				RasPrimitiveType pType = (RasPrimitiveType)rb;
                                switch(pType.getTypeID())
				  {
                                    case RAS_BOOLEAN:
                                    case RAS_BYTE:
                                    case RAS_CHAR:
                                        res = new RasMArrayByte(new RasMInterval(domain));
					break;
                                    case RAS_SHORT:
                                        res = new RasMArrayShort(new RasMInterval(domain));
                                        break;

                                    case RAS_USHORT:
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
                                        res = new RasMArrayInteger(new RasMInterval(domain));
                                        break;
                                    case RAS_ULONG:
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
                                        res = new RasMArrayFloat(new RasMInterval(domain));
                                        break;
                                    case RAS_DOUBLE:
                                        res = new RasMArrayDouble(new RasMInterval(domain));
                                        break;
                                    default:
                                        res = new RasGMArray(new RasMInterval(domain), pType.getSize());
                                  }
                                res.setArray(binData);
                                res.setOID(roid);
                                resultBag.add(res);
                              }
			  }
		        else 
			  {
                            Debug.talkCritical( "RasRNPImplementation.getResponse: type is not base type." );
                            Debug.leaveVerbose( "RasRNPImplementation.getResponse: done, type is not base type." );
			    throw new RasClientInternalException("RasHttpRequest","execute()","Type of MDD is no Base Type");
			  }
                      } // for

		    result = resultBag;
                    in.close();

                    break;

	       // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    	       case RESPONSE_SKALARS:
                    // read Endianess
                    while(in.read(b1) == 0);
                    endianess = b1[0];
                    // read Collection Type
                    collType = RasUtils.readString(in);
                    RasType rt = new RasType();
                    try
                      {
                        rt = rt.getAnyType(collType);
                      }
                    catch(Exception e)
                      {
                        Debug.talkCritical( "RasRNPImplementation.getResponse: type not supported: " + rt );
                        Debug.leaveVerbose( "RasRNPImplementation.getResponse: done, unsupported type" );
                        throw new RasTypeNotSupportedException(rt + " as RasCollectionType");
                      }
                    if(rt.getTypeID()!=RasGlobalDefs.RAS_COLLECTION)
                      {
                        Debug.leaveCritical( "RasRNPImplementation.getResponse: done. type not supported: " + rt );
                        throw new RasTypeNotSupportedException(rt + " as RasCollectionType");
                      }

                    // read NumberOfResults
                    while(in.available() < 4)
                        ;
                    in.read(b4);
                    numberOfResults = RasUtils.ubytesToInt(b4,endianess);

		    // Initailize return-list
                    resultBag = new RasBag();

                    // do this for each result
                    for(int x = 0; x < numberOfResults; x++)
                        {
                        // read elementType
                        String elementType = RasUtils.readString(in);
                        RasType et = new RasType();
                        et = ((RasCollectionType)rt).getElementType();
                        // read size of binData
                        while(in.available() < 4)
                            ;
                        in.read(b4);
                        dataSize = RasUtils.ubytesToInt(b4,endianess);
                        // read binData
                        binData = new byte[dataSize];
                        readBytes = 0;
			readBytesTmp = 0;
                        while( (readBytesTmp != -1) && (readBytes < dataSize) )
                          {
                            readBytesTmp = in.read(binData,readBytes,dataSize-readBytes);
                            readBytes += readBytesTmp;
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
                              Debug.talkCritical( "RasRNPImplementation.getResponse: type not supported: " + et );
                              Debug.leaveVerbose( "RasRNPImplementation.getResponse: done, unsupported type." );
                              throw new RasTypeNotSupportedException(et + " as ElementType ");
                        }
                     }
                     result = resultBag;
                     // close stream
                     in.close();
                     break;

	        //++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	        case RESPONSE_ERROR:
                    // read Endianess
                    while(in.read(b1) == 0);
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
                    Debug.leaveCritical( "RasRNPImplementation.getResponse: query failed, errNo=" + errNo + ", lineNo=" + lineNo + ", colNo=" + colNo + ", token=" + token );
                    throw new RasQueryExecutionFailedException(errNo,lineNo,colNo,token);
 	        //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	        case RESPONSE_INT:
                    // read Integer Value
                    while(in.available() < 4)
                        ;
                    in.read(b4);
                    result = new Integer(RasUtils.ubytesToInt(b4,endianess));
	 	    break;

                //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                case RESPONSE_OID:
                    // read Values
                    String sys = RasUtils.readString(in);
                    String base = RasUtils.readString(in);
                    double d = in.readDouble();
                    resultBag = new RasBag();
                    resultBag.add(new RasOID(sys, base, d));
                    result = resultBag;
                    // close stream
                    in.close();
		    break;
  	        default:
                    break;
	      }
	  } 
        catch( IOException e )
	  {
            Debug.talkCritical( "RasRNPImplementation.getResponse: " + e.getMessage() );
            Debug.leaveVerbose( "RasRNPImplementation.getResponse: done, communication exception." );
            throw new RasClientInternalException("RasRNPImplementation","getResponse()",e.getMessage());
	  }          
        catch( RasResultIsNoIntervalException e )
	  {
            Debug.talkCritical( "RasRNPImplementation.getResponse: " + e.getMessage() );
            Debug.leaveVerbose( "RasRNPImplementation.getResponse: done, result not an interval." );
            throw new RasClientInternalException("RasRNPImplementation","getResponse()",e.getMessage());
	  }

        Debug.leaveVerbose( "RasRNPImplementation.getResponse: done. result=" + result );
        return result; 	  
      }

    public void setUserIdentification(String userName, String plainPass)
      { 
        Debug.enterVerbose( "RasRNPImplementation.setUserIdentification: start." );
	MD5 md5 = new MD5();
        String hex;
        md5.Init();
        md5.Update(plainPass);
        hex = md5.asHex();
        userIdentification= userName + ":" + hex;	   
        Debug.leaveVerbose( "RasRNPImplementation.setUserIdentification: done." );
      }     

    private String strHostID = null;
    static  private int    idcounter = 0;

    /**
    generate a unique request ID using IP address, some bit mangling
    is done only once, this id then is used always
    **/
    private String uniqueRequestID()
      {
        Debug.enterVerbose( "RasRNPImplementation.uniqueRequestID: start." );
        if(strHostID == null)
         {
           long hostid = 0;
           try 
             {
               InetAddress addr = InetAddress.getLocalHost();
               // Get IP Address
               byte[] ipAddr = addr.getAddress();
	   
	       for(int i=0;i<ipAddr.length; i++) 
	         { 
	           int ss = (int)ipAddr[i];
                   if(ss<0) ss = 256 + ss;
	           hostid = hostid * 256 + ss;
	         }
	     }
           catch (UnknownHostException e) 		// cannot obtain IP address?
	     {
	       Random random = new Random();		// then use other mechanism
	       hostid = random.nextInt();
             }
           idcounter = (idcounter + 1) &0xF;
                                      // it's unique enough, we don't need such a huge number
           strHostID = "" + hostid + ':' + (System.currentTimeMillis() & 0xFFFFFFF0) + idcounter;
         }
   
        Debug.leaveVerbose( "RasRNPImplementation.uniqueRequestID: done. result=" + strHostID );
        return strHostID;
      }

  } // RasRNPImplementation

//########################################################################################## 
class MD5State 
  {
    /**
    * 128-byte state
    */
    int	state[];

    /**
    * 64-bit character count (could be true Java long?)
    */
    int	count[];

    /**
    * 64-byte buffer (512 bits) for storing to-be-hashed characters
    */
    byte	buffer[];

    public MD5State() 
      {
        buffer = new byte[64];
        count = new int[2];
        state = new int[4];

        state[0] = 0x67452301;
        state[1] = 0xefcdab89;
        state[2] = 0x98badcfe;
        state[3] = 0x10325476;

        count[0] = count[1] = 0;
      }

    /** Create this State as a copy of another state */
    public MD5State (MD5State from) 
      {
        this();

        int i;

        for (i = 0; i < buffer.length; i++)
            this.buffer[i] = from.buffer[i];

        for (i = 0; i < state.length; i++)
            this.state[i] = from.state[i];

        for (i = 0; i < count.length; i++)
            this.count[i] = from.count[i];
      }
  }; // MD5State

/**
 * Implementation of RSA's MD5 hash generator
 *
 * @version	$Revision: 1.8 $
 * @author	Santeri Paavolainen <sjpaavol@cc.helsinki.fi>
 */

class MD5 
  {
  /**
   * MD5 state
   */
  MD5State	state;

  /**
   * If Final() has been called, finals is set to the current finals
   * state. Any Update() causes this to be set to null.
   */
  MD5State 	finals;

  /**
   * Padding for Final()
   */
  static byte	padding[] = 
    {
    (byte) 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };

  /**
   * Initialize MD5 internal state (object can be reused just by
   * calling Init() after every Final()
   */
  public synchronized void Init () 
    {
    state = new MD5State();
    finals = null;
    }

  /**
   * Class constructor
   */
  public MD5 () 
    {
    this.Init();
     }

  /**
   * Initialize class, and update hash with ob.toString()
   *
   * @param	ob	Object, ob.toString() is used to update hash
   *			after initialization
   */
  public MD5 (Object ob) 
    {
    this();
    Update(ob.toString());
    }

  private int rotate_left (int x, int n) 
    {
    return (x << n) | (x >>> (32 - n));
    }

  /* I wonder how many loops and hoops you'll have to go through to
     get unsigned add for longs in java */

  private int uadd (int a, int b) 
    {
    long aa, bb;
    aa = ((long) a) & 0xffffffffL;
    bb = ((long) b) & 0xffffffffL;

    aa += bb;

    return (int) (aa & 0xffffffffL);
    }

  private int uadd (int a, int b, int c) 
    {
    return uadd(uadd(a, b), c);
    }

  private int uadd (int a, int b, int c, int d) 
    {
    return uadd(uadd(a, b, c), d);
    }

  private int FF (int a, int b, int c, int d, int x, int s, int ac) 
    {
    a = uadd(a, ((b & c) | (~b & d)), x, ac);
    return uadd(rotate_left(a, s), b);
    }

  private int GG (int a, int b, int c, int d, int x, int s, int ac) 
    {
    a = uadd(a, ((b & d) | (c & ~d)), x, ac);
    return uadd(rotate_left(a, s), b);
    }

  private int HH (int a, int b, int c, int d, int x, int s, int ac) 
    {
    a = uadd(a, (b ^ c ^ d), x, ac);
    return uadd(rotate_left(a, s) , b);
    }

  private int II (int a, int b, int c, int d, int x, int s, int ac) 
    {
    a = uadd(a, (c ^ (b | ~d)), x, ac);
    return uadd(rotate_left(a, s), b);
    }

  private int[] Decode (byte buffer[], int len, int shift)  
    {
    int		out[];
    int 	i, j;

    out = new int[16];

    for (i = j = 0; j < len; i++, j += 4) 
      {
      out[i] = ((int) (buffer[j + shift] & 0xff)) |
	(((int) (buffer[j + 1 + shift] & 0xff)) << 8) |
	(((int) (buffer[j + 2 + shift] & 0xff)) << 16) |
	(((int) (buffer[j + 3 + shift] & 0xff)) << 24);
        }
     return out;
   }

  private void Transform (MD5State state, byte buffer[], int shift) 
    {
    int
      a = state.state[0],
      b = state.state[1],
      c = state.state[2],
      d = state.state[3],
      x[];

    x = Decode(buffer, 64, shift);

    /* Round 1 */
    a = FF (a, b, c, d, x[ 0],   7, 0xd76aa478); /* 1 */
    d = FF (d, a, b, c, x[ 1],  12, 0xe8c7b756); /* 2 */
    c = FF (c, d, a, b, x[ 2],  17, 0x242070db); /* 3 */
    b = FF (b, c, d, a, x[ 3],  22, 0xc1bdceee); /* 4 */
    a = FF (a, b, c, d, x[ 4],   7, 0xf57c0faf); /* 5 */
    d = FF (d, a, b, c, x[ 5],  12, 0x4787c62a); /* 6 */
    c = FF (c, d, a, b, x[ 6],  17, 0xa8304613); /* 7 */
    b = FF (b, c, d, a, x[ 7],  22, 0xfd469501); /* 8 */
    a = FF (a, b, c, d, x[ 8],   7, 0x698098d8); /* 9 */
    d = FF (d, a, b, c, x[ 9],  12, 0x8b44f7af); /* 10 */
    c = FF (c, d, a, b, x[10],  17, 0xffff5bb1); /* 11 */
    b = FF (b, c, d, a, x[11],  22, 0x895cd7be); /* 12 */
    a = FF (a, b, c, d, x[12],   7, 0x6b901122); /* 13 */
    d = FF (d, a, b, c, x[13],  12, 0xfd987193); /* 14 */
    c = FF (c, d, a, b, x[14],  17, 0xa679438e); /* 15 */
    b = FF (b, c, d, a, x[15],  22, 0x49b40821); /* 16 */

    /* Round 2 */
    a = GG (a, b, c, d, x[ 1],   5, 0xf61e2562); /* 17 */
    d = GG (d, a, b, c, x[ 6],   9, 0xc040b340); /* 18 */
    c = GG (c, d, a, b, x[11],  14, 0x265e5a51); /* 19 */
    b = GG (b, c, d, a, x[ 0],  20, 0xe9b6c7aa); /* 20 */
    a = GG (a, b, c, d, x[ 5],   5, 0xd62f105d); /* 21 */
    d = GG (d, a, b, c, x[10],   9,  0x2441453); /* 22 */
    c = GG (c, d, a, b, x[15],  14, 0xd8a1e681); /* 23 */
    b = GG (b, c, d, a, x[ 4],  20, 0xe7d3fbc8); /* 24 */
    a = GG (a, b, c, d, x[ 9],   5, 0x21e1cde6); /* 25 */
    d = GG (d, a, b, c, x[14],   9, 0xc33707d6); /* 26 */
    c = GG (c, d, a, b, x[ 3],  14, 0xf4d50d87); /* 27 */
    b = GG (b, c, d, a, x[ 8],  20, 0x455a14ed); /* 28 */
    a = GG (a, b, c, d, x[13],   5, 0xa9e3e905); /* 29 */
    d = GG (d, a, b, c, x[ 2],   9, 0xfcefa3f8); /* 30 */
    c = GG (c, d, a, b, x[ 7],  14, 0x676f02d9); /* 31 */
    b = GG (b, c, d, a, x[12],  20, 0x8d2a4c8a); /* 32 */

    /* Round 3 */
    a = HH (a, b, c, d, x[ 5],   4, 0xfffa3942); /* 33 */
    d = HH (d, a, b, c, x[ 8],  11, 0x8771f681); /* 34 */
    c = HH (c, d, a, b, x[11],  16, 0x6d9d6122); /* 35 */
    b = HH (b, c, d, a, x[14],  23, 0xfde5380c); /* 36 */
    a = HH (a, b, c, d, x[ 1],   4, 0xa4beea44); /* 37 */
    d = HH (d, a, b, c, x[ 4],  11, 0x4bdecfa9); /* 38 */
    c = HH (c, d, a, b, x[ 7],  16, 0xf6bb4b60); /* 39 */
    b = HH (b, c, d, a, x[10],  23, 0xbebfbc70); /* 40 */
    a = HH (a, b, c, d, x[13],   4, 0x289b7ec6); /* 41 */
    d = HH (d, a, b, c, x[ 0],  11, 0xeaa127fa); /* 42 */
    c = HH (c, d, a, b, x[ 3],  16, 0xd4ef3085); /* 43 */
    b = HH (b, c, d, a, x[ 6],  23,  0x4881d05); /* 44 */
    a = HH (a, b, c, d, x[ 9],   4, 0xd9d4d039); /* 45 */
    d = HH (d, a, b, c, x[12],  11, 0xe6db99e5); /* 46 */
    c = HH (c, d, a, b, x[15],  16, 0x1fa27cf8); /* 47 */
    b = HH (b, c, d, a, x[ 2],  23, 0xc4ac5665); /* 48 */

    /* Round 4 */
    a = II (a, b, c, d, x[ 0],   6, 0xf4292244); /* 49 */
    d = II (d, a, b, c, x[ 7],  10, 0x432aff97); /* 50 */
    c = II (c, d, a, b, x[14],  15, 0xab9423a7); /* 51 */
    b = II (b, c, d, a, x[ 5],  21, 0xfc93a039); /* 52 */
    a = II (a, b, c, d, x[12],   6, 0x655b59c3); /* 53 */
    d = II (d, a, b, c, x[ 3],  10, 0x8f0ccc92); /* 54 */
    c = II (c, d, a, b, x[10],  15, 0xffeff47d); /* 55 */
    b = II (b, c, d, a, x[ 1],  21, 0x85845dd1); /* 56 */
    a = II (a, b, c, d, x[ 8],   6, 0x6fa87e4f); /* 57 */
    d = II (d, a, b, c, x[15],  10, 0xfe2ce6e0); /* 58 */
    c = II (c, d, a, b, x[ 6],  15, 0xa3014314); /* 59 */
    b = II (b, c, d, a, x[13],  21, 0x4e0811a1); /* 60 */
    a = II (a, b, c, d, x[ 4],   6, 0xf7537e82); /* 61 */
    d = II (d, a, b, c, x[11],  10, 0xbd3af235); /* 62 */
    c = II (c, d, a, b, x[ 2],  15, 0x2ad7d2bb); /* 63 */
    b = II (b, c, d, a, x[ 9],  21, 0xeb86d391); /* 64 */

    state.state[0] += a;
    state.state[1] += b;
    state.state[2] += c;
    state.state[3] += d;
   }

  /**
   * Updates hash with the bytebuffer given (using at maximum length bytes from
   * that buffer)
   *
   * @param state	Which state is updated
   * @param buffer	Array of bytes to be hashed
   * @param offset	Offset to buffer array
   * @param length	Use at maximum `length' bytes (absolute
   *			maximum is buffer.length)
   */
  public void Update (MD5State stat, byte buffer[], int offset, int length) 
    {
    int	index, partlen, i, start;
    finals = null;
    /* Length can be told to be shorter, but not inter */
    if ((length - offset)> buffer.length)
      length = buffer.length - offset;
    /* compute number of bytes mod 64 */
    index = (int) (stat.count[0] >>> 3) & 0x3f;
    if ((stat.count[0] += (length << 3)) <
	(length << 3))
      stat.count[1]++;
    stat.count[1] += length >>> 29;
    partlen = 64 - index;
    if (length >= partlen) 
      {
      for (i = 0; i < partlen; i++)
	stat.buffer[i + index] = buffer[i + offset];

      Transform(stat, stat.buffer, 0);

      for (i = partlen; (i + 63) < length; i+= 64)
	Transform(stat, buffer, i);

      index = 0;
      } 
       else
         i = 0;
    /* buffer remaining input */
    if (i < length) 
       {
       start = i;
       for (; i < length; i++)
	stat.buffer[index + i - start] = buffer[i + offset];
       }
   }


  public void Update (byte buffer[], int offset, int length) 
      {
      Update(this.state, buffer, offset, length);
      }

  public void Update (byte buffer[], int length) 
     {
      Update(this.state, buffer, 0, length);
      }

  /**
   * Updates hash with given array of bytes
   *
   * @param buffer	Array of bytes to use for updating the hash
   */
  public void Update (byte buffer[]) 
      {
      Update(buffer, 0, buffer.length);
      }

  /**
   * Updates hash with a single byte
   *
   * @param b		Single byte to update the hash
   */
  public void Update (byte b) 
    {
    byte buffer[] = new byte[1];
    buffer[0] = b;

    Update(buffer, 1);
    }

  /**
   * Update buffer with given string.
   *
   * @param s		String to be update to hash (is used as
   *		       	s.getBytes())
   */
  public void Update (String s) 
    {
    byte	chars[];

    /* deprecated chars = new byte[s.length()];
       s.getBytes(0, s.length(), chars, 0);
     */
    chars = s.getBytes();

    Update(chars, chars.length);
    }

  /**
   * Update buffer with a single integer (only & 0xff part is used,
   * as a byte)
   *
   * @param i		Integer value, which is then converted to
   *			byte as i & 0xff
   */

  public void Update (int i) 
      {
      Update((byte) (i & 0xff));
      }

  private byte[] Encode (int input[], int len) 
    {
    int		i, j;
    byte	out[];

    out = new byte[len];

    for (i = j = 0; j  < len; i++, j += 4) 
      {
      out[j] = (byte) (input[i] & 0xff);
      out[j + 1] = (byte) ((input[i] >>> 8) & 0xff);
      out[j + 2] = (byte) ((input[i] >>> 16) & 0xff);
      out[j + 3] = (byte) ((input[i] >>> 24) & 0xff);
      }

    return out;
    }

  /**
   * Returns array of bytes (16 bytes) representing hash as of the
   * current state of this object. Note: getting a hash does not
   * invalidate the hash object, it only creates a copy of the real
   * state which is finalized.
   *
   * @return	Array of 16 bytes, the hash of all updated bytes
   */
  public synchronized byte[] Final () 
    {
    byte	bits[];
    int		index, padlen;
    MD5State	fin;

    if (finals == null) 
      {
      fin = new MD5State(state);

      bits = Encode(fin.count, 8);

      index = (int) ((fin.count[0] >>> 3) & 0x3f);
      padlen = (index < 56) ? (56 - index) : (120 - index);

      Update(fin, padding, 0, padlen);
      /**/
      Update(fin, bits, 0, 8);

      /* Update() sets finalds to null */
      finals = fin;
      }

    return Encode(finals.state, 16);
   }

  /**
   * Turns array of bytes into string representing each byte as
   * unsigned hex number.
   *
   * @param hash	Array of bytes to convert to hex-string
   * @return	Generated hex string
   */
  public static String asHex (byte hash[]) 
    {
    StringBuffer buf = new StringBuffer(hash.length * 2);
    int i;

    for (i = 0; i < hash.length; i++) 
      {
      if (((int) hash[i] & 0xff) < 0x10)
	buf.append("0");

      buf.append(Long.toString((int) hash[i] & 0xff, 16));
      }

    return buf.toString();
   }

  /**
   * Returns 32-character hex representation of this objects hash
   *
   * @return String of this object's hash
   */
  public String asHex () 
    {
    return asHex(this.Final());
    }
  } // MD5


