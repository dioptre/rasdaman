package grammar;

/** IntervalExpr
 *
 * @author Andrei Aiordachioaie
 */
public class IntervalExpr  implements IParseTreeNode
{
    String function;
    IParseTreeNode e1, e2;

    public IntervalExpr(IndexExpr n1, IndexExpr n2)
    {
        this.e1 = n1;
        this.e2 = n2;
        function = "two indexes";
    }

    public IntervalExpr(String coverage, String axis)
    {
        function = "crs metadata";
        this.e1 = new ImageCrsDomainMetadataExpr(coverage, axis);
    }

    public String toXML()
    {
        String result = "";
        if (function.equals("two indexes"))
            result = e1.toXML() + e2.toXML();
        else if (function.equals("crs metadata"))
            result = e1.toXML();
        return result;
    }

}
