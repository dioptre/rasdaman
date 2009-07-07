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
import java.util.HashSet;
import java.util.Iterator;
import java.util.Set;

//This is an axis in geographic coordinates. See the WCPS standard.

class DomainElement implements Cloneable
{
	public static final String IMAGE_CRS = "CRS:1";
	private Set<String> crss;
	private String name;
	private Double numHi;
	private Double numLo;
	private String strHi;
	private String strLo;
	private String type;
    private Collection<String> allowedAxes;

	public DomainElement(String name, String type, Double numLo, Double numHi, String strLo,
			     String strHi, Set<String> crss, Collection<String> axes)
	    throws InvalidMetadataException
	{
        this.allowedAxes = axes;

		if ((name == null) || (type == null))
		{
			throw new InvalidMetadataException(
			    "Invalid domain element: Element name and type cannot be null");
		}

		if (name.equals(""))
		{
			throw new InvalidMetadataException(
			    "Invalid domain element: Element name cannot be empty");
		}

        if (allowedAxes.contains(type) == false)
		{
			throw new InvalidMetadataException(
			    "Invalid domain element: Invalid element type: " + type +
                ". Allowed element types are: " + allowedAxes.toString());
		}

		if ((numLo != null) && (numHi != null) && (strLo == null) && (strHi == null))
		{
			if (numLo.compareTo(numHi) == 1)
			{
				throw new InvalidMetadataException(
				    "Invalid domain element: Lower integer bound cannot be larger than upper integer bound");
			}

			this.numLo = numLo;
			this.numHi = numHi;
		}
		else if ((strLo != null) && (numHi != null) && (numLo == null) && (numHi == null))
		{
			if (strLo.equals("") || strHi.equals(""))
			{
				throw new InvalidMetadataException(
				    "Invalid domain element: String bounds cannot be empty");
			}

			this.strLo = strLo;
			this.strHi = strHi;
		}
		else
		{
			throw new InvalidMetadataException(
			    "Invalid domain element: Integer bounds must both be non-null if string bounds are null, and vice versa at "
			    + name + ":" + type);
		}

		if ((type.equals("x") || type.equals("y")) && (numLo == null))
		{
			throw new InvalidMetadataException(
			    "Invalid domain element: A spatial axis must have integer extent");
		}
		else if (type.equals("temporal") && (strLo == null))
		{
			throw new InvalidMetadataException(
			    "Invalid domain element: A temporal axis must have string extent");
		}

		this.name = name;
		this.type = type;

		if ((crss == null) || !crss.contains(IMAGE_CRS))
		{
			throw new InvalidMetadataException(
			    "Invalid domain element: CRS set does not contain image CRS '"
			    + IMAGE_CRS + "'");
		}

		this.crss = crss;

	}

	public DomainElement clone()
	{
		Set<String> c      = new HashSet<String>(crss.size());
		Iterator<String> i = crss.iterator();

		while (i.hasNext())
		{
			c.add(new String(i.next()));
		}

		try
		{
			if (numLo != null)
			{
				return new DomainElement(new String(name), new String(type),
							 new Double(numLo), new Double(numHi),
							 null, null, c, allowedAxes);
			}
			else
			{
				return new DomainElement(new String(name), new String(type), null,
							 null, new String(strLo),
							 new String(strHi), c, allowedAxes);
			}
		}
		catch (InvalidMetadataException ime)
		{
			throw new RuntimeException(
			    "Invalid metadata while cloning DomainElement. This is a software bug in WCPS.",
			    ime);
		}

	}

	public boolean equals(DomainElement de)
	{
		if ((numLo == null) && (de.numLo == null))
		{
			return strLo.equals(de.strLo) && strHi.equals(strHi)
			       && name.equals(de.name) && type.equals(de.type);
		}
		else if ((strLo == null) && (de.strLo == null))
		{
			return numLo.equals(de.numLo) && numHi.equals(numHi)
			       && name.equals(de.name) && type.equals(de.type);
		}
		else
		{
			return false;
		}

	}

	public String getName()
	{
		return name;

	}

	public Double getNumHi()
	{
		return numHi;

	}

	public Double getNumLo()
	{
		return numLo;

	}

	public String getStrHi()
	{
		return strHi;

	}

	public String getStrLo()
	{
		return strLo;

	}

	public String getType()
	{
		return type;

	}
}
