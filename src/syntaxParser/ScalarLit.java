package syntaxParser;
/**
 * ScalarLit
 * Creation date: (3/3/2003 2:28:43 AM)
 * @author: mattia parigiani
 */
class ScalarLit {

	AtomicLit atomicLit;
	ComplexLit complexLit;
	String flag;

	/**
	 * ScalarLit constructor comment.
	 */
	public ScalarLit() {
		super();
	}
	public ScalarLit( AtomicLit al){	
		atomicLit = al;
		flag = "atomicLit";
	}
	
	public ScalarLit( ComplexLit cl){	
		complexLit = cl;
		flag = "complexLit";
	}
		
	String toXML(){
		String result="" ;
		try {
			if (flag.equals("complexLit")) {
				result += complexLit.toXML();	
			} else if (flag.equals("atomicLit")) {
				result += atomicLit.toXML();	
			}
		} catch (Exception e ) {
			e.printStackTrace();	
		}
		return result;
	}
}
