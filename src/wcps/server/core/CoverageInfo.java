package wcps.server.core;

import java.util.ArrayList;
import java.util.List;
import java.util.Iterator;

public class CoverageInfo {

    private List<CellDomainElement> cellDomains;
    private List<DomainElement> domains;
    

    public CoverageInfo(CoverageInfo other) {
    cellDomains = new ArrayList<CellDomainElement>();
    domains = new ArrayList<DomainElement>();
    int N = other.getNumDimensions();
    for (int i = 0; i < N; ++i) {
        cellDomains.add(other.getCellDomainElement(i));
        domains.add(other.getDomainElement(i));     
    }
    }
    
    public CoverageInfo(Metadata m) {
    cellDomains = new ArrayList<CellDomainElement>();
    domains = new ArrayList<DomainElement>();
    Iterator<CellDomainElement> itcde = m.getCellDomainIterator();
    
    while (itcde.hasNext())
        cellDomains.add (itcde.next());

    Iterator<DomainElement> itde = m.getDomainIterator();
    while (itde.hasNext())
        domains.add(itde.next());
    }    

    public boolean isCompatible(CoverageInfo other) {
    if (getNumDimensions() != other.getNumDimensions()) {
        System.err.println("The number of dimensions doesn't match");
        return false;
    }
    {
        Iterator<CellDomainElement> it = cellDomains.iterator();
        int index = 0;
        while (it.hasNext()) {
            CellDomainElement me, you;
            me = it.next();
            you = other.getCellDomainElement(index ++);
            if (!me.getHi().equals(you.getHi())) {
                System.err.println("High values don't match: " + me.getHi().toString() + ", " + you.getHi().toString());
                return false;
            }
            if (!me.getLo().equals(you.getLo())) {
                System.err.println("Low values don't match: " + me.getLo().toString() + ", " + you.getLo().toString());
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
            you = other.getDomainElement(index ++);
            if (!me.getName().equals(you.getName())) {
                System.err.println("Domain element names don't match: '" + me.getName() + "' and '"+
                                   you.getName() + "'.");
                return false;
            }
            //if (me.getName() != you.getName()) return false;
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
    
    public void removeDimension(int dim) {
    cellDomains.remove(dim);
    domains.remove(dim);
    }
    
    public void setDimension(int dim,CellDomainElement cde, DomainElement de) {
    cellDomains.set(dim, cde);
    domains.set(dim, de);
    }

    public void setCellDimension(int dim, CellDomainElement cde) {
    cellDomains.set(dim, cde);
    }
    
    public int getDomainIndexByName(String name) throws WCPSException {
    Iterator<DomainElement> it = domains.iterator();
    int index = 0;
    while (it.hasNext() ) {
        if (name.equals(it.next().getName())) {
        return index;
        }
        index ++;
    }
    throw new WCPSException("Domain name not found :" + name);
    }
    
}
