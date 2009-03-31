package syntaxParser;
/* Author: Sorin Stancu-Mara
 * Date: 8 Feb 2008
 */

class CoverageConstructorExpr implements IParseTreeNode {
	String name;
	VariableList variables;
	ScalarExpr expr;

	public CoverageConstructorExpr(String name, VariableList vl, ScalarExpr expr) {
		this.name = name;
		this.variables = vl;
		this.expr = expr;
	}

	public String toXML() {
		return "<construct><coverage>" + name + "</coverage>" + variables.toXML() + expr.toXML() + "</construct>";
	}

}
