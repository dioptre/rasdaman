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
import rasj.global.*;
import java.util.*;

/**
 * This class implements the ODMG DBag interface.
 * @version $Revision: 1.3 $
 */
public class RasBag extends RasCollection implements DBag
  {
    static final String rcsid = "@(#)Package rasj.odmg, class RasBag: $Header: /home/rasdev/CVS-repository/rasdaman/java/rasj/odmg/RasBag.java,v 1.3 2003/12/19 15:42:56 rasdev Exp $";

    List content;

    /**
     * constructor
     */
    public RasBag()
      {
        Debug.enterVerbose( "RasBag.constructor start." );
        content = new ArrayList();
        Debug.leaveVerbose( "RasBag.constructor done." );
      }

    /**
     * Appends the specified element to this Bag.
     */
    public boolean add(Object o)
      {
        boolean result = content.add(o);
        Debug.talkVerbose( "RasBag.add: result=" + result );
        return result;
      }

    /**
     * This method returns the number of occurrences of the object obj in the DBag collection.
     */
    public int occurrences(java.lang.Object obj)
      {
        Debug.enterVerbose( "RasBag.occurrences start." );

        DBag tmpBag = new RasBag();
        tmpBag.addAll(content);
        int count = 0;
        while(tmpBag.contains(obj))
          {
            count++;
            tmpBag.remove(obj);
          }

        Debug.leaveVerbose( "RasBag.occurrences done. count=" + count );
        return count;
      }

    /**
     * A new DBag instance is created that contains the difference of this object and the DBag instance referenced by otherBag.
     */
    public DBag difference(DBag otherBag)
      {
        Debug.enterVerbose( "RasBag.difference start." );

        DBag diffBag = new RasBag();
        diffBag.addAll(otherBag);
        DBag retBag = new RasBag();
        Iterator it = content.iterator();
        Object tmp;
        while(it.hasNext())
          {
            tmp = it.next();
            if(!diffBag.contains(tmp))
                retBag.add(tmp);
            else
                diffBag.remove(tmp);
          }
        retBag.addAll(diffBag);

        Debug.leaveVerbose( "RasBag.difference done." );
        return retBag;
      }

    /**
     * A new DBag instance is created that contains the intersection of this object and the DBag referenced by otherBag.
     */
    public DBag intersection(DBag otherBag)
      {
        Debug.enterVerbose( "RasBag.intersection start." );

        DBag intBag = new RasBag();
        intBag.addAll(otherBag);
        DBag retBag = new RasBag();
        Iterator it = content.iterator();
        Object tmp;
        while(it.hasNext())
          {
            tmp = it.next();
            if(intBag.contains(tmp))
              {
                retBag.add(tmp);
                intBag.remove(tmp);
              }
          }

        Debug.leaveVerbose( "RasBag.intersection done." );
        return retBag;
      }

    /**
     * A new DBag instance is created that is the union of this object and otherBag.
     */
    public DBag union(DBag otherBag)
      {
        Debug.enterVerbose( "RasBag.union start." );

        DBag retBag = new RasBag();
        retBag.addAll(content);
        retBag.addAll(otherBag);

        Debug.leaveVerbose( "RasBag.union done." );
        return retBag;
      }

    /**
     * Returns an iterator over the elements in this Bag in proper sequence.
     */
    public Iterator iterator()
      {
        Debug.talkVerbose( "RasBag.iterator." );
        return content.iterator();
      }

    /**
     * Returns the number of elements in this Bag.
     */
    public int size()
      {
        int result = content.size();
        Debug.talkVerbose( "RasBag.size: size=" + result );
        return result;
      }

} // RasBag

