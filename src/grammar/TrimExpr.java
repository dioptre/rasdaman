package grammar;
/**
 * TrimExpr
 * Creation date: (3/3/2003 2:28:43 AM)
 * @author: mattia parigiani, Sorin Stancu-Mara, Andrei Aiordachioaie
 */
public class TrimExpr implements IParseTreeNode
{

	CoverageExpr expr;
	DimensionIntervalList list;
	
	public TrimExpr( CoverageExpr ce, DimensionIntervalList al )
    {
		expr = ce;
		list = al;
	}

	public String toXML()
    {
	 	return "<trim>"	+ expr.toXML() + list.toXML() + "</trim>";
	}
}
