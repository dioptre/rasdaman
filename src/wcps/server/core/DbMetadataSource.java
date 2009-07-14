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

//~--- JDK imports ------------------------------------------------------------

import java.math.BigInteger;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;

import java.util.ArrayList;
import java.util.Collection;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Set;

/**
 * The DbMetadataSource is a IMetadataSource that uses a relational database. It
 * keeps a global connection which is reused on future requests, as well as between
 * threads. Before each read, the connection is verified to be valid, and
 * recreated if necessary. This IMetadataSource is not particularly efficient,
 * because it accesses the database at least once for every read. To increase
 * efficiency, wrap a CachedMetadataSource around this one.
 *
 */

public class DbMetadataSource implements IMetadataSource
{
	private static final int CONNECTION_TIMEOUT = 4;
	private Map<Integer, String> axisTypes;
	private boolean checkAtInit;
	private Connection connection;
	private Map<Integer, String> crss;
	private Map<Integer, String> dataTypes;
	private String driver;
	private boolean initializing;
	private Map<Integer, String> interpolationTypes;
	private Map<Integer, String> nullResistances;
	private String pass;
	private Map<String, String> supportedFormats;
	private String url;
	private String user;

	public DbMetadataSource(String driver, String url, String user, String pass) throws ResourceException, InvalidMetadataException
	{
		this(driver, url, user, pass, true);

	}

	public DbMetadataSource(String driver, String url, String user, String pass, boolean checkAtInit) throws ResourceException, InvalidMetadataException
	{
		try
		{
			Class.forName(driver).newInstance();
		}
		catch (ClassNotFoundException e)
		{
			throw new ResourceException("Metadata database error: Could not find JDBC driver: " + driver, e);
		}
		catch (InstantiationException e)
		{
			throw new ResourceException("Metadata database error: Could not instantiate JDBC driver: " + driver, e);
		}
		catch (IllegalAccessException e)
		{
			throw new ResourceException("Metadata database error: Access denied to JDBC driver: " + driver, e);
		}

		this.driver = driver;
		this.url = url;
		this.user = user;
		this.pass = pass;
		Statement s = null;

		try
		{
			ensureConnection();

			axisTypes = new HashMap<Integer, String>(5);
			s = connection.createStatement();
			ResultSet r = s.executeQuery("SELECT id, axisType FROM PS_AxisType");

			while (r.next())
			{
				axisTypes.put(r.getInt("id"), r.getString("axisType"));
			}

			dataTypes = new HashMap<Integer, String>(13);
			r = s.executeQuery("SELECT id, dataType FROM PS_Datatype");

			while (r.next())
			{
				dataTypes.put(r.getInt("id"), r.getString("dataType"));
			}

			interpolationTypes = new HashMap<Integer, String>(5);
			r = s.executeQuery("SELECT id, interpolationType FROM PS_InterpolationType");

			while (r.next())
			{
				interpolationTypes.put(r.getInt("id"), r.getString("interpolationType"));
			}

			nullResistances = new HashMap<Integer, String>(4);
			r = s.executeQuery("SELECT id, nullResistance FROM PS_NullResistance");

			while (r.next())
			{
				nullResistances.put(r.getInt("id"), r.getString("nullResistance"));
			}

			crss = new HashMap<Integer, String>();
			r = s.executeQuery("SELECT id, name FROM PS_Crs");

			while (r.next())
			{
				crss.put(r.getInt("id"), r.getString("name"));
			}

			supportedFormats = new HashMap<String, String>();
			r = s.executeQuery("SELECT name, mimetype FROM PS_Format");

			while (r.next())
			{
				supportedFormats.put(r.getString("name"), r.getString("mimetype"));
			}

			s.close();

			this.checkAtInit = checkAtInit;

			if ( checkAtInit )
			{
				initializing = true;
				boolean coveragesOk = false;

				while ( ! coveragesOk)
				{
					try
					{
						Iterator<String> coverages = coverages().iterator();

						while (coverages.hasNext())
						{
							read(coverages.next());
						}
					}
					catch (InvalidRequestException ire)
					{
						throw(InvalidMetadataException) ire.getCause();
					}

					coveragesOk = true;
				}

				initializing = false;
			}
		}
		catch (SQLException sqle)
		{
			if ( s != null )
			{
				try
				{
					s.close();
				}
				catch (SQLException e){}
			}

			close();

			throw new ResourceException("Metadata database error", sqle);
		}

	}

	public void close()
	{
		synchronized (this)
		{
			if ( connection != null )
			{
				try
				{
					connection.close();
				}
				catch (SQLException sqle){}
			}
		}

	}

	public Set<String> coverages() throws ResourceException
	{
		Statement s = null;
		Set<String> coverages;

		try
		{
			ensureConnection();
			s = connection.createStatement();

			ResultSet r = s.executeQuery("SELECT name FROM PS_Coverage");

			coverages = new HashSet<String>(r.getFetchSize());

			while (r.next())
			{
				coverages.add(r.getString("name"));
			}

			s.close();

			return coverages;
		}
		catch (SQLException sqle)
		{
			if ( s != null )
			{
				try
				{
					s.close();
				}
				catch (SQLException f){}
			}

			close();

			throw new ResourceException("Metadata database error", sqle);
		}

	}

