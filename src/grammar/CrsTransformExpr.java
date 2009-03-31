package grammar;

/** CrsTransformExpr
 *
 * @author Andrei Aiordachioaie
 */
public class CrsTransformExpr implements IParseTreeNode
{
    IParseTreeNode e1, e2, e3;

    public CrsTransformExpr(CoverageExpr expr, DimensionIntervalList list1, FieldInterpolationList list2)
    {
        e1 = expr;
        e2 = list1;
        e3 = list2;
    }
    
    public String toXML()
    {
        String result = "";

        result += e1.toXML();
        result += e2.toXML();
        result += e3.toXML();

        result = "<crsTransform>" + result + "</crsTransform>";
        return result;
    }
    
}
