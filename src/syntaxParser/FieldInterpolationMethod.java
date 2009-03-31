package syntaxParser;
/**
 * InterpMethod
 * Creation date: (3/3/2003 2:28:43 AM)
 * @author: mattia parigiani, Sorin Stancu-Mara
 */
class FieldInterpolationMethod implements IParseTreeNode {
	String type;
	String nullresistence;
	
	public FieldInterpolationMethod(String type, String nr ) {
		this.type = type;
		nullresistence = nr;
	}

	public String toXML(){
		return "<interpolationMethod>" + type + "</interpolationMethod>" +
			   "<nullResistence>" + nullresistence + "</nullResistence>";
	}
}
