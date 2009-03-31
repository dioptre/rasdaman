package syntaxParser;
/* Author: Sorin stancu-Mara
 * date: 8 feb 2008
 * All the lists have the same caracteristics, therfore all of them derive from this class
 * No need to verify that the list contain same type of elements since they are created using the grammar.
 */

class List implements IParseTreeNode {
	IParseTreeNode element;
	List next;
	String tag;

	public List(IParseTreeNode elem) {
		element = elem;
		next = null;		
	}

	public List(IParseTreeNode elem, List next) {
		element = elem;
		this.next = next;
	}

	public void setTag(String tagName) {
		tag = tagName;
		if (next != null) {
			next.setTag(tag);
		}
	}

	public String toXML() {
		String result = "";
		if (tag != null) {
			result += "<" + tag + ">";
			result += element.toXML();
			result += "</" + tag + ">";
		} else
			result += element.toXML();
		if (next != null)
			result += next.toXML();
		return result;
	}
}
