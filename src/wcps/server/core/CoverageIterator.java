package wcps.server.core;

import org.w3c.dom.*;
import java.util.List;
import java.util.ArrayList;
import java.util.Iterator;

public class CoverageIterator implements IRasNode {
    
    private String iteratorName;
    
    private List<String> coverageNames;
    
    public CoverageIterator(Node x, ProcessCoverageRequest pcr) throws WCPSException {
	MetadataSource source = pcr.getMetadataSource();
	coverageNames = new ArrayList<String>();
	if (x.getNodeName() != "coverageIterator")
	    throw new WCPSException("Invalid cast form " + x.getNodeName() + " XML node to CoverageIterator class");
	Node it = x.getFirstChild();
	while(it != null) {
	    if (it.getNodeName().equals("#text")) {
		it = it.getNextSibling();
		continue;
	    }
	    if (it.getNodeName().equals("iteratorVar")) {
		iteratorName = it.getFirstChild().getNodeValue();
		System.err.println("Found iterator name : " + iteratorName);
	    } else if (it.getNodeName().equals("coverageName")) {
		String cn = it.getFirstChild().getNodeValue();
		System.err.println("Found coverage reference : " + cn);
		try {		
		    if (!source.coverages().contains(cn))
			throw new WCPSException("Unknown coverage " + cn);
		} catch (ResourceException e) {
		    throw new WCPSException("Cannot load coverage information!", e);
		} 
		coverageNames.add(cn);
	    }
	    it = it.getNextSibling();
	}
    }

    public Iterator<String> getCoverages() {
	return coverageNames.iterator();
    }

    public String getIteratorName() {
	return iteratorName;
    }

    public String toRasQL() {
	// TODO(smsorin) : How to translate multiple coverages?
	return coverageNames.get(0) + " AS " + iteratorName;
    }
}
