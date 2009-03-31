package grammar;

/** DimensionPointElement
 *
 * @author Andrei Aiordachioaie
 */
public class DimensionPointElement implements IParseTreeNode
{
    String axis;
    String crs;
    ScalarExpr point;
    
    public DimensionPointElement(String a, ScalarExpr dp)
    {
        axis = a;
        point = dp;
    }

    public DimensionPointElement(String a, String c, ScalarExpr dp)
    {
        axis = a;
        crs = c;
        point = dp;
    }

    
    public String toXML()
    {
        String result = "";

        result += "<axis>" + axis + "</axis>";
        if (crs != null)
            result += "<crs>" + crs + "</crs>";
        result += "<slicingPoint>" + point.toXML() + "</slicingPoint>";
        
        return result;
    }
    
}
