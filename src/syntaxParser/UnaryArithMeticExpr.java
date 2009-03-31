package syntaxParser;
/**
 * UnaryArithmeticExpr
 * Creation date: (3/3/2003 2:28:43 AM)
 * @author: mattia parigiani, Sorin Stancu-Mara
 */
class UnaryArithMeticExpr implements IParseTreeNode {

	String operator;
	CoverageExpr coverageExpr;
	
	public UnaryArithMeticExpr( CoverageExpr ce,  String op){	
		operator = op;
		coverageExpr = ce;		
	}
	public String toXML(){
		String result=  ""  ;
		String op;
		if (operator.equals("+")) {
			op = "unaryPlus";
		} else if (operator.equals("-")) {
			op = "unaryMinus";
		} else {
			op = operator;	
		}
		result += "<" + op + ">\n";
		result += coverageExpr.toXML() ;
		result += "</" + op + ">\n";
		return result;
	}	
}
