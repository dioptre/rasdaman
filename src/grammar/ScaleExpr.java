package grammar;
/**
 * ScaleExpr
 * Creation date: (3/3/2003 2:28:43 AM)
 * @author: mattia parigiani, Sorin Stancu-Mara, Andrei Aiordachioaie
 */
public class ScaleExpr implements IParseTreeNode {

	CoverageExpr coverageExpr;
	DimensionIntervalList intervalList;
	FieldInterpolationList interpMethods;
	
	public ScaleExpr( CoverageExpr ce, DimensionIntervalList ail, FieldInterpolationList iml){
		coverageExpr = ce;
		intervalList = ail;
		interpMethods =  iml;
	}

	public String toXML(){
		String result = "";

        result += coverageExpr.toXML() + intervalList.toXML() + interpMethods.toXML();
        result = "<scale>" + result + "</scale>";

        return result;
	}
}
