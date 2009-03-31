package syntaxParser;
/**
 * TrigonometricExpr
 * Creation date: (3/3/2003 2:28:43 AM)
 * @author: mattia parigiani, Sorin Stancu-Mara
 */
class TrigonometricExpr implements IParseTreeNode {
	String trigOperator;
	CoverageExpr coverageExpr;
	
	public TrigonometricExpr( CoverageExpr ce,  String op){	
		trigOperator = op;
		coverageExpr = ce;		
	}

	public String toXML(){
		String result=  ""  ;
		result += "<" + trigOperator + ">";
		result += coverageExpr.toXML() ;
		result += "</" + trigOperator + ">";
		return result;
	}
}
