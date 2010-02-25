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

//~--- JDK imports ------------------------------------------------------------


import petascope.wcps.server.exceptions.InvalidMetadataException;
import java.math.BigInteger;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.Iterator;
import java.util.List;
import java.util.Set;
import petascope.wcs.server.core.TimeString;

/**
 * This class implements WCPS metadata. For information on what each field
 * means, see the WCPS standard. This class provides extensive error cheching
 * and well as various utility functions.
 */

public class Metadata implements Cloneable
{
	private List<CellDomainElement> cellDomain;
	private String coverageName;
    private int coverageId = -1;    // Used when reading metadata from the DB
	private List<DomainElement> domain;
	private InterpolationMethod interpolationDefault;
	private Set<InterpolationMethod> interpolationSet;
	private String nullDefault;
	private Set<String> nullSet;
	private List<RangeElement> range;
    private String titleStr = "";
    private String abstractStr = "";
    private String keywordsStr = "";
    private Wgs84Crs crs = null;
    private CellDomainElement cellX, cellY, cellT;
    private DomainElement domX, domY, domT;

    public Metadata(List<CellDomainElement> cellDomain, List<RangeElement> range, Set<String> nullSet, String nullDefault, Set<InterpolationMethod> interpolationSet, InterpolationMethod interpolationDefault, String coverageName, List<DomainElement> domain, Wgs84Crs crs, String title, String abstr, String keywords) throws InvalidMetadataException
    {
        this(cellDomain, range, nullSet, nullDefault, interpolationSet, interpolationDefault, coverageName, domain, crs);
        this.titleStr = title;
        this.abstractStr = abstr;
        this.keywordsStr = keywords;
    }

