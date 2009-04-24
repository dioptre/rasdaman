package rasj;

import rasj.odmg.*;
import rasj.global.*;		// RASJ_VERSION
import rasj.rnp.*;
import org.odmg.*;
import rasj.clientcommhttp.*;

import java.io.*;
import java.net.*;
import java.lang.*;
import java.util.*;

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
 * ODMG Implementation Bootstrap Object
 * This class implements an ODMG Bootstrap Object. This is the only vendor-dependent
 * object a user needs for performing database operations.
 * @version $Revision: 1.16 $
 *
 *
 * COMMENTS:
 *
 * </pre>
 *********************************************************** */


public class RasImplementation implements Implementation
    {
    /**
     * This is the "real" Implementation object that is used. Because it contains a lot of
     * internal methods that have to be used by the rasj.odmg package, it has been put
     * into that package in order to avoid making this internal methods publicly visible and
     * available.
     **/
    private RasImplementationInterface imp = null;
    
    private static boolean isRNP           = true; // use RNP by default

    /**
     * C/s protocol indicators.
     */
    public static final String PROTOCOL_RNP  = "RNP" ;	// RNP protocol indicator
    public static final String PROTOCOL_RPC  = "RPC" ;	// RPC protocol indicator
    public static final String PROTOCOL_HTTP = "HTTP" ;	// HTTP protocol indicator
    public static final String PROTOCOL_COMP = "COMPAT" ;	// "compatiblity", means: using RNP (legacy)

    /**
     * Standard constructor.
     * @param server - Complete URL of the RasDaMan httpserver (including port number)
     */
    public RasImplementation(String server)
    {   
        Debug.talkCritical( RasGlobalDefs.RASJ_VERSION );
        Debug.talkCritical( " Using server " + server );

        String envVar = System.getProperty("RMANPROTOCOL");	// uses "-D" option  
       
        boolean useRNP = isRNP;
       
        if(envVar != null)
          {
            Debug.talkWarning( "environment variable RMANPROTOCOL enforces protocol " + envVar );
	    if ( envVar.equalsIgnoreCase( PROTOCOL_RNP ) )
                useRNP = true;
	    else if ( envVar.equalsIgnoreCase( PROTOCOL_RPC ) )
            {
	        Debug.talkCritical( "Error: protocol " + PROTOCOL_RPC + " not supported by rasj: using " + PROTOCOL_RNP + " instead." ); 
                useRNP = true;
            }
            else if ( envVar.equalsIgnoreCase( PROTOCOL_HTTP ) )
                useRNP = false;
            else if ( envVar.equalsIgnoreCase( PROTOCOL_COMP ) )
            {
	        Debug.talkCritical( "Compatibility mode specified, using " + PROTOCOL_RNP + "." ); 
                useRNP = true;
            }
            else
            {
                Debug.talkCritical( "Error: unknown protocol: " + envVar + "; using protocol " + PROTOCOL_RNP + "." );
                useRNP = true;
            }
	  }  

        if(useRNP) 
          {	  
	    Debug.talkVerbose( "RasImplementation.constructor: using protocol " + PROTOCOL_RNP + "." ); 
	    imp = new RasRNPImplementation(server);
	  }
        else
          {
	    Debug.talkVerbose( "RasImplementation.constructor: using protocol " + PROTOCOL_HTTP + "." ); 
            imp = new RasODMGImplementation(server);	
	  }	    
       } // RasImplementation()

    /**
     * Extended constructor, allowing to set protocol (does not query env var).
     * If the protocol is illegal, RNP is assumed as default.
     * @param server - Complete URL of the RasDaMan httpserver (including port number)
     * @param protocol - c/s communication protocol selector, one of: "RNP", "HTTP"
     */
    public RasImplementation(String server, String protocol)
    {
        Debug.talkCritical( RasGlobalDefs.RASJ_VERSION );
        Debug.talkCritical( " Using server " + server );

        if( protocol.equalsIgnoreCase( PROTOCOL_RNP ) )
          {
            Debug.talkVerbose( "Using protocol " + PROTOCOL_RNP + "." );
            imp = new RasRNPImplementation(server);
          }
        else if( protocol.equalsIgnoreCase( PROTOCOL_HTTP ) )
          {
            Debug.talkVerbose( "Using protocol " + PROTOCOL_HTTP + "." );
            imp = new RasODMGImplementation(server);
          }
        else
          {
            Debug.talkCritical( "Error: unknown protocol: " + protocol + "; using " + PROTOCOL_RNP + " instead." );
            imp = new RasRNPImplementation(server);
          }

       } // RasImplementation()

     public static void setRNP()
       {
	 isRNP = true;
       }

     public static void setHTTP()
       {
	 isRNP = false;
       }

     public boolean isDefaultRNP()
       {
	 Debug.talkVerbose( "RasImplementation.isDefaultRNP: RNP=" + isRNP + "." ); 
         return isRNP;
       }

    /**
     *  returns 1 if an openDB command is executed (closeDB sets it back to 0).
     */
    public int dbIsOpen()
       {
         return imp.dbIsOpen();
       }

    /*
     * Create a new transaction object and associate it with the current thread.
     */
    public Transaction newTransaction()
      {
         return imp.newTransaction();
      }

    /**
     * Get current transaction for thread, or NULL if none.
     */
    public Transaction currentTransaction()
      {
	 return imp.currentTransaction();
      }

    /**
     * Create a new database object.
     */
    public Database newDatabase()
      {
	 return imp.newDatabase();
      }

    /**
     * Create a new query object.
     */
    public OQLQuery newOQLQuery()
        {
	return imp.newOQLQuery();
        }

    /**
     * Create a new DList object.
     */
    public DList newDList()
        {
	return imp.newDList();
        }

    /**
     * Create a new DBag object.
     */
    public DBag newDBag()
        {
	return imp.newDBag();
        }

    /**
     * Create a new DSet object.
     */
    public DSet newDSet()
        {
        return imp.newDSet();
        }

    /**
     * Not implemented yet.
     */
    public DArray newDArray()
        {
	throw new NotImplementedException();
        }

    /**
     * Not implemented yet.
     */
    public DMap newDMap()
        {
	throw new NotImplementedException();
        }

    /**
     * Get a String representation of the object's identifier.
     */
    public String getObjectId(Object obj)
        {
	return imp.getObjectId(obj);
        }

    /**
     * Not implemented yet.
     */
    public Database getDatabase(Object obj)
        {
	throw new NotImplementedException();
        }

   /**
    * Open database
    */
    public void openDB(String name, int accessMode) throws ODMGException
        {
	imp.openDB(name,accessMode);
        }

    /**
     * Closes an open database. At the moment, only one database can be open at
     * a given time and thus no parameter "database" is necessary here.
     */
    public  void closeDB() throws ODMGException
        {
	imp.closeDB();
        }

    /**
     * Begin a transaction.
     */
    public void beginTA()throws ODMGException
        {
	imp.beginTA();
        }

    /**
     * Returns TRUE if a transaction is currently open.
     */
    public boolean isOpenTA()throws ODMGException
        {
        Debug.talkCritical( "RasImplementation::isOpenTA: calling imp.isOpenTA()" );
	return imp.isOpenTA();
        }

    /**
     * Commit a transaction.
     */
    public void commitTA()throws ODMGException
        {
	imp.commitTA();
        }

    /**
     * Abort a transaction.
     */
    public void abortTA()throws ODMGException
        {
	imp.abortTA();
        }

    /**
     * Set the maximum retry parameter
     */
    public void setMaxRetry(int newRetry)
       {
       imp.setMaxRetry(newRetry);
       }

    /**
     * Get the maximum retry parameter
     */
    public int getMaxRetry()
       {
       return imp.getMaxRetry();
       }

   /**
    * Set user identification : name/plain password
    * (default is rasguest/rasguest)
    */
   public void setUserIdentification(String userName, String plainPass)
     {
     imp.setUserIdentification(userName,plainPass);
     }

    /**
     * Set trace output threshold
     * (0 = minimal, 4 = verbose; 1 = default)
     */
    public void setTraceThreshold( int level )
       {
	Debug.talkCritical( "setting trace level to " + level );
       rasj.global.Debug.setDebugThreshold( level );
       }

}


