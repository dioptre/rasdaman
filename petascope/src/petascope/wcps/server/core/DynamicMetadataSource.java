/*
 * This file is part of rasdaman community.
 *
 * Rasdaman community is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Rasdaman community is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with rasdaman community.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright 2003, 2004, 2005, 2006, 2007, 2008, 2009 Peter Baumann /
 rasdaman GmbH.
 *
 * For more information please see <http://www.rasdaman.org>
 * or contact Peter Baumann via <baumann@rasdaman.com>.
 */
package petascope.wcps.server.core;

import petascope.wcps.server.exceptions.ResourceException;
import petascope.wcps.server.exceptions.InvalidWcpsRequestException;
import petascope.wcps.server.exceptions.InvalidMetadataException;
import java.util.Collection;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Iterator;
import java.util.Map;
import java.util.Set;

/**
 * A IMetadataSource that allows WCPS to store information about on-the-fly
 * constructed coverages (for example Construct Coverage expr).
 * Needs another MetadataSource as a backend, to retrieve metadata about
 * static coverages.
 */
public class DynamicMetadataSource implements IDynamicMetadataSource {
    // Static coverages, served by the server at all times

    private Set<String> staticCoverageNames;
    // Dynamic coverages, built on-the-fly in a query
    private Set<String> dynamicCoverageNames;
    // Union of static and dynamic coverages
    private Set<String> allCoverageNames;
    // Metadata information for all available coverages
    private Map<String, Metadata> metadata;
    // Other metadata class that serves as backend
    private IMetadataSource metadataSource;

    public DynamicMetadataSource(IMetadataSource metadataSource)
            throws ResourceException, InvalidMetadataException {
        this.metadataSource = metadataSource;
        staticCoverageNames = metadataSource.coverages();
        dynamicCoverageNames = new HashSet<String>();
        allCoverageNames = staticCoverageNames;
        metadata = new HashMap<String, Metadata>();

        // Init metadata for static coverages
        Iterator<String> i = staticCoverageNames.iterator();
        try {
            while (i.hasNext()) {
                String coverage = i.next();
                metadata.put(coverage, metadataSource.read(coverage));
            }
        } catch (InvalidWcpsRequestException ire) {
            throw (InvalidMetadataException) ire.getCause();
        }
    }

    public Set<String> coverages() throws ResourceException {
        return metadataSource.coverages();
    }

    public String formatToMimetype(String format) {
        return metadataSource.formatToMimetype(format);
    }

    public Metadata read(String coverageName) throws InvalidWcpsRequestException, ResourceException {
        if ((coverageName == null) || coverageName.equals("")) {
            throw new InvalidWcpsRequestException(
                    "Cannot retrieve coverage with null or empty name");
        }

        if (!this.coverages().contains(coverageName)) {
            throw new InvalidWcpsRequestException("Coverage '" + coverageName
                    + "' is not served by this server");
        }

        return metadataSource.read(coverageName);
    }

    public void addDynamicMetadata(String coverageName, Metadata meta) {
        metadata.put(coverageName, meta);
        dynamicCoverageNames.add(coverageName);
        allCoverageNames = staticCoverageNames;
        allCoverageNames.addAll(dynamicCoverageNames);
    }

    public Collection<String> getAxisNames() {
        return metadataSource.getAxisNames();
    }
}
