/*
 * This file is part of rasdaman community.
 *
 * Rasdaman community is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Rasdaman community is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with rasdaman community.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright 2003 - 2010 Peter Baumann / rasdaman GmbH.
 *
 * For more information please see <http://www.rasdaman.org>
 * or contact Peter Baumann via <baumann@rasdaman.com>.
 */
/************************************************************************
 * 
 * PURPOSE
 * provide access to the "Layer" table of rasgeo commander.
 * There are insert, update, and fetch statements.
 * 
 * COMMENTS
 * - uses CommanderConnection for JDBC access.
 *
 *********************************************************** */

package petascope.wms.commander;

//import java.sql.*;

import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/**
 * This class accomplishes tuple-at-a-time access to the Layer table,
 * encapsulating SQL statements.
 **/
public class Table_Layer
{
    private static Logger log = LoggerFactory.getLogger(Table_Layer.class);
            
    /**
     * string & int result for invalid call of getter method
     * (we tentatively use null here to cause problems elsewhere when working with such an invalid result)
     */
    private static final String UNDEF_STRING = null;
    private static final int    UNDEF_INT    = -1;
    private static final double UNDEF_FLOAT  = (double) -1.;
    
    /**
     * result set for fetch*() calls
     */
    private ResultSet resultSet = null;
    
    /**
     * flag indicating whether current tuple (see next) contains a value set retrieved from the database
     */
    private boolean tupleValid = false;
    
    /**
     * value set for one tuple of this table; values are good only if tupleValid==true
     */
    private int layerId = UNDEF_INT;
    private String name = UNDEF_STRING;
    private String title = UNDEF_STRING;
    private String srs = UNDEF_STRING;
    private String authority = UNDEF_STRING;
    private String latLonBoxXmin = UNDEF_STRING;
    private String latLonBoxXmax = UNDEF_STRING;
    private String latLonBoxYmin = UNDEF_STRING;
    private String latLonBoxYmax = UNDEF_STRING;
    private String bBoxXmin = UNDEF_STRING;
    private String bBoxXmax = UNDEF_STRING;
    private String bBoxYmin = UNDEF_STRING;
    private String bBoxYmax = UNDEF_STRING;
    private String attributionURL = UNDEF_STRING;
    private String attributionTitle = UNDEF_STRING;
    private int logoWidth = UNDEF_INT;
    private int logoHeight = UNDEF_INT;
    private String logoFormat = UNDEF_STRING;
    private String logoURL = UNDEF_STRING;
    private String featureURL = UNDEF_STRING;
    private double resolution = UNDEF_FLOAT;
    
    /**
     * acceptable values for map type
     */
    private MapType mapType = MapType.RGB;
    
    /**
     * constructor
     * @return CommanderConnection
     **/
    public Table_Layer()
    {
        log.info( "Table_Layer::Table_Layer()" );
        tupleValid = false;
        log.info( "Table_Layer::Table_Layer()" );
    }
    
