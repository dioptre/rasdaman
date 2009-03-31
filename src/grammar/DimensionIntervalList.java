package grammar;

import java.util.*;

/** DimensionIntervalList
 *
 * @author Andrei Aiordachioaie
 */
public class DimensionIntervalList  implements IParseTreeNode
{

    LinkedList<DimensionIntervalElement> list;

    public DimensionIntervalList(DimensionIntervalElement e)
    {
        list = new LinkedList();
        list.add(e);
    }

    public void add(DimensionIntervalElement meth)
    {
        list.add(meth);
    }
    
    public String toXML()
    {
        String result = "";
        Iterator<DimensionIntervalElement> it = list.iterator();
        while (it.hasNext())
        {
            result += it.next().toXML();
        }
        
        return result;
    }
    
}
