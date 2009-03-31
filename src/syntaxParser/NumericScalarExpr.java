package syntaxParser;
/**
 * NumericScalarExpr
 * Creation date: (3/3/2003 2:28:43 AM)
 * @author: mattia parigiani, Sorin Stancu-Mara
 */
class NumericScalarExpr implements IParseTreeNode {
	NumericScalarExpr leftNumericScalarExpr, rightNumericScalarExpr;
	String function;
	String constValue;

	public NumericScalarExpr(String scalar){
		constValue = scalar;
		function = "scalar";
	}
	
	
	public NumericScalarExpr(String op, NumericScalarExpr expr) throws Exception{
		leftNumericScalarExpr = expr;
		if (op.equals("+")) function = "scalarUnaryPlus";
		else if (op.equals("-")) function = "scalarUnaryMinus";
		else if (op.equals("abs")) function = "scalarAbs";
		else throw new Exception("Operator " + op + " is not recognized!");
	}

	public NumericScalarExpr(String op, NumericScalarExpr lbe, NumericScalarExpr rbe) throws Exception {
		leftNumericScalarExpr = lbe ;
		rightNumericScalarExpr = rbe ;
		if (op.equals("+")) function = "scalarPlus";
		else if (op.equals("-")) function = "scalarMinus";
		else if (op.equals("*")) function = "scalarMult";
		else if (op.equals("/")) function = "scalarDiv";
		else throw new Exception("Operator " + op + " is not recognized!");
	}	
	
	public String toXML() {
		String result;
		result = "<" + function + ">";
		if (function.equals("scalar")) {
			result += constValue;
		} else {
			result += leftNumericScalarExpr.toXML();
			if (rightNumericScalarExpr != null) {
				result += rightNumericScalarExpr.toXML();
			}
		}
		result += "</" + function + ">";
		return result;
	}
	
}
