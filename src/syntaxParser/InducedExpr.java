package syntaxParser;
/**
 * InducedExpr
 * Creation date: (3/3/2003 2:28:43 AM)
 * @author: mattia parigiani, Sorin Stancu-Mara
 */
class InducedExpr implements IParseTreeNode {

	IParseTreeNode expr;
	
	public InducedExpr( IParseTreeNode e ){
		expr =e;
	}

	public String toXML(){
		return expr.toXML();
	}
	

	
}
