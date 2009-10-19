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

//A single component of a coverage's range. See the WCPS standard for more information.

public class RangeElement implements Cloneable
{
	private String name;
	private String type;

	public RangeElement(String name, String type) throws InvalidMetadataException
	{
		if ((name == null) || (type == null))
		{
			throw new InvalidMetadataException(
			    "Invalid range element: Element name and type cannot be null");
		}

		if (name.equals(""))
		{
			throw new InvalidMetadataException(
			    "Invalid range element: Element name cannot be empty");
		}

		if (!(type.equals("boolean") || type.equals("char") || type.equals("unsigned char")
		    || type.equals("short") || type.equals("unsigned short") || type.equals("int")
		    || type.equals("unsigned int") || type.equals("long")
		    || type.equals("unsigned long") || type.equals("float")
		    || type.equals("double") || type.equals("complex") || type.equals("complex2")))
		{
			throw new InvalidMetadataException(
			    "Invalid range element: Invalid element type:" + type);
		}

		this.name = name;
		this.type = type;

	}

	public RangeElement clone()
	{
		try
		{
			return new RangeElement(new String(name), new String(type));
		}
		catch (InvalidMetadataException ime)
		{
			throw new RuntimeException(
			    "Invalid metadata while cloning RangeElement. This is a software bug in WCPS.",
			    ime);
		}

	}

	public boolean equals(RangeElement re)
	{
		return name.equals(re.type);

	}

	public String getName()
	{
		return name;

	}

	public String getType()
	{
		return type;

	}

	public boolean isBoolean()
	{
		return type.equals("boolean");

	}

	public static boolean isBoolean(String type)
	{
		return type.equals("boolean");

	}

	public boolean isComplex()
	{
		return type.equals("complex") || type.equals("complex2");

	}

	public static boolean isComplex(String type)
	{
		return type.equals("complex") || type.equals("complex2");

	}

	public boolean isIntegral()
	{
		return type.equals("char") || type.equals("short") || type.equals("unsigned short")
		       || type.equals("int") || type.equals("unsigned int") || type.equals("long")
		       || type.equals("unsigend long");

	}

	public static boolean isIntegral(String type)
	{
		return type.equals("char") || type.equals("short") || type.equals("unsigned short")
		       || type.equals("int") || type.equals("unsigned int") || type.equals("long")
		       || type.equals("unsigend long");

	}

	public boolean isFloating()
	{
		return type.equals("float") || type.equals("double");

	}

	public static boolean isFloating(String type)
	{
		return type.equals("float") || type.equals("double");

	}

	public boolean isNumeric()
	{
		return type.equals("char") || type.equals("short") || type.equals("unsigned short")
		       || type.equals("int") || type.equals("unsigned int") || type.equals("long")
		       || type.equals("unsigend long") || type.equals("float")
		       || type.equals("double") || type.equals("complex")
		       || type.equals("complex2");

	}

	public static boolean isNumeric(String type)
	{
		return type.equals("char") || type.equals("short") || type.equals("unsigned short")
		       || type.equals("int") || type.equals("unsigned int") || type.equals("long")
		       || type.equals("unsigend long") || type.equals("float")
		       || type.equals("double") || type.equals("complex")
		       || type.equals("complex2");

	}

	public void setType(String type) throws InvalidMetadataException
	{
		if (!(type.equals("boolean") || type.equals("char") || type.equals("unsigned char")
		    || type.equals("short") || type.equals("unsigned short") || type.equals("int")
		    || type.equals("unsigned int") || type.equals("long")
		    || type.equals("unsigned long") || type.equals("float")
		    || type.equals("double") || type.equals("complex") || type.equals("complex2")))
		{
			throw new InvalidMetadataException(
			    "Invalid range element: Invalid element type:" + type);
		}

		this.type = type;

	}

    public String toString()
    {
        String r = "Range Element { Name '" + name + "', Type '" + type + "'}";
        return r;
    }
}