    /**
     * insert one tuple; instance's local attribute set is not modified.
     * precondition:
     *	JDBC connection opened
     *	all strings non-null
     * @param conn		JDBC connection
     * @param id		service tuple id
     * @return tuple id of new entry
     * @throws SQLException
     **/
    public int insertTuple( CommanderConnection conn, String name, String title, String srs, String authority, String latLonBoxXmin, String latLonBoxXmax, String latLonBoxYmin, String latLonBoxYmax, String bBoxXmin, String bBoxXmax, String bBoxYmin, String bBoxYmax, String attributionURL, String attributionTitle, int logoWidth, int logoHeight, String logoFormat, String logoURL, String featureURL, double resolution, MapType mapType )
            throws SQLException, ConnectionFailedException, InvalidInputException
    {
        log.info( "Table_Layer::insertTuple()" );
        
        // --- plausi checks ----------------------------------
        // - connection object
        if (conn == null)
        {
            log.info( "Table_Layer::insertTuple() -- Error: no database connection." );
            throw new ConnectionFailedException( "no database connection." );
        }
        
        // - null string
        if (name==null || title==null || srs==null || authority==null || attributionURL==null || attributionTitle==null || logoFormat==null || logoURL==null || featureURL==null)
        {
            log.info( "Table_Layer::insertTuple() -- Error: null parameter passed" );
            throw new InvalidInputException( "null parameter passed" );
        }
        
        // - non-positive resolution
        if (resolution <= 0.0)
        {
            log.info( "Table_Layer::insertTuple() -- Error: resolution must be positive float value." );
            throw new InvalidInputException( "resolution must be positive float value." );
        }
        
        // --- action -----------------------------------------
        // do this until we find out how to get back a new value from INSERT
        String query = "select max(" + Globals.TABLE_LAYER_LAYERID + ")"
                + " from " + Globals.TABLE_LAYER;
        log.debug( "want to execute: " + query );
        Statement stmt = conn.createStatement();
        stmt.executeQuery( query );
        ResultSet auxResultSet = stmt.executeQuery( query );
        int newId = 0;
        if (auxResultSet != null && auxResultSet.next())
        {
            newId = auxResultSet.getInt(1) + 1;
            log.debug( "new id is: " + newId );
        }
        else
        {
            log.info( "Table_Layer::insertTuple() -- Error: cannot get new Layers id" );
            throw new InvalidInputException( "Error: cannot get new Layers id" );
        }
        // make VERY sure that attribute names and values are listed in the same sequence!
        query = "insert into " + Globals.TABLE_LAYER + " ( "
                + Globals.TABLE_LAYER_LAYERID + ", "
                + Globals.TABLE_LAYER_NAME + ", "
                + Globals.TABLE_LAYER_TITLE + ", "
                + Globals.TABLE_LAYER_SRS + ", "
                + Globals.TABLE_LAYER_AUTHORITY + ", "
                + Globals.TABLE_LAYER_LATLONXMIN + ", "
                + Globals.TABLE_LAYER_LATLONXMAX + ", "
                + Globals.TABLE_LAYER_LATLONYMIN + ", "
                + Globals.TABLE_LAYER_LATLONYMAX + ", "
                + Globals.TABLE_LAYER_BBOXXMIN + ", "
                + Globals.TABLE_LAYER_BBOXXMAX + ", "
                + Globals.TABLE_LAYER_BBOXYMIN + ", "
                + Globals.TABLE_LAYER_BBOXYMAX + ", "
                + Globals.TABLE_LAYER_ATTRIBUTIONURL + ", "
                + Globals.TABLE_LAYER_ATTRIBUTIONTITLE + ", "
                + Globals.TABLE_LAYER_LOGOWIDTH + ", "
                + Globals.TABLE_LAYER_LOGOHEIGHT + ", "
                + Globals.TABLE_LAYER_LOGOFORMAT + ", "
                + Globals.TABLE_LAYER_LOGOURL + ", "
                + Globals.TABLE_LAYER_FEATUREURL + ", "
                + Globals.TABLE_LAYER_RESOLUTION + ", "
                + Globals.TABLE_LAYER_MAPTYPE + " )"
                + " values( "
                + newId + ", "
                + "'" + name + "', "
                + "'" + title + "', "
                + "'" + srs + "', "
                + "'" + authority + "', "
                + latLonBoxXmin + ", "
                + latLonBoxXmax + ", "
                + latLonBoxYmin + ", "
                + latLonBoxYmax + ", "
                + bBoxXmin + ", "
                + bBoxXmax + ", "
                + bBoxYmin + ", "
                + bBoxYmax + ", "
                + "'" + attributionURL + "', "
                + "'" + attributionTitle + "', "
                + logoWidth + ", "
                + logoHeight + ", "
                + "'" + logoFormat + "', "
                + "'" + logoURL + "', "
                + "'" + featureURL + "', "
                + resolution + ", "
                + "'" + mapType + "' )";
        log.debug( "want to execute: " + query );
        stmt.executeUpdate( query );
        
        log.info( "Table_Layer::insertTuple() -> "  + newId );
        return newId;
    }
    
    /**
     * update tuple from all parameters passed.
     * null strings will be set to ""
     * precondition:
     *	JDBC connection opened
     *	tuple with id exists
     * @param conn		JDBC connection
     * @param id		service tuple id
     * @throws SQLException
     **/
    public void updateTuple( CommanderConnection conn, int layerId, String name, String title, String srs, String authority, String latLonBoxXmin, String latLonBoxXmax, String latLonBoxYmin, String latLonBoxYmax, String bBoxXmin, String bBoxXmax, String bBoxYmin, String bBoxYmax, String attributionURL, String attributionTitle, int logoWidth, int logoHeight, String logoFormat, String logoURL, String featureURL, double resolution, MapType mapType )
            throws SQLException, ConnectionFailedException
    {
        log.info( "Table_Layer::updateTuple( id=" + layerId + " )" );
        
        // --- plausi checks ----------------------------------
        // - connection object
        if (conn == null)
        {
            log.info( "Table_Layer::updateTuple() -- Error: no database connection." );
            throw new ConnectionFailedException( "no database connection." );
        }
        // - null string -> ""
        if (name==null)
            name = "";
        if (title==null)
            title = "";
        if (srs==null)
            srs = "";
        if (authority==null)
            authority = "";
        if (attributionURL==null)
            attributionURL = "";
        if (attributionTitle==null)
            attributionTitle = "";
        if (logoFormat==null)
            logoFormat = "";
        if (logoURL==null)
            logoURL = "";
        if (featureURL==null)
            featureURL = "";
        
        // --- action -----------------------------------------
        String query = "update " + Globals.TABLE_LAYER + " set "
                + Globals.TABLE_LAYER_NAME + " = '" + name + "', "
                + Globals.TABLE_LAYER_TITLE + " = '" + title + "', "
                + Globals.TABLE_LAYER_SRS + " = '" + srs + "', "
                + Globals.TABLE_LAYER_AUTHORITY + " = '" + authority + "', "
                + Globals.TABLE_LAYER_LATLONXMIN + " = " + latLonBoxXmin + ", "
                + Globals.TABLE_LAYER_LATLONXMAX + " = " + latLonBoxXmax + ", "
                + Globals.TABLE_LAYER_LATLONYMIN + " = " + latLonBoxYmin + ", "
                + Globals.TABLE_LAYER_LATLONYMAX + " = " + latLonBoxYmax + ", "
                + Globals.TABLE_LAYER_BBOXXMIN + " = " + bBoxXmin + ", "
                + Globals.TABLE_LAYER_BBOXXMAX + " = " + bBoxXmax + ", "
                + Globals.TABLE_LAYER_BBOXYMIN + " = " + bBoxYmin + ", "
                + Globals.TABLE_LAYER_BBOXYMAX + " = " + bBoxYmax + ", "
                + Globals.TABLE_LAYER_ATTRIBUTIONURL + " = '" + attributionURL + "', "
                + Globals.TABLE_LAYER_ATTRIBUTIONTITLE + " = '" + attributionTitle + "', "
                + Globals.TABLE_LAYER_LOGOWIDTH + " = " + logoWidth + ", "
                + Globals.TABLE_LAYER_LOGOHEIGHT + " = " + logoHeight + ", "
                + Globals.TABLE_LAYER_LOGOFORMAT + " = '" + logoFormat + "', "
                + Globals.TABLE_LAYER_LOGOURL + " = '" + logoURL + "', "
                + Globals.TABLE_LAYER_FEATUREURL + " = '" + featureURL + "', "
                + Globals.TABLE_LAYER_RESOLUTION + " = " + resolution + ", "
                + Globals.TABLE_LAYER_MAPTYPE + " = '" + mapType + "' "
                + " where " + Globals.TABLE_LAYER_LAYERID + " = " + layerId;
        log.debug( "want to execute: " + query );
        Statement stmt = conn.createStatement();
        stmt.executeUpdate( query );
        
        log.info( "Table_Layer::updateTuple()" );
    }
    
