package grammar;

import java.util.*;

/** DimensionPointList
 *
 * @author Andrei Aiordachioaie
 */
public class DimensionPointList  implements IParseTreeNode
{

    LinkedList<DimensionPointElement> list;

    public DimensionPointList(DimensionPointElement e)
    {
        list = new LinkedList();
        list.add(e);
    }

    public void add(DimensionPointElement meth)
    {
        list.add(meth);
    }
    
    public String toXML()
    {
        String result = "";
        Iterator<DimensionPointElement> it = list.iterator();
        while (it.hasNext())
        {
            result += it.next().toXML();
        }
        
        return result;
    }
    
}
