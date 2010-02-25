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


package petascope.wcps.server.core;

import petascope.wcps.server.exceptions.ResourceException;
import petascope.wcps.server.exceptions.InvalidWcpsRequestException;
import java.util.Collection;
import java.util.Set;

/**
 * A IMetadataSource is anything that can read metadata for a given coverage
 * name. It must be able to list all coverages which it knows, return Metadata
 * for each one, and also map a format (e.g. "png") to its formatToMimetype
 * (e.g. "image/png").
 */

public interface IMetadataSource
{
	public Set<String> coverages() throws ResourceException;

	public String formatToMimetype(String format);

	public Metadata read(String coverageName) throws InvalidWcpsRequestException, ResourceException;

    public Collection<String> getAxisNames();
}
