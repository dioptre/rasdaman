package syntaxParser;
/* Author: Sorin Stancu-Mara
Date: 7 Feb 2007
*/

class WhereClause implements IParseTreeNode {
    private BooleanScalarExpr expr;
    public WhereClause(BooleanScalarExpr e) {
	expr = e;
    }
    public String toXML() {
	return "<where>" + expr.toXML() + "</where>";		    
    }
}
