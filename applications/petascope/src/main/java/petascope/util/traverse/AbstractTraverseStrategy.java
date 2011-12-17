/*
 * JOMDoc - A Java library for OMDoc documents (http://omdoc.org/jomdoc).
 * 
 * Original author    Dimitar Misev <d.misev@jacobs-university.de>
 * Web                http://kwarc.info/dmisev/
 * Created            Jul 13, 2008, 4:25:50 PM
 * 
 * Filename           $Id: AbstractTraverseStrategy.java 1086 2009-08-08 16:49:31Z vzholudev $
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
 * Common traversing strategy, which allows a filter to be applied to the nodes when 
 * traversing the tree.
 * 
 * @author <a href="mailto:d.misev@jacobs-university.de">Dimitar Misev</a>
 */
public abstract class AbstractTraverseStrategy<T extends Object> implements TraverseStrategy<T> {

    /**
     * Specification on how to filter the elements while traversing the tree.
     * If it isn't specified, an <code>IdentityFilter</code> is assumed.
     */
    protected Filter spec;
    
    /**
     * After how many found elements should the traversing stop?
     * Note: the traversing stops only when this is positive.
     */
    protected int howMany = -1;

    /**
     * Default constructor
     */
    protected AbstractTraverseStrategy() {
        spec = new IdentityFilter();
    }

    /**
     * @param spec specification on how to filter the elements while traversing the tree
     */
    protected AbstractTraverseStrategy(Filter<T> spec) {
        if (spec != null) {
            this.spec = spec;
        } else {
            this.spec = new IdentityFilter();
        }
    }

    /**
     * @param spec specification on how to filter the elements while traversing the tree
     * @param howMany after how many found elements should the traversing stop
     */
    protected AbstractTraverseStrategy(Filter<T> spec, int howMany) {
        this(spec);
        this.howMany = howMany;
    }

    public int getHowMany() {
        return howMany;
    }

    public void setHowMany(int howMany) {
        this.howMany = howMany;
    }

    public Filter<T> getSpec() {
        return spec;
    }

    public void setSpec(Filter<T> spec) {
        this.spec = spec;
    }
}
