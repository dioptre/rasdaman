package syntaxParser;
/**
 * ExtendExpr
 * Creation date: (8/2/2008)
 * @author: Sorin Stancu-Mara
 */
class ExtendExpr implements IParseTreeNode {

	CoverageExpr coverageExpr;
	AxisIntervalList intervalList;
	public ExtendExpr( CoverageExpr ce, AxisIntervalList apl){	
		coverageExpr = ce;
		intervalList = apl;
		intervalList.setTag("extendAxisSpec");
	}
	
	public String toXML(){
		return "<extend>" + intervalList.toXML() + coverageExpr.toXML() + "</extend>";
	}
}
