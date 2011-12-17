/*
 * JOMDoc - A Java library for OMDoc documents (http://omdoc.org/jomdoc).
 * 
 * Original author    Dimitar Misev <d.misev@jacobs-university.de>
 * Web                http://kwarc.info/dmisev/
 * Created            Jul 13, 2008, 4:53:40 PM
 * 
 * Filename           $Id: Traversable.java 1086 2009-08-08 16:49:31Z vzholudev $
 * Revision           $Revision: 1086 $
 * Last modified on   $Date: 2009-08-08 18:49:31 +0200 (Sat, 08 Aug 2009) $
 *               by   $Author: vzholudev $
 * 
 * Copyright (C) 2007,2008 the KWARC group (http://kwarc.info)
 * Licensed under the GNU Public License v3 (GPL3).
 * For other licensing contact Michael Kohlhase <m.kohlhase@jacobs-university.de>
 */
package petascope.util.traverse;

/**
 * Common representation of tree like structures (from traversing point of view)
 *
 * @param <T> the type of the nodes wrapped by this interface
 * @author <a href="mailto:d.misev@jacobs-university.de">Dimitar Misev</a>
 */
public interface Traversable<T extends Object> {

    Traversable child(int position);

    int childCount();

    Traversable parent();
    
    int indexOf(Traversable child);
    
    T getValue();
    
    boolean isTraversable();
}
