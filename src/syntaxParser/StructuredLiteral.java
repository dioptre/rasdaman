package syntaxParser;
/* Author: Sorin Stancu-Mara
 * Date 9 Feb 2007
 */

class StructuredLiteral implements IParseTreeNode {
	RangeValueList values;
	public StructuredLiteral(RangeValueList rvl) {
		values = rvl;
	}

	public String toXML() {
		return values.toXML();
	}
}
