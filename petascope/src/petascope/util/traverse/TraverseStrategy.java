/*
 * JOMDoc - A Java library for OMDoc documents (http://omdoc.org/jomdoc).
 * 
 * Original author    Dimitar Misev <d.misev@jacobs-university.de>
 * Web                http://kwarc.info/dmisev/
 * Created            Jul 13, 2008, 4:20:59 PM
 * 
 * Filename           $Id: TraverseStrategy.java 1086 2009-08-08 16:49:31Z vzholudev $
 * Revision           $Revision: 1086 $
 * Last modified on   $Date: 2009-08-08 18:49:31 +0200 (Sat, 08 Aug 2009) $
 *               by   $Author: vzholudev $
 * 
 * Copyright (C) 2007,2008 the KWARC group (http://kwarc.info)
 * Licensed under the GNU Public License v3 (GPL3).
 * For other licensing contact Michael Kohlhase <m.kohlhase@jacobs-university.de>
 */
package petascope.util.traverse;

import java.util.List;

/**
 * Different algorithms for traversing a tree should implement this interface.
 * 
 * @param <T> the type of content a traversor will filter while traversing a tree
 * @author <a href="mailto:d.misev@jacobs-university.de">Dimitar Misev</a>
 */
public interface TraverseStrategy<T extends Object> {

    /**
     * Traverse a tree
     * 
     * @param node the node from which to start
     * @return the nodes in the tree in the order as they have been traversed.
     */
    List<T> traverse(Traversable<T> node);
}
