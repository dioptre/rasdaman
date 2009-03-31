package wcps.server.core;

// This exception is thrown whenever some required resource is not available - a database connection, file, whatever.

public class ResourceException extends WCPSException {

    private static final long serialVersionUID = 45562458L;

    public ResourceException( String message) {
	super (message);
    }

    public ResourceException( String message, Throwable cause ) {

        super( message, cause );

    }

}
