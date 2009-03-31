package grammar;

/**
 * SetMetaDataExpr
 * Creation date: (3/3/2003 2:28:43 AM)
 * @author: mattia parigiani, Sorin Stancu-Mara, Andrei Aiordachioaie
 */
public class SetMetaDataExpr implements IParseTreeNode {

	String function;
	CoverageExpr expr;
	IParseTreeNode param;
	String field;

    // Set Identifier
    public SetMetaDataExpr(String op, CoverageExpr expr, String constant)
    {
        function = op;
        this.expr = expr;
        this.field = constant;
    }

    // Set Crs Set
    public SetMetaDataExpr(String op, CoverageExpr expr, CrsList clist)
    {
        function = op;
        this.expr = expr;
        this.param = clist;
    }

    // Set Null Set
	public SetMetaDataExpr(String op, CoverageExpr expr, RangeExprList param)
    {
		function = op;
		this.expr = expr;
		this.param = param;
	}

    // Set Interpolation Default
	public SetMetaDataExpr(String op, CoverageExpr expr, InterpolationMethod param, String field)
    {
		function = op;
		this.expr = expr;
		this.param = param;
        this.field = field;
	}

    // Set Interpolation Set
    public SetMetaDataExpr(String op, CoverageExpr expr, InterpolationMethodList param, String field)
    {
		function = op;
		this.expr = expr;
		this.param = param;
        this.field = field;
	}
    
	
	public String toXML()
    {
		String result = "";

        if (function.equalsIgnoreCase("setIdentifier"))
        {
            result += "<identifier>" + field + "</identifier>";
            result += expr.toXML();

            result = "<setIdentifier>" + result + "</setIdentifier>";
        }
        else if (function.equalsIgnoreCase("setCrsSet"))
        {
            result += expr.toXML();
            if (param != null)
                result += param.toXML();

            result = "<setCrsSet>" + result + "</setCrsSet>";
        }
        else if (function.equalsIgnoreCase("setNullSet"))
        {
            result += expr.toXML();
            if (param != null)
                result += param.toXML();

            result = "<setNullSet>" + result + "</setNullSet>";
        }
        else if (function.equalsIgnoreCase("setInterpolationDefault"))
        {
            result += expr.toXML();
            result += "<field>" + field + "</field>";
            result += param.toXML();

            result = "<setInterpolationDefault>" + result + "</setInterpolationDefault>";
        }
        else if (function.equalsIgnoreCase("setInterpolationSet"))
        {
            result += expr.toXML();
            result += "<field>" + field + "</field>";
            result += param.toXML();

            result = "<setInterpolationSet>" + result + "</setInterpolationSet>";
        }
        else
        {
            System.err.println("Unknown SetMetadataExpr operation: " + function);
        }

		return result;
	}
	
}
