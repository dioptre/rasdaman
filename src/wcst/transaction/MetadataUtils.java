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


package wcst.transaction;

//~--- non-JDK imports --------------------------------------------------------

import wcps.server.core.CellDomainElement;
import wcps.server.core.DomainElement;
import wcps.server.core.InterpolationMethod;
import wcps.server.core.Metadata;
import wcps.server.core.RangeElement;

//~--- JDK imports ------------------------------------------------------------

import java.math.BigInteger;

import java.sql.ResultSet;
import java.sql.SQLException;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;

/**
 * Contains utility functions related to the coverage metadata
 *
 * @author Andrei Aiordachioaie
 */
public class MetadataUtils
{
	private static boolean printLog = true;
	private static boolean inited = false;
	private static Map<Integer, String> axisTypes;
	private static Map<Integer, String> crss;
	private static Map<Integer, String> dataTypes;
	private static Map<Integer, String> interpolationTypes;
	private static Map<Integer, String> nullResistances;
	private static Map<String, String> supportedFormats;

	/**
	 * Log a message to System.out
	 * @param str Text of the message
	 */
	private static void log(String str)
	{
		if ( printLog )
			System.out.println("<MetadataDB> " + str);
	}

	/**
	 * Reads the static metadata tables for future use
	 *
	 * @param meta MetadataDb object
	 */
	public static void readStaticTables(MetadataDb meta)
	{
		if ( inited == true )
			return;

		try
		{
			axisTypes = new HashMap<Integer, String>(5);
			ResultSet r = meta.executeSelectQuery("SELECT id, axisType FROM PS_AxisType");

			while (r.next())
			{
				axisTypes.put(r.getInt("id"), r.getString("axisType"));
			}

			dataTypes = new HashMap<Integer, String>(13);
			r = meta.executeSelectQuery("SELECT id, dataType FROM PS_Datatype");

			while (r.next())
			{
				dataTypes.put(r.getInt("id"), r.getString("dataType"));
			}

			interpolationTypes = new HashMap<Integer, String>(5);
			r = meta.executeSelectQuery("SELECT id, interpolationType FROM PS_InterpolationType");

			while (r.next())
			{
				interpolationTypes.put(r.getInt("id"), r.getString("interpolationType"));
			}

			nullResistances = new HashMap<Integer, String>(4);
			r = meta.executeSelectQuery("SELECT id, nullResistance FROM PS_NullResistance");

			while (r.next())
			{
				nullResistances.put(r.getInt("id"), r.getString("nullResistance"));
			}

			crss = new HashMap<Integer, String>();
			r = meta.executeSelectQuery("SELECT id, name FROM PS_Crs");

			while (r.next())
			{
				crss.put(r.getInt("id"), r.getString("name"));
			}

			supportedFormats = new HashMap<String, String>();
			r = meta.executeSelectQuery("SELECT name, mimetype FROM PS_Format");

			while (r.next())
			{
				supportedFormats.put(r.getString("name"), r.getString("mimetype"));
			}
		}
		catch (SQLException e)
		{
			e.printStackTrace();
		}
	}

	/**
	 * Checks the metadata DB to ,ake sure that the WCPS server has coverage "name"
	 * @param meta metadata DB object
	 * @param name the name of the coverage to check
	 * @return boolean whether the coverage is available
	 */
	public static boolean existsCoverage(MetadataDb meta, String name)
	{
		String query = "SELECT * FROM ps_coverage AS cov " + "WHERE cov.name='" + name + "'";

		try
		{
			log(query);
			ResultSet r = meta.executeSelectQuery(query);

			if ( r.next() )
			{
				log("Coverage " + name + " is in the database !");
			}
			else
			{
				log("Coverage " + name + " is NOT in the database !");

				return false;
			}
		}
		catch (SQLException e)
		{
			return false;
		}

		return true;
	}

