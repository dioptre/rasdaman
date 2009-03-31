package grammar;

/** ReduceExpr
 *
 * @author Andrei Aiordachioaie
 */
public class ReduceExpr  implements IParseTreeNode
{
    String op;
    CoverageExpr expr;

    public ReduceExpr(String op, CoverageExpr e)
    {
        this.op = op;
        this.expr = e;
    }
    
    public String toXML()
    {
        String result = "<" + op + ">" + expr.toXML() + "</" + op + ">";
        
        return result;
    }
    
}
