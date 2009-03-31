package wcps.server.core;

// This exception is throws whenever a user submits a request that is not valid in some way.

public class InvalidRequestException extends WCPSException {

    private static final long serialVersionUID = 65492846L;

    public InvalidRequestException( String message ) {

        super( message, null );

    }

    public InvalidRequestException( String message, Throwable cause ) {

        super( message, cause );

    }

}
