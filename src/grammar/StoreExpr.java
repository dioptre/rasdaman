package grammar;

/*
Author: Sorin Stancu-Mara, Andrei Aiordachioaie
Date: 7 Feb 2007
*/

public class StoreExpr implements IParseTreeNode
{
    private EncodedCoverageExpr expr;

    public StoreExpr(EncodedCoverageExpr e)
    {
    	expr = e;
        expr.setStoreFlag();
    }

    public String toXML()
    {
        return expr.toXML();
    }
}
