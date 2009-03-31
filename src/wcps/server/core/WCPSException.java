package wcps.server.core;

// This is the superclass of all WCPS exceptions.

public class WCPSException extends Exception {

    private static final long serialVersionUID = 113213254L;

    public WCPSException( String message) {
	super(message);
    }
    
    public WCPSException( String message, Throwable cause ) {

        super( message, cause );

    }

}
