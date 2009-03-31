package grammar;

/**
 * MetaDataExpr
 * Creation date: (3/3/2003 2:28:43 AM)
 * @author: mattia parigiani, Sorin Stancu-Mara, Andrei Aiordachioaie
 */
public class MetaDataExpr implements IParseTreeNode {

	String function;
	CoverageExpr expr;
	IParseTreeNode param;
	String field;

    // Identifier, ImageCRS, ImageCRSDomain, CrsSet, NullSet
    public MetaDataExpr(String op, CoverageExpr expr)
    {
        function = op;
        this.expr = expr;
    }

    // ImageCRSDomain, interpolationDefault, interpolationSet
	public MetaDataExpr(String op, CoverageExpr expr, String str)
    {
		function = op;
		this.expr = expr;
		this.field = str;
	}

    public MetaDataExpr(DomainExpr dom)
    {
        function = "domain";
        param = dom;
    }

	public String toXML()
    {
		String result = "";

        if (function.equalsIgnoreCase("imageCrsDomain"))
        {
            result += "<imageCrsDomain>";
            result += expr.toXML();
            if (field != null)
                result += "<axis>" + field + "</axis>";
            result += "</imageCrsDomain>";
        }
        else if (function.equalsIgnoreCase("domain"))
        {
            result = "<DomainMetadata>" + param.toXML() + "</DomainMetadata>";
        }
        else if (function.equalsIgnoreCase("interpolationDefault"))
        {
            result += "<interpolationDefault>";
            result += expr.toXML();
            result += "<name>" + param + "</param>";
            result += "</interpolationDefault>";
        }
        else if (function.equalsIgnoreCase("interpolationSet"))
        {
            result += "<interpolationSet>";
            result += expr.toXML();
            result += "<name>" + param + "</param>";
            result += "</interpolationSet>";
        }
        else if (function.equalsIgnoreCase("identifier") || function.equalsIgnoreCase("imageCRS")
                || function.equalsIgnoreCase("crsSet") || function.equalsIgnoreCase("nullSet"))
        {
            result += "<" + function + ">";
            result += expr.toXML();
            result += "</" + function + ">";
        }
        else
        {
            System.err.println("Unknown MetadataExpr operation: " + function);
        }

		return result;
	}
	
}