    /**
     * delete tuple given by its (unique) id
     * precondition:
     *	JDBC connection opened
     * @param conn		JDBC connection
     * @param id		service tuple id
     * @throws SQLException
     **/
    public void deleteTuple( CommanderConnection conn, int id )
            throws SQLException, ConnectionFailedException
    {
        log.info( "Table_Layer::deleteTuple()" );
        
        // --- plausi checks ----------------------------------
        // - connection object
        if (conn == null)
        {
            log.info( "Table_Layer::deleteTuple() -- Error: no database connection." );
            throw new ConnectionFailedException( "no database connection." );
        }
        
        // --- action -----------------------------------------
        String query = "delete from " + Globals.TABLE_LAYER + " where " + Globals.TABLE_LAYER_LAYERID + " = " + id;
        Statement stmt = conn.createStatement();
        stmt.executeUpdate( query );
        
        log.info( "Table_Layer::deleteTuple() -- ok" );
    }
    
    /**
     * fetch one tuple by its (unique) id, keep in local result set
     * precondition:
     *	JDBC connection opened
     * @param conn		JDBC connection
     * @param id		service tuple id
     * @throws SQLException
     **/
    public void fetchTupleById( CommanderConnection conn, int id )
            throws SQLException, InvalidInputException
    {
        log.info( "Table_Layer::fetchTupleById()" );
        
        // execute query
        String query = "select "
                + Globals.TABLE_LAYER+"."+Globals.TABLE_LAYER_LAYERID + ", "
                + Globals.TABLE_LAYER_NAME + ", "
                + Globals.TABLE_LAYER_TITLE + ", "
                + Globals.TABLE_LAYER_SRS + ", "
                + Globals.TABLE_LAYER_AUTHORITY + ", "
                + Globals.TABLE_LAYER_LATLONXMIN + ", "
                + Globals.TABLE_LAYER_LATLONXMAX + ", "
                + Globals.TABLE_LAYER_LATLONYMIN + ", "
                + Globals.TABLE_LAYER_LATLONYMAX + ", "
                + Globals.TABLE_LAYER_BBOXXMIN + ", "
                + Globals.TABLE_LAYER_BBOXXMAX + ", "
                + Globals.TABLE_LAYER_BBOXYMIN + ", "
                + Globals.TABLE_LAYER_BBOXYMAX + ", "
                + Globals.TABLE_LAYER_ATTRIBUTIONURL + ", "
                + Globals.TABLE_LAYER_ATTRIBUTIONTITLE + ", "
                + Globals.TABLE_LAYER_LOGOWIDTH + ", "
                + Globals.TABLE_LAYER_LOGOHEIGHT + ", "
                + Globals.TABLE_LAYER_LOGOFORMAT + ", "
                + Globals.TABLE_LAYER_LOGOURL + ", "
                + Globals.TABLE_LAYER_FEATUREURL + ", "
                + Globals.TABLE_LAYER_RESOLUTION + ", "
                + Globals.TABLE_LAYER_MAPTYPE
                + " from " + Globals.TABLE_LAYER + ", " + Globals.TABLE_SERVICELAYER
                + " where "  + Globals.TABLE_LAYER        + "." + Globals.TABLE_LAYER_LAYERID
                + "     = " + id
                //I dont know why this	+ "   and "+ Globals.TABLE_SERVICELAYER + "." + Globals.TABLE_SERVICELAYER_LAYERID
                //should be heere	+ "     = " + Globals.TABLE_LAYER        + "." + Globals.TABLE_LAYER_LAYERID
                + " order by " + Globals.TABLE_SERVICELAYER        + "." + Globals.TABLE_SERVICELAYER_LAYERSEQ;
        log.debug( "want to execute: " + query );
        Statement stmt = conn.createStatement();
        resultSet = stmt.executeQuery( query );
        
        log.info( "Table_Layer::fetchTupleById()" );
    }
    
