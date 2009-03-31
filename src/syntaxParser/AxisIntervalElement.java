package syntaxParser;
/* Author: Sorin Stancu-Mara
 * Date: 8 Feb 2008
 */

class AxisIntervalElement implements IParseTreeNode {
	String axisName;
	String lo, hi;
	String crs;

	public AxisIntervalElement(String name, String lo, String hi) {
		axisName = name;
		this.lo = lo;
		this.hi = hi;
	}

	public AxisIntervalElement(String name,String crs, String lo, String hi) {
		axisName = name;
		this.crs = crs;
		this.lo = lo;
		this.hi = hi;
	}

	public String toXML() {
		String result = "<axis>" + axisName + "</axis>";
		if (crs != null) 
			result += "<crs>" + crs + "</crs>";
		result += "<coord>" + lo + "</coord><coord>" + hi + 
			      "</coord>";
		return result;
	}
}
