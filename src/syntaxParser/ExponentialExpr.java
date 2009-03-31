package syntaxParser;
/**
 * ExponentialExpr
 * Creation date: (3/3/2003 2:28:43 AM)
 * @author: mattia parigiani, Sorin Stancu-Mara
 */
class ExponentialExpr implements IParseTreeNode {
	String expOperator;
	CoverageExpr coverageExpr;
	
	public ExponentialExpr( CoverageExpr ce,  String op){	
		expOperator = op;
		coverageExpr = ce;		
	}
	public String toXML(){
		String result=  ""  ;
		result += "<" + expOperator + ">";
		result += coverageExpr.toXML() ;
		result += "</" + expOperator + ">";
		return result;
	}
}
