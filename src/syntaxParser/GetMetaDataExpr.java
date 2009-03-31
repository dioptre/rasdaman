package syntaxParser;
/**
 * GetMetaDataExpr
 * Creation date: (3/3/2003 2:28:43 AM)
 * @author: mattia parigiani
 */
class GetMetaDataExpr implements IParseTreeNode {
	CoverageExpr coverageExpr;
	String function;
	String field;
	
	/**
	 * GetMetaDataExpr constructor comment.
	 */
	public GetMetaDataExpr() {
		super();
	}
	
	public GetMetaDataExpr( String f, CoverageExpr ce){
		coverageExpr = ce ;
		function =f;
		field = null;
	}
	public GetMetaDataExpr( String f, CoverageExpr exp, String field) {
		coverageExpr = exp;
		function = f;
		this.field = field;
	}
	
	public String toXML() {
		String op = "operationNotSet";
		if (function.equals("identifier")) op = "getIdentifier";
		if (function.equals("imageCrs")) op = "getImageCrs";
		if (function.equals("imageCrsDomain")) op = "getImageCrsDomain"; //TODO(smsorin): The schema requires an extra parameter axis not documented in the protocol specfication
		if (function.equals("crsSet")) op = "crsSet";
		if (function.equals("generalDomain")) op = "getGeneralDomain"; //TODO(smsorin): The schema requires an extra parameter axis and optional* parameter crs
		if (function.equals("nullDefault")) op = "getNullDefault";
		if (function.equals("nullSet")) op = "getNullSet";
		if (function.equals("interpolationDefault")) op = "getInterpolationDefault";
		if (function.equals("interpolationSet")) op = "getInterpolationSet";
		String result = "<" + op + ">";
		result += coverageExpr.toXML();
		if (field != null) {
		    result += "<field>" + field + "</field>";		    		    
		}
		result += "</" + op + ">";
		return result;
	}
	
}
