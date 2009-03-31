package syntaxParser;
/**
 * ReduceExpr
 * Creation date: (3/3/2003 2:28:43 AM)
 * @author: mattia parigiani, Sorin Stancu-Mara
 */
class ReduceExpr implements IParseTreeNode{
	CoverageExpr coverageExpr;
	String operator;

	public ReduceExpr( CoverageExpr ce, String op){	
		coverageExpr = ce;
		operator = op;
	}
		
	public String toXML(){
		return "<" + operator + ">" + coverageExpr.toXML() + "</" + operator + ">";
	}	
}
