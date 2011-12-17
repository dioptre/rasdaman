/*
 * JOMDoc - A Java library for OMDoc documents (http://omdoc.org/jomdoc).
 *
 * Original author    Dimitar Misev <d.misev@jacobs-university.de>
 * Web                http://kwarc.info/dmisev/
 * Created            Jul 8, 2009, 12:17:38 AM
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

/**
 * Identity filter, match all nodes.
 *
 * @author <a href="mailto:d.misev@jacobs-university.de">Dimitar Misev</a>
 */
public class IdentityFilter extends AbstractFilter {

    public boolean evaluate(Object node, int depth) {
        return true;
    }
}
