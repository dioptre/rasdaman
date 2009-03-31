package syntaxParser;
/**
 * CastExpr
 * Creation date: (3/3/2003 2:28:43 AM)
 * @author: mattia parigiani, Sorin Stancu-Mara
 */
class CastExpr implements IParseTreeNode {
	String castType;
	CoverageExpr coverageExpr;
	
	public CastExpr( CoverageExpr ce,  String ct){	
		castType = ct;
		coverageExpr = ce;		
	}
	
	public String toXML(){
		String result=  ""  ;	
		result += "<cast>";
		result += "<type>" + castType + "</type>";
		result += coverageExpr.toXML(); 
		result += "</cast>";
		return result;
	}
}
