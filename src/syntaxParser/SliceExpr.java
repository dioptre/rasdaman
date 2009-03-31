package syntaxParser;
/**
 * SliceExpr
 * Creation date: (3/3/2003 2:28:43 AM)
 * @author: mattia parigiani, Sorin Stancu-Mara
 */
class SliceExpr implements IParseTreeNode {

	CoverageExpr coverageExpr;
	AxisPointList pointList;
	public SliceExpr( CoverageExpr ce, AxisPointList apl){	
		coverageExpr = ce;
		pointList = apl;
		pointList.setTag("sliceAxisSpec");
	}
	
	public String toXML(){
		return "<slice>" + pointList.toXML() + coverageExpr.toXML() + "</slice>";
	}
}
