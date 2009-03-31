package grammar;

/** BooleanExpr
 *
 * @author Andrei Aiordachioaie
 */
public class BooleanExpr implements IParseTreeNode
{
    IParseTreeNode e1, e2;
    String op;
    
    public BooleanExpr(String op, CoverageExpr cov)
    {
        this.op = op;
        e1 = cov;
    }

    public BooleanExpr(String op, CoverageExpr cov, IndexExpr i)
    {
        this.op = op;
        e1 = cov;
        e2 = i;
    }
    
    public String toXML()
    {
        String result = "";
        if (op.equalsIgnoreCase("not"))
            result = e1.toXML();
        else if (op.equalsIgnoreCase("bit"))
            result = e1.toXML() + e2.toXML();

        result = "<" + op + ">" + result + "</" + op + ">";
        return result;
    }
    
}
