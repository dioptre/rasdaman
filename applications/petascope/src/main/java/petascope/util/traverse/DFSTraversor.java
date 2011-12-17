/*
 * JOMDoc - A Java library for OMDoc documents (http://omdoc.org/jomdoc).
 * 
 * Original author    Dimitar Misev <d.misev@jacobs-university.de>
 * Web                http://kwarc.info/dmisev/
 * Created            Jul 13, 2008, 4:45:31 PM
 * 
 * Filename           $Id: DFSTraversor.java 1252 2009-10-07 15:45:57Z dmisev $
 * Revision           $Revision: 1252 $
 * Last modified on   $Date: 2009-10-07 17:45:57 +0200 (Wed, 07 Oct 2009) $
 *               by   $Author: dmisev $
 * 
 * Copyright (C) 2007,2008 the KWARC group (http://kwarc.info)
 * Licensed under the GNU Public License v3 (GPL3).
 * For other licensing contact Michael Kohlhase <m.kohlhase@jacobs-university.de>
 */
package petascope.util.traverse;

import java.util.ArrayList;
import java.util.List;

/**
 * Do a depth first search.
 * 
 * @author <a href="mailto:d.misev@jacobs-university.de">Dimitar Misev</a>
 */
public class DFSTraversor extends AbstractTraverseStrategy {

    public DFSTraversor() {
        super();
    }

    public DFSTraversor(Filter spec) {
        super(spec);
    }

    public DFSTraversor(Filter spec, int howMany) {
        super(spec, howMany);
    }

    public List<Object> traverse(Traversable node) {
        List<Object> ret = new ArrayList<Object>();
        // #553 - consider also root node
        if (spec.evaluate(node.getValue(), 0)) {
            ret.add(node.getValue());
        }
        dfs(node, ret, 1);
        return ret;
    }

    private void dfs(Traversable current, List<Object> ret, int depth) {

        for (int i = 0; i < current.childCount(); i++) {
            Traversable child = current.child(i);
            if (child == null || !child.isTraversable()) {
                continue;
            }
            if (spec.evaluate(child.getValue(), depth)) {
                ret.add(child.getValue());
            }
            if (howMany > -1 && ret.size() >= howMany) {
                return;
            }
            dfs(child, ret, depth + 1);
        }
    }
}