	/**
	 * Return the ID of the default Type Interpolation
	 *
	 * @param meta metadataDb object
	 * @param cov name of the coverage that we want to add
	 * @return int ID of the default interpolation
	 */
	public static int getInterpolationCalledNone(MetadataDb meta)
	{
		int id = -1;
		String query = "SELECT * FROM ps_interpolationtype AS it " + "WHERE it.interpolationtype='none'";

		try
		{
			log(query);
			ResultSet r = meta.executeSelectQuery(query);

			if ( r.next() )
				id = r.getInt("id");
		}
		catch (SQLException e)
		{
			System.err.println("'None' interpolation method doesn't exist in DB! trying another...");

			// Try to get a random interpolation method, if "none" is not available
			String query2 = "SELECT * FROM ps_interpolationtype";

			try
			{
				log(query2);
				ResultSet r2 = meta.executeSelectQuery(query2);

				if ( r2.next() )
					id = r2.getInt("id");
			}
			catch (SQLException e2)
			{
				System.err.println("SQL Error : " + e2.getMessage());
				System.err.println("Please populate the table 'ps_interpolationtype' with relevant values! ");
			}
		}

		return id;
	}

	/**
	 * Return the ID of the default Type Interpolation
	 *
	 * @param meta metadataDb object
	 * @param cov name of the coverage that we want to add
	 * @return int ID of the default interpolation
	 */
	public static int getNullResistanceCalledNone(MetadataDb meta)
	{
		int id = -1;
		String query = "SELECT * FROM ps_nullresistance AS it " + "WHERE it.nullresistance='none'";

		try
		{
			log(query);
			ResultSet r = meta.executeSelectQuery(query);

			if ( r.next() )
				id = r.getInt("id");
		}
		catch (SQLException e)
		{
			System.err.println("'None' null resistance doesn't exist in DB! trying another...");

			// Try to get a random null resistance, if "none" is not available
			String query2 = "SELECT * FROM ps_interpolationtype";

			try
			{
				log(query2);
				ResultSet r2 = meta.executeSelectQuery(query2);

				if ( r2.next() )
					id = r2.getInt("id");
			}
			catch (SQLException e2)
			{
				System.err.println("SQL Error : " + e2.getMessage());
				System.err.println("Please populate the table 'ps_nullresistance' with relevant values! ");
			}
		}

		return id;
	}

	public static boolean renameCoverage(MetadataDb meta, String oldName, String newName)
	{
		int rows = -1;
		String query = "UPDATE ps_coverage SET (name) = ('" + newName + "') " + " WHERE name = '" + oldName + "'";

		try
		{
			log(query);
			rows = meta.executeUpdateQuery(query);
			if ( rows != 0 )
				log("Updated " + Integer.toString(rows) + " rows ...");
			else
			{
				log("Nothing updated ...");

				return false;
			}
		}
		catch (SQLException e)
		{
			System.err.println("SQL Error : " + e.getMessage());

			return false;
		}

		return true;
	}

	/**
	 * Inserts metadata about one coverage, in table ps_coverage.
	 *
	 * @param meta metadataDb object
	 * @param name name of the coverage that we want to add
	 * @return boolean whether the operation was successful
	 */
	public static boolean setAllCoverageMetadata(MetadataDb meta, String name, Object onullvalue, Object ocrs, Object onulldefault, Object ointerpdefault, Object oresistdefault)
	{
		if ( name == null )
			return false;

		int rows = -1;

		name = "'" + name + "'";
		String nullvalue = (onullvalue != null)
						   ? "'" + (String) onullvalue + "'"
						   : "null";
		String crs = (ocrs != null)
					 ? "'" + (String) ocrs + "'"
					 : "null";
		String nulldefault = (onulldefault != null)
							 ? "'" + (String) onulldefault + "'"
							 : "null";
		String interpdefault = (ointerpdefault != null)
							   ? "'" + (String) ointerpdefault + "'"
							   : "null";
		String resistdefault = (oresistdefault != null)
							   ? "'" + (String) oresistdefault + "'"
							   : "null";

		String query = "INSERT INTO ps_coverage  " + "(name, nullvalue, crs, nulldefault, interpolationtypedefault, nullresistancedefault)" + " VALUES (" + name + ", " + nullvalue + ", " + crs + ", " + nulldefault + ", " + interpdefault + ", " + resistdefault + ");";

		try
		{
			log(query);
			rows = meta.executeUpdateQuery(query);
			if ( rows != 0 )
				log("Inserted " + Integer.toString(rows) + " rows ...");
			else
			{
				log("Nothing inserted ...");

				return false;
			}
		}
		catch (SQLException e)
		{
			System.err.println("SQL Error : " + e.getMessage());

			return false;
		}

		return true;
	}

