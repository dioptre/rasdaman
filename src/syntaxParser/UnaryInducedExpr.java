package syntaxParser;
/**
 * UnaryInducedExpr
 * Creation date: (3/3/2003 2:28:43 AM)
 * @author: mattia parigiani, Sorin Stancu-Mara
 */
class UnaryInducedExpr implements IParseTreeNode {

	IParseTreeNode expr;	
	String flag;
	
	public UnaryInducedExpr( IParseTreeNode e ){	
		expr = e;
	}
	

	public String toXML(){
		return expr.toXML();
	}
	

	
}
