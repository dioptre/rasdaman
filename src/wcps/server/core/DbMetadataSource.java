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
import java.sql.Savepoint;
import java.sql.Statement;

import java.util.ArrayList;
import java.util.Collection;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.Vector;
import petascope.ConfigManager;

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
    /* Status variables */
	private boolean initializing;
    private boolean checkAtInit;

    /* Contents of static tables */
	private Map<Integer, String> axisTypes;
	private Map<Integer, String> crss;
	private Map<Integer, String> dataTypes;
	private Map<Integer, String> interpolationTypes;
	private Map<Integer, String> nullResistances;
	private Map<String, String> supportedFormats;

    /* Contents of static tables (reversed, for easy access if you
     know the something's name and want to find out its id) */
    private Map<String, Integer> revAxisTypes;
	private Map<String, Integer> revCrss;
	private Map<String, Integer> revDataTypes;
	private Map<String, Integer> revInterpolationTypes;
	private Map<String, Integer> revNullResistances;
	private Map<String, String> revSupportedFormats;    // Not used

    /* Database access info */
    private String driver;
    private String pass;
	private String url;
	private String user;

    /* Global database key variables */
    private Connection conn;
    private Savepoint savepoint;
    private String query;

	public DbMetadataSource(String driver, String url, String user, String pass) throws ResourceException, InvalidMetadataException
	{
		this(driver, url, user, pass, true);
	}

	public DbMetadataSource(String driver, String url, String user, String pass, boolean checkAtInit) throws ResourceException, InvalidMetadataException
	{
		try
		{
            this.driver = driver;
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

            /* Read contents of static metadata tables */
			ensureConnection();

			axisTypes = new HashMap<Integer, String>();
            revAxisTypes = new HashMap<String, Integer>();
			s = conn.createStatement();
			ResultSet r = s.executeQuery("SELECT id, axisType FROM PS_AxisType");

			while (r.next())
			{
				axisTypes.put(r.getInt("id"), r.getString("axisType"));
                revAxisTypes.put(r.getString("axisType"), r.getInt("id"));
			}

			dataTypes = new HashMap<Integer, String>();
            revDataTypes = new HashMap<String, Integer>();
			r = s.executeQuery("SELECT id, dataType FROM PS_Datatype");

			while (r.next())
			{
				dataTypes.put(r.getInt("id"), r.getString("dataType"));
                revDataTypes.put(r.getString("dataType"), r.getInt("id"));
			}

			interpolationTypes = new HashMap<Integer, String>();
            revInterpolationTypes = new HashMap<String, Integer>();
			r = s.executeQuery("SELECT id, interpolationType FROM PS_InterpolationType");

			while (r.next())
			{
				interpolationTypes.put(r.getInt("id"), r.getString("interpolationType"));
                revInterpolationTypes.put(r.getString("interpolationType"), r.getInt("id"));
			}

			nullResistances = new HashMap<Integer, String>();
            revNullResistances = new HashMap<String, Integer>();
			r = s.executeQuery("SELECT id, nullResistance FROM PS_NullResistance");

			while (r.next())
			{
				nullResistances.put(r.getInt("id"), r.getString("nullResistance"));
                revNullResistances.put(r.getString("nullResistance"), r.getInt("id"));
			}

			crss = new HashMap<Integer, String>();
            revCrss = new HashMap<String, Integer>();
			r = s.executeQuery("SELECT id, name FROM PS_Crs");

			while (r.next())
			{
				crss.put(r.getInt("id"), r.getString("name"));
                revCrss.put(r.getString("name"), r.getInt("id"));
			}

			supportedFormats = new HashMap<String, String>();
            revSupportedFormats = new HashMap<String, String>();
			r = s.executeQuery("SELECT name, mimetype FROM PS_Format");

			while (r.next())
			{
				supportedFormats.put(r.getString("name"), r.getString("mimetype"));
                revSupportedFormats.put(r.getString("mimetype"), r.getString("name"));
			}

			s.close();

            /* Debug checks for reverse static tables */
//            log(revAxisTypes.toString());
//            log(revCrss.toString());
//            log(revDataTypes.toString());
//            log(revInterpolationTypes.toString());
//            log(revNullResistances.toString());
//            log(revSupportedFormats.toString());

            /* Check Metadata consistency at startup, if needed */
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
        if ( conn != null )
        {
            try
            {
                conn.close();
            }
            catch (SQLException sqle){}
            conn = null;
        }
	}

    /* List all available coverages */
    @Override
	public Set<String> coverages() throws ResourceException
	{
		Statement s = null;
		Set<String> coverages;

		try
		{
			ensureConnection();
			s = conn.createStatement();

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

    @Override
	public String formatToMimetype(String format)
	{
		return supportedFormats.get(format);
	}

    @Override
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
			s = conn.createStatement();

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
            CellDomainElement X = null, Y = null, cell;
            boolean twoDCoverage = true;
			List<CellDomainElement> cellDomain = new ArrayList<CellDomainElement>(r.getFetchSize());

			while (r.next())
			{
                cell = new CellDomainElement(BigInteger.valueOf(r.getInt("lo")), BigInteger.valueOf(r.getInt("hi")));
				cellDomain.add(cell);
                if (X == null)
                    X = cell;
                else
                if (X != null && Y == null)
                    Y = cell;
                else
                    twoDCoverage = false;
			}
            if (X == null || Y == null)
                twoDCoverage = false;

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
			Statement ss = conn.createStatement();

			while (r.next())
			{
                String strLo = r.getString("strLo");
                String strHi = r.getString("strHi");
                
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

                DomainElement d = new DomainElement(r.getString("name"),axisTypes.get(r.getInt("type")),
                        numLo, numHi, strLo, strHi, crsSet, axisTypes.values());
				domain.add(d);
			}

            String abstr = "";
            String title = "";
            String keywords = "";
            r = s.executeQuery("SELECT title, abstract, keywords FROM PS_Descriptions WHERE coverage = '" + coverage + "'");
            if (r.next())
            {
                abstr = r.getString("abstract");
                title = r.getString("title");
                keywords = r.getString("keywords");
            }

            /* WGS84 is the only CRS we understand (except IMAGE_CRS, of course) */
            Wgs84Crs crs = null;
            Double l1 = 0.0, l2 = 0.0, h1 = 0.0, h2 = 0.0;
            Double o1 = null, o2 = null;
            r = s.executeQuery("SELECT * FROM PS_CrsDetails WHERE coverage = '" + coverage + "'");
            if (r.next())
            {
                /* Domain extent */
                int x0 = X.getLo().intValue();
                int y0 = Y.getLo().intValue();
                int x1 = X.getHi().intValue();
                int y1 = Y.getHi().intValue();
                /* CRS Bounding box */
                l1 = r.getDouble("low1");
                l2 = r.getDouble("low2");
                h1 = r.getDouble("high1");
                h2 = r.getDouble("high2");
                String off1 = r.getString("offset1");
                String off2 = r.getString("offset2");
                if (off1 != null)
                    o1 = Double.parseDouble(off1);
                if (off2 != null)
                    o2 = Double.parseDouble(off2);
                /* Compute axis offsets if not predefined */
                if (o1 == null && o2 == null)
                {
                    o1 = (h1-l1) / (double)(x1-x0);
                    o2 = (h2-l2) / (double)(y1-y0);
                    log("Calculated CRS axis offsets. For X: " + o1 + ", for Y: " + o2);
                    log(X.toString());
                    log(Y.toString());
                }
                crs = new Wgs84Crs(l1, h1, l2, h2, o1, o2);
                System.out.println(crs);
            }
            else
                if (twoDCoverage)
                    log(" WGS84 bounding box missing for 2-D coverage '" + coverageName + "'...");

            /* Done with SQL statements */
			s.close();

            /* Build the complete metadata object */
			Metadata meta = new Metadata(cellDomain, range, nullSet, nullDefault, interpolationSet,
                    new InterpolationMethod(interpolationTypeDefault, nullResistanceDefault),
                    coverageName, domain, crs, title, abstr, keywords);
            meta.setCoverageId(coverage);
            return meta;
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

    /** Write a coverage's metadata to the database. This function can be used
     * for both inserting new coverages and updating coverage metadata. 
     *
     * @param meta Metadata container for the information to be stored in the metadata database
     * @param commit Boolean value, specifying if we want to commit immediately or not
     */
    private void write(Metadata meta, boolean commit) throws ResourceException
	{
        String coverageName = meta.getCoverageName();
        if (existsCoverageName(coverageName))
            updateCoverageMetadata(meta, commit);
        else
            insertNewCoverageMetadata(meta, commit);
    }

    public void delete(Metadata meta, boolean commit) throws ResourceException
    {
        String coverageName = meta.getCoverageName();
        if (existsCoverageName(coverageName) == false)
            throw new ResourceException("Cannot delete inexistent coverage: " + coverageName);

        /* Delete main coverage entry from "PS_Coverage". Auxiliary metadata are
         * automatically deleted by the DB (via CASCADING) on
         * deletion of the main entry in ps_coverage */
        Statement s = null;
        try
        {
            s = conn.createStatement();
            setQuery("DELETE FROM PS_Coverage WHERE name = '" + coverageName + "'");
            int count = s.executeUpdate(query);
            log("Affected rows: " + count);
            s.close();

            if (commit)
                commitAndClose();
        }
        catch (SQLException e)
        {
            e.printStackTrace();
        }
        finally
        {
            try { s.close(); } catch (Exception e) {}
        }
    }

    /**
     * Check if there is metadata available for a given coverage name
     * @param name coverage name
     * @return true is coverage already exists
     */
    public boolean existsCoverageName(String name)
    {
        boolean result = false;
        Statement s = null;
        try
        {
            s = conn.createStatement();
            setQuery("SELECT * FROM PS_Coverage WHERE name = '" + name + "'");
            ResultSet r = s.executeQuery(query);
            if (r.next())
                result = true;
            else
                result = false;
            s.close();
        }
        catch (SQLException e)
        {
            e.printStackTrace();
        }
        finally
        {
            try { s.close(); } catch (Exception e) {}
        }
        return result;
    }

    /**
     * Check if there exists a coverage with a given ID in the metadata database.
     * @param id coverage id
     * @return true is coverage already exists
     */
    private boolean existsCoverageId(int id)
    {
        boolean result = false;
        Statement s = null;
        try
        {
            s = conn.createStatement();
            ResultSet r = s.executeQuery(
                    "SELECT * FROM PS_Coverage WHERE id = '" + id + "'");
            if (r.next())
                result = true;
            else
                result = false;
            s.close();
        }
        catch (SQLException e)
        {
            e.printStackTrace();
        }
        finally
        {
            try { s.close(); } catch (Exception e) {}
        }
        System.out.println("Coverage with ID " + id + (result == false? " does not " : "") + " exists.");
        return result;
    }

    /** Update metadata for an existing coverage. All information may change (including 
     * name), but the ID of the tuple in PS_Coverage will stay the same.
     *
     * @param meta Metadata object, container of the new information.
     * @param commit True if we want to commit immediately, false to delay commit indefinitely
     */
    public void updateCoverageMetadata(Metadata meta, boolean commit) throws ResourceException
    {
        Statement s = null;
        int coverageId = meta.getCoverageId();
        int count = -1;
        String coverageName = meta.getCoverageName();
        
		try
		{
			ensureConnection();
			s = conn.createStatement();

            String name = coverageName;
            String nulldefault = meta.getNullDefault();
            int interpolatiotypendefault = revInterpolationTypes.get(meta.getInterpolationDefault());
            int nullresistancedefault = revNullResistances.get(meta.getNullResistanceDefault());

            // Table PS_Coverage
            setQuery("UPDATE PS_Coverage SET " +
                        "(name, nullDefault, interpolationTypeDefault, nullResistanceDefault) " +
                        " = ('" + name + "', '" + nulldefault + "', '"
                        + interpolatiotypendefault + "', '" + nullresistancedefault + "')" +
                        " WHERE id='" + coverageId + "'");
            count = s.executeUpdate(query);
            if (count <= 0)
                throw new SQLException("Could not update table PS_Coverage.");

            // Table PS_CellDomain
            /* Delete old data */
            setQuery("DELETE FROM PS_CellDomain WHERE coverage='" + coverageId + "'");
            count = s.executeUpdate(query);
            if (count <= 0)
                throw new SQLException("Could not delete old entries from table PS_CellDomain.");
            /* Insert new data */
            Iterator<CellDomainElement> cellIt = meta.getCellDomainIterator();
            int i = 0;
            while (cellIt.hasNext())
            {
                CellDomainElement cell = cellIt.next();
                setQuery("INSERT INTO PS_CellDomain (coverage, i, lo, hi) VALUES " +
                        " ('" + coverageId + "', '" + i + "', '" + cell.getLo() + "', '" +
                        cell.getHi() + "')");
                if (s.executeUpdate(query) <= 0)
                    throw new SQLException("Could not insert axis " + i + " for coverage " +
                            coverageName + "( id " + coverageId + ") into table PS_CellDomain" );
                i++;
            }

            // Table PS_Range
            /* Delete old data */
            setQuery("DELETE FROM PS_Range WHERE coverage='" + coverageId + "'");
            count = s.executeUpdate(query);
            if (count <= 0)
                throw new SQLException("Could not delete old entries from table PS_Range.");
            /* Insert new data */
            Iterator<RangeElement> rangeIt = meta.getRangeIterator();
            i = 0;
            while (rangeIt.hasNext())
            {
                RangeElement range = rangeIt.next();
                System.out.println(range);
                int dataType = revDataTypes.get(range.getType());
                setQuery("INSERT INTO PS_Range (coverage, i, name, type) VALUES " +
                        " ('" + coverageId + "', '" + i + "', '" + range.getName() + "', '" +
                         dataType + "')");
                if (s.executeUpdate(query) <= 0)
                    throw new SQLException("Could not insert range " + range.getName() +
                            "( id " + i + " ) for coverage " +
                            coverageName + "( id " + coverageId + ") into table PS_Range" );
                i++;
            }

            // Table PS_InterpolationSet
            /* Delete old data */
            setQuery("DELETE FROM PS_InterpolationSet WHERE coverage='" + coverageId + "'");
            count = s.executeUpdate(query);
            if (count <= 0)
                throw new SQLException("Could not delete old entries from table PS_InterpolationSet.");
            /* Insert new data */
            Iterator<InterpolationMethod> methodIt = meta.getInterpolationMethodIterator();
            i = 0;
            while (methodIt.hasNext())
            {
                InterpolationMethod method = methodIt.next();
                int interp = revInterpolationTypes.get(method.getInterpolationType());
                int nullRes = revNullResistances.get(method.getNullResistance());
                setQuery("INSERT INTO PS_InterpolationSet (coverage, interpolationType, nullResistance) VALUES " +
                        " ('" + coverageId + "', '" + interp + "', '" + nullRes + "')");
                if (s.executeUpdate(query) <= 0)
                    throw new SQLException("Could not insert interpolation method (" + method.getInterpolationType() +
                            ", " + method.getNullResistance() + " ) for coverage " +
                            coverageName + "( id " + coverageId + ") into table PS_InterpolationSet" );
                i++;
            }

            // Table PS_NullSet
            /* Delete old data */
            setQuery("DELETE FROM PS_NullSet WHERE coverage='" + coverageId + "'");
            count = s.executeUpdate(query);
            if (count <= 0)
                throw new SQLException("Could not delete old entries from table PS_NullSet.");
            /* Insert new data */
            Iterator<String> nullIt = meta.getNullSetIterator();
            while (nullIt.hasNext())
            {
                String nullValue = nullIt.next();
                setQuery("INSERT INTO PS_NullSet (coverage, nullValue) VALUES " +
                        " ('" + coverageId + "', '" + nullValue + "')");
                if (s.executeUpdate(query) <= 0)
                    throw new SQLException("Could not insert null value '" + nullValue +
                            "' for coverage " + coverageName + "( id " + coverageId +
                            ") into table PS_NullSet" );
            }

            // Tables PS_Domain and PS_CrsSet
            /* Delete old data */
            setQuery("SELECT * FROM PS_Domain WHERE coverage='" + coverageId + "'");
            ResultSet r1 = s.executeQuery(query);
            Vector axes = new Vector(5);
            while (r1.next())
            {
                int axisId = r1.getInt("id");
                axes.add((Integer)axisId);
            }
            System.out.println(axes);
            for (i = 0; i < axes.size(); i++)
            {
                int axisId = ((Integer)axes.elementAt(i)).intValue();
                setQuery("DELETE FROM PS_CrsSet WHERE axis='" + axisId + "'");
                count = s.executeUpdate(query);
                /* Do not worry about inexisting entries in CRS Sets: this implementation
                 does not really use CRSs. */
//                if (count <= 0)
//                    throw new SQLException("Could not delete old entries from table PS_CrsSet.");

            }

            setQuery("DELETE FROM PS_Domain WHERE coverage='" + coverageId + "'");
            count = s.executeUpdate(query);
            if (count <= 0)
                throw new SQLException("Could not delete old entries from table PS_Domain.");
            /* Insert new data */
            Iterator<DomainElement> domIt = meta.getDomainIterator();
            i = 0;
            while (domIt.hasNext())
            {
                // Table PS_Domain
                DomainElement dom = domIt.next();
                int axisType = revAxisTypes.get(dom.getType());
                Double numLo = dom.getNumLo();
                Double numHi = dom.getNumHi();
                String sNumLo = numLo == null ? "null" : "'" + numLo.toString() + "'";
                String sNumHi = numHi == null ? "null" : "'" + numHi.toString() + "'";
                String strLo = dom.getStrLo();
                String strHi = dom.getStrHi();
                if (strLo != null && strLo.equals("null") == false)
                    strLo = "'" + strLo + "'";
                if (strHi != null && strHi.equals("null") == false)
                    strHi = "'" + strHi + "'";

                String sqlQuery = "INSERT INTO PS_Domain " +
                        "(coverage, i, name, type, numLo, numHi, strLo, strHi) VALUES " +
                        "('" + coverageId + "', '" + i + "', '" + dom.getName() + "', '" +
                        axisType + "', " + sNumLo + ", " + sNumHi +
                        ", " + strLo + ", " + strHi + ")";
                /* Need to get ID of the newly inserted tuple. Postgres has a cool construct (RETURNING),
                 * but we can also fall-back to another generic JDBC driver */
                int axisId = -1;
                if (driver.equals("org.postgresql.Driver"))
                {
                    /* RETURNING clause is not standard SQL, only PostgreSQL understands it*/
                    setQuery(sqlQuery + " RETURNING id");

                    ResultSet r = s.executeQuery(query);
                    if (r.next() == false)
                        throw new SQLException("Could not insert domain element (i = " + i +
                            ", name=" + dom.getName() + ", type=" + dom.getType() +
                            ", numLo=" + dom.getNumLo() + ", numHi=" + dom.getNumHi() +
                            ", strLo=" + dom.getStrLo() + ", strHi=" + dom.getStrHi() +
                            ") for coverage " + coverageName + "( id " + coverageId +
                            ") into table PS_Domain" );
                    /* Retrieve the ID of the newly inserted tuple (PS_Domain) */
                    axisId = r.getInt("id");
                }
                else
                {
                    /* Fallback to specific driver support on returning autogenerated keys. */
                    String[] keys = new String[1];
                    keys[0] = "id";
                    int c = s.executeUpdate(sqlQuery, keys);
                    if (c <= 0)
                        throw new SQLException("Could not insert domain element (i = " + i +
                            ", name=" + dom.getName() + ", type=" + dom.getType() +
                            ", numLo=" + dom.getNumLo() + ", numHi=" + dom.getNumHi() +
                            ", strLo=" + dom.getStrLo() + ", strHi=" + dom.getStrHi() +
                            ") for coverage " + coverageName + "( id " + coverageId +
                            ") into table PS_Domain" );
                    /* Retrieve the ID of the newly inserted tuple (PS_Domain) */
                    ResultSet rr = s.getGeneratedKeys();
                    if (rr.next() == false)
                        throw new SQLException("Could not retrieve ID of the newly insterted tuple in table PS_Domain.");
                    axisId = rr.getInt("id");
                }
                /* check ID */
                if (axisId < 0)
                    throw new SQLException("Generated ID (" + axisId +
                            ") for the tuple in PS_Domain is not valid !");

                /* Increment axis number */
                i++;

                // Table PS_CrsSet
                Set<String> crsSet = dom.getCrsSet();
                System.out.println(crsSet);
                Iterator<String> crsIt = crsSet.iterator();
                while (crsIt.hasNext())
                {
                    String crs = crsIt.next();
                    int crsId = revCrss.get(crs);
                    setQuery("INSERT INTO PS_CrsSet (axis, crs) VALUES " +
                            "('" + axisId + "', '" + crsId + "')");
                    if (s.executeUpdate(query) <= 0)
                        throw new SQLException("Could not insert Crs " + crs +
                                "for axis =" + axisId + " in table PS_CrsSet");
                }
            }

            // Table PS_Descriptions
            /* Delete old data */
            setQuery("DELETE FROM PS_Descriptions WHERE coverage='" + coverageId + "'");
            count = s.executeUpdate(query);
            /* Coverage descriptions are not essential, do not throw an error if missing */
//            if (count <= 0)
//                throw new SQLException("Could not delete old data from table PS_Descriptions");
            /* Insert new data */
            String title = meta.getTitle();
            String abstr = meta.getAbstract();
            String keywords = meta.getKeywords();
            if (title != null)
                title = "'" + title + "'";
            if (abstr != null)
                abstr = "'" + abstr + "'";
            if (keywords != null)
                keywords = "'" + keywords + "'";
            setQuery("INSERT INTO PS_Descriptions (coverage, title, abstract, keywords)" +
                    " VALUES ('" + coverageId + "', " + title + ", " + abstr + ", " + keywords + ")");
            if (s.executeUpdate(query) <= 0)
                throw new SQLException("Could not insert descriptive metadata in table " +
                        "PS_Descriptions for coverage " + coverageName + " (ID " + coverageId + ")");


            s.close();

            if (commit)
                commitAndClose();
		}
		catch (SQLException sqle)
		{
            /* Abort this transaction */
            try
            {
                if (s != null)
                    s.close();
                abortAndClose();
            }
            catch (SQLException f){}

			throw new ResourceException("Metadata database error", sqle);
		}
    }

    /** Insert metadata for a new coverage.
     *
     * @param meta Metadata object, container of information
     * @param commit Boolean value, specifying if we want to commit immediately or not
     */
    public void insertNewCoverageMetadata(Metadata meta, boolean commit) throws ResourceException
    {
		Statement s = null;
        String coverageName = meta.getCoverageName();
        int coverageId = -1;

		try
		{
			ensureConnection();
			s = conn.createStatement();

            String name = coverageName;
            /*  FIXME: Table PS_COVERAGE: nullValue and CRS fields not USED !!! */
//            String nullvalue = "";
//            String crs = "";
            String nulldefault = meta.getNullDefault();
            int interpolatiotypendefault = revInterpolationTypes.get(meta.getInterpolationDefault());
            int nullresistancedefault = revNullResistances.get(meta.getNullResistanceDefault());

            // Table PS_Coverage
            String sqlQuery = "INSERT INTO PS_Coverage " +
                        "(name, nullDefault, interpolationTypeDefault, nullResistanceDefault) " +
                        " VALUES ('" + name + "', '" + nulldefault + "', '"
                        + interpolatiotypendefault + "', '" + nullresistancedefault + "')";
            /* Need to get ID of the newly inserted tuple. Postgres has a cool construct (RETURNING),
             * but we can also fall-back to another generic JDBC driver */
            if (driver.equals("org.postgresql.Driver"))
            {
                /* RETURNING clause is not standard SQL, only PostgreSQL understands it*/
                setQuery(sqlQuery + " RETURNING id");

                ResultSet r = s.executeQuery(query);
                if (r.next() == false)
                    throw new SQLException("Could not insert new coverage in table PS_Coverage.");
                /* Retrieve the ID of the newly inserted tuple (PS_Coverage) */
                coverageId = r.getInt("id");
            }
            else
            {
                /* Fallback to specific driver support on returning autogenerated keys. */
                String[] keys = new String[1];
                keys[0] = "id";
                int c = s.executeUpdate(sqlQuery, keys);
                if (c <= 0)
                    throw new SQLException("Could not insert new coverage in table PS_Coverage.");
                /* Retrieve the ID of the newly inserted tuple (PS_Coverage) */
                ResultSet rr = s.getGeneratedKeys();
                if (rr.next() == false)
                    throw new SQLException("Could not retrieve ID of the newly insterted tuple in table PS_Coverage.");
                coverageId = rr.getInt("id");
            }
            /* check ID */
            if (coverageId < 0)
                throw new SQLException("Generated ID (" + coverageId +
                        ") for the tuple in PS_Coverage is not valid !");

            // Table PS_CellDomain
            Iterator<CellDomainElement> cellIt = meta.getCellDomainIterator();
            int i = 0;
            while (cellIt.hasNext())
            {
                CellDomainElement cell = cellIt.next();
                setQuery("INSERT INTO PS_CellDomain (coverage, i, lo, hi) VALUES " +
                        " ('" + coverageId + "', '" + i + "', '" + cell.getLo() + "', '" +
                        cell.getHi() + "')");
                if (s.executeUpdate(query) <= 0)
                    throw new SQLException("Could not insert axis " + i + " for coverage " +
                            coverageName + "( id " + coverageId + ") into table PS_CellDomain" );
                i++;
            }

            // Table PS_Range
            Iterator<RangeElement> rangeIt = meta.getRangeIterator();
            i = 0;
            while (rangeIt.hasNext())
            {
                RangeElement range = rangeIt.next();
                System.out.println(range);
                int dataType = revDataTypes.get(range.getType());
                setQuery("INSERT INTO PS_Range (coverage, i, name, type) VALUES " +
                        " ('" + coverageId + "', '" + i + "', '" + range.getName() + "', '" +
                         dataType + "')");
                if (s.executeUpdate(query) <= 0)
                    throw new SQLException("Could not insert range " + range.getName() +
                            "( id " + i + " ) for coverage " +
                            coverageName + "( id " + coverageId + ") into table PS_Range" );
                i++;
            }

            // Table PS_InterpolationSet
            Iterator<InterpolationMethod> methodIt = meta.getInterpolationMethodIterator();
            i = 0;
            while (methodIt.hasNext())
            {
                InterpolationMethod method = methodIt.next();
                int interp = revInterpolationTypes.get(method.getInterpolationType());
                int nullRes = revNullResistances.get(method.getNullResistance());
                setQuery("INSERT INTO PS_InterpolationSet (coverage, interpolationType, nullResistance) VALUES " +
                        " ('" + coverageId + "', '" + interp + "', '" + nullRes + "')");
                if (s.executeUpdate(query) <= 0)
                    throw new SQLException("Could not insert interpolation method (" + method.getInterpolationType() +
                            ", " + method.getNullResistance() + " ) for coverage " +
                            coverageName + "( id " + coverageId + ") into table PS_InterpolationSet" );
                i++;
            }

            // Table PS_NullSet
            Iterator<String> nullIt = meta.getNullSetIterator();
            while (nullIt.hasNext())
            {
                String nullValue = nullIt.next();
                setQuery("INSERT INTO PS_NullSet (coverage, nullValue) VALUES " +
                        " ('" + coverageId + "', '" + nullValue + "')");
                if (s.executeUpdate(query) <= 0)
                    throw new SQLException("Could not insert null value '" + nullValue +
                            "' for coverage " + coverageName + "( id " + coverageId +
                            ") into table PS_NullSet" );
            }

            Iterator<DomainElement> domIt = meta.getDomainIterator();
            i = 0;
            while (domIt.hasNext())
            {
                // Table PS_Domain
                DomainElement dom = domIt.next();
                int axisType = revAxisTypes.get(dom.getType());
                Double numLo = dom.getNumLo();
                Double numHi = dom.getNumHi();
                String sNumLo = numLo == null ? "null" : "'" + numLo.toString() + "'";
                String sNumHi = numHi == null ? "null" : "'" + numHi.toString() + "'";
                String strLo = dom.getStrLo();
                String strHi = dom.getStrHi();
                if (strLo != null && strLo.equals("null") == false)
                    strLo = "'" + strLo + "'";
                if (strHi != null && strHi.equals("null") == false)
                    strHi = "'" + strHi + "'";

                sqlQuery = "INSERT INTO PS_Domain " +
                        "(coverage, i, name, type, numLo, numHi, strLo, strHi) VALUES " +
                        "('" + coverageId + "', '" + i + "', '" + dom.getName() + "', '" +
                        axisType + "', " + sNumLo + ", " + sNumHi +
                        ", " + strLo + ", " + strHi + ")";
                /* Need to get ID of the newly inserted tuple. Postgres has a cool construct (RETURNING),
                 * but we can also fall-back to another generic JDBC driver */
                int axisId = -1;
                if (driver.equals("org.postgresql.Driver"))
                {
                    /* RETURNING clause is not standard SQL, only PostgreSQL understands it*/
                    setQuery(sqlQuery + " RETURNING id");

                    ResultSet r = s.executeQuery(query);
                    if (r.next() == false)
                        throw new SQLException("Could not insert domain element (i = " + i +
                            ", name=" + dom.getName() + ", type=" + dom.getType() +
                            ", numLo=" + dom.getNumLo() + ", numHi=" + dom.getNumHi() +
                            ", strLo=" + dom.getStrLo() + ", strHi=" + dom.getStrHi() +
                            ") for coverage " + coverageName + "( id " + coverageId +
                            ") into table PS_Domain" );
                    /* Retrieve the ID of the newly inserted tuple (PS_Domain) */
                    axisId = r.getInt("id");
                }
                else
                {
                    /* Fallback to specific driver support on returning autogenerated keys. */
                    String[] keys = new String[1];
                    keys[0] = "id";
                    int c = s.executeUpdate(sqlQuery, keys);
                    if (c <= 0)
                        throw new SQLException("Could not insert domain element (i = " + i +
                            ", name=" + dom.getName() + ", type=" + dom.getType() +
                            ", numLo=" + dom.getNumLo() + ", numHi=" + dom.getNumHi() +
                            ", strLo=" + dom.getStrLo() + ", strHi=" + dom.getStrHi() +
                            ") for coverage " + coverageName + "( id " + coverageId +
                            ") into table PS_Domain" );
                    /* Retrieve the ID of the newly inserted tuple (PS_Domain) */
                    ResultSet rr = s.getGeneratedKeys();
                    if (rr.next() == false)
                        throw new SQLException("Could not retrieve ID of the newly insterted tuple in table PS_Domain.");
                    axisId = rr.getInt("id");
                }
                /* check ID */
                if (axisId < 0)
                    throw new SQLException("Generated ID (" + axisId +
                            ") for the tuple in PS_Domain is not valid !");

                /* Increment axis number */
                i++;

                // Table PS_CrsSet
                Set<String> crsSet = dom.getCrsSet();
                Iterator<String> crsIt = crsSet.iterator();
                while (crsIt.hasNext())
                {
                    String crs = crsIt.next();
                    int crsId = revCrss.get(crs);
                    setQuery("INSERT INTO PS_CrsSet (axis, crs) VALUES " +
                            "('" + axisId + "', '" + crsId + "')");
                    if (s.executeUpdate(query) <= 0)
                        throw new SQLException("Could not insert Crs " + crs +
                                "for axis =" + axisId + " in table PS_CrsSet");
                }
            }

            // Table PS_Descriptions
            String title = meta.getTitle();
            String abstr = meta.getAbstract();
            String keywords = meta.getKeywords();
            if (title != null)
                title = "'" + title + "'";
            if (abstr != null)
                abstr = "'" + abstr + "'";
            if (keywords != null)
                keywords = "'" + keywords + "'";
            setQuery("INSERT INTO PS_Descriptions (coverage, title, abstract, keywords)" +
                    " VALUES ('" + coverageId + "', " + title + ", " + abstr + ", " + keywords + ")");
            if (s.executeUpdate(query) <= 0)
                throw new SQLException("Could not insert descriptive metadata in table " +
                        "PS_Descriptions for coverage " + coverageName + " (ID " + coverageId + ")");

            s.close();

            if (commit)
                commitAndClose();
		}
		catch (SQLException sqle)
		{
            /* Abort this transaction */
            try
            {
                if (s != null)
                    s.close();
                abortAndClose();
            }
            catch (SQLException f){}

			throw new ResourceException("Metadata database error", sqle);
		}

	}

	public void ensureConnection() throws SQLException
	{
		synchronized (this)
		{
            log("Ensuring connection is valid...");
//          if( connection == null || !connection.isValid( CONNECTION_TIMEOUT ) ) { // Not implemented by PostgreSQL yet.
			if ( (conn == null) || conn.isClosed() )
			{
                log("*** Opening new DB connection !");
				close();
				openConnection();
                log("*** ok.");
//                try { throw new Exception(""); } catch (Exception e) {e.printStackTrace();}
			}
		}

	}

    public void openConnection() throws SQLException
    {
        conn = DriverManager.getConnection(url, user, pass);
        conn.setAutoCommit(false);
        savepoint = conn.setSavepoint();
    }

    public void abortAndClose() throws SQLException
    {
        if (conn != null)
        {
            conn.rollback(savepoint);
            conn.close();
            conn = null;
        }
    }

    public void commitAndClose() throws SQLException
    {
        if (conn != null)
        {
            conn.commit();
            conn.close();
            conn = null;
        }
    }

    @Override
	public Collection<String> getAxisNames()
	{
		return axisTypes.values();
	}

    /* Logging function for SQL queries. */
    private void setQuery(String q)
    {
        this.query = q;
        System.out.println("----------------------\n" + q + "\n---------------------");
    }

    /* Returns the available formatToMimetype formats, as stored in the metadata database */
    public String[] getMimetypesList()
    {
        return supportedFormats.values().toArray(new String[1]);
    }

    /* Translate a mime-type to a format name, if known to rasdaman. */
    public String mimetypeToFormat(String mime)
    {
        log(revSupportedFormats.toString());
        return revSupportedFormats.get(mime);
    }

    private void log(String msg)
    {
        if (ConfigManager.PRINT_LOG)
            System.out.println("DB: " + msg);
    }

    /**
     * @return the dataTypes
     */
    public Collection<String> getDataTypes() {
        return dataTypes.values();
    }

    /**
     * @return the interpolationTypes
     */
    public Collection<String> getInterpolationTypes() {
        return interpolationTypes.values();
    }

    /**
     * @return the nullResistances
     */
    public Collection<String> getNullResistances() {
        return nullResistances.values();
    }
}
