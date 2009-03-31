package grammar;

/** CoverageConstantExpr
 *
 * @author Andrei Aiordachioaie
 */
public class CoverageConstantExpr implements IParseTreeNode {
    
    String name;
    AxisIteratorList alist;
    ConstantList clist;
    
    public CoverageConstantExpr(String name, AxisIteratorList alist, ConstantList clist)
    {
        this.name = name;
        this.alist = alist;
        this.clist = clist;
    }
    
    public String toXML()
    {
        String result = "";

        result += "<name>" + name + "</name>";
        result += alist.toXML();
        result += clist.toXML();

        result = "<const>" + result + "</const>";
        return result;
    }
}