    /**
     * fetch all tuples from table, store result locally as a JDBC ResultList
     * precondition:
     *	JDBC connection opened
     * @param conn		JDBC connection
     * @throws SQLException
     **/
    public void fetchAllTuples( CommanderConnection conn )
            throws SQLException
    {
        log.info( "Table_Layer::fetchAllTuples()" );
        
        // execute query
        String query = "select "
                + Globals.TABLE_LAYER_LAYERID + ", "
                + Globals.TABLE_LAYER_NAME + ", "
                + Globals.TABLE_LAYER_TITLE + ", "
                + Globals.TABLE_LAYER_SRS + ", "
                + Globals.TABLE_LAYER_AUTHORITY + ", "
                + Globals.TABLE_LAYER_LATLONXMIN + ", "
                + Globals.TABLE_LAYER_LATLONXMAX + ", "
                + Globals.TABLE_LAYER_LATLONYMIN + ", "
                + Globals.TABLE_LAYER_LATLONYMAX + ", "
                + Globals.TABLE_LAYER_BBOXXMIN + ", "
                + Globals.TABLE_LAYER_BBOXXMAX + ", "
                + Globals.TABLE_LAYER_BBOXYMIN + ", "
                + Globals.TABLE_LAYER_BBOXYMAX + ", "
                + Globals.TABLE_LAYER_ATTRIBUTIONURL + ", "
                + Globals.TABLE_LAYER_ATTRIBUTIONTITLE + ", "
                + Globals.TABLE_LAYER_LOGOWIDTH + ", "
                + Globals.TABLE_LAYER_LOGOHEIGHT + ", "
                + Globals.TABLE_LAYER_LOGOFORMAT + ", "
                + Globals.TABLE_LAYER_LOGOURL + ", "
                + Globals.TABLE_LAYER_FEATUREURL + ", "
                + Globals.TABLE_LAYER_RESOLUTION + ", "
                + Globals.TABLE_LAYER_MAPTYPE
                + " from " + Globals.TABLE_LAYER;
        log.debug( "want to execute: " + query );
        Statement stmt = conn.createStatement();
        stmt.setFetchSize(0);			// Turn cursor off
        resultSet = stmt.executeQuery( query );
        
        log.info( "Table_Layer::fetchAllTuples()" );
    }
    
    
    /**
     * fetch all layers pertaining to a particular service, store result locally as a JDBC ResultList
     * precondition:
     *	JDBC connection opened
     * @param conn		JDBC connection
     * @throws SQLException
     **/
    public void fetchTuplesByServiceId( CommanderConnection conn, int sId )
            throws SQLException
    {
        log.info( "Table_Layer::fetchTuplesByServiceId()" );
        
        // execute query
        String query = "select "
                + Globals.TABLE_LAYER + "." + Globals.TABLE_LAYER_LAYERID + ", "
                + Globals.TABLE_LAYER + "." + Globals.TABLE_LAYER_NAME + ", "
                + Globals.TABLE_LAYER + "." + Globals.TABLE_LAYER_TITLE + ", "
                + Globals.TABLE_LAYER + "." + Globals.TABLE_LAYER_SRS + ", "
                + Globals.TABLE_LAYER + "." + Globals.TABLE_LAYER_AUTHORITY + ", "
                + Globals.TABLE_LAYER + "." + Globals.TABLE_LAYER_LATLONXMIN + ", "
                + Globals.TABLE_LAYER + "." + Globals.TABLE_LAYER_LATLONXMAX + ", "
                + Globals.TABLE_LAYER + "." + Globals.TABLE_LAYER_LATLONYMIN + ", "
                + Globals.TABLE_LAYER + "." + Globals.TABLE_LAYER_LATLONYMAX + ", "
                + Globals.TABLE_LAYER + "." + Globals.TABLE_LAYER_BBOXXMIN + ", "
                + Globals.TABLE_LAYER + "." + Globals.TABLE_LAYER_BBOXXMAX + ", "
                + Globals.TABLE_LAYER + "." + Globals.TABLE_LAYER_BBOXYMIN + ", "
                + Globals.TABLE_LAYER + "." + Globals.TABLE_LAYER_BBOXYMAX + ", "
                + Globals.TABLE_LAYER + "." + Globals.TABLE_LAYER_ATTRIBUTIONURL + ", "
                + Globals.TABLE_LAYER + "." + Globals.TABLE_LAYER_ATTRIBUTIONTITLE + ", "
                + Globals.TABLE_LAYER + "." + Globals.TABLE_LAYER_LOGOWIDTH + ", "
                + Globals.TABLE_LAYER + "." + Globals.TABLE_LAYER_LOGOHEIGHT + ", "
                + Globals.TABLE_LAYER + "." + Globals.TABLE_LAYER_LOGOFORMAT + ", "
                + Globals.TABLE_LAYER + "." + Globals.TABLE_LAYER_LOGOURL + ", "
                + Globals.TABLE_LAYER + "." + Globals.TABLE_LAYER_FEATUREURL + ", "
                + Globals.TABLE_LAYER + "." + Globals.TABLE_LAYER_RESOLUTION + ", "
                + Globals.TABLE_LAYER + "." + Globals.TABLE_LAYER_MAPTYPE
                + " from " + Globals.TABLE_LAYER + ", "  + Globals.TABLE_SERVICELAYER
                + " where " + Globals.TABLE_LAYER        + "." + Globals.TABLE_LAYER_LAYERID
                + "     = " + Globals.TABLE_SERVICELAYER + "." + Globals.TABLE_SERVICELAYER_LAYERID
                + "   and " + Globals.TABLE_SERVICELAYER + "." + Globals.TABLE_SERVICELAYER_SERVICEID
                + "     = " + sId
                + " order by " + Globals.TABLE_SERVICELAYER + "." + Globals.TABLE_SERVICELAYER_LAYERSEQ;
        log.debug( "want to execute: " + query );
        Statement stmt = conn.createStatement();
        stmt.setFetchSize(0);			// Turn cursor off
        resultSet = stmt.executeQuery( query );
        
        log.info( "Table_Layer::fetchTuplesByServiceId()" );
    }
    
