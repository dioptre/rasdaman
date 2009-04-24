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
 * This class implements the ODMG DSet interface.
 * @version $Revision: 1.7 $
 */
public class RasSet extends RasCollection implements DSet

{
    static final String rcsid = "@(#)Package rasj.odmg, class RasSet: $Header: /home/rasdev/CVS-repository/rasdaman/java/rasj/odmg/RasSet.java,v 1.7 2003/12/19 15:42:56 rasdev Exp $";

    Set content;

    /**
     * constructor
     */
    public RasSet()
    {
	content = new HashSet();
    }

    /**
     * Returns an iterator over the elements in this Set in proper sequence.
     */
    public Iterator iterator()
    {
	return content.iterator();
    }

    /**
     * Not implemented yet.
     */
    public DSet difference(DSet otherSet)
    {
	throw new NotImplementedException();
    }

    /**
     * Not implemented yet.
     */
    public DSet intersection(DSet otherSet)
    {
	throw new NotImplementedException();
    }

    /**
     * Not implemented yet.
     */
    public boolean properSubsetOf(DSet otherSet)
    {
	throw new NotImplementedException();
    }

    /**
     * Not implemented yet.
     */
    public boolean properSupersetOf(DSet otherSet)
    {
	throw new NotImplementedException();
    }

    /**
     * Not implemented yet.
     */
    public boolean subsetOf(DSet otherSet)
    {
	throw new NotImplementedException();
    }

    /**
     * Not implemented yet.
     */
    public boolean supersetOf(DSet otherSet)
    {
	throw new NotImplementedException();
    }

    /**
     * Not implemented yet.
     */
    public DSet union(DSet otherSet)
    {
	throw new NotImplementedException();
    }

    /**
     * Appends the specified element to this Bag.
     */
    public boolean add(Object element)
    {
	return content.add(element);
    }

    /**
     * Removes all of the elements from this set.
     */
    public void clear()
    {
      content.clear();
    }

    /**
     * Returns true if this set contains the specified element.
     */
    public boolean contains(Object o)
    {
      return content.contains(o);
    }

    /**
     * Returns true if this set contains no elements.
     */
    public boolean isEmpty()
    {
      return content.isEmpty();
    }

    /**
     * Removes the given element from this set if it is present.
     */
    public boolean remove(Object o)
    {
      return content.remove(o);
    }

    /**
     * Returns the number of elements in this Set.
     */
    public int size()
    {
      return content.size();
    }

}