	public Metadata(List<CellDomainElement> cellDomain, List<RangeElement> range, Set<String> nullSet, String nullDefault, Set<InterpolationMethod> interpolationSet, InterpolationMethod interpolationDefault, String coverageName, List<DomainElement> domain, Wgs84Crs crs) throws InvalidMetadataException
	{
		if ( (cellDomain == null) || (range == null) || (coverageName == null) || (nullSet == null) || (interpolationSet == null) )
		{
			throw new InvalidMetadataException("Cell domain, range list, " +
                    "coverage name, null set, and interpolation set cannot be null for coverage " + coverageName);
		}

		if ( cellDomain.size() == 0 )
		{
			throw new InvalidMetadataException("Invalid cell domain: At least " +
                    "one element is required for coverage " + coverageName);
		}

		this.cellDomain = cellDomain;

		if ( range.size() == 0 )
		{
			throw new InvalidMetadataException("At least one range element is " +
                    "required for coverage " + coverageName);
		}

		this.range = new ArrayList<RangeElement>(range.size());
		Iterator<RangeElement> ir = range.iterator();

		while (ir.hasNext())
		{
			RangeElement next = ir.next();
			Iterator<RangeElement> j = this.range.iterator();

			while (j.hasNext())
			{
				if ( j.next().getName().equals(next.getName()) )
				{
					throw new InvalidMetadataException("Duplicate range element" +
                            " name encountered for coverage " + coverageName);
				}
			}

			this.range.add(next);
		}

		if ( nullSet.size() == 0 )
		{
			throw new InvalidMetadataException("Invalid null set: At least one " +
                    "null value is required for coverage " + coverageName);
		}

		if ( nullDefault == null )
		{
			throw new InvalidMetadataException("Invalid null default: Null " +
                    "default cannot be null for coverage " + coverageName);
		}

		if ( ! nullSet.contains(nullDefault) )
		{
			throw new InvalidMetadataException("Invalid null default: Default " +
                    "null value " + nullDefault + " is not part of the null set" +
                    " for coverage " + coverageName);
		}

		Iterator<String> ns = nullSet.iterator();

		while (ns.hasNext())
		{
			String nullVal = ns.next();
			List<String> nulls = SDU.str2string(nullVal);

			if ( nulls.size() != range.size() )
			{
				throw new InvalidMetadataException("Invalid null value: " +
                        nullVal + " must have " + range.size() + " axes " +
                        "according to the range specified for coverage " + coverageName);
			}

			Iterator<String> i = nulls.iterator();
			Iterator<RangeElement> j = range.iterator();

			while (j.hasNext())
			{
				RangeElement re = j.next();

				if ( re.isBoolean() )
				{
					SDU.str2boolean(i.next());
				}
				else if ( re.isIntegral() )
				{
					SDU.str2integer(i.next());
				}
				else if ( re.isFloating() )
				{
					SDU.str2double(i.next());
				}
				else if ( re.isComplex() )
				{
					SDU.str2complex(i.next());
				}
			}
		}

		this.nullSet = nullSet;
		this.nullDefault = nullDefault;

		if ( interpolationSet.size() == 0 )
		{
			throw new InvalidMetadataException("Invalid interpolation set: " +
                    "At least one interpolation method is required for " +
                    "coverage " + coverageName);
		}

		if ( interpolationDefault == null )
		{
			interpolationDefault = new InterpolationMethod("none", "none");
		}

		boolean defaultContainedInSet = false;
		Iterator<InterpolationMethod> is = interpolationSet.iterator();

		while (is.hasNext())
		{
			if ( interpolationDefault.equals(is.next()) )
			{
				defaultContainedInSet = true;
			}
		}

		if ( ! defaultContainedInSet )
		{
			throw new InvalidMetadataException("Invalid interpolation default:" +
                    " Default interpolation method (" +
                    interpolationDefault.getInterpolationType() + "," +
                    interpolationDefault.getNullResistance() + ") is not part " +
                    "of the interpolation set for coverage " + coverageName);
		}

		this.interpolationSet = interpolationSet;
		this.interpolationDefault = interpolationDefault;
        this.crs = crs;

		this.coverageName = coverageName;

		if ( domain != null )
		{
			if ( domain.size() != cellDomain.size() )
			{
				throw new InvalidMetadataException("Domain and cell domain " +
                        "must have equal number of elements for coverage " + coverageName);
			}

			this.domain = new ArrayList<DomainElement>(domain.size());
			Iterator<DomainElement> i = domain.iterator();
            Iterator<CellDomainElement> ci = cellDomain.iterator();

			while (i.hasNext() && ci.hasNext())
			{
				DomainElement next = i.next();
                CellDomainElement cell = ci.next();
				Iterator<DomainElement> j = this.domain.iterator();
                if (next.getType().equals("x"))
                    cellX = cell;
                if (next.getType().equals("y"))
                    cellY = cell;
                if (next.getType().equals("t")
//                        || next.getType().equals("temporal")
                        )
                {
                    cellT = cell;
                    domT = next;
                }

				while (j.hasNext())
				{
					DomainElement previous = j.next();

                    // don't compare same objects
                    if (next == previous)
                        continue;

					if ( previous.getName().equals(next.getName()) )
					{
						throw new InvalidMetadataException("Duplicate domain " +
                                "element name encountered for coverage " + coverageName);
					}

					if ( previous.getType().equals("temporal") && next.getType().equals("temporal") )
					{
						throw new InvalidMetadataException("Domain can contain" +
                                " at most one temporal axis for coverage " + coverageName);
					}

					if ( previous.getType().equals("elevation") && next.getType().equals("elevation") )
					{
						throw new InvalidMetadataException("Domain can contain" +
                                " at most one elevation axis for coverage " + coverageName);
					}

					if ( previous.getType().equals("x") && next.getType().equals("x") )
					{
						throw new InvalidMetadataException("Domain can contain" +
                                " at most one x axis for coverage " + coverageName);
					}

					if ( previous.getType().equals("y") && next.getType().equals("y") )
					{
						throw new InvalidMetadataException("Domain can contain" +
                                " at most one y axis for coverage " + coverageName);
					}

					if ( next.getType().equals("x") )
					{
						boolean l = false;
						Iterator<DomainElement> k = domain.iterator();

						while (k.hasNext())
						{
							if ( k.next().getType().equals("y") )
							{
								l = true;
							}
						}

						if ( l == false )
						{
							throw new InvalidMetadataException("If domain " +
                                    "contains a x axis, it must contain a y " +
                                    "axis as well for coverage " + coverageName);
						}
					}
					else if ( next.getType().equals("y") )
					{
						boolean l = false;
						Iterator<DomainElement> k = domain.iterator();

						while (k.hasNext())
						{
							if ( k.next().getType().equals("x") )
							{
								l = true;
							}
						}

						if ( l == false )
						{
							throw new InvalidMetadataException("If domain " +
                                    "contains a y axis, it must contain a x " +
                                    "axis as well for coverage " + coverageName);
						}
					}
				}

				this.domain.add(next);
			}
		}

	}

