package syntaxParser;
/* Author: Sorin Stancu-Mara
 * Date: 9 Feb 2008
 * Outside the other Lists becasuse this one works with strings.
 */

class CrsList implements IParseTreeNode {
	String elem;
	String tag;
	CrsList next;
	public CrsList(String e) {
		elem = e;
	}
	public CrsList(String e, CrsList n) {
		elem = e;
		next = n;
	}
	public void setTag(String newTag) {
		tag =newTag;
		if (next != null) {
			next.setTag(tag);
		}
	}
	public String toXML() {
		String result = "";
		if (tag != null)  result += "<" + tag + ">" + elem + "</" + tag + ">";
		else result += elem;
		if (next != null)
			result += next.toXML();
		return result;
		
	}
}
