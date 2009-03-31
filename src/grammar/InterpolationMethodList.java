package grammar;

import java.util.*;

/** InterpolationMethodList
 *
 * @author Andrei Aiordachioaie
 */
public class InterpolationMethodList  implements IParseTreeNode
{

    LinkedList<InterpolationMethod> list;

    public InterpolationMethodList()
    {
        list = new LinkedList();
    }

    public void add(InterpolationMethod meth)
    {
        list.add(meth);
    }
    
    public String toXML()
    {
        String result = "";
        Iterator<InterpolationMethod> it = list.iterator();
        while (it.hasNext())
        {
            result += it.next().toXML();
        }
        
        return result;
    }
    
}
