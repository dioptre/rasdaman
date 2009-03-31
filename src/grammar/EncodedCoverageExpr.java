package grammar;
/**
 * EncodedCoverageExpr
 * Creation date: (3/3/2003 2:28:43 AM)
 * @author: mattia parigiani, Sorin Stancu-Mara, Andrei Aiordachioaie
 */
public class EncodedCoverageExpr implements IParseTreeNode
{
	CoverageExpr expr;
	String format;
	String extraParams;
	boolean store;
    
	public EncodedCoverageExpr( CoverageExpr ce , String fn )
    {
		expr = ce;
		format = fn;
		extraParams = null;
		store = false;
	}
	
	public void setParams(String params)
    {
	    extraParams = params;
	}

    public void setStoreFlag()
    {
        store = true;
    }

	public String toXML()
    {
		String result = "";
		if (store)
            result = "<encode store=\"true\">";
        else
            result = "<encode store=\"false\">";

        result += expr.toXML();
		result += "<format>" + format + "</format>" ;
		if (extraParams != null )
		    result += "<extraParams>" + extraParams + "</extraParams>";

        result+=  "</encode>"  ;
        
		return result;
	}	
}
