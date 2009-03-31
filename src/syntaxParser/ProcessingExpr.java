package syntaxParser;
/**
 * ProcessingExpr
 * Creation date: (3/3/2003 2:28:43 AM)
 * @author: mattia parigiani, Sorin Stancu-Mara
 */
class ProcessingExpr implements IParseTreeNode {
	IParseTreeNode expr;
	public ProcessingExpr( IParseTreeNode e ) {
	    expr = e;
	}
	public String toXML() {
		return expr.toXML();
	}
	

	
}
