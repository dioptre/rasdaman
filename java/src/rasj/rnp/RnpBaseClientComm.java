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
 *
 * </pre>
 *********************************************************** */
package rasj.rnp;

import rasj.*;
import rasj.global.*;
import java.io.*;
import java.net.*;


public class RnpBaseClientComm
  {  
    RnpEncoder encoder;
    RnpDecoder decoder;     
    int serverID;
    String rasServerHost;
    int    rasServerPort;
     
    public RnpBaseClientComm(int serverType)
      {
        Debug.enterVerbose( "RnpBaseClientComm.RnpBaseClientComm: start. serverType=" + serverType );
        encoder = new RnpEncoder();
	serverID = serverType;
        Debug.leaveVerbose( "RnpBaseClientComm.RnpBaseClientComm: done." );
      }
	  
   public void setConnectionParameters(String s, int p)
      {
        Debug.enterVerbose( "RnpBaseClientComm.setConnectionParameters: start. server=" + s + ", port=" + p );
        rasServerHost = s;
	rasServerPort = p;
        Debug.leaveVerbose( "RnpBaseClientComm.setConnectionParameters: done." );
      }
     
    public void startRequest(int commandCode)
      {
        Debug.enterVerbose( "RnpBaseClientComm.startRequest: start. cmdCode=" + commandCode );
	encoder.startMessage(serverID);
	encoder.startFragment(Rnp.fgt_Command,commandCode);
        Debug.leaveVerbose( "RnpBaseClientComm.startRequest: done." );
      }

    public void startMessage()
      {
        Debug.enterVerbose( "RnpBaseClientComm.startMessage: start." );
	encoder.startMessage(serverID);
        Debug.leaveVerbose( "RnpBaseClientComm.startMessage: done." );
      }	

    public void startFragment(int commandCode)
      {
        Debug.enterVerbose( "RnpBaseClientComm.startFragment: start. cmdCode=" + commandCode );
	encoder.startFragment(Rnp.fgt_Command,commandCode);
        Debug.leaveVerbose( "RnpBaseClientComm.startFragment: done." );
      } 

    public void endFragment()
      {
        Debug.enterVerbose( "RnpBaseClientComm.endFragment: start." );
	encoder.endFragment();
        Debug.leaveVerbose( "RnpBaseClientComm.endFragment: done." );
      }

    public void endMessage()
      {
        Debug.enterVerbose( "RnpBaseClientComm.endMessage: start." );
	encoder.endMessage();
        Debug.leaveVerbose( "RnpBaseClientComm.endMessage: done." );
      }	

    public void turboSendRequestGetAnswer()
      {
        Debug.enterVerbose( "RnpBaseClientComm.turboSendRequestGetAnswer: start." );
	RnpMessage answer=communicate(rasServerHost,rasServerPort,encoder.message);	 
	decoder = new RnpDecoder(answer);
        Debug.leaveVerbose( "RnpBaseClientComm.turboSendRequestGetAnswer: done." );
      }

    public void sendRequestGetAnswer()
      {
        Debug.enterVerbose( "RnpBaseClientComm.sendRequestGetAnswer: start." );

	encoder.endFragment();
	encoder.endMessage();     	
	RnpMessage answer=communicate(rasServerHost,rasServerPort,encoder.message);	 
	decoder = new RnpDecoder(answer);
	decoder.getFirstFragment();

        Debug.leaveVerbose( "RnpBaseClientComm.sendRequestGetAnswer: done." );
      }

    public void checkForError() throws RasQueryExecutionFailedException
      {
        Debug.enterVerbose( "RnpBaseClientComm.checkForError: start." );
	if(decoder.getFragmentType() != Rnp.fgt_Error)
	  { 
            Debug.leaveVerbose( "RnpBaseClientComm.checkForError: done." );
            return;
	  } 
	decoder.getFirstParameter();// error type
	int errorType = decoder.getParameterType();
	if(errorType == 3) //"ert_Other"
	  { 
	    decoder.getNextParameter();
	    String s = decoder.getDataAsString();
            Debug.leaveCritical( "RnpBaseClientComm.checkForError: done. query failed: " + s );
	    throw new RasQueryExecutionFailedException(s);
	  } 
        else
	  { 
            Debug.leaveCritical( "RnpBaseClientComm.checkForError: done. query failed, unexpected error: " + errorType );
	    throw new RasQueryExecutionFailedException(1000,0,0,"");
	  } 
        // we never get here
      } 

    public RnpMessage communicate(String server, int port, RnpMessage message)
      {
        Debug.enterVerbose( "RnpBaseClientComm.communicate: start. server=" + server + ", port=" + port );

	Socket socket=null;        
	RnpMessage receivedMessage= null;
	DataOutputStream dos = null;

        try
	  {
            Debug.talkVerbose( "RnpBaseClientComm.communicate: server=" + server + ", port=" + port );
	    socket=new Socket(server, port);       
	  }
        catch(UnknownHostException e11)  
	  {
            Debug.leaveCritical( "RnpBaseClientComm.communicate: error: socket target unknown: " + e11.getMessage() );
	    throw new RasConnectionFailedException(RasGlobalDefs.MANAGER_CONN_FAILED,server);
	  }
	catch(IOException e12)  
	  {
            Debug.leaveCritical( "RnpBaseClientComm.communicate: error: socket io exception: " + e12.getMessage() );
	    throw new RasClientInternalException("RnpBaseClientComm","communicate()",e12.getMessage());
	  }

	try
          {
            dos = new DataOutputStream(socket.getOutputStream());       
            message.write(dos);	
	  }
	catch(IOException e2)  
	  {
            Debug.leaveCritical( "RnpBaseClientComm.communicate: error: cannot write to socket: " + e2.getMessage() );
	    throw new RasClientInternalException("RnpBaseClientComm","communicate()",e2.getMessage());
	  }

        try
	  {
            DataInputStream dis = new DataInputStream(socket.getInputStream());
            receivedMessage = new RnpMessage();	
            receivedMessage.read(dis);	
	    //receivedMessage.print();
            dos.close();
            dis.close();	
	  }
        catch(IOException e3)  
	  {
            Debug.leaveCritical( "RnpBaseClientComm.communicate: error: rcv io exception: " + e3.getMessage() );
	    throw new RasClientInternalException("RnpBaseClientComm","communicate()", e3.getMessage() );
	  }
	catch(RnpException e4)  
	  {
            Debug.leaveCritical( "RnpBaseClientComm.communicate: error: RnpException: " + e4.getMessage() );
	    throw new RasClientInternalException("RnpBaseClientComm","communicate()",e4.getMessage());
	  }

        Debug.leaveVerbose( "RnpBaseClientComm.communicate: done. result=" + receivedMessage );
        return receivedMessage;
      }
           
  } // RnpBaseClientComm