    @Override
	public Metadata clone()
	{
		try
		{
			List<CellDomainElement> cd = new ArrayList<CellDomainElement>(cellDomain.size());
			Iterator<CellDomainElement> i = cellDomain.iterator();

			while (i.hasNext())
			{
				cd.add(i.next().clone());
			}

			List<RangeElement> r = new ArrayList<RangeElement>(range.size());
			Iterator<RangeElement> j = range.iterator();

			while (j.hasNext())
			{
				r.add(j.next().clone());
			}

			List<DomainElement> d = new ArrayList<DomainElement>(domain.size());
			Iterator<DomainElement> k = domain.iterator();

			while (k.hasNext())
			{
				d.add(k.next().clone());
			}

			Set<String> ns = new HashSet<String>(nullSet.size());
			Iterator<String> l = nullSet.iterator();

			while (l.hasNext())
			{
				ns.add(new String(l.next()));
			}

			Set<InterpolationMethod> is = new HashSet<InterpolationMethod>(interpolationSet.size());
			Iterator<InterpolationMethod> m = interpolationSet.iterator();

			while (m.hasNext())

			{
				is.add(m.next().clone());
			}

			return new Metadata(cd, r, ns, new String(nullDefault), is, interpolationDefault.clone(), new String(coverageName), d, crs,getAbstract(), getTitle(), getKeywords());
		}
		catch (InvalidMetadataException ime)
		{
			throw new RuntimeException("Invalid metadata while cloning " +
                    "Metadata. This is a software bug in WCPS.", ime);
		}

	}

//  public CellDomainElement getCellDomain( int index ) {
//
//      return cellDomain.get( index );
//
//  }

    protected void setCoverageId(int id)
    {
        this.coverageId = id;
    }

    public int getCoverageId()
    {
        return coverageId;
    }

    public String getCoverageName()
    {
        return coverageName;
    }

    public String getAbstract()
    {
        return abstractStr;
    }

    public String getTitle()
    {
        return titleStr;
    }

    public String getKeywords()
    {
        return keywordsStr;
    }

	public Iterator<CellDomainElement> getCellDomainIterator()
	{
		return cellDomain.iterator();

	}

	public Iterator<DomainElement> getDomainIterator()
	{
		return domain.iterator();
	}

    public Iterator<RangeElement> getRangeIterator()
    {
        return range.iterator();
    }

    public Iterator<InterpolationMethod> getInterpolationMethodIterator()
    {
        return interpolationSet.iterator();
    }

//    public Iterator<CrsName> getCrsSetIterator()
//    {
//        return crsset.iterator();
//    }

    public Iterator<String> getNullSetIterator()
    {
        return nullSet.iterator();
    }

	public int getDimension()
	{
		return cellDomain.size();
	}

	public int getDomainIndexByName(String name)
	{
		Iterator<DomainElement> i = domain.iterator();

		for (int index = 0; i.hasNext(); index++)
		{
			if ( i.next().getName().equals(name) )
			{
				return index;
			}
		}

		return -1;
	}

