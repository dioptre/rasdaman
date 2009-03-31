package grammar;

/** ReturnClause
 *
 * @author Andrei Aiordachioaie
 */
public class ReturnClause implements IParseTreeNode
{
    IParseTreeNode expr;

    public ReturnClause(IParseTreeNode e)
    {
        expr = e;
    }
    
    public String toXML()
    {
        return expr.toXML();
    }
    
}
