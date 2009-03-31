package grammar;

/** ImageCrsDomainMetadataExpr Group
 *
 * @author Andrei Aiordachioaie
 */
public class ImageCrsDomainMetadataExpr implements IParseTreeNode
{
    CoverageExpr cov;
    String axis;
    NumericScalarExpr n1, n2;

    public ImageCrsDomainMetadataExpr(String covName, String axis)
    {
        this.cov = new CoverageExpr(covName);
        this.axis = axis;
    }
    
    public String toXML()
    {
        String result = cov.toXML() + "<axis>" + axis + "</axis>";
        return result;
    }
    
}
