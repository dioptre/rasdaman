/*
 * This file is part of PetaScope.
 *
 * PetaScope is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * PetaScope is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with PetaScope. If not, see <http://www.gnu.org/licenses/>.
 *
 * For more information please see <http://www.PetaScope.org>
 * or contact Peter Baumann via <baumann@rasdaman.com>.
 *
 * Copyright 2009 Jacobs University Bremen, Peter Baumann.
 */


package wcps.server.core;

import java.util.Collection;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;
import java.util.Set;

/**
 * A IMetadataSource that caches reads from another IMetadataSource. It reads
 * and caches all coverages at startup, in order to make reads faster and so
 * that WCPS will continue to work once it initializes correctly, even if the
 * database cannot be accessed, or metadata is changed into an invalid state.
 */

public class CachedMetadataSource implements IMetadataSource
{
	private Set<String> coverageNames;
	private Map<String, Metadata> metadata;
	private IMetadataSource metadataSource;
	private Map<String, String> supportedFormats;

	public CachedMetadataSource(IMetadataSource metadataSource)
	    throws ResourceException, InvalidMetadataException
	{
		this.metadataSource = metadataSource;

		coverageNames       = metadataSource.coverages();
		metadata            = new HashMap<String, Metadata>(coverageNames.size());
		supportedFormats    = new HashMap<String, String>();
		Iterator<String> i = coverageNames.iterator();

		try
		{
			while (i.hasNext())
			{
				String coverage = i.next();
				metadata.put(coverage, metadataSource.read(coverage));
			}
		}
		catch (InvalidRequestException ire)
		{
			throw(InvalidMetadataException) ire.getCause();
		}

	}

	public Set<String> coverages()
	{
		return coverageNames;

	}

	public String mimetype(String format)
	{
		if (supportedFormats.containsKey(format))
		{
			return supportedFormats.get(format);
		}
		else
		{
			String mimetype = metadataSource.mimetype(format);

			synchronized (this)
			{
				supportedFormats.put(format, mimetype);
			}

			return mimetype;
		}

	}

	public Metadata read(String coverageName) throws InvalidRequestException
	{
		if ((coverageName == null) || coverageName.equals(""))
		{
			throw new InvalidRequestException(
			    "Cannot retrieve coverage with null or empty name");
		}

		if (!coverageNames.contains(coverageName))
		{
			throw new InvalidRequestException("Coverage '" + coverageName
							  + "' is not served by this server");
		}

		return metadata.get(coverageName).clone();

	}

    public Collection<String> getAxisNames()
    {
        return metadataSource.getAxisNames();
    }
}
