/*
 * JOMDoc - A Java library for OMDoc documents (http://omdoc.org/jomdoc).
 * 
 * Original author    Dimitar Misev <d.misev@jacobs-university.de>
 * Web                http://kwarc.info/dmisev/
 * Created            Jul 13, 2008, 9:09:02 PM
 * 
 * Filename           $Id: BDFSTraversor.java 1086 2009-08-08 16:49:31Z vzholudev $
 * Revision           $Revision: 1086 $
 * Last modified on   $Date: 2009-08-08 18:49:31 +0200 (Sat, 08 Aug 2009) $
 *               by   $Author: vzholudev $
 * 
 * Copyright (C) 2007,2008 the KWARC group (http://kwarc.info)
 * Licensed under the GNU Public License v3 (GPL3).
 * For other licensing contact Michael Kohlhase <m.kohlhase@jacobs-university.de>
 */
package petascope.util.traverse;

import java.util.ArrayList;
import java.util.List;

/**
 * A kind of backwards DFS (from the leafs to the root), not going deeper (actually shallower) 
 * than the depth of the given element. E.g. in the following tree:
 *
 * 0
 *  1
 *   1.1
 *   1.2
 *     1.2.1
 *     1.2.2
 *   1.3
 *  2
 *   2.1
 *     2.1.1
 *   2.2
 * 
 * if the current node is 2.1, the order of traversing is:
 * 
 * 2.1 -> 2 -> 1.3 -> 1.2 -> 1.1 -> 1 -> 0
 * 
 * Note: shallower than 2.1 is 2.1.1 and deeper is 2
 * 
 * @author <a href="mailto:d.misev@jacobs-university.de">Dimitar Misev</a>
 */
public class BDFSTraversor extends AbstractTraverseStrategy {

    private List<Object> ret;
    
    private int maxDepth = 0;

    public BDFSTraversor(Filter spec, int howMany, int maxDepth) {
        super(spec, howMany);
        this.maxDepth = maxDepth;
    }

    public BDFSTraversor(Filter spec, int howMany) {
        super(spec, howMany);
    }

    public BDFSTraversor(Filter spec) {
        super(spec);
    }

    public BDFSTraversor() {
        super();
    }

    public List<Object> traverse(Traversable node) {
        ret = new ArrayList<Object>();
        bdfs(null, node, 0, 0);
        return ret;
    }

    private void bdfs(Traversable currParent, Traversable curr, int currIndex, int depth) {
        if (howMany > -1 && ret.size() >= howMany) {
            return; // it was enough

        }
        if (spec.evaluate(curr.getValue(), depth)) {
            ret.add(curr.getValue());
        }


        // shallower
        if (depth > maxDepth && currParent != null) {
            int nextIndex = curr.childCount();
            Traversable child = null;
            do {
                if (--nextIndex < 0) {
                    break;
                }
                child = curr.child(nextIndex);
            } while (!child.isTraversable());
            if (nextIndex >= 0) {
                bdfs(curr, child, nextIndex, depth - 1);
            }
        }


        // previous sibling
        Traversable newParent = currParent;
        if (currIndex == -2 || currParent == null) {
            newParent = curr.parent();
            if (newParent != null) {
                currIndex = newParent.indexOf(curr);
            } else {
                return; // curr is the root

            }
        }
        if (currIndex > 0) {
            int nextIndex = currIndex;
            Traversable child = null;
            do {
                if (--nextIndex < 0) {
                    break;
                }
                child = newParent.child(nextIndex);
            } while (!child.isTraversable());
            if (nextIndex >= 0) {
                bdfs(newParent, child, nextIndex, depth);
            }
        }


        // deeper
        if (currParent == null) {
            if (newParent.isTraversable()) {
                bdfs(null, newParent, -2, depth + 1);
            }
        }
    }
}
