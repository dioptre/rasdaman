package grammar;

import java.util.*;

/** CrsList
 *
 * @author Andrei Aiordachioaie
 */
public class CrsList implements IParseTreeNode
{
    LinkedList<String> list;
    
    public CrsList(String crs)
    {
        list = new LinkedList<String>();
        list.add(crs);
    }

    public CrsList()
    {
        list = new LinkedList<String>();
    }

    public void add(String crs)
    {
        list.add(crs);
    }

    public String toXML()
    {
        String result = "";
        Iterator<String> it = list.iterator();
        while (it.hasNext())
        {
            result += "<crs>" + it.next() + "</crs>";
        }
        
        return result;
    }
    
}
