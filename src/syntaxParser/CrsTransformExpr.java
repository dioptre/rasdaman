package syntaxParser;
/**
 * CrsTransformExpr
 * Creation date: (3/3/2003 2:28:43 AM)
 * @author: mattia parigiani, Sorin Stancu-Mara
 */
class CrsTransformExpr implements IParseTreeNode {
	CoverageExpr coverageExpr;
	AxisCrsList axisCrsList;
	FieldInterpolationList interpList;
	
	public CrsTransformExpr( CoverageExpr ce, AxisCrsList axisList, FieldInterpolationList fil ){	
		coverageExpr = ce;
		axisCrsList = axisList;
		axisCrsList.setTag("crsTransformAxisSpec");		
		interpList = fil;
		interpList.setTag("crsTransformFieldSpec");
	}
	public String toXML(){
		return "<crsTransform>" +  axisCrsList.toXML() + interpList.toXML() +
			coverageExpr.toXML() + "</crsTransform>";
	}
}
