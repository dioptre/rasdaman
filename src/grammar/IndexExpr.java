package grammar;

/** IndexExpr
 *
 * @author Andrei Aiordachioaie
 */
public class IndexExpr implements IParseTreeNode
{

    String op;
    IParseTreeNode e1, e2;
    String constant;
    String function;
    
    public IndexExpr(String op, IndexExpr e1, IndexExpr e2)
    {
        this.op = op;
        this.e1 = e1;
        this.e2 = e2;
        function = "op2";
    }

    public IndexExpr(String constant)
    {
        function = "constant";
        this.constant = constant;
    }

    public IndexExpr(String op, NumericScalarExpr e1)
    {
        this.op = op;
        this.e1 = e1;
        function = "op1";
    }

    public String toXML()
    {
        String result = "";
        String tag1 = "<" + op + ">", tag2 = "</" + op + ">";
        if (function.equals("constant"))
            result = "<numericConstant>" + constant + "</numericConstant>";
        else if (function.equals("op1"))
            result = tag1 + e1.toXML() + tag2;
        else if (function.equals("op2"))
            result = tag1 + e1.toXML() + e2.toXML() + tag2;
        
        return result;
    }
    
}
