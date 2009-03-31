package syntaxParser;
/**
 * GeneralCondenseExpr
 * Creation date: (3/3/2003 2:28:43 AM)
 * @author: mattia parigiani, Sorin stancu-Mara
 */
class GeneralCondenseExpr implements IParseTreeNode {
	
	String condenseOpType;
	ScalarExpr scalarExpr;
	VariableList variables;
	BooleanScalarExpr condition;

	public GeneralCondenseExpr(String op, VariableList vars, ScalarExpr val) {
	    condenseOpType = op;
	    variables = vars;
	    scalarExpr = val;
	}
	
	public GeneralCondenseExpr(String op, VariableList vars, BooleanScalarExpr cond, ScalarExpr val) {
	    condenseOpType = op;
	    variables = vars;
	    scalarExpr = val;
	    condition = cond;
	}	
	
	public String toXML(){
    	    String result = "<condense>" + variables.toXML();
	    if (condition !=  null) 
	        result += "<where>" + condition.toXML() + "</where>";
	    result += scalarExpr.toXML() + "</condense>"; 	
	    return result;
	}
	

	
}
