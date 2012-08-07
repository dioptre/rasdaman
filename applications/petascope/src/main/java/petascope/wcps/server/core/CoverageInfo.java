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
 * Copyright 2003 - 2010 Peter Baumann / rasdaman GmbH.
 *
 * For more information please see <http://www.rasdaman.org>
 * or contact Peter Baumann via <baumann@rasdaman.com>.
 */
package petascope.wcps.server.core;

import petascope.core.Metadata;
import petascope.exceptions.WCPSException;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import petascope.util.WCPSConstants;

public class CoverageInfo {
    
    private static Logger log = LoggerFactory.getLogger(CoverageInfo.class);

    private List<CellDomainElement> cellDomains;
    private List<DomainElement> domains;
    private String coverageName;
    private Bbox bbox;

    public CoverageInfo(CoverageInfo other) {
        cellDomains = new ArrayList<CellDomainElement>();
        domains = new ArrayList<DomainElement>();
        int N = other.getNumDimensions();

        for (int i = 0; i < N; ++i) {
            cellDomains.add(other.getCellDomainElement(i));
            domains.add(other.getDomainElement(i));
        }

        coverageName = other.getCoverageName();        
        bbox = other.getBbox();
    }

    public CoverageInfo(Metadata m) {
        cellDomains = new ArrayList<CellDomainElement>();
        domains = new ArrayList<DomainElement>();
        Iterator<CellDomainElement> itcde = m.getCellDomainIterator();

        while (itcde.hasNext()) {
            cellDomains.add(itcde.next());
        }

        Iterator<DomainElement> itde = m.getDomainIterator();

        while (itde.hasNext()) {
            domains.add(itde.next());
        }

        coverageName = m.getCoverageName();
        bbox = m.getBbox();        
    }

    public boolean isCompatible(CoverageInfo other) {
        if (getNumDimensions() != other.getNumDimensions()) {
            log.error(WCPSConstants.ERRTXT_NUMBER_DIM_DOES_NOT_MATCH);
            return false;
        }

        {
            Iterator<CellDomainElement> it = cellDomains.iterator();
            int index = 0;

            while (it.hasNext()) {
                CellDomainElement me, you;

                me = it.next();
                you = other.getCellDomainElement(index++);

                if (!me.getHi().equals(you.getHi())) {
                    log.error(WCPSConstants.ERRTXT_HIGH_VALUES_DONOT_MATCH + ": "
                            + me.getHi().toString() + ", "
                            + you.getHi().toString());
                    return false;
                }

                if (!me.getLo().equals(you.getLo())) {
                    log.error(WCPSConstants.ERRTXT_LOW_VALUES_DONOT_MATCH + ": "
                            + me.getLo().toString() + ", "
                            + you.getLo().toString());
                    return false;
                }
            }
        }
        {
            Iterator<DomainElement> it = domains.iterator();
            int index = 0;

            while (it.hasNext()) {
                DomainElement me, you;

                me = it.next();
                you = other.getDomainElement(index++);

                if (!me.getName().equals(you.getName())) {
                    log.error(WCPSConstants.ERRTXT_DOMAIN_ELEMENT_DONNOT_MATCH + ": '"
                            + me.getName() + "' " + WCPSConstants.MSG_AND + " '"
                            + you.getName() + "'.");
                    return false;
                }

                // if (me.getName() != you.getName()) return false;
            }
        }
        return true;
    }

    int getNumDimensions() {
        return cellDomains.size();
    }

    public CellDomainElement getCellDomainElement(int dim) {
        return cellDomains.get(dim);
    }

    public DomainElement getDomainElement(int dim) {
        return domains.get(dim);
    }

    public String getCoverageName() {
        return coverageName;
    }
    
    public Bbox getBbox() {
        return bbox;
    }

    public void removeDimension(int dim) {
        cellDomains.remove(dim);
        domains.remove(dim);
    }

    public void setDimension(int dim, CellDomainElement cde, DomainElement de) {
        cellDomains.set(dim, cde);
        domains.set(dim, de);
    }

    public void setCellDimension(int dim, CellDomainElement cde) {
        cellDomains.set(dim, cde);
    }

    public int getDomainIndexByName(String name) throws WCPSException {
        Iterator<DomainElement> it = domains.iterator();
        int index = 0;

        while (it.hasNext()) {
            if (name.equals(it.next().getName())) {
                return index;
            }

            index++;
        }

        log.error(WCPSConstants.ERRTXT_AXIS_NAME_NOT_FOUND + ": " + name);
        throw new WCPSException(WCPSConstants.ERRTXT_DOMAIN_NAME_NOT_FOUND + ": " + name);
    }

    @Override
    public String toString() {
        return coverageName;
    }
    
}
