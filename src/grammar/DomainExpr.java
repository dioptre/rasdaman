package grammar;

/** DomainExpr
 *
 * @author Andrei Aiordachioaie
 */
public class DomainExpr  implements IParseTreeNode
{
    String var, axis, crs;

    public DomainExpr(String v, String a, String c)
    {
        var = v;
        axis = a;
        crs = c;
    }
    
    public String toXML()
    {
        String result = "";
        result += "<coverage>" + var + "</coverage>";
        result += "<axis>" + axis + "</axis>";
        result += "<crs>" + crs + "</crs>";
        return result;
    }
    
}
