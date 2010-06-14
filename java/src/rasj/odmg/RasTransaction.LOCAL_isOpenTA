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
import rasj.*;
import rasj.global.*;
import rasj.clientcommhttp.*;

/**
 * This class implements the ODMG Transaction interface.
 *
 * @version 1.0  (07-Apr-2000)
 *
 * @author Peter Zoller
 */
public class RasTransaction implements Transaction
{
    /**
     * This variable holds a reference to the RasODMGImplementation object which created 
     * this RasDatabase object
     */
    //private RasODMGImplementation rasImplementation=null;
    private RasImplementationInterface rasImplementation=null;
    
    /**
     * local state keeper whether TA is open;
     * serves to optimize OQL queries: special method isOpenLocally() does not require server calls
     */
    private boolean isOpenTA = false;

    public RasTransaction(RasImplementationInterface imp)
    {
      rasImplementation=imp;
      isOpenTA = false;			// local state keeper, maintained by TA methods
     }

    /**
     * Not implemented yet.
     */
    public void join()
    {
        Debug.talkCritical( "RasTransaction::join: not yet implemented." );
	throw new NotImplementedException();
    }

    /**
     * Not implemented yet.
     */
    public void leave()
    {
        Debug.talkCritical( "RasTransaction::leave: not yet implemented." );
	throw new NotImplementedException();
    }

    /**
     * Begin a transaction.
     */
    public void begin()
    {
        rasImplementation.beginTA(); 
        isOpenTA = true;		// TA is considered open if everything went fine.
    }

    /**
     * Returns TRUE if a transaction is currently open.
     */
    public boolean isOpen()
    {
        boolean result = rasImplementation.isOpenTA();  
        Debug.talkVerbose( "RasTransaction::isOpen: result=" + result );
        return result;  
    }

    /**
     * Returns TRUE if a transaction is supposed to be open as seen from client side.
     * This is an optimization to save one server call within query execution,
     * it is NOT an official interface method.
     */
    public boolean isOpenLocally()
    {
        boolean result = isOpenTA;
        Debug.talkVerbose( "RasTransaction::isOpenLocally: result=" + result );
        return result;
    }

    /**
     * Commit a transaction.
     */
    public void commit()
    {
        isOpenTA = false;		// if commit fails something went wrong, so consider TA closed anyway
        rasImplementation.commitTA();
    }

    /**
     * Abort a transaction.
     */
    public void abort()
    {
        isOpenTA = false;		// if abort tails something went wrong, so consider TA closed anyway
        rasImplementation.abortTA();    
    }

    /**
     * Not implemented yet.
     */
    public void checkpoint()
    {
        Debug.talkCritical( "RasTransaction::checkpoint: not yet implemented." );
	throw new NotImplementedException();
    }

    /**
     * Not implemented yet.
     */
    public void lock(Object obj, int mode) throws LockNotGrantedException
    {
        Debug.talkCritical( "RasTransaction::lock: not yet implemented." );
	throw new NotImplementedException();
    }

    /**
     * Not implemented yet.
     */
    public boolean tryLock(Object obj, int mode)
    {
        Debug.talkCritical( "RasTransaction::tryLock: not yet implemented." );
	throw new NotImplementedException();
    }

    /**
     * Returns the errorStatus.
     */

    public String getErrorStatus()
    {
	String result = rasImplementation.getErrorStatus();
        Debug.talkSparse( "RasTransaction::getErrorStatus: status=" + result );
	return result;
    }

} // RasTransaction
