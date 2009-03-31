package syntaxParser;
/**
 * ComplexLit
 * Creation date: (3/3/2003 2:28:43 AM)
 * @author: mattia parigiani
 */
class ComplexLit {

	ScalarLitList scalarLitList;

	/**
	 * ComplexLit constructor comment.
	 */
	 
	public ComplexLit() {
		super();
	}
	public ComplexLit( ScalarLitList l ){	
		scalarLitList = l;
	}
		
	
	String toXML(){
		String result="" ;
		try {
			result += "complex literal not defined XSD";
		} catch (Exception e ) {
			e.printStackTrace();	
		}
		return result;
	}

}
