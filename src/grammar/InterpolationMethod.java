package grammar;

/** InterpolationMethod
 *
 * @author Andrei Aiordachioaie
 */
public class InterpolationMethod  implements IParseTreeNode
{

    String interp, resistance;

    public InterpolationMethod(String interp, String resist)
    {
        this.interp = interp;
        this.resistance = resist;
    }
    
    public String toXML()
    {
        String result = "";
        result += "<interpolationMethod>" + interp + "</interpolationMethod>";
        result += "<nullResistance>" + resistance + "</nullResistance>";
        return result;
    }
    
}