	/**
	 * Inserts domain metadata for a coverage, into table ps_domain.
	 *
	 * @param meta metadataDb object
	 * @param covId ID of the coverage
	 * @param axisId ID of the current axis
	 * @param axisName name of current axis
	 * @param axisType type of current axis
	 * @param numLo numeric low
	 * @param numHi numeric high
	 * @param strLo string low
	 * @param strHi string high
	 * @return boolean success value
	 */
	public static boolean setDomain(MetadataDb meta, int covId, int axisId, String axisName, int axisType, Double numLo, Double numHi, String strLo, String strHi)
	{
		String query;

		if ( (strLo == null) && (strHi == null) )
			query = "INSERT INTO ps_domain " + "(coverage, i, name, type, numLo, numHi, strLo, strHi) VALUES" + " ('" + covId + "', '" + axisId + "', '" + axisName + "', '" + axisType + "', '" + numLo + "', '" + numHi + "', null, null); ";
		else
			query = "INSERT INTO ps_domain " + "(coverage, i, name, type, numLo, numHi, strLo, strHi) VALUES" + " ('" + covId + "', '" + axisId + "', '" + axisName + "', '" + axisType + "', null, null, " + strLo + "', '" + strHi + "'); ";
		try
		{
			log(query);
			meta.executeUpdateQuery(query);
		}
		catch (SQLException e)
		{
			System.err.println("SQL Error : " + e.getMessage());

			return false;
		}

		return true;
	}

	/**
	 * Insert cell domain information for a coverage, in table ps_celldomains.
	 *
	 * @param meta MetadataDb object
	 * @param covId Coverage ID
	 * @param axisId axis ID
	 * @param low lowest value
	 * @param high highest value
	 * @return boolean success value
	 */
	public static boolean setCellDomain(MetadataDb meta, int covId, int axisId, int low, int high)
	{
		String query = "INSERT INTO ps_celldomain " + "(coverage, i, lo, hi) VALUES" + " ('" + covId + "', '" + axisId + "', '" + low + "', '" + high + "');";

		try
		{
			log(query);
			meta.executeUpdateQuery(query);
		}
		catch (SQLException e)
		{
			System.err.println("SQL Error : " + e.getMessage());

			return false;
		}

		return true;
	}

	/**
	 * Set range metadata for a coverage, in table ps_range.
	 *
	 * @param meta MetadataDb object
	 * @param covId ID of the coverage
	 * @param fieldId ID of the field
	 * @param fieldName Name of the field
	 * @param datatypeId ID of the datatype used
	 * @return boolean success value
	 */
	public static boolean setRange(MetadataDb meta, int covId, int fieldId, String fieldName, int datatypeId)
	{
		String query = "INSERT INTO ps_range " + "(coverage, i, name, type) VALUES" + " ('" + covId + "', '" + fieldId + "', '" + fieldName + "', '" + datatypeId + "');";

		try
		{
			log(query);
			meta.executeUpdateQuery(query);
		}
		catch (SQLException e)
		{
			System.err.println("SQL Error : " + e.getMessage());

			return false;
		}

		return true;
	}

	/**
	 * Update range metadata for a coverage, (table ps_range)
	 *
	 * @param meta MetadataDb object
	 * @param tupleId ID of the table tuple to update
	 * @param covId ID of the coverage
	 * @param fieldId ID of the field
	 * @param fieldName Name of the field
	 * @param datatypeId ID of the datatype used
	 * @return boolean success value
	 */
	public static boolean updateRange(MetadataDb meta, int tupleId, int covId, int fieldId, String fieldName, int datatypeId)
	{
		String query = "UPDATE ps_range SET " + "(coverage, i, name, type) = " + " ('" + covId + "', '" + fieldId + "', '" + fieldName + "', '" + datatypeId + "') " + " WHERE id = '" + tupleId + "'";

		try
		{
			log(query);
			meta.executeUpdateQuery(query);
		}
		catch (SQLException e)
		{
			System.err.println("SQL Error : " + e.getMessage());

			return false;
		}

		return true;
	}

	/**
	 * Set interpolation method and null resistance metadata for a coverage, in table ps_interpolationset;
	 *
	 * @param meta MetadataDb object
	 * @param covId ID of the coverage
	 * @param interpId ID of the interpolation method
	 * @param nullId ID of the null resistance
	 * @return boolean success value
	 */
	public static boolean setInterpolationSets(MetadataDb meta, int covId, int interpId, int nullId)
	{
		String query = "INSERT INTO ps_interpolationset " + "(coverage, interpolationtype , nullresistance) VALUES" + " ('" + covId + "', '" + interpId + "', '" + nullId + "');";

		try
		{
			log(query);
			meta.executeUpdateQuery(query);
		}
		catch (SQLException e)
		{
			System.err.println("SQL Error : " + e.getMessage());

			return false;
		}

		return true;
	}

