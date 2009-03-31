package syntaxParser;
/**
 * BooleanScalarExpr
 * Creation date: (3/3/2003 2:28:43 AM)
 * @author: mattia parigiani, Sorin Stancu-Mara
 */
class BooleanScalarExpr implements IParseTreeNode {
	BooleanScalarExpr leftBooleanScalarExpr, rightBooleanScalarExpr;
	String booleanConstant;
	String op;	
	public BooleanScalarExpr(String bc) {
		booleanConstant = bc;		
	}	
	
	public BooleanScalarExpr(String op, BooleanScalarExpr be){
		this.op = op;
		leftBooleanScalarExpr = be ;
	}
	
	public BooleanScalarExpr(String op, BooleanScalarExpr lbe, BooleanScalarExpr rbe){
		this.op = op;
		leftBooleanScalarExpr = lbe ;
		rightBooleanScalarExpr = rbe ;
	}	
	
	public String toXML(){
		if (op == null) return "<boolean>" + booleanConstant + "</boolean>";
		else if (op.equals("not")) return "<scalarNot>" + leftBooleanScalarExpr.toXML() + "</scalarNot>";
		else {
		    if (op.equals("and")) op = "scalarAnd";
		    else if (op.equals("or")) op = "scalarOr";
		    else if (op.equals("xor")) op = "scalarXor";
		    return "<" + op + ">" + leftBooleanScalarExpr.toXML() + rightBooleanScalarExpr.toXML() + "</" + op + ">";
		}
		
	}
	
}