	public String mimetype(String format)
	{
		return supportedFormats.get(format);
	}

	public Metadata read(String coverageName) throws InvalidRequestException, ResourceException
	{
		System.err.println("DB: " + coverageName);

		if ( (coverageName == null) || coverageName.equals("") )
		{
			throw new InvalidRequestException("Cannot retrieve coverage with null or empty name");
		}

		Statement s = null;

		try
		{
			ensureConnection();
			s = connection.createStatement();

			ResultSet r = s.executeQuery("SELECT id, nullDefault, interpolationTypeDefault, nullResistanceDefault FROM PS_Coverage WHERE name = '" + coverageName + "'");

			if ( ! r.next() )
			{
				throw new InvalidRequestException("Coverage '" + coverageName + "' is not served by this server");
			}

			int coverage = r.getInt("id");
			String nullDefault = r.getString("nullDefault");
			String interpolationTypeDefault = interpolationTypes.get(r.getInt("interpolationTypeDefault"));
			String nullResistanceDefault = nullResistances.get(r.getInt("nullResistanceDefault"));

			r = s.executeQuery("SELECT lo, hi FROM PS_CellDomain WHERE coverage = '" + coverage + "' ORDER BY i ASC");
			List<CellDomainElement> cellDomain = new ArrayList<CellDomainElement>(r.getFetchSize());

			while (r.next())
			{
				cellDomain.add(new CellDomainElement(BigInteger.valueOf(r.getInt("lo")), BigInteger.valueOf(r.getInt("hi"))));
			}

			r = s.executeQuery("SELECT name, type FROM PS_Range WHERE coverage = '" + coverage + "' ORDER BY i ASC");
			List<RangeElement> range = new ArrayList<RangeElement>(r.getFetchSize());

			while (r.next())
			{
				range.add(new RangeElement(r.getString("name"), dataTypes.get(r.getInt("type"))));
			}

			r = s.executeQuery("SELECT interpolationType, nullResistance FROM PS_InterpolationSet WHERE coverage = '" + coverage + "'");
			Set<InterpolationMethod> interpolationSet = new HashSet<InterpolationMethod>(r.getFetchSize());

			while (r.next())
			{
				interpolationSet.add(new InterpolationMethod(interpolationTypes.get(r.getInt("interpolationType")), nullResistances.get(r.getInt("nullResistance"))));
			}

			r = s.executeQuery("SELECT nullValue FROM PS_NullSet WHERE coverage = '" + coverage + "'");
			Set<String> nullSet = new HashSet<String>(r.getFetchSize());

			while (r.next())
			{
				nullSet.add(r.getString("nullValue"));
			}

			r = s.executeQuery("SELECT id, name, type, numLo, numHi, strLo, strHi FROM PS_Domain WHERE coverage = '" + coverage + "' ORDER BY i ASC");
			List<DomainElement> domain = new ArrayList<DomainElement>(r.getFetchSize());
			Statement ss = connection.createStatement();

			while (r.next())
			{
				Double numLo = r.getDouble("numLo");

				if ( r.wasNull() )
				{
					numLo = null;
				}

				Double numHi = r.getDouble("numHi");

				if ( r.wasNull() )
				{
					numHi = null;
				}

				ResultSet rr = ss.executeQuery("SELECT crs FROM PS_CrsSet WHERE axis = '" + r.getInt("id") + "'");
				Set<String> crsSet = new HashSet<String>(rr.getFetchSize());

				while (rr.next())
				{
					crsSet.add(crss.get(rr.getInt("crs")));
				}

				crsSet.add(DomainElement.IMAGE_CRS);
				domain.add(new DomainElement(r.getString("name"), axisTypes.get(r.getInt("type")), numLo, numHi, r.getString("strLo"), r.getString("strHi"), crsSet, axisTypes.values()));
			}

			s.close();

			return new Metadata(cellDomain, range, nullSet, nullDefault, interpolationSet, new InterpolationMethod(interpolationTypeDefault, nullResistanceDefault), coverageName, domain);
		}
		catch (InvalidMetadataException ime)
		{
			if ( checkAtInit && ! initializing )
			{
				throw new ResourceException("Previously valid metadata is now invalid. The metadata for coverage '" + coverageName + "' has been modified incorrectly.", ime);
			}
			else
			{
				throw new InvalidRequestException("Coverage '" + coverageName + "' has invalid metadata", ime);
			}
		}
		catch (SQLException sqle)
		{
			if ( s != null )
			{
				try
				{
					s.close();
				}
				catch (SQLException f){}
			}

			close();

			throw new ResourceException("Metadata database error", sqle);
		}

	}

	private void ensureConnection() throws SQLException
	{
		synchronized (this)
		{
//          if( connection == null || !connection.isValid( CONNECTION_TIMEOUT ) ) { // Not implemented by PostgreSQL yet.
			if ( (connection == null) || ! connection.isClosed() )
			{
				close();
				connection = DriverManager.getConnection(url, user, pass);
			}
		}

	}

	public Collection<String> getAxisNames()
	{
		return axisTypes.values();
	}
}
