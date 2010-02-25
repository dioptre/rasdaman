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
import petascope.wcps.server.exceptions.WCPSException;
import org.w3c.dom.*;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

public class CoverageIterator implements IRasNode
{
	private List<String> coverageNames;
	private String iteratorName;
    private boolean dynamic = false;    // created from a Construct Coverage expr?

	public CoverageIterator(Node x, XmlQuery xq) throws WCPSException
	{
		IDynamicMetadataSource source = xq.getMetadataSource();
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
						throw new WCPSException("Unknown coverage " + cn);
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

    public CoverageIterator(String iterator, String coverage) throws WCPSException
	{
		coverageNames = new ArrayList<String>();
        iteratorName = iterator;
        coverageNames.add(coverage);
        this.dynamic = true;
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
		// TODO(andreia) : How to translate multiple coverages?
		return coverageNames.get(0) + " AS " + iteratorName;
	}
}
