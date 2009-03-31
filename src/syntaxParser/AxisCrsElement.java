package syntaxParser;
/* Author: Sorin Stancu-Mara
 * Date: 8 Feb 2008
 */

class AxisCrsElement implements IParseTreeNode {
	String axis;
	String crs;

	public AxisCrsElement(String name, String crs) {
		axis = name;
		this.crs = crs;
	}

	public String toXML() {
		return "<axis>" + axis + "</axis><crs>" + crs + "</crs>";
	}
	
}
