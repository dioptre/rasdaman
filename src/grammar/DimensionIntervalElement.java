package grammar;

/** DimensionIntervalElement
 *
 * @author Andrei Aiordachioaie
 */
public class DimensionIntervalElement implements IParseTreeNode
{
    String axis;
    String crs;
    DimensionIntervalExpr expr;
    
    public DimensionIntervalElement(String a)
    {
        axis = a;
    }

    public void setCrs(String c)
    {
        crs = c;
    }

    public void setIntervalExpr(DimensionIntervalExpr e)
    {
        expr = e;
    }
    
    public String toXML()
    {
        String result = "";

        result += "<axis>" + axis + "</axis>";
        if (crs != null)
            result += "<crs>" + crs + "</crs>";
        result += expr.toXML();
        
        return result;
    }
    
}
