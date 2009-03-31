package grammar;

/* Author: Sorin Stancu-Mara, Andrei Aiordachioaie
Date: 7 Feb 2007
*/

public class WhereClause implements IParseTreeNode
{
    private BooleanScalarExpr expr;

    public WhereClause(BooleanScalarExpr e)
    {
        expr = e;
    }

    public String toXML()
    {
        return "<where>" + expr.toXML() + "</where>";
    }
}