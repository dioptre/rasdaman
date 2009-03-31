package syntaxParser;
/* Author: Sorin Stancu-Mara
Date: 7 Feb 2007
*/

class StoreExpr implements IParseTreeNode {
    private EncodedCoverageExpr expr;
    public StoreExpr(EncodedCoverageExpr e) {
	expr = e;
	expr.setStoreFlag();	
    }
    public String toXML() {
	return expr.toXML();
    }
}
