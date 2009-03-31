package grammar;

/* Author: Sorin Stancu-Mara, Andrei Aiordachioaie
 * Date: 8 Feb 2008
 */

public class VariableList implements IParseTreeNode {
	String axisType, iteratorName;
	String lo,hi;
	VariableList next;

	public VariableList(String type, String name, String lo, String hi) {
		axisType = type;
		iteratorName = name;
		this.lo = lo;
		this.hi = hi;
		next = null;
	}

	public VariableList(String type, String name, String lo, String hi, VariableList next) {
		axisType = type;
		iteratorName = name;
		this.lo = lo;
		this.hi = hi;
		this.next = next;
	}

	public String toXML() {
		String result = "<axisIterator><axisType>" + axisType + "</axisType>" +
						"<iteratorVar>" + iteratorName + "</iteratorVar>" + 
						"<coord>" + lo + "</coord>" +
						"<coord>" + hi + "</coord></axisIterator>";
		
		if (next != null) result += next.toXML();
		return result;
	}

}
