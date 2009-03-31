package grammar;

/** AxisIteratorList
 *
 * @author Andrei Aiordachioaie
 */
public class AxisIteratorList  implements IParseTreeNode
{
    private String tag;
    private AxisIterator it;
    private AxisIteratorList next;
    
    public AxisIteratorList(AxisIterator it)
    {
        this.it = it;
        next=null;
        tag = "";
    }

    public AxisIteratorList(AxisIterator it, AxisIteratorList n)
    {
        this.it = it;
        next = n;
        tag = "";
    }

    public void setTag(String tag)
    {
        this.tag = tag;
    }

    public String toXML()
    {
        String tag1 = "<" + tag + ">";
        String tag2 = "</" + tag + ">";

        if (tag.equals(""))
            tag1 = tag2 = "";
        
        String result = tag1 + it.toXML() + tag2
                             + next.toXML();
        return result;
    }
}
