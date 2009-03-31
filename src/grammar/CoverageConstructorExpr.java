package grammar;

/** CoverageConstructorExpr
 *
 * @author Andrei Aiordachioaie
 */
public class CoverageConstructorExpr implements IParseTreeNode {
    
    String name;
    AxisIteratorList alist;
    ScalarExpr expr;
    
    public CoverageConstructorExpr(String name, AxisIteratorList alist, ScalarExpr expr)
    {
        this.name = name;
        this.alist = alist;
        this.expr = expr;
    }
    
    public String toXML()
    {
        String result = "";

        result += "<name>" + name + "</name>";
        result += alist.toXML();
        result += expr.toXML();

        result = "<construct>" + result + "</construct>";
        return result;
    }
}
