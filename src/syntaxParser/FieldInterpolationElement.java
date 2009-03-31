package syntaxParser;
/* Authod: Sorin Stancu-Mara
 * Date: 8 Feb 2008
 */

class FieldInterpolationElement implements IParseTreeNode {
	String field;
	FieldInterpolationMethod method;

	public FieldInterpolationElement(String field, FieldInterpolationMethod m) {
		this.field = field;
		this.method = m;
	}

	public String toXML() {
		return "<field>" + field + "</field>" + method.toXML();
	}
}
