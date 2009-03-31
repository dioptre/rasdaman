package syntaxParser;
/**
 * CoverageList class represents a CoverageList.
 * Creation date: (3/3/2003 2:52:55 AM)
 * @author: mattia parigiani, Sorin Stancu-Mara
 */
import java.util.*;
public class CoverageList implements IParseTreeNode {
	private String coverageName;
	private CoverageList next;
	
	public CoverageList(String c) {
	    coverageName = c;
	    next = null;
	}
	
	public CoverageList(String c, CoverageList l) {
		coverageName = c;
		next = l;
	}

	
	public String toXML(){
		String result =  "<coverageName>" + coverageName + "</coverageName>";
		if (next != null) {
		    result += next.toXML();
		}		
		return result;
	}
		
	
	
}
