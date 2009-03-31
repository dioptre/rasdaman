package syntaxParser;
/**
 * EncodedCoverageExpr
 * Creation date: (3/3/2003 2:28:43 AM)
 * @author: mattia parigiani, Sorin Stancu-Mara
 */
class EncodedCoverageExpr implements IParseTreeNode {
	CoverageExpr coverageExpr;
	String formatName;
	String extraParams;
	boolean store;
	public EncodedCoverageExpr( CoverageExpr ce , String fn ){
		coverageExpr = ce;
		formatName = fn;
		extraParams = null;
		store = false;
	}
	
	public EncodedCoverageExpr( CoverageExpr ce , String fn, String ep ){
		coverageExpr = ce;
		formatName = fn;
		extraParams = ep;
		store = false;
	}	
	
	public void setStoreFlag() {
	    store = true;
	}

	public String toXML(){
		String result=  "<encode>"  ;
		if (store)  result = "<encode store=\"true\">";
		
		result += "<format>" + formatName + "</format>" ;
		if (extraParams != null ) { 
		    result += "<extraParams>" + extraParams + "</extraParams>" ;   
		}
		result +=  coverageExpr.toXML()  ;			
		result+=  "</encode>"  ;
		return result;
	}	
}
