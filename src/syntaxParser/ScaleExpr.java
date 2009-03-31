package syntaxParser;
/**
 * ScaleExpr
 * Creation date: (3/3/2003 2:28:43 AM)
 * @author: mattia parigiani, Sorin Stancu-Mara
 */
class ScaleExpr implements IParseTreeNode {

	CoverageExpr coverageExpr;
	AxisIntervalList intervalList;
	FieldInterpolationList interpMethods;
	
	public ScaleExpr( CoverageExpr ce, AxisIntervalList ail, FieldInterpolationList iml){	
		coverageExpr = ce;
		intervalList = ail;
		intervalList.setTag("scaleAxisSpec");
		interpMethods =  iml;
		interpMethods.setTag("scaleFieldSpec");
	}

	public String toXML(){
		return "<scale>" + intervalList.toXML() + interpMethods.toXML() + coverageExpr.toXML() + "</scale>";
	}
}
