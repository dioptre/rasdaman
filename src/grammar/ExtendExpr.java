package grammar;
/**
 * ExtendExpr
 * Creation date: (8/2/2008)
 * @author: Sorin Stancu-Mara, Andrei Aiordachioaie
 */
public class ExtendExpr implements IParseTreeNode
{

	CoverageExpr coverageExpr;
	DimensionIntervalList intervalList;
	public ExtendExpr( CoverageExpr ce, DimensionIntervalList apl){
		coverageExpr = ce;
		intervalList = apl;
	}
	
	public String toXML(){
		return "<extend>" + intervalList.toXML() + coverageExpr.toXML() + "</extend>";
	}
}
