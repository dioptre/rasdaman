package wcps.server.core;

// Thrown when WCPS received a request, but could not evaluate it because the metadata for some coverage does not conform to the WCPS standard.

public class InvalidMetadataException extends WCPSException {

    private static final long serialVersionUID = 48924938L;

    public InvalidMetadataException( String message ) {

        super( message, null );

    }

}
