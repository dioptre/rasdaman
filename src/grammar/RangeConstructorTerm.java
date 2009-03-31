package grammar;

/** RangeConstructorTerm, part of a RangeConstructorExpr
 *
 * @author Andrei Aiordachioaie
 */
public class RangeConstructorTerm implements IParseTreeNode
{

    String field;
    CoverageExpr expr;

    public RangeConstructorTerm(String f, CoverageExpr c)
    {
        field = f;
        expr = c;
    }
    
    public String toXML()
    {
        String result = "";
        result += "<field>" + field + "</field>";
        result += expr.toXML();

        result = "<component>" + result + "</component>";
        return result;
    }
    
}
