package syntaxParser;
/**
 * AtomicLit
 * Creation date: (3/3/2003 2:28:43 AM)
 * @author: mattia parigiani
 */
class AtomicLit {


	BooleanExpr booleanExpr;
	int integerExpr;
	float floatExpr;
	String stringExpr;
	
	String flag;

	/**
	 * AtomicLit constructor comment.
	 */
	public AtomicLit() {
		super();
	}
	public AtomicLit( BooleanExpr be){	
		booleanExpr = be;
		flag = "boolean";
	}
	
	public AtomicLit( int ie){	
		stringExpr = ie.toString();
		flag = "int";
	}		
	
	public AtomicLit( float fe){	
		stringExpr = fe.toString();
		flag = "float";
	}

	public AtomicLit( String se){	
		stringExpr = se;
		flag = "string";
	}	
	
	String toXML(){
		String result="" ;
		try {
			if (flag.equals("boolean")) {
				result += "<boolean>"  + booleanExpr.toXML() + "</boolean>";
			} else if (flag.equals("int") || flag.equals("float") || flag.equals("string")) {
                result += "<scalar>" + stringExpr + "</scalar>
			}
		} catch (Exception e) {
			e.printStackTrace();	
		}
		return result;
	}
}