    /**
     * cursor over result set of previous fetchAllTuples()
     * if successful, the tuple variables are set with the current fetch values
     */
    public boolean next()
            throws SQLException
    {
        boolean moreAvailable = resultSet.next();
        
        if (moreAvailable)
        {
            layerId		= resultSet.getInt(    Globals.TABLE_LAYER_LAYERID );
            name		= resultSet.getString( Globals.TABLE_LAYER_NAME );
            title		= resultSet.getString( Globals.TABLE_LAYER_TITLE );
            srs		= resultSet.getString( Globals.TABLE_LAYER_SRS );
            authority	= resultSet.getString( Globals.TABLE_LAYER_AUTHORITY );
            latLonBoxXmin	= resultSet.getString( Globals.TABLE_LAYER_LATLONXMIN );
            latLonBoxXmax	= resultSet.getString( Globals.TABLE_LAYER_LATLONXMAX );
            latLonBoxYmin	= resultSet.getString( Globals.TABLE_LAYER_LATLONYMIN );
            latLonBoxYmax	= resultSet.getString( Globals.TABLE_LAYER_LATLONYMAX );
            bBoxXmin	= resultSet.getString( Globals.TABLE_LAYER_BBOXXMIN );
            bBoxXmax	= resultSet.getString( Globals.TABLE_LAYER_BBOXXMAX );
            bBoxYmin	= resultSet.getString( Globals.TABLE_LAYER_BBOXYMIN );
            bBoxYmax	= resultSet.getString( Globals.TABLE_LAYER_BBOXYMAX );
            attributionURL	= resultSet.getString( Globals.TABLE_LAYER_ATTRIBUTIONURL );
            attributionTitle= resultSet.getString( Globals.TABLE_LAYER_ATTRIBUTIONTITLE );
            logoWidth	= resultSet.getInt(    Globals.TABLE_LAYER_LOGOWIDTH );
            logoHeight	= resultSet.getInt(    Globals.TABLE_LAYER_LOGOHEIGHT );
            logoFormat	= resultSet.getString( Globals.TABLE_LAYER_LOGOFORMAT );
            logoURL		= resultSet.getString( Globals.TABLE_LAYER_LOGOURL );
            featureURL	= resultSet.getString( Globals.TABLE_LAYER_FEATUREURL );
            resolution	= resultSet.getFloat(  Globals.TABLE_LAYER_RESOLUTION );
            mapType		= MapType.valueOf( resultSet.getString( Globals.TABLE_LAYER_MAPTYPE ) );
            
            tupleValid = true;
        }
        else
        {
            tupleValid = false;
            resultSet.getStatement().close();
            resultSet.close();
            resultSet = null;
        }
        
        return moreAvailable;
    }
    
    /**
     * close current result set, discarding all items in result set and local tuple variables
     * precondition:
     *	successful call to fetchTuple()
     **/
    public void close()
            throws SQLException
    {
        log.info( "Table_Layer::close()" );
        
        resultSet.getStatement().close();
        resultSet.close();
        resultSet = null;
        
        tupleValid = false;
        
        log.info( "Table_Layer::close()" );
    }
    
    /**
     * is current tuple valid, ie, has there been a fetch before?
     * precondition:
     *	successful call of fetchTuple
     **/
    public boolean isTupleValid()
    {
        return tupleValid;
    }
    