	/**
	 * Set null value information, in table ps_nullsets;
	 *
	 * @param meta MetadataDb object
	 * @param covId ID of the coverage
	 * @param nullSet null set of the coverage
	 * @return boolean success value
	 */
	public static boolean setNullSets(MetadataDb meta, int covId, String nullSet)
	{
		String query = "INSERT INTO ps_nullset " + "(coverage, nullvalue) VALUES" + " ('" + covId + "', '" + nullSet + "');";

		try
		{
			log(query);
			meta.executeUpdateQuery(query);
		}
		catch (SQLException e)
		{
			System.err.println("SQL Error : " + e.getMessage());

			return false;
		}

		return true;
	}

	/**
	 * Updates textual descriptions of a coverage in table "ps_descriptions".
	 *
	 * @param meta MetadataDb object
	 * @param covId ID of the coverage
	 * @param title Title of coverage
	 * @param abstr Abstract of coverage
	 * @param keyworks Keywords of coverage, separated by comma
	 * @return boolean success value
	 */
	public static boolean updateDescriptions(MetadataDb meta, int covId, String title, String abstr, String keywords)
	{
		title = (title != null)
				? "'" + title + "'"
				: "null";
		abstr = (abstr != null)
				? "'" + abstr + "'"
				: "null";
		keywords = (keywords != null)
				   ? "'" + keywords + "'"
				   : "null";
		String query = "UPDATE ps_descriptions SET " + "(title, abstract, keywords) = " + " (" + title + ", " + abstr + ", " + keywords + ")" + " WHERE coverage = " + covId;

		try
		{
			log(query);
			meta.executeUpdateQuery(query);
		}
		catch (SQLException e)
		{
			System.err.println("SQL Error : " + e.getMessage());

			return false;
		}

		return true;
	}

	/**
	 * Inserts textual descriptions of a coverage in table "ps_descriptions".
	 *
	 * @param meta MetadataDb object
	 * @param covId ID of the coverage
	 * @param title Title of coverage
	 * @param abstr Abstract of coverage
	 * @param keyworks Keywords of coverage, separated by comma
	 * @return boolean success value
	 */
	public static boolean insertDescription(MetadataDb meta, int covId, String title, String abstr, String keywords)
	{
		title = (title != null)
				? "'" + title + "'"
				: "null";
		abstr = (abstr != null)
				? "'" + abstr + "'"
				: "null";
		keywords = (keywords != null)
				   ? "'" + keywords + "'"
				   : "null";
		String query = "INSERT INTO ps_descriptions " + "(coverage, title, abstract, keywords) VALUES " + " ('" + covId + "', " + title + ", " + abstr + ", " + keywords + ")";

		try
		{
			log(query);
			meta.executeUpdateQuery(query);
		}
		catch (SQLException e)
		{
			System.err.println("SQL Error : " + e.getMessage());

			return false;
		}

		return true;
	}

	/**
	 * Retrieve the ID of a datatype, from the ps_datatype. It is -1 if an error occured.
	 *
	 * @param meta MetadataDb object
	 * @param name Name of coverage
	 * @return int the datatype ID
	 */
	public static int getDatatypeIdByName(MetadataDb meta, String name)
	{
		int result = -1;
		String query = "SELECT * FROM ps_datatype AS data " + "WHERE data.name='" + name + "'";

		try
		{
			log(query);
			ResultSet r = meta.executeSelectQuery(query);

			if ( r.next() )
			{
				log("Datatype " + name + " is in the database !");
				result = r.getInt("id");
			}
			else
			{
				log("Datatype " + name + " is NOT in the database !");
			}
		}
		catch (SQLException e)
		{
			e.printStackTrace();
			result = -1;
		}

		return result;
	}

