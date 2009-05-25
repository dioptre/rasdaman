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

import java.math.BigInteger;

//A coverage axis in pixel coordinates. See the WCPS standard.

class CellDomainElement implements Cloneable
{
	private BigInteger hi;
	private BigInteger lo;

	public CellDomainElement(BigInteger lo, BigInteger hi) throws InvalidMetadataException
	{
		if ((lo == null) || (hi == null))
		{
			throw new InvalidMetadataException(
			    "Invalid cell domain element: Bounds may not be null");
		}

		if (lo.compareTo(hi) == 1)
		{
			throw new InvalidMetadataException(
			    "Invalid cell domain element: Lower bound cannot be larger than upper bound");
		}

		this.lo = lo;
		this.hi = hi;

	}

	public CellDomainElement clone()
	{
		try
		{
			return new CellDomainElement(BigInteger.ZERO.add(lo),
						     BigInteger.ZERO.add(hi));
		}
		catch (InvalidMetadataException ime)
		{
			throw new RuntimeException(
			    "Invalid metadata while cloning CellDomainElement. This is a software bug in WCPS.",
			    ime);
		}

	}

	public boolean equals(CellDomainElement cde)
	{
		return lo.equals(cde.lo) && hi.equals(cde.hi);

	}

	public BigInteger getHi()
	{
		return hi;

	}

	public BigInteger getLo()
	{
		return lo;

	}
}
