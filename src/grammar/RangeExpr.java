package grammar;

/** RangeExpr
 *
 * @author Andrei Aiordachioaie
 */
public class RangeExpr implements IParseTreeNode
{
    String xml;

    public RangeExpr()
    {
        this.xml = "";
    }

    public RangeExpr(String field, ScalarExpr expr)
    {
        this();
        add(field, expr);
    }

    public void add(String field, ScalarExpr expr)
    {
        String tmp = "";
        tmp += "<field>" + field + "</field>";
        tmp += expr.toXML();
        xml += "<componen>" + tmp + "</component>";
    }

    public String toXML()
    {
        return xml;
    }
    
}