	/**
	 * Retrieve the axis number for a coverage, from table ps_domain.
	 * It is -1 if an error occured.
	 *
	 * @param meta MetadataDb object
	 * @param coverage ID of coverage
	 * @param axis Name of axis
	 * @return int axis number
	 */
	public static int getAxisNumberByCoverageAndName(MetadataDb meta, int coverage, String axis)
	{
		int result = -1;
		String query = "SELECT * FROM ps_domain " + "WHERE coverage='" + coverage + "' AND name='" + axis + "'";

		try
		{
			log(query);
			ResultSet r = meta.executeSelectQuery(query);

			if ( r.next() )
			{
				result = r.getInt("i");
			}
			else
			{
				log("Could not find field !");
			}
		}
		catch (SQLException e)
		{
			e.printStackTrace();
			result = -1;
		}

		return result;
	}

	/**
	 * Retrieve the ID of a null resistance object, from the ps_nullresistance. It is -1 if an error occured.
	 *
	 * @param meta MetadataDb object
	 * @param name Name of coverage
	 * @return int the null resistance ID
	 */
	public static int getNullResistanceIdByName(MetadataDb meta, String name)
	{
		int result = -1;
		String query = "SELECT * FROM ps_nullresistance AS data " + "WHERE data.name='" + name + "'";

		try
		{
			log(query);
			ResultSet r = meta.executeSelectQuery(query);

			if ( r.next() )
			{
				log("Null Resistance '" + name + "' is in the database !");
				result = r.getInt("id");
			}
			else
			{
				log("Null resistance '" + name + "' is NOT in the database !");
			}
		}
		catch (SQLException e)
		{
			e.printStackTrace();
			result = -1;
		}

		return result;
	}

	/**
	 * Retrieve the ID of an interpolation method, from the ps_interpolationmethod.
	 * It is -1 if an error occured.
	 *
	 * @param meta MetadataDb object
	 * @param name Name of coverage
	 * @return int the interpolation method ID
	 */
	public static int getInterpolationMethodIdByName(MetadataDb meta, String name)
	{
		int result = -1;
		String query = "SELECT * FROM ps_interpolationmethod AS data " + "WHERE data.name='" + name + "'";

		try
		{
			log(query);
			ResultSet r = meta.executeSelectQuery(query);

			if ( r.next() )
			{
				log("Null Resistance '" + name + "' is in the database !");
				result = r.getInt("id");
			}
			else
			{
				log("Null resistance '" + name + "' is NOT in the database !");
			}
		}
		catch (SQLException e)
		{
			e.printStackTrace();
			result = -1;
		}

		return result;
	}

	/**
	 * Retrieve the ID of a coverage, from the ps_coverage. It is -1 if an error occured.
	 *
	 * @param meta
	 * @param name Name of coverage
	 * @return int
	 */
	public static int getCoverageID(MetadataDb meta, String name)
	{
		int result = -1;
		String query = "SELECT * FROM ps_coverage AS cov " + "WHERE cov.name='" + name + "'";

		try
		{
			log(query);
			ResultSet r = meta.executeSelectQuery(query);

			if ( r.next() )
			{
				log("Coverage " + name + " is in the database !");
				result = r.getInt("id");
			}
			else
			{
				log("Coverage " + name + " is NOT in the database !");
			}
		}
		catch (SQLException e)
		{
			e.printStackTrace();
			result = -1;
		}

		return result;
	}

	/**
	 * Retrieve the ID of a row in a table, based on the name. Returns -1 if an error occured.
	 *
	 * @param meta MetadataDb object
	 * @param table Table to search
	 * @param column Field to search
	 * @param name Value of the field
	 * @return int
	 */
	public static int getIdByNameGeneral(MetadataDb meta, String table, String column, String name)
	{
		int result = -1;
		String query = "SELECT * FROM " + table + " WHERE " + column + "='" + name + "'";

		try
		{
			log(query);
			ResultSet r = meta.executeSelectQuery(query);

			if ( r.next() )
			{
				log("Found value '" + name + "' in the table " + table + "." + column + "!");
				result = r.getInt("id");
			}
			else
			{
				log("UNKNOWN Value '" + name + "' in the table " + table + "." + column + "!");
			}
		}
		catch (SQLException e)
		{
			e.printStackTrace();
			result = -1;
		}

		return result;
	}

