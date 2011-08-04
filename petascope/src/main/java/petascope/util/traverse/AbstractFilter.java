/*
 * JOMDoc - A Java library for OMDoc documents (http://omdoc.org/jomdoc).
 *
 * Original author    Dimitar Misev <d.misev@jacobs-university.de>
 * Web                http://kwarc.info/dmisev/
 * Created            Sep 26, 2009, 3:08:40 PM
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
import petascope.util.MiscUtil;

/**
 * All filters should extend this abstract class.
 *
 * @author <a href="mailto:d.misev@jacobs-university.de">Dimitar Misev</a>
 */
public abstract class AbstractFilter implements Filter {

    protected Filter parentFilter;
    protected List<Filter> orFilters;

    public AbstractFilter() {
        this(null);
    }

    public AbstractFilter(Filter parentFilter) {
        this.parentFilter = parentFilter;
        orFilters = new ArrayList<Filter>();
    }

    public boolean evaluate(Object node, int depth) {
        for (Filter filter : orFilters) {
            if (filter.evaluate(node, depth))
                return true;
        }
        if (parentFilter != null)
            return parentFilter.evaluate(node, depth);
        return true;
    }

    public Filter getParentFilter() {
        return parentFilter;
    }

    public void setParentFilter(Filter parentFilter) {
        this.parentFilter = parentFilter;
    }

    public List<Filter> getOrFilters() {
        return orFilters;
    }

    public void or(Filter... filters) {
        orFilters = MiscUtil.toList(filters);
    }
}
