package syntaxParser;
/**
 * ScalarLitList class represents a CoverageList.
 * Creation date: (3/3/2003 2:52:55 AM)
 * @author: mattia parigiani
 */
import java.util.*;
public class ScalarLitList {
	private Vector scalarLits;
	
	
	/**
	 * CoverageList constructor comment.
	 */
	public ScalarLitList() {
		super();
	}
	public ScalarLitList(ScalarLit sl) {
		scalarLits = new Vector();
		scalarLits.add(sl);
	}
	
	public ScalarLitList(ScalarLit sl, ScalarLitList l) {
		scalarLits = l.getScalarLits();
		scalarLits.add(sl);
	}

	
	public Vector getScalarLits(){
		return scalarLits;
	}
	
	String toXML(){
		String result="" ;
		
		
		for (Enumeration e=scalarLits.elements(); e.hasMoreElements(); ){
			/*String s= (String)e.nextElement();
			result += "\n<coverageOrSetName>";
			result += (s instanceof String)? s: " ";
			result += "</coverageOrSetName>\n";
			*/
		}
		
		
		result += "just parsing";
		return result;
	}
		
	
	
}
