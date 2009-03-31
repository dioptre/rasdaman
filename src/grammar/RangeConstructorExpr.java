package grammar;

import java.util.*;

/** RangeConstructorExpr
 *
 * @author Andrei Aiordachioaie
 */
public class RangeConstructorExpr implements IParseTreeNode
{
    LinkedList<RangeConstructorTerm> list;
    
    public RangeConstructorExpr()
    {
        list = new LinkedList();
    }

    public RangeConstructorExpr(String field, CoverageExpr expr)
    {
        this();
        add(field, expr);
    }

    public void add(String field, CoverageExpr expr)
    {
        list.add(new RangeConstructorTerm(field, expr));
    }

    public String toXML()
    {
        String result = "";
        Iterator<RangeConstructorTerm> it = list.iterator();

        while (it.hasNext())
            result += it.next().toXML();

        result = "<rangeConstructor>" + result + "</rangeConstructor>";
        return result;
    }
    
}
