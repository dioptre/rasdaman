package grammar;

/**
 * NumericScalarExpr
 * Creation date: (3/3/2003 2:28:43 AM)
 * @author: mattia parigiani, Sorin Stancu-Mara, Andrei Aiordachioaie
 */
public class NumericScalarExpr implements IParseTreeNode
{
    String constValue;
    String function;
    NumericScalarExpr leftNumericScalarExpr, rightNumericScalarExpr;
    CondenseExpr condense;

    public NumericScalarExpr(String val)
    {
        if (val.contains("+i"))
        {
            ComplexConst cc = new ComplexConst(val);

            constValue = cc.toXML();
            function = "complexConstant";
        }
        else
        {
            constValue = val;
            function = "numericConstant";
        }
    }

    public NumericScalarExpr(String op, NumericScalarExpr expr)
    {
        leftNumericScalarExpr = expr;

        if (op.equals("-"))
            function = "numericUnaryMinus";
        else
            System.err.println("Unary Operator " + op + " is not recognized!");
    }

    public NumericScalarExpr(String op, NumericScalarExpr lbe, NumericScalarExpr rbe)
    {
        leftNumericScalarExpr = lbe;
        rightNumericScalarExpr = rbe;

        if (op.equals("+"))
        {
            function = "numericAdd";
        }
        else if (op.equals("-"))
        {
            function = "numericMinus";
        }
        else if (op.equals("*"))
        {
            function = "numericMult";
        }
        else if (op.equals("/"))
        {
            function = "numericDiv";
        }
        else
        {
            System.err.println("Operator " + op + " is not recognized!");
        }
    }

    public NumericScalarExpr(CondenseExpr c)
    {
        condense = c;
        function = "condense";
    }

    public String toXML()
    {
        String result;

        if (function.equals("complexConstant"))
            return constValue;

        result = "<" + function + ">";

        if (function.equals("numericConstant"))
            result += constValue;
        else
        {
            result += leftNumericScalarExpr.toXML();
            if (rightNumericScalarExpr != null)
                result += rightNumericScalarExpr.toXML();
        }

        result += "</" + function + ">";

        return result;
    }
}
