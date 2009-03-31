package syntaxParser;
/**
 * CoverageExpr
 * Creation date: (3/3/2003 2:28:43 AM)
 * @author: mattia parigiani, Sorin Stancu-Mara 
 */
class CoverageExpr implements IParseTreeNode {

	String coverageName;
	IParseTreeNode expr;
	String function;
	
	public CoverageExpr( String n ){	
		coverageName = n;
		function = "name";
	}
	
	public CoverageExpr( SetMetaDataExpr smde ){	
		expr = smde;
		function = "setMetaDataExpr";
	}	
	
	public CoverageExpr( InducedExpr ie ){	
		expr = ie;
		function = "inducedExpr";
	}
	
	public CoverageExpr( SubsetExpr se ){	
		expr = se;
		function = "subsetExpr";
	}		
	
	public CoverageExpr( CrsTransformExpr cte ){	
		expr = cte;
		function = "crsTransformExpr";
	}		

	public CoverageExpr( ScaleExpr se ){	
		expr =  se;
		function = "scaleExpr";
	}	
	
	public CoverageExpr( CoverageExpr ce ){	
		expr = ce;
		function = "coverageExpr";
	}	

	public CoverageExpr( CoverageConstructorExpr ce ){	
		expr = ce;
		function = "coverageConstructorExpr";
	}

	public String toXML(){
		String result=  ""  ;
		if (function.equals("name")) {
				result = "<coverage>" + coverageName + "</coverage>";
		} else 
			result = expr.toXML();		
		return result;
	}
}
