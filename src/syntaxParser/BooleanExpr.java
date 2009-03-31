package syntaxParser;
/**
 * BooleanExpr
 * Creation date: (3/3/2003 2:28:43 AM)
 * @author: mattia parigiani, Sorin Stancu-Mara
 */
class BooleanExpr implements IParseTreeNode {	
	CoverageExpr coverageExpr;
	String operator;
	int integerExpr;
	
	public BooleanExpr( CoverageExpr ce, String op ){	
		coverageExpr = ce;
		operator = op;
	}
	
	public BooleanExpr( CoverageExpr ce, int ie, String op ){	
		coverageExpr = ce;
		integerExpr = ie;
		operator = op;
	}
	
	public String toXML(){
		String result="";
		result += "<"+operator+">";
		if (operator.equals("bit")) {
			result += "<position>" + integerExpr +"<position>";
		}
		result += coverageExpr.toXML();			
		result += "</"+operator+">";
		return result;
	}
}
