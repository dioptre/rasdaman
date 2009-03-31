package grammar;
/**
 * SliceExpr
 * Creation date: (3/3/2003 2:28:43 AM)
 * @author: mattia parigiani, Sorin Stancu-Mara, Andrei Aiordachioaie
 */
public class SliceExpr implements IParseTreeNode {

	CoverageExpr expr;
	DimensionPointList list;

	public SliceExpr( CoverageExpr ce, DimensionPointList apl){
		expr = ce;
		list = apl;
	}
	
	public String toXML(){
		return "<slice>" + expr.toXML() + list.toXML() + "</slice>";
	}
}
