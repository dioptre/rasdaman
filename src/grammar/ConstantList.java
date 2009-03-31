package grammar;

import java.util.*;

/** ConstantList
 *
 * @author Andrei Aiordachioaie
 */
public class ConstantList  implements IParseTreeNode
{
    LinkedList<String> list;
    
    public ConstantList(String val)
    {
        list = new LinkedList<String>();
        list.add(val);
    }

    public void add(String val)
    {
        list.add(val);
    }
    
    public String toXML()
    {
        String result = "";
        Iterator<String> it = list.iterator();
        while (it.hasNext())
        {
            result += "<value>" + it.next() + "</value>";
        }
        
        return result;
    }
    
}
