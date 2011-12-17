/*
 * JOMDoc - A Java library for OMDoc documents (http://omdoc.org/jomdoc).
 *
 * Original author    Dimitar Misev <d.misev@jacobs-university.de>
 * Web                http://kwarc.info/dmisev/
 * Created            Jan 22, 2009, 3:24:29 PM
 *
 * Filename           $Id$
 * Revision           $Revision$
 * Last modified on   $Date$
 *               by   $Author$
 *
 * Copyright (C) 2007,2008 the KWARC group (http://kwarc.info)
 * Licensed under the GNU Public License v3 (GPL3).
 * For other licensing contact Michael Kohlhase <m.kohlhase@jacobs-university.de>
 */

package petascope.util.traverse;

import java.util.ArrayList;
import java.util.List;

/**
 * Traverse throught the parents of a node.
 *
 * @author <a href="mailto:d.misev@jacobs-university.de">Dimitar Misev</a>
 */
public class ParentTraversor extends AbstractTraverseStrategy {


    public ParentTraversor() {
        super();
    }

    public ParentTraversor(Filter spec) {
        super(spec);
    }

    public ParentTraversor(Filter spec, int howMany) {
        super(spec, howMany);
    }

    public List<Object> traverse(Traversable node) {
        List<Object> ret = new ArrayList<Object>();
        traverse(node, ret);
        return ret;
    }

    private void traverse(Traversable current, List<Object> ret) {
        Traversable parent = current;
        while ((parent = parent.parent()).getValue() != null) {

            if (spec.evaluate(parent.getValue(), 0)) {
                ret.add(parent.getValue());
            }
            if (howMany > -1 && ret.size() >= howMany) {
                return;
            }
        }
    }
}
