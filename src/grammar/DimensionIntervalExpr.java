package grammar;

/** DimensionIntervalExpr
 *
 * @author Andrei Aiordachioaie
 */
public class DimensionIntervalExpr  implements IParseTreeNode
{
    
    IParseTreeNode e1, e2;
    String fun;
    String str;

    public DimensionIntervalExpr(ScalarExpr exp1, ScalarExpr exp2)
    {
        fun = "scalars";
        e1 = exp1;
        e2 = exp2;
    }

    public DimensionIntervalExpr(String cov, String axis, String crs)
    {
        fun = "domain metadata";
        str = "<coverage>" + cov + "</coverage>";
        str += "<axis>" + axis + "</axis>";
        str += "<crs>" + crs + "</crs>";
    }
    
    public String toXML()
    {
        String result = "";

        if (fun.equals("scalars"))
        {
            result += "<lowerBound>" + e1.toXML() + "</lowerBound>";
            result += "<uppedBound>" + e2.toXML() + "</upperBound>";
        }
        else if (fun.equals("domain metadata"))
            result = str;
        
        return result;
    }
    
}