	/**
	 * Retrieve a field of a row in a table, based on the row id. Returns -1 if an error occured.
	 *
	 * @param meta MetadataDb object
	 * @param table Table to search
	 * @param id the row ID
	 * @param field field name to return
	 * @return String null if no row was found, otherwise the contents of the field
	 */
	public static String getNameByIdGeneral(MetadataDb meta, String table, int id, String field)
	{
		String result = null;
		String query = "SELECT * FROM " + table + " WHERE id='" + id + "'";

		try
		{
			log(query);
			ResultSet r = meta.executeSelectQuery(query);

			if ( r.next() )
			{
				log("Found row with ID='" + id + "' in the table " + table + "!");
				result = r.getString(field);
			}
			else
			{
				log("UNKNOWN Row with ID='" + id + "' in the table " + table + "!");
			}
		}
		catch (SQLException e)
		{
			e.printStackTrace();
			result = null;
		}

		return result;
	}

	public static boolean deleteRowFromTable(MetadataDb meta, String table, String field, String value)
	{
		String query = "DELETE FROM " + table + " WHERE " + field + " = '" + value + "'";
		int rows = -1;

		try
		{
			log(query);
			rows = meta.executeUpdateQuery(query);
			if ( rows != 0 )
				log("Deleted " + Integer.toString(rows) + " rows ...");
			else
			{
				log("Nothing deleted ...");

				return false;
			}
		}
		catch (SQLException e)
		{
			System.err.println("SQL Error : " + e.getMessage());

			return false;
		}

		return true;
	}

	/**
	 * Updates the coverage default interpolation method + null resistance (table ps_coverage)
	 *
	 * @param meta metadataDb object
	 * @param name name of the coverage that we want to add
	 * @param interp ID of the default interpolation method
	 * @param resist ID of the default null resistance
	 * @return boolean whether the operation was successful
	 */
	public static boolean setDefaultCoverageInterpolation(MetadataDb meta, String name, int interp, int resist)
	{
		int rows = -1;

		String query = "UPDATE ps_coverage SET " + "(interpolationtypedefault, nullresistancedefault)" + " = (" + interp + ", " + resist + ")" + " WHERE name = '" + name + "'";

		try
		{
			log(query);
			rows = meta.executeUpdateQuery(query);
			if ( rows != 0 )
				log("Updated " + Integer.toString(rows) + " rows ...");
			else
			{
				log("Nothing updated ...");

				return false;
			}
		}
		catch (SQLException e)
		{
			System.err.println("SQL Error : " + e.getMessage());

			return false;
		}

		return true;
	}

	/**
	 * Update cell domain information for a coverage, in table ps_celldomains.
	 *
	 * @param meta MetadataDb object
	 * @param covId Coverage ID
	 * @param axisId axis ID
	 * @param low lowest value
	 * @param high highest value
	 * @return boolean success value
	 */
	public static boolean updateCellDomain(MetadataDb meta, int covId, int axisId, long low, long high)
	{
		String query = "UPDATE ps_celldomain SET " + "(lo, hi) = " + " ('" + low + "', '" + high + "') WHERE " + " coverage='" + covId + "' AND i = '" + axisId + "'";

		try
		{
			log(query);
			meta.executeUpdateQuery(query);
		}
		catch (SQLException e)
		{
			System.err.println("SQL Error : " + e.getMessage());

			return false;
		}

		return true;
	}

	/**
	 * Update textual descriptions for a coverage, in table ps_descriptions.
	 * In case some of the parameters are null, they are ignored. If all parameters
	 * are null, query succeeds as nothing is modified.
	 *
	 * @param meta MetadataDb object
	 * @param covId ID of coverage to update
	 * @param title Coverage Title
	 * @param abstr Abstract of coverage
	 * @param keys Set of keywords, as a String
	 * @return boolean success value
	 */
	public static boolean updateDescriptionMetadata(MetadataDb meta, int covId, String title, String abstr, String keys)
	{
		String columns = "", values = "";

		if ( title != null )
		{
			columns += ",title";
			values += ",'" + title + "'";
		}
		if ( abstr != null )
		{
			columns += ",abstract";
			values += ",'" + abstr + "'";
		}
		if ( keys != null )
		{
			columns += ",keywords";
			values += ",'" + keys + "'";
		}
		// Get rid of the beginning comma
		if ( columns.length() > 0 )
			columns = columns.substring(1);
		else
			return true;
		if ( values.length() > 0 )
			values = values.substring(1);
		else
			return true;
		columns = "(" + columns + ")";
		values = "(" + values + ")";

		String query = "UPDATE ps_descriptions SET " + columns + "=" + values + " WHERE " + " coverage='" + covId + "';";

		try
		{
			log(query);
			meta.executeUpdateQuery(query);
		}
		catch (SQLException e)
		{
			System.err.println("SQL Error : " + e.getMessage());

			return false;
		}

		return true;
	}

	/**
	 * Check new metadata for a coverage if it is valid. Throws an error if
	 * metadata is invalid.
	 *
	 * NOTE: This function was copied and adapted from PetaScope (WCPS),
	 * class wcps.server.core.DbMetadataSource
	 *
	 * @param meta MetadataDb object
	 * @param coverageName desired coverage name
	 * @return a Metadata Object
	 * @throws WCSException
	 */
	public static Metadata checkCoverageMetadata(MetadataDb meta, String coverageName) throws WCSException
	{
		readStaticTables(meta);

		if ( (coverageName == null) || coverageName.equals("") )
		{
			throw new WCSException("NoApplicableCode", "Cannot retrieve coverage with null or empty name");
		}

		try
		{
			ResultSet r = meta.executeSelectQuery("SELECT id, nullDefault, interpolationTypeDefault, nullResistanceDefault FROM PS_Coverage WHERE name = '" + coverageName + "'");

			if ( ! r.next() )
			{
				throw new WCSException("NoApplicableCode", "Coverage '" + coverageName + "' is not served by this server");
			}

			int coverage = r.getInt("id");
			String nullDefault = r.getString("nullDefault");
			String interpolationTypeDefault = getNameByIdGeneral(meta, "PS_InterpolationType", r.getInt("interpolationTypeDefault"), "interpolationType");
			String nullResistanceDefault = getNameByIdGeneral(meta, "PS_NullResistance", r.getInt("nullResistanceDefault"), "nullResistance");

			r = meta.executeSelectQuery("SELECT lo, hi FROM PS_CellDomain WHERE coverage = '" + coverage + "' ORDER BY i ASC");
			List<CellDomainElement> cellDomain = new ArrayList<CellDomainElement>(r.getFetchSize());

			while (r.next())
			{
				cellDomain.add(new CellDomainElement(BigInteger.valueOf(r.getInt("lo")), BigInteger.valueOf(r.getInt("hi"))));
			}

			r = meta.executeSelectQuery("SELECT name, type FROM PS_Range WHERE coverage = '" + coverage + "' ORDER BY i ASC");
			List<RangeElement> range = new ArrayList<RangeElement>(r.getFetchSize());

			while (r.next())
			{
				range.add(new RangeElement(r.getString("name"), dataTypes.get(r.getInt("type"))));
			}

			r = meta.executeSelectQuery("SELECT interpolationType, nullResistance FROM PS_InterpolationSet WHERE coverage = '" + coverage + "'");
			Set<InterpolationMethod> interpolationSet = new HashSet<InterpolationMethod>(r.getFetchSize());

			while (r.next())
			{
				interpolationSet.add(new InterpolationMethod(interpolationTypes.get(r.getInt("interpolationType")), nullResistances.get(r.getInt("nullResistance"))));
			}

			r = meta.executeSelectQuery("SELECT nullValue FROM PS_NullSet WHERE coverage = '" + coverage + "'");
			Set<String> nullSet = new HashSet<String>(r.getFetchSize());

			while (r.next())
			{
				nullSet.add(r.getString("nullValue"));
			}

			r = meta.executeSelectQuery("SELECT id, name, type, numLo, numHi, strLo, strHi FROM PS_Domain WHERE coverage = '" + coverage + "' ORDER BY i ASC");
			List<DomainElement> domain = new ArrayList<DomainElement>(r.getFetchSize());

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

				ResultSet rr = meta.executeSelectQuery("SELECT crs FROM PS_CrsSet WHERE axis = '" + r.getInt("id") + "'");
				Set<String> crsSet = new HashSet<String>(rr.getFetchSize());

				while (rr.next())
				{
					crsSet.add(crss.get(rr.getInt("crs")));
				}

				crsSet.add(DomainElement.IMAGE_CRS);
				domain.add(new DomainElement(r.getString("name"), axisTypes.get(r.getInt("type")), numLo, numHi, r.getString("strLo"), r.getString("strHi"), crsSet, axisTypes.values()));
			}

			return new Metadata(cellDomain, range, nullSet, nullDefault, interpolationSet, new InterpolationMethod(interpolationTypeDefault, nullResistanceDefault), coverageName, domain);
		}
		catch (Exception e)
		{
			e.printStackTrace();

			throw new WCSException("NoApplicableCode", e.getMessage());
		}

	}

}