    /**
     * getter methods for attributes.
     * precondition:
     *	tupleValid
     **/
    public int getLayerId()
    {
        return ( tupleValid ? layerId : UNDEF_INT );
    }
    public String getName()
    {
        return ( tupleValid ? name : UNDEF_STRING );
    }
    public String getTitle()
    {
        return ( tupleValid ? title : UNDEF_STRING );
    }
    public String getSrs()
    {
        return ( tupleValid ? srs : UNDEF_STRING );
    }
    public String getAuthority()
    {
        return ( tupleValid ? authority : UNDEF_STRING );
    }
    public String getLatLonBoxXmin()
    {
        return ( tupleValid ? latLonBoxXmin : UNDEF_STRING );
    }
    public String getLatLonBoxXmax()
    {
        return ( tupleValid ? latLonBoxXmax : UNDEF_STRING );
    }
    public String getLatLonBoxYmin()
    {
        return ( tupleValid ? latLonBoxYmin : UNDEF_STRING );
    }
    public String getLatLonBoxYmax()
    {
        return ( tupleValid ? latLonBoxYmax : UNDEF_STRING );
    }
    public String getBBoxXmin()
    {
        return ( tupleValid ? bBoxXmin : UNDEF_STRING );
    }
    public String getBBoxXmax()
    {
        return ( tupleValid ? bBoxXmax : UNDEF_STRING );
    }
    public String getBBoxYmin()
    {
        return ( tupleValid ? bBoxYmin : UNDEF_STRING );
    }
    public String getBBoxYmax()
    {
        return ( tupleValid ? bBoxYmax : UNDEF_STRING );
    }
    public String getAttributionURL()
    {
        return ( tupleValid ? attributionURL : UNDEF_STRING );
    }
    public String getAttributionTitle()
    {
        return ( tupleValid ? attributionTitle : UNDEF_STRING );
    }
    public int getLogoWidth()
    {
        return ( tupleValid ? logoWidth : UNDEF_INT );
    }
    public int getLogoHeight()
    {
        return ( tupleValid ? logoHeight : UNDEF_INT );
    }
    public String getLogoFormat()
    {
        return ( tupleValid ? logoFormat : UNDEF_STRING );
    }
    public String getLogoURL()
    {
        return ( tupleValid ? logoURL : UNDEF_STRING );
    }
    public String getFeatureURL()
    {
        return ( tupleValid ? featureURL : UNDEF_STRING );
    }
    public double getResolution()
    {
        return ( tupleValid ? resolution : UNDEF_FLOAT );
    }
    public MapType getMapType()
    {
        return ( tupleValid ? mapType : null );	// we have no undef value
    }
    
    /**
     * Method toString
     * @return String
     */
    public String toString()
    {
        String stringRepresentation = "Layer( "
                + "tupleValid='" + tupleValid + "'; "
                + "layerId=" + layerId	+ ", "
                + "name='" + name		+ "', "
                + "title='" + title		+ "', "
                + "srs='" + srs		+ "', "
                + "authority='" + authority	+ "', "
                + "latLonBoxXmin=" + latLonBoxXmin	+ ", "
                + "latLonBoxXmax=" + latLonBoxXmax	+ ", "
                + "latLonBoxYmin=" + latLonBoxYmin	+ ", "
                + "latLonBoxYmax=" + latLonBoxYmax	+ ", "
                + "bBoxXmin=" + bBoxXmin	+ ", "
                + "bBoxXmax=" + bBoxXmax	+ ", "
                + "bBoxYmin=" + bBoxYmin	+ ", "
                + "bBoxYmax=" + bBoxYmax	+ ", "
                + "attributionURL='" + attributionURL	+ "', "
                + "attributionTitle='" + attributionTitle+ "', "
                + "logoWidth=" + logoWidth	+ ", "
                + "logoHeight=" + logoHeight	+ ", "
                + "logoFormat='" + logoFormat	+ "', "
                + "logoURL='" + logoURL		+ "', "
                + "featureURL='" + featureURL	+ "', "
                + "resolution=" + resolution	+ ", "
                + "mapType='" + mapType	+ "' )";
        return( stringRepresentation );
    }
    
    
    /*
     * --- here test driver starts --------------------------------
     */
    
    /**
     * main() for standalone testing
     * @param String url database URL
     * @param String user database login name
     * @param String password database login password
     * @param String jdbc database JDBC connectivity
     */
    // --- test constants ---------------------------------
    // - value set 1:
    public static final String NAME1 = "name1";
    public static final String TITLE1 = "title1";
    public static final String SRS1 = "srs1";
    public static final String AUTHORITY1 = "authority1";
    public static final String LATLONXMIN1 = "1111.";
    public static final String LATLONXMAX1 = "2222.";
    public static final String LATLONYMIN1 = "3333.";
    public static final String LATLONYMAX1 = "4444.";
    public static final String BBOXXMIN1 = "111.";
    public static final String BBOXXMAX1 = "222.";
    public static final String BBOXYMIN1 = "333.";
    public static final String BBOXYMAX1 = "444.";
    public static final String ATTRIBUTIONURL1 = "attributionURL1";
    public static final String ATTRIBUTIONTITLE1 = "attributionTitle1";
    public static final int    LOGOWIDTH1 = 75;
    public static final int    LOGOHEIGHT1 = 125;
    public static final String LOGOFORMAT1 = "logoFormat1";
    public static final String LOGOURL1 = "logoURL1";
    public static final String FEATUREURL1 = "featureURL1";
    public static final double RESOLUTION1 = 1.111;
    public static final MapType MAPTYPE1 = MapType.GREY;
    
