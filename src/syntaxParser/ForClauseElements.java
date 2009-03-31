package syntaxParser;
/**
 * ForClauseElements
 * Creation date: (3/3/2003 2:28:43 AM)
 * @author: mattia parigiani, Sorin Stancu-Mara
 */
 
import java.util.*;
 
class ForClauseElements implements IParseTreeNode{
	String var;
	CoverageList coverageList;
	ForClauseElements next;
	
	
	public ForClauseElements( String v, CoverageList c ){
		var = v; 
		coverageList = c;
	}
	
	public ForClauseElements( String v, CoverageList c, ForClauseElements next ){
		var = v;
		coverageList = c;
		this.next = next;
	}
		
	public String toXML(){
		
		String result="";
			result += "<coverageIterator>";
			result +="<iteratorVar>" + var + "</iteratorVar>" ;	
			result +=  coverageList.toXML() ;
			result += "</coverageIterator>";
			if (next != null) {
			    result += next.toXML();
			}
		return result;
	}
}
