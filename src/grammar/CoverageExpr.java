package grammar;
/**
 * CoverageExpr
 * Creation date: (3/3/2003 2:28:43 AM)
 * @author: mattia parigiani, Sorin Stancu-Mara, Andrei Aiordachioaie
 */
public class CoverageExpr implements IParseTreeNode
{

	String coverageName;
	IParseTreeNode expr, e1, e2;
	String function;
    String op;
	
	public CoverageExpr( String n )
    {
		coverageName = n;
		function = "coverage";
	}

    public CoverageExpr(IParseTreeNode n)
    {
        expr = n;
        function = "child";
    }

    /* Unary Induced Expressions */
    public CoverageExpr (String op, CoverageExpr ce)
    {
        expr = ce;
        function = "unaryOp";
        this.op = op;
    }

    public CoverageExpr (String op, CoverageExpr e1, CoverageExpr e2)
    {
        function = "binaryOp";
        this.op = op;
        this.e1 = e1;
        this.e2 = e2;
    }

	public String toXML()
    {
		String result = "";
        
		if (function.equals("coverage"))
        {
			result = "<coverage>" + coverageName + "</coverage>";
        }
        else if (function.equals("binaryOp"))
        {
            result = "<" + op + ">" + e1.toXML() + e2.toXML() + "</" + op + ">";
        }
        else if (function.equals("unaryOp"))
			result = expr.toXML();
        else if (function.equals("child"))
            result = expr.toXML();
        
		return result;
	}

/*
    public CoverageExpr( CoverageConstantExpr cce)
    {
        expr = cce;
        function = "coverageConstantExpr";
    }

    public CoverageExpr( CoverageConstructorExpr ce ){
		expr = ce;
		function = "coverageConstructorExpr";
	}

	public CoverageExpr( SetMetaDataExpr smde ){
		expr = smde;
		function = "setMetaDataExpr";
	}

    public CoverageExpr( RangeConstructorExpr rce)
    {
        expr = rce;
        function = "rangeConstructorExpr";
    }

	public CoverageExpr( CoverageExpr ce ){
		expr = ce;
		function = "coverageExpr";
	}

	public CoverageExpr( CrsTransformExpr cte ){
		expr = cte;
		function = "crsTransformExpr";
	}

	public CoverageExpr( ScaleExpr se ){
		expr =  se;
		function = "scaleExpr";
	}

    public CoverageExpr( ScalarExpr se ){
		expr = se;
		function = "scalarExpr";
	}

	public CoverageExpr( SubsetExpr se ){
		expr = se;
		function = "subsetExpr";
	}

*/
}
