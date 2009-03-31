package grammar;
/**
 * CondenseExpr
 * Creation date: (3/3/2003 2:28:43 AM)
 * @author: mattia parigiani, Sorin Stancu-Mara, Andrei Aiordachioaie
 */

public class CondenseExpr implements IParseTreeNode{

	IParseTreeNode expr;
    String fun;

	public CondenseExpr(IParseTreeNode re)
    {
		expr = re;
	}

	public String toXML() 
    {    
    	return expr.toXML();
	}
}
