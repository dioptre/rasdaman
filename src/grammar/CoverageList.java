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




package grammar;

/**
 * CoverageList class represents a CoverageList.
 * Creation date: (3/3/2003 2:52:55 AM)
 * @author: mattia parigiani, Sorin Stancu-Mara, Andrei Aiordachioaie
 */

public class CoverageList implements IParseTreeNode
{
	private String coverageName;
	private CoverageList next;

	public CoverageList(String c)
	{
		coverageName = c;
		next         = null;
	}

	public CoverageList(String c, CoverageList l)
	{
		coverageName = c;
		next         = l;
	}

	public String toXML()
	{
		String result = "<coverageName>" + coverageName + "</coverageName>";

		if (next != null)
		{
			result += next.toXML();
		}

		return result;
	}

    public String toString()
    {
        String result = coverageName;
        if (next != null)
            result += next.toString();

        return result;
    }
}
