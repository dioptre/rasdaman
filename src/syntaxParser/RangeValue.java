package syntaxParser;
/* Author: Sorin stancu-Mara
 * Date: 9 Feb 2008
 */

class RangeValue implements IParseTreeNode {
	String value;
	public RangeValue(String value) {
	    this.value = value;
	}
	public RangeValue(StructuredLiteral value) {
	    this.value = value.toXML();
	}
	public String toXML() {
	    return value;
	}
}
