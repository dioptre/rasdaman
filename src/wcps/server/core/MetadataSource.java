package wcps.server.core;

import java.util.Set;

// A MetadataSource is anything that can read metadata for a given coverage name. It must be able to list all coverages which it knows, return Metadata for each one, and also map a format (e.g. "png") to its mimetype (e.g. "image/png").

public interface MetadataSource {

    public Set<String> coverages() throws ResourceException;
    public String mimetype( String format );
    public Metadata read( String coverageName ) throws InvalidRequestException, ResourceException;

}
