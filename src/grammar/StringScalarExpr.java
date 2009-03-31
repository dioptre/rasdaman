package grammar;

/**
 * @author: Andrei Aiordachioaie
 */

public class StringScalarExpr implements IParseTreeNode
{
    String val;
    String function;
    CoverageExpr cov;
    String op;

    public StringScalarExpr(String val)
    {
        this.val = val;
        this.function = "stringConstant";
    }

    public StringScalarExpr(String op, CoverageExpr cov)
    {
        this.cov = cov;
        function = "stringIdentifier";
        this.op = op;
    }

    public String toXML()
    {
        String result = "<" + function + ">";

        if (function.equals("stringConstant"))
            result += val;
        else if (function.equals("stringIdentifier"))
            result += cov.toXML();
        result += "</" + function + ">";

        return result;
    }
}
