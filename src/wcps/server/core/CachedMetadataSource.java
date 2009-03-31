package wcps.server.core;

import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Set;

// A MetadataSource that caches reads from another MetadataSource. It reads and caches all coverages at startup, in order to make reads faster and so that WCPS will continue to work once it initializes correctly, even if the database cannot be accessed, or metadata is changed into an invalid state.

public class CachedMetadataSource implements MetadataSource {

    private Set<String> coverageNames;
    private MetadataSource metadataSource;
    private Map<String,Metadata> metadata;
    private Map<String,String> supportedFormats;

    public CachedMetadataSource( MetadataSource metadataSource ) throws ResourceException, InvalidMetadataException {

        this.metadataSource = metadataSource;

        coverageNames = metadataSource.coverages();
        metadata = new HashMap<String,Metadata>( coverageNames.size() );
        supportedFormats = new HashMap<String,String>();
        Iterator<String> i = coverageNames.iterator();
        try {
            while( i.hasNext() ) {
                String coverage = i.next();
                metadata.put( coverage, metadataSource.read( coverage ) );
            }
        }
        catch( InvalidRequestException ire ) {
            throw (InvalidMetadataException) ire.getCause();
        }

    }

    public Set<String> coverages() {

        return coverageNames;

    }

    public String mimetype( String format ) {

        if( supportedFormats.containsKey( format ) ) {
            return supportedFormats.get( format );
        }
        else {
            String mimetype = metadataSource.mimetype( format );
            synchronized( this ) {
                supportedFormats.put( format, mimetype );
            }
            return mimetype;
        }

    }

    public Metadata read( String coverageName ) throws InvalidRequestException {

        if( coverageName == null || coverageName.equals( "" ) ) {
            throw new InvalidRequestException( "Cannot retrieve coverage with null or empty name" );
        }

        if( !coverageNames.contains( coverageName ) ) {
            throw new InvalidRequestException( "Coverage '" + coverageName + "' is not served by this server" );
        }

        return metadata.get( coverageName ).clone();

    }

}
