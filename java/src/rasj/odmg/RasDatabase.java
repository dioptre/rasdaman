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
 * COMMENTS:
 *
 * </pre>
 *********************************************************** */
package rasj.odmg;

import org.odmg.*;
import rasj.global.*;
import rasj.*;

/**
 * This class implements the ODMG Database interface.
 *
 * @version 1.0  (07-Apr-2000)
 *
 * @author Peter Zoller
 */
public class RasDatabase implements Database
{
    /**
     * This variable holds a reference to the RasODMGImplementation object which created 
     * this RasDatabase object
     */
    //private RasODMGImplementation rasImplementation=null;
    private RasImplementationInterface rasImplementation=null;
 
    public RasDatabase(RasImplementationInterface imp)
      { 
        Debug.enterVerbose( "RasDatabase.constructor start." );
        rasImplementation=imp;
        Debug.leaveVerbose( "RasDatabase.constructor done." );
      }

    /**
     * Opens a database on the RasDaMan server ( which has been specified when the 
     * RasODMG bootstrap object has been initialized).
     *
     * @param name Name of the database
     * @param accessMode access mode. Available options: OPEN_READ_ONLY, OPEN_READ_WRITE, OPEN_EXCLUSIVE
     */
    public void open(String name, int accessMode) throws ODMGException
      {
        Debug.enterVerbose( "RasDatabase.open start. name=" + name + ", accessmode=" + accessMode + "." );
        rasImplementation.openDB(name,accessMode);
        Debug.leaveVerbose( "RasDatabase.open done." );
      }

    /**
     * Closes an open database. At the moment, only one database can be open at
     * a given time and thus no parameter "database" is necessary here.
     */
    public void close() throws ODMGException
      {
        Debug.enterVerbose( "RasDatabase.close start." );
	rasImplementation.closeDB();	
        Debug.leaveVerbose( "RasDatabase.close done." );
      }

    /**
     * Not implemented yet.
     */
    public void bind(Object object, String name) throws ObjectNameNotUniqueException
      {
        Debug.talkCritical( "RasDatabase.bind: not yet implemented." );
	throw new NotImplementedException();
      }

    /**
     * Not implemented yet.
     */
    public Object lookup(String name) throws ObjectNameNotFoundException
      {
        Debug.talkCritical( "RasDatabase.lookup: not yet implemented." );
	throw new NotImplementedException();
      }

    /**
     * Not implemented yet.
     */
    public void unbind(String name) throws ObjectNameNotFoundException
      {
        Debug.talkCritical( "RasDatabase.unbind: not yet implemented." );
	throw new NotImplementedException();
      }

    /**
     * Not implemented yet.
     */
    public void makePersistent(Object object)
      {
        Debug.talkCritical( "RasDatabase.makePersistent: not yet implemented." );
	throw new NotImplementedException();
      }

    /**
     * Not implemented yet.
     */
    public void deletePersistent(Object object)
      {
        Debug.talkCritical( "RasDatabase.deletePersistent: not yet implemented." );
	throw new NotImplementedException();
      }

}

