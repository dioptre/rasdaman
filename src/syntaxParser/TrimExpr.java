package syntaxParser;
/**
 * TrimExpr
 * Creation date: (3/3/2003 2:28:43 AM)
 * @author: mattia parigiani, Sorin Stancu-Mara
 */
class TrimExpr implements IParseTreeNode {

	CoverageExpr coverageExpr;
	AxisIntervalList axisIntervals;
	
	public TrimExpr( CoverageExpr ce, AxisIntervalList al ){	
		coverageExpr = ce;
		axisIntervals = al;
		axisIntervals.setTag("trimAxisSpec");
	}

	public String toXML(){
	 	return "<trim>"	+ axisIntervals.toXML() + coverageExpr.toXML() + "</trim>";
	}
}
