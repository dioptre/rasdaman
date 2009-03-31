package grammar;

/** CondenseOperationType
 *
 * @author Andrei Aiordachioaie
 */
public class CondenseOperationType  implements IParseTreeNode
{
    String op;

    public CondenseOperationType(String op)
    {
        this.op = op;
    }
    
    public String toXML()
    {
        String result = "";

        if (op.equalsIgnoreCase("plus"))
            result = "opPlus";
        else if (op.equalsIgnoreCase("mult"))
            result = "opMult";
        else if (op.equalsIgnoreCase("max"))
            result = "opMax";
        else if (op.equalsIgnoreCase("min"))
            result = "opMin";
        else if (op.equalsIgnoreCase("and"))
            result = "opAnd";
        else if (op.equalsIgnoreCase("or"))
            result = "opOr";
        
        return result;
    }
    
}
