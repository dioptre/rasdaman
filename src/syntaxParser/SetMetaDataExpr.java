package syntaxParser;
/**
 * SetMetaDataExpr
 * Creation date: (3/3/2003 2:28:43 AM)
 * @author: mattia parigiani, Sorin Stancu-Mara
 */
class SetMetaDataExpr implements IParseTreeNode {

	String function;
	CoverageExpr expr;
	IParseTreeNode param;
	String field;

	public SetMetaDataExpr(String op, CoverageExpr expr,IParseTreeNode param) {
		function = op;
		this.expr = expr;
		this.param = param;
	}
	public SetMetaDataExpr(String op, CoverageExpr expr, IParseTreeNode param, String field) {
		function = op;
		this.expr = expr;
		this.param = param;
	}
	
	public String toXML(){
		String result=  ""  ;

		if (function.equals("setNullDefault")) {
			result = "<setNullDefault>" + "<null>" +  param.toXML()  + "</null>"
				+ expr.toXML() 
				+ "</setNullDefault>";
		} else if (function.equals("setNullSet")) {
			result = "<setNullSet>";
			RangeValueList list = (RangeValueList) param;
			list.setTag("null");
			result += list.toXML();
			result += expr.toXML() + "</setNullSet>";
		} else if (function.equals("setInterpolationDefault")) {
			result = "<setInterpolationDefault><field>" + field + "</field>" 
				+ param.toXML()	+ expr.toXML() + "</setInterpolationDefault>";
		} else if (function.equals("setInterpolationSet")) {
			result = "<setInterpolationSet><field>" + field + "</field>" + param.toXML() +
					 expr.toXML() + "</setInterpolationSet>";
		} else if (function.equals("setCrsSet")) {
			// TODO(smsorin): schema/grammar inconsistencies
			result += "<setCrsSet>" + expr.toXML()  
				+ "</setCrsSet>";
		}	

		result+=  "";
		return result;
	}
	
}
