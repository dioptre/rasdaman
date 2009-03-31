package wcps.server.core;

import org.w3c.dom.*;

// TODO(smsorin): how do we do combine this ?
public class CombineRangeCoverageExprType implements IRasNode, ICoverageInfo {

    public CombineRangeCoverageExprType(Node node, ProcessCoverageRequest pcr) throws WCPSException{
	throw new WCPSException("Method not implemented");
    }
    
    public String toRasQL() { return ""; }
    public CoverageInfo getCoverageInfo() { return null; }
}