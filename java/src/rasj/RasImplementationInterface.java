package rasj;

import org.odmg.*;
import java.io.*;

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
 * This interface contains the methods implemented by RasOdmgImplementation and RasRnpImplementation
 *
 *
 *
 * COMMENTS:
 *
 * </pre>
 *********************************************************** */
 
 public interface RasImplementationInterface extends Implementation 
    {    
     String getRasServer();
     
     int dbIsOpen();
     
     int getClientID();
    
     int getAccessMode();
     
     String getErrorStatus();
    
     Transaction newTransaction();
    
     Transaction currentTransaction();
     
     Database newDatabase();
    
     OQLQuery newOQLQuery();
     
     DList newDList();
     
     DBag newDBag();
     
     DSet newDSet();
     
     DArray newDArray();
     
     DMap newDMap();
     
     String getObjectId(Object obj);

     Database getDatabase(Object obj);
     
     void openDB(String name, int accessMode)throws ODMGException;
     
     void closeDB() throws ODMGException;
     
     void beginTA();
     
     boolean isOpenTA();
     
     void commitTA();
     
     void abortTA();
     
     void setMaxRetry(int newRetry);
     
     int getMaxRetry();
     
     void setUserIdentification(String userName, String plainPass);
     
     Object queryRequest(String parameters) throws RasQueryExecutionFailedException;
     }
     
