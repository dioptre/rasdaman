package syntaxParser;
/* Author: Sorin Stancu-Mara
 * Date: 8 Feb 2008
 */

class AxisPointElement implements IParseTreeNode {
	String axis;
	String crs;
	String point;

	public AxisPointElement(String name, String p) {
		axis = name;
		point = p;
	}

	public AxisPointElement(String name, String crs, String p) {
		axis = name;
		point = p;
		this.crs = crs;
	}

	public String toXML() {
		String result = "<axis>" + axis + "</axis>";
		if (crs != null)
			result += "<crs>" + crs + "</crs>";
		result += "<coord>" + point + "</coord>";
		return result;
	}
	
}
