package grammar;

/**
 * WCPSRequest class represents a WCPSRequest.
 * Creation date: (3/3/2003 2:52:55 AM)
 * @author: *, Sorin Stancu-Mara, Andrei Aiordachioaie
 */

public class WCPSRequest implements IParseTreeNode
{
    private ForClause forClause;
    private ReturnClause returnClause;
    private WhereClause whereClause;

    public WCPSRequest()
    {
        super();
    }

    public WCPSRequest(ForClauseElements f)
    {
        forClause = new ForClause(f);
        whereClause = null;
        returnClause = null;
    }

    public void setWhere(WhereClause w)
    {
        whereClause = w;
    }

    public void setReturn(ReturnClause r)
    {
        returnClause = r;
    }

    public String toXML()
    {
        String result = "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n";
        result += "<processCoverage xmlns=\"http://www.opengis.net/wcps/1.0\" service=\"WCPS\" version=\"1.0.2\">\n";

        result += forClause.toXML();
        if (whereClause != null)
            result += whereClause.toXML();
        if (returnClause != null)
            result += returnClause.toXML();

        result += "</processCoverage>";

        return result;
    }
}