    // - value set 2:
    public static final String NAME2 = "name2";
    public static final String TITLE2 = "title2";
    public static final String SRS2 = "srs2";
    public static final String AUTHORITY2 = "authority2";
    public static final String LATLONXMIN2 = "11.";
    public static final String LATLONXMAX2 = "22.";
    public static final String LATLONYMIN2 = "33.";
    public static final String LATLONYMAX2 = "44.";
    public static final String BBOXXMIN2 = "1.";
    public static final String BBOXXMAX2 = "2.";
    public static final String BBOXYMIN2 = "3.";
    public static final String BBOXYMAX2 = "4.";
    public static final String ATTRIBUTIONURL2 = "attributionURL2";
    public static final String ATTRIBUTIONTITLE2 = "attributionTitle2";
    public static final int    LOGOWIDTH2 = 200;
    public static final int    LOGOHEIGHT2 = 100;
    public static final String LOGOFORMAT2 = "logoFormat2";
    public static final String LOGOURL2 = "logoURL2";
    public static final String FEATUREURL2 = "featureURL2";
    public static final double RESOLUTION2 = 2.222;
    public static final MapType MAPTYPE2 = MapType.RGB;
    
    // - value set 3:
    public static final String NAME3 = "name3";
    public static final String TITLE3 = "title3";
    public static final String SRS3 = "srs3";
    public static final String AUTHORITY3 = "authority3";
    public static final String LATLONXMIN3 = "11.";
    public static final String LATLONXMAX3 = "22.";
    public static final String LATLONYMIN3 = "33.";
    public static final String LATLONYMAX3 = "44.";
    public static final String BBOXXMIN3 = "1.";
    public static final String BBOXXMAX3 = "2.";
    public static final String BBOXYMIN3 = "3.";
    public static final String BBOXYMAX3 = "4.";
    public static final String ATTRIBUTIONURL3 = "attributionURL3";
    public static final String ATTRIBUTIONTITLE3 = "attributionTitle3";
    public static final int    LOGOWIDTH3 = 300;
    public static final int    LOGOHEIGHT3 = 100;
    public static final String LOGOFORMAT3 = "logoFormat3";
    public static final String LOGOURL3 = "logoURL3";
    public static final String FEATUREURL3 = "featureURL3";
    public static final double RESOLUTION3 = 3.333;
    public static final MapType MAPTYPE3 = MapType.DEM;
    
