package grammar;

import java.util.*;

/** FieldInterpolationList
 *
 * @author Andrei Aiordachioaie
 */
public class FieldInterpolationList  implements IParseTreeNode
{

    LinkedList<FieldInterpolationElement> list;

    public FieldInterpolationList(FieldInterpolationElement e)
    {
        list = new LinkedList();
        list.add(e);
    }

    public void add(FieldInterpolationElement meth)
    {
        list.add(meth);
    }
    
    public String toXML()
    {
        String result = "";
        Iterator<FieldInterpolationElement> it = list.iterator();
        while (it.hasNext())
        {
            result += it.next().toXML();
        }
        
        return result;
    }
    
}
