package syntaxParser;
/**
 * ReturnClause
 * Creation date: (3/3/2003 2:28:43 AM)
 * @author: mattia parigiani, Sorin Stancu-Mara
 */
class ReturnClause implements IParseTreeNode {
	private ProcessingExpr processingExpr;
	public ReturnClause( ProcessingExpr pr){
		processingExpr =pr;
	}
	
	public String toXML(){
		return processingExpr.toXML();
	}
}