    public DomainElement getDomainByName(String name)
	{
		Iterator<DomainElement> i = domain.iterator();

		for (int index = 0; i.hasNext(); index++)
		{
            DomainElement dom = i.next();
            if (dom.getName().equals(name))
                return dom;
		}

		return null;
	}

	public String getNullDefault()
	{
		return nullDefault;

	}

	public Set<String> getNullSet()
	{
		return nullSet;

	}

	public boolean isRangeBoolean()
	{
		Iterator<RangeElement> i = range.iterator();

		while (i.hasNext())
		{
			if ( ! i.next().isBoolean() )
			{
				return false;
			}
		}

		return true;

	}

	public boolean isRangeComplex()
	{
		Iterator<RangeElement> i = range.iterator();

		while (i.hasNext())
		{
			if ( ! i.next().isComplex() )
			{
				return false;
			}
		}

		return true;

	}

	public boolean isRangeIntegral()
	{
		Iterator<RangeElement> i = range.iterator();

		while (i.hasNext())
		{
			if ( ! i.next().isIntegral() )
			{
				return false;
			}
		}

		return true;

	}

	public boolean isRangeFloating()
	{
		Iterator<RangeElement> i = range.iterator();

		while (i.hasNext())
		{
			if ( ! i.next().isFloating() )
			{
				return false;
			}
		}

		return true;

	}

	public boolean isRangeNumeric()
	{
		Iterator<RangeElement> i = range.iterator();

		while (i.hasNext())
		{
			if ( ! i.next().isNumeric() )
			{
				return false;
			}
		}

		return true;

	}

	public void setCoverageName(String coverageName) throws InvalidMetadataException
	{
		if ( coverageName == null )
		{
			throw new InvalidMetadataException("Metadata transformation: Coverage name cannot be null");
		}

		this.coverageName = coverageName;

	}

//  public void setCrs( String crs ) throws InvalidMetadataException {
//
//      if( crs == null ) {
//          throw new InvalidMetadataException( "Metadata transformation: CRS cannot be null" );
//      }
//      if( !(crs.equals( "" ) || crs.startsWith( "urn:ogc:def:crs:EPSG::" )) ) {
//          throw new InvalidMetadataException( "Metadata transformation: Invalid CRS" );
//      }
//      this.crs = crs;
//
//  }

//  public void setInterpolationMethodList( List<String> interpolationMethodList ) throws InvalidMetadataException {
//
//      this.interpolationMethodList = interpolationMethodList;
//      Iterator<String> is = interpolationMethodList.iterator();
//      while( is.hasNext() ) {
//          String next = is.next();
//          if( !(next.equals( "nearest neighbor" ) || next.equals( "bilinear" ) || next.equals( "bicubic" ) || next.equals( "lost area" ) || next.equals( "barycentric" )) ) {
//              throw new InvalidMetadataException( "Metadata transformation: Invalid interpolation method" );
//          }
//      }
//
//  }

//  public void setNullValue( String nullValue ) {
//
//      this.nullValue = nullValue;
//
//  }

	public void setRangeType(String type) throws InvalidMetadataException
	{
		Iterator<RangeElement> i = range.iterator();

		while (i.hasNext())
		{
			i.next().setType(type);
		}

	}

//  public void setRangeType( List<String> types ) throws InvalidMetadataException {
//
//      if( types.size() != range.size() ) {
//          throw new InvalidMetadataException( "Invalid range type: New type has " + types.size() + " elements, but range has " + range.size() + " elements" );
//      }
//      Iterator<RangeElement> i = range.iterator();
//      Iterator<String> j = types.iterator();
//      while( i.hasNext() ) {
//          i.next().setType( j.next() );
//      }
//
//  }

//  public void updateCellDomain( int index, BigInteger lo, BigInteger hi ) throws InvalidMetadataException {
//
//      cellDomain.set( index, new CellDomainElement( lo, hi ) );
//
//  }

//  public void updateDomain( int index, Double numLo, Double numHi, String strLo, String strHi ) throws InvalidMetadataException {
//
//      DomainElement old = domain.get( index );
//      domain.set( index, new DomainElement( old.getName(), old.getType(), numLo, numHi, strLo, strHi ) );
//
//  }

