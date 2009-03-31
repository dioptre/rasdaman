package grammar;

/** AxisIterator
 *
 * @author Andrei Aiordachioaie
 */
public class AxisIterator implements IParseTreeNode
{
    String var;
    String axis;
    NumericScalarExpr hi, lo;
    IntervalExpr interval;
    
    public AxisIterator(String var, String axis, IntervalExpr interval)
    {
        this.var = var;
        this.axis = axis;
        this.interval = interval;
    }
    
    public String toXML()
    {
        String result = "";

        result += "<axisIterator>";
        result += "<iteratorVar>" + var + "</iteratorVar>";
        result += "<axis>" + axis + "</axis>";
        result += interval.toXML();
        result += "</axisIterator>";

        return result;
    }
    
}
