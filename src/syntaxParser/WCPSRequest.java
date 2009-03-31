package syntaxParser;
/**
 * WCPSRequest class represents a WCPSRequest.
 * Creation date: (3/3/2003 2:52:55 AM)
 * @author: *, Sorin Stancu-Mara
 */
import java.util.*;
public class WCPSRequest implements IParseTreeNode {
	private ForClause forClause;
	private WhereClause whereClause;
	private ReturnClause returnClause;
	
	/**
	 * WCPSRequest constructor comment.
	 */
	public WCPSRequest() {
		super();
	}
	public WCPSRequest(ForClause f, ReturnClause r) {
		forClause = f;
		whereClause = null;
		returnClause = r;
	}
	public WCPSRequest(ForClause f, WhereClause w, ReturnClause r) {
		forClause = f;
		whereClause = w;
		returnClause = r;
	}

	public String toXML(){
		String result="<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n";
		result += "<processCoverage xmlns=\"http://www.opengis.net/wcps/1.0.0\" service=\"WCPS\" version=\"1.0.0\">\n";
		result += forClause.toXML();
		if (whereClause != null) {
			result += whereClause.toXML();
		}
		result += returnClause.toXML();
		result += "</processCoverage>";
		return result;
	}
	
}