	public void updateNulls(Set<String> nullSet, String nullDefault) throws InvalidMetadataException
	{
		if ( nullSet.size() == 0 )
		{
			throw new InvalidMetadataException("Invalid null set: At least one null value is required");
		}

		if ( nullDefault == null )
		{
			nullDefault = "0";
		}

		if ( ! nullSet.contains(nullDefault) )
		{
			throw new InvalidMetadataException("Invalid null default: Default null value " + nullDefault + " is not part of the null set");
		}

		this.nullSet = nullSet;
		this.nullDefault = nullDefault;

	}

    public String getInterpolationDefault()
    {
        return interpolationDefault.getInterpolationType();
    }

    public String getNullResistanceDefault()
    {
        return interpolationDefault.getNullResistance();
    }

    public Wgs84Crs getCrs()
    {
        return crs;
    }

    /**
     * @return the X if it exists
     */
    public CellDomainElement getXCellDomain() {
        return cellX;
    }

    /**
     * @return the Y if it exists
     */
    public CellDomainElement getYCellDomain() {
        return cellY;
    }

    /**
     * @return the T if it exists
     */
    public CellDomainElement getTCellDomain() {
        return cellT;
    }

    /**
     * @param titleStr the titleStr to set
     */
    public void setTitle(String titleStr) {
        this.titleStr = titleStr;
    }

    /**
     * @param abstractStr the abstractStr to set
     */
    public void setAbstract(String abstractStr) {
        this.abstractStr = abstractStr;
    }

    /**
     * @param keywordsStr the keywordsStr to set
     */
    public void setKeywords(String keywordsStr) {
        this.keywordsStr = keywordsStr;
    }

    /**
     * @param cellDomain the cellDomain to set
     */
    public void setCellDomain(List<CellDomainElement> cellDomain) {
        this.cellDomain = cellDomain;
    }

    /**
     * @param domain the domain to set
     */
    public void setDomain(List<DomainElement> domain) {
        this.domain = domain;
    }

    /**
     * @param range the range to set
     */
    public void setRange(List<RangeElement> range) {
        this.range = range;
    }

    /**
     * @param interpolationSet the interpolationSet to set
     */
    public void setInterpolationSet(Set<InterpolationMethod> interpolationSet) {
        this.interpolationSet = interpolationSet;
    }

    /**
     * @param interpolationDefault the interpolationDefault to set
     */
    public void setDefaultInterpolation(InterpolationMethod interpolationDefault) {
        this.interpolationDefault = interpolationDefault;
    }

    /**
     *  Returns the maximal time position of the current coverage in ISO 8601 format, as string.
     * If there is no time-axis, returns null
     */
    public String getTimePeriodBeginning()
    {
        if (domT == null)
            return null;
        return domT.getStrLo();
    }

    /**
     *  Returns the minimal time position of the current coverage in ISO 8601 format, as string.
     * If there is no time-axis, returns null
     */
    public String getTimePeriodEnd()
    {
        if (domT == null)
            return null;
        return domT.getStrHi();
    }

    /**
     * Returns the time span of the current coverage, as described in the metadata (in miliseconds).
     * If there is no metadata, returns -1.
     * Note that this function returns the absolute difference. It is the administrator's
     * responsibility to make sure that the metadata values are correct.
     */
    public long getTimeSpan()
    {
        if (domT == null)
            return -1;
        long result = TimeString.difference(getTimePeriodEnd(), getTimePeriodBeginning());
        return Math.abs(result);
    }

    /* Returns the difference between the maximum and the minimum time axis index.
     Returns -1 if there is no metadata. */
    public long getTimeIndexesSpan()
    {
        if (cellT == null)
            return -1;
        BigInteger big = cellT.getHi().subtract(cellT.getLo());
        return big.longValue();
    }
}
