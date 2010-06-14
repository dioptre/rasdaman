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
import java.util.*;

/**
 * This abstract class should implement the ODMG DCollection interface but is not implemented yet,
 * please use subclass RasBag
 * @version $Revision: 1.5 $
 */
public abstract class RasCollection extends AbstractCollection implements DCollection //extends RasObject

{
    static final String rcsid = "@(#)Package rasj.odmg, class RasCollection: $Header: /home/rasdev/CVS-repository/rasdaman/java/rasj/odmg/RasCollection.java,v 1.5 2003/12/19 15:42:56 rasdev Exp $";

    public RasCollection()
    {
    }

    /**
     * Not implemented yet.
     */
    public DCollection query(String predicate)
    {
	throw new NotImplementedException();
    }

    /**
     * Not implemented yet.
     */
    public Iterator select(String predicate)
    {
	throw new NotImplementedException();
    }

    /**
     * Not implemented yet.
     */
    public boolean existsElement(String predicate)
    {
	throw new NotImplementedException();
    }

    /**
     * Not implemented yet.
     */
    public Object selectElement(String predicate)
    {
	throw new NotImplementedException();
    }

    /**
     * Not implemented yet.
     */
    public int size()
    {
    	throw new NotImplementedException();
    }

    /**
     * Not implemented yet.
     */
    public Iterator iterator()
    {
    	throw new NotImplementedException();
    }

}



