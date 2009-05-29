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

import org.w3c.dom.*;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

public class CoverageIterator implements IRasNode
{
	private List<String> coverageNames;
	private String iteratorName;

	public CoverageIterator(Node x, ProcessCoveragesRequest pcr) throws WCPSException
	{
		MetadataSource source = pcr.getMetadataSource();

		coverageNames = new ArrayList<String>();

		if (! x.getNodeName().equals("coverageIterator"))
		{
			throw new WCPSException("Invalid cast from " + x.getNodeName()
						+ " XML node to CoverageIterator node");
		}

		Node it = x.getFirstChild();

		while (it != null)
		{
			if (it.getNodeName().equals("#text"))
			{
				it = it.getNextSibling();
				continue;
			}

			if (it.getNodeName().equals("iteratorVar"))
			{
				iteratorName = it.getFirstChild().getNodeValue();
				System.err.println("*** Iterator variable : " + iteratorName);
			}
			else if (it.getNodeName().equals("coverageName"))
			{
				String cn = it.getFirstChild().getNodeValue();

				System.err.println("*** Coverage reference : " + cn);

				try
				{
					if (!source.coverages().contains(cn))
					{
						throw new WCPSException("Unknown coverage " + cn);
					}
				}
				catch (ResourceException e)
				{
					throw new WCPSException(
					    "Cannot load coverage information!", e);
				}

				coverageNames.add(cn);
			}

			it = it.getNextSibling();
		}
	}

	public Iterator<String> getCoverages()
	{
		return coverageNames.iterator();
	}

	public String getIteratorName()
	{
		return iteratorName;
	}

	public String toRasQL()
	{
		// TODO(smsorin) : How to translate multiple coverages?
		return coverageNames.get(0) + " AS " + iteratorName;
	}
}