    public static void main( String[] args )
    {
        // --- start action -----------------------------------
        
        System.out.println( "START class Table_Layer component test" );
        
        // --- cmd line parameter check ------------------------
        if (args.length != 6)
        {
            System.err.println( "Usage: Table_Layer msgFile url user password jdbc debugLevel" );
            return;
        }
        String msgFile  = args[0];
        String url      = args[1];
        String usr      = args[2];
        String pwd      = args[3];
        String jdbc     = args[4];
        log.info( "Parameters: msgFile=" + msgFile + ", url=" + url + ", user=" + usr + ", password=" + pwd + ", jdbc=" + jdbc );
        
        try
        {
            // --- test preparation -------------------------------
            
            CommanderConnection c = new CommanderConnection( url, usr, pwd, jdbc );
            
            // --- test -------------------------------------------
            
            // - create a reference instance
            Table_Layer myLayer1 = new Table_Layer();
            int id1 = myLayer1.insertTuple( c, NAME1, TITLE1, SRS1, AUTHORITY1, LATLONXMIN1, LATLONXMAX1, LATLONYMIN1, LATLONYMAX1, BBOXXMIN1, BBOXXMAX1, BBOXYMIN1, BBOXYMAX1, ATTRIBUTIONURL1, ATTRIBUTIONTITLE1, LOGOWIDTH1, LOGOHEIGHT1, LOGOFORMAT1, LOGOURL1, FEATUREURL1, RESOLUTION1, MAPTYPE1 );
            System.out.println( "insert tuple #1 -> " + id1 );
            
            // - update reference instance
            myLayer1.updateTuple( c, id1, NAME2, TITLE2, SRS2, AUTHORITY2, LATLONXMIN2, LATLONXMAX2, LATLONYMIN2, LATLONYMAX2, BBOXXMIN2, BBOXXMAX2, BBOXYMIN2, BBOXYMAX2, ATTRIBUTIONURL2, ATTRIBUTIONTITLE2, LOGOWIDTH2, LOGOHEIGHT2, LOGOFORMAT2, LOGOURL2, FEATUREURL2, RESOLUTION2, MAPTYPE2 );
            
            // - fetch values via 2nd instance, check all values
            Table_Layer myLayer2 = new Table_Layer();
            myLayer2.fetchTupleById( c, id1 );
            if (myLayer2.getLayerId() != id1 )
                System.out.println( "Error: attribute updated != attributed fetched: " + Globals.TABLE_LAYER_LAYERID );
            if (! myLayer2.getName().equals( NAME2 ) )
                System.out.println( "Error: attribute updated != attributed fetched: " + Globals.TABLE_LAYER_NAME );
            if (! myLayer2.getTitle().equals( TITLE2 ) )
                System.out.println( "Error: attribute updated != attributed fetched: " + Globals.TABLE_LAYER_TITLE );
            if (! myLayer2.getSrs().equals( SRS2 ) )
                System.out.println( "Error: attribute updated != attributed fetched: " + Globals.TABLE_LAYER_SRS );
            if (! myLayer2.getAuthority().equals( AUTHORITY2 ) )
                System.out.println( "Error: attribute updated != attributed fetched: " + Globals.TABLE_LAYER_AUTHORITY );
            if (myLayer2.getLatLonBoxXmin() != LATLONXMIN2 )
                System.out.println( "Error: attribute updated != attributed fetched: " + Globals.TABLE_LAYER_LATLONXMIN );
            if (myLayer2.getLatLonBoxXmax() != LATLONXMAX2 )
                System.out.println( "Error: attribute updated != attributed fetched: " + Globals.TABLE_LAYER_LATLONXMAX );
            if (myLayer2.getLatLonBoxYmin() != LATLONYMIN2 )
                System.out.println( "Error: attribute updated != attributed fetched: " + Globals.TABLE_LAYER_LATLONYMIN );
            if (myLayer2.getLatLonBoxYmax() != LATLONYMAX2 )
                System.out.println( "Error: attribute updated != attributed fetched: " + Globals.TABLE_LAYER_LATLONYMAX );
            if (myLayer2.getBBoxXmin() != BBOXXMIN2 )
                System.out.println( "Error: attribute updated != attributed fetched: " + Globals.TABLE_LAYER_BBOXXMIN );
            if (myLayer2.getBBoxXmax() != BBOXXMAX2 )
                System.out.println( "Error: attribute updated != attributed fetched: " + Globals.TABLE_LAYER_BBOXXMAX );
            if (myLayer2.getBBoxYmin() != BBOXYMIN2 )
                System.out.println( "Error: attribute updated != attributed fetched: " + Globals.TABLE_LAYER_BBOXYMIN );
            if (myLayer2.getBBoxYmax() != BBOXYMAX2 )
                System.out.println( "Error: attribute updated != attributed fetched: " + Globals.TABLE_LAYER_BBOXYMAX );
            if (! myLayer2.getAttributionURL().equals( ATTRIBUTIONURL2 ) )
                System.out.println( "Error: attribute updated != attributed fetched: " + Globals.TABLE_LAYER_ATTRIBUTIONURL );
            if (! myLayer2.getAttributionTitle().equals( ATTRIBUTIONTITLE2 ) )
                System.out.println( "Error: attribute updated != attributed fetched: " + Globals.TABLE_LAYER_ATTRIBUTIONTITLE );
            if (myLayer2.getLogoWidth() != LOGOWIDTH2 )
                System.out.println( "Error: attribute updated != attributed fetched: " + Globals.TABLE_LAYER_LOGOWIDTH );
            if (myLayer2.getLogoHeight() != LOGOHEIGHT2 )
                System.out.println( "Error: attribute updated != attributed fetched: " + Globals.TABLE_LAYER_LOGOHEIGHT );
            if (! myLayer2.getLogoFormat().equals( LOGOFORMAT2 ) )
                System.out.println( "Error: attribute updated != attributed fetched: " + Globals.TABLE_LAYER_LOGOFORMAT );
            if (! myLayer2.getLogoURL().equals( LOGOURL2 ) )
                System.out.println( "Error: attribute updated != attributed fetched: " + Globals.TABLE_LAYER_LOGOURL );
            if (! myLayer2.getFeatureURL().equals( FEATUREURL2 ) )
                System.out.println( "Error: attribute updated != attributed fetched: " + Globals.TABLE_LAYER_FEATUREURL );
            if ( myLayer2.getResolution() != ( RESOLUTION2 ) )
                System.out.println( "Error: attribute updated != attributed fetched: " + Globals.TABLE_LAYER_RESOLUTION );
            if (! myLayer2.getMapType().equals( MAPTYPE2 ) )
                System.out.println( "Error: attribute updated != attributed fetched: " + Globals.TABLE_LAYER_MAPTYPE );
            
            // - delete instance 1 and check it's gone
            myLayer1.deleteTuple( c, id1 );
            try
            {
                myLayer2.fetchTupleById( c, id1 );
                System.out.println( "Error: deleted tuple still seems to exist." );
            }
            catch( Exception e )
            {
                System.out.println( "fetching tuple deleted caused exception, that's ok." );
            }
            
            // - create second instance
            int newId2 = myLayer2.insertTuple( c, NAME3, TITLE3, SRS3, AUTHORITY3, LATLONXMIN3, LATLONXMAX3, LATLONYMIN3, LATLONYMAX3, BBOXXMIN3, BBOXXMAX3, BBOXYMIN3, BBOXYMAX3, ATTRIBUTIONURL3, ATTRIBUTIONTITLE3, LOGOWIDTH3, LOGOHEIGHT3, LOGOFORMAT3, LOGOURL3, FEATUREURL3, RESOLUTION3, MAPTYPE3 );
            System.out.println( "insert tuple #2 -> " + newId2 );
            
            // --- test cleanup -----------------------------------
            c.abort();
            c.close();
            
        }
        catch (Exception e)
        {
            System.out.println( "Error: caught exception: " + e.getMessage() );
        }
        
        System.out.println( "END class Table_Layer component test" );
    }
}
