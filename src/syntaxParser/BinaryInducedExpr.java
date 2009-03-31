package syntaxParser;
/**
 * BinaryInducedExpr
 * Creation date: (3/3/2003 2:28:43 AM)
 * @author: mattia parigiani, Sorin Stancu-Mara
 */
class BinaryInducedExpr implements IParseTreeNode {

	IParseTreeNode leftCoverageExpr;
	IParseTreeNode rightCoverageExpr;
    int wrapInScalar;
	String operator;

	public BinaryInducedExpr(  String o, IParseTreeNode le, IParseTreeNode re ){	
		leftCoverageExpr = le;
		rightCoverageExpr = re;
        wrapInScalar = 0;
		operator = o;
	}
	public BinaryInducedExpr(  String o, IParseTreeNode le, IParseTreeNode re, int wis ){	
		leftCoverageExpr = le;
		rightCoverageExpr = re;
        wrapInScalar = wis;
		operator = o;
	}
	public String toXML(){
		String result="";
		result = "<" + operator + ">";
        if (wrapInScalar == -1) result += "<scalar>";
		result += leftCoverageExpr.toXML();
        if (wrapInScalar == -1) result += "</scalar>";
        if (wrapInScalar == 1) result += "<scalar>";
		result += rightCoverageExpr.toXML();
        if (wrapInScalar == 1) result += "</scalar>";
		result += "</" + operator + ">";
		return result;
	}
}
