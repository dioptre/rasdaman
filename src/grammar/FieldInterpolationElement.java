package grammar;

/** FieldInterpolationElement
 *
 * @author Andrei Aiordachioaie
 */
public class FieldInterpolationElement  implements IParseTreeNode
{

    String name;
    IParseTreeNode node;

    public FieldInterpolationElement(String name, InterpolationMethod method)
    {
        this.name = name;
        node = method;
    }
    
    public String toXML()
    {
        String result = "<name>" + name + "</name>" + node.toXML();
        
        return result;
    }
    
}
