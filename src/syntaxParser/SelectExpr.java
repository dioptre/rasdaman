package syntaxParser;
/**
 * SelectExpr
 * Creation date: (3/3/2003 2:28:43 AM)
 * @author: mattia parigiani, Sorin Stancu-Mara
 */
class SelectExpr implements IParseTreeNode {
	CoverageExpr coverageExpr;
	String field;
	
	public SelectExpr( CoverageExpr ce, String se){	
		coverageExpr = ce;
		field = se;
	}
	public String toXML(){
		String result="<select>" ;
		result += "<field>" + field + "</field>";		
		result += coverageExpr.toXML();
		result += "</select>";
		return result;
	}
}
