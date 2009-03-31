package grammar;

/** GeneralCondenseExpr
 *
 * @author Andrei Aiordachioaie
 */
public class GeneralCondenseExpr  implements IParseTreeNode
{
    CondenseOperationType op;
    AxisIteratorList alist;
    BooleanScalarExpr where;
    ScalarExpr using;

    public GeneralCondenseExpr(CondenseOperationType op, AxisIteratorList al)
    {
        this.op = op;
        alist = al;
        alist.setTag("iterator");
        where = null;
        using = null;
    }

    public void setWhere(BooleanScalarExpr bse)
    {
        where = bse;
    }

    public void setUsing(ScalarExpr se)
    {
        using = se;
    }
    
    public String toXML()
    {
        String result = "";

        result += op.toXML();
        result += alist.toXML();
        if (where != null)
                result += "<where>" + where.toXML() + "</where>";
        result += using.toXML();

        return result;
    }
    
}
