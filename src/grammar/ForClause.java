package grammar;

/**
 * ForClause
 * Creation date: (3/3/2003 2:28:43 AM)
 * @author: mattia parigiani, Sorin Stancu-Mara, Andrei Aiordachioaie
 */

public class ForClause implements IParseTreeNode {
	ForClauseElements forClauseElements;

    public ForClause( ForClauseElements fce )
    {
		forClauseElements = fce;
	}
	
	public String toXML()
    {
		return forClauseElements.toXML();
	}
}
