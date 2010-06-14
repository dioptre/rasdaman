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
 * This class implements the ODMG DList interface.
 * @version $Revision: 1.3 $
 */
public class RasList extends RasCollection implements DList

{
    static final String rcsid = "@(#)Package rasj.odmg, class RasList: $Header: /home/rasdev/CVS-repository/rasdaman/java/rasj/odmg/RasList.java,v 1.3 2003/12/19 15:42:56 rasdev Exp $";

    List content;

    /**
     * constructor
     */
    public RasList()
    {
	content = new ArrayList();
    }

    /**
     * Creates a new DList object that contains the contents of this DList object concatenated with the contents of the otherList object.
     */
    public DList concat(DList otherList)
    {
        for(int i=0; i<otherList.size(); i++)
        {
          content.add(otherList.get(i));
        }
        return this;
    }

    /**
     * Returns a view of the portion of this list between fromIndex, inclusive, and toIndex, exclusive.
     */
    public List subList(int fromIndex, int toIndex)
    {
        List l = new RasList();
        for(int j=fromIndex; j<toIndex; j++)
        {
          l.add(content.get(j));
        }
        return l;
    }

    /**
     * Returns a list iterator of the elements in this list (in proper sequence).
     */
    public ListIterator listIterator()
    {
        return content.listIterator();
    }

    /**
     * Returns an iterator over the elements in this list in proper sequence.
     */
    public Iterator iterator()
    {
        return content.iterator();
    }


    /**
     * Returns a list iterator of the elements in this list (in proper sequence), starting at the specified position in the list.
     */
    public ListIterator listIterator(int index)
    {
        return content.listIterator(index);
    }

    /**
     * Returns the index in this list of the last occurence of the specified element, or -1 if the list does not contain this element.
     */
    public int lastIndexOf(Object o)
    {
        return content.lastIndexOf(o);
    }


    /**
     * Returns the index in this list of the first occurence of the specified element, or -1 if the list does not contain this element.
     */
    public int indexOf(Object o)
    {
        return content.indexOf(o);
    }

    /**
     * Removes the element at the specified position in this list.
     */
    public Object remove(int index)
    {
        return content.remove(index);
    }

    /**
     * Appends the specified element to the end of this List.
     */
    public boolean add(Object o)
    {
      return content.add(o);
    }

    /**
     * Inserts the specified element at the specified position in this list.
     */
    public void add(int index, Object element)
    {
        content.add(index, element);
    }

    /**
     * Replaces the element at the specified position in this list with the specified element.
     */
    public Object set(int index, Object element)
    {
      return content.set(index, element);
    }

    /**
     * Returns the element at the specified position in this list.
     */
    public Object get(int index)
    {
      return content.get(index);
    }

    /**
     * Inserts all of the elements in the specified collection into this list at the specified position.
     */
    public boolean addAll(int index, Collection c)
    {
      return content.addAll(index, c);
    }

    /**
     * Returns the number of elements in this List.
     */
    public int size()
    {
      return content.size();
    }

}

