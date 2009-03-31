package grammar;
/**
 * TrigonometricExpr
 * Creation date: (3/3/2003 2:28:43 AM)
 * @author: mattia parigiani, Sorin Stancu-Mara
 */
public class TrigonometricExpr implements IParseTreeNode {
	String trigOperator;
	CoverageExpr coverageExpr;
	
	public TrigonometricExpr(String op, CoverageExpr ce){
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
