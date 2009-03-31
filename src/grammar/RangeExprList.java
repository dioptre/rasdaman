package grammar;

import java.util.*;

/** RangeExprList
 *
 * @author Andrei Aiordachioaie
 */
public class RangeExprList  implements IParseTreeNode
{
    LinkedList<RangeExpr> list;
    
    public RangeExprList()
    {
        list = new LinkedList();
    }

    public void add(RangeExpr expr)
    {
        list.add(expr);
    }

    public String toXML()
    {
        String result = "";

        Iterator<RangeExpr> it = list.iterator();
        while (it.hasNext())
            result += "<null>" + it.next().toXML() + "</xml>";
        return result;
    }   
}