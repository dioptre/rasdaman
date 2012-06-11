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
 * provide access to the "PyramidLevel" table of rasgeo commander.
 * There are insert, update, and fetch statements.
 *
 * COMMENTS
 * - uses CommanderConnection for JDBC access.
 *
 *********************************************************** */

package petascope.wms.commander;

import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;


/**
 * This class accomplishes tuple-at-a-time access to the PyramidLevel table,
 * encapsulating SQL statements.
 **/
public class Table_PyramidLevels
{
    private static Logger log = LoggerFactory.getLogger(Table_PyramidLevels.class);
            
    /**
     * string & int result for invalid call of getter method
     * (we tentatively use null here to cause problems elsewhere when working with such an invalid result)
     */
    private static final String UNDEF_STRING = null;
    private static final int    UNDEF_INT    = -1;
    private static final double  UNDEF_FLOAT    = (double)-1;
    
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
    private int levelId = UNDEF_INT;
    private int layerId = UNDEF_INT;
    private String collectionName = UNDEF_STRING;
    private double scaleFactor = UNDEF_FLOAT;
    
    /**
     * constructor
     * @return CommanderConnection
     **/
    public Table_PyramidLevels()
    {
        log.info( "Table_PyramidLevels::Table_PyramidLevel()" );
        tupleValid = false;
        log.info( "Table_PyramidLevels::Table_PyramidLevel()" );
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
    public int insertTuple( CommanderConnection conn, int layerId, String collectionName, double scaleFactor )
            throws SQLException, ConnectionFailedException, InvalidInputException
    {
        log.info( "Table_PyramidLevels::insertTuple()" );
        
        // --- plausi checks ----------------------------------
        // - connection object
        if (conn == null)
        {
            log.info( "Table_PyramidLevels::insertTuple() -- Error: no database connection." );
            throw new ConnectionFailedException( "no database connection." );
        }
        
        // - null string
        if (collectionName==null)
        {
            log.info( "Table_PyramidLevels::insertTuple() -- Error: null parameter passed" );
            throw new InvalidInputException( "null parameter passed" );
        }
        
        // - illegal numeric values
        if (scaleFactor < 1.0)
        {
            log.info( "Table_PyramidLevels::insertTuple() -- Error: scale factor <1" );
            throw new InvalidInputException( "scale factor <1 passed" );
        }
        
        // --- action -----------------------------------------
        // do this until we find out how to get back a new value from INSERT
        String query = "select max(" + Globals.TABLE_PYRAMIDLEVELS_LEVELID + ")"
                + " from " + Globals.TABLE_PYRAMIDLEVELS;
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
            log.info( "Table_PyramidLevels::insertTuple() -- Error: cannot get new PyramidLevels id" );
            throw new InvalidInputException( "Error: cannot get new PyramidLevels id" );
        }
        // make VERY sure that attribute names and values are listed in the same sequence!
        query = "insert into " + Globals.TABLE_PYRAMIDLEVELS + " ( "
                + Globals.TABLE_PYRAMIDLEVELS_LEVELID + ", "
                + Globals.TABLE_PYRAMIDLEVELS_LAYERID + ", "
                + Globals.TABLE_PYRAMIDLEVELS_COLLNAME + ", "
                + Globals.TABLE_PYRAMIDLEVELS_SCALEFACTOR + " )"
                + " values( "
                + newId + ", "
                + layerId + ", "
                + "'" + collectionName + "', "
                + scaleFactor + " )";
        log.debug( "want to execute: " + query );
        stmt.executeUpdate( query );
        
        log.info( "Table_PyramidLevels::insertTuple() -- "  + newId );
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
    public void updateTuple( CommanderConnection conn, int levelId, int layerId, String collectionName, double scaleFactor )
            throws SQLException, ConnectionFailedException, InvalidInputException
    {
        log.info( "Table_PyramidLevels::updateTuple()" );
        
        // --- plausi checks ----------------------------------
        // - connection object
        if (conn == null)
        {
            log.info( "Table_PyramidLevels::updateTuple() -- Error: no database connection." );
            throw new ConnectionFailedException( "no database connection." );
        }
        // - null string -> ""
        if (collectionName==null)
            collectionName = "";
        
        // - negative values
        if (scaleFactor < 1)
        {
            log.info( "Table_PyramidLevels::updateTuple() -- Error: scale factor <1" );
            throw new InvalidInputException( "scale factor <1 passed" );
        }
        // --- action -----------------------------------------
        String query = "update " + Globals.TABLE_PYRAMIDLEVELS + " set "
                + Globals.TABLE_PYRAMIDLEVELS_LAYERID + " = " + layerId + ", "
                + Globals.TABLE_PYRAMIDLEVELS_COLLNAME + " = '" + collectionName + "', "
                + Globals.TABLE_PYRAMIDLEVELS_SCALEFACTOR + " = " + scaleFactor
                + " where " + Globals.TABLE_PYRAMIDLEVELS_LEVELID + " = " + levelId;
        log.debug( "want to execute: " + query );
        Statement stmt = conn.createStatement();
        stmt.executeUpdate( query );
        
        log.info( "Table_PyramidLevels::updateTuple()" );
    }
    
    /**
     * delete tuple given by its (unique) id
     * precondition:
     *	JDBC connection opened
     * @param conn		JDBC connection
     * @param layerId
     * @param levelId
     * @throws SQLException
     **/
    public void deleteTuple( CommanderConnection conn, int layerId, int levelId )
            throws SQLException, ConnectionFailedException
    {
        log.info( "Table_PyramidLevels::deleteTuple()" );
        
        // --- plausi checks ----------------------------------
        // - connection object
        if (conn == null)
        {
            log.info( "Table_PyramidLevels::deleteTuple() -- Error: no database connection." );
            throw new ConnectionFailedException( "no database connection." );
        }
        
        // --- action -----------------------------------------
        String query = "delete from " + Globals.TABLE_PYRAMIDLEVELS
                + " where " + Globals.TABLE_PYRAMIDLEVELS_LAYERID + " = " + layerId
                + "   and " + Globals.TABLE_PYRAMIDLEVELS_LEVELID + " = " + levelId;
        log.debug( "want to execute: " + query );
        Statement stmt = conn.createStatement();
        stmt.executeUpdate( query );
        
        log.info( "Table_PyramidLevels::deleteTuple() -- ok" );
    }
    
    /**
     * delete tuples given by their layer id
     * precondition:
     *	JDBC connection opened
     * @param conn		JDBC connection
     * @param layerId
     * @throws SQLException
     **/
    public void deleteTuplesByLayerId( CommanderConnection conn, int layerId )
            throws SQLException, ConnectionFailedException
    {
        log.info( "Table_PyramidLevels::deleteTuplesByLayerId()" );
        
        // --- plausi checks ----------------------------------
        // - connection object
        if (conn == null)
        {
            log.info( "Table_PyramidLevels::deleteTuplesByLayerId() -- Error: no database connection." );
            throw new ConnectionFailedException( "no database connection." );
        }
        
        // --- action -----------------------------------------
        String query = "delete from " + Globals.TABLE_PYRAMIDLEVELS
                + " where " + Globals.TABLE_PYRAMIDLEVELS_LAYERID + " = " + layerId;
        log.debug( "want to execute: " + query );
        Statement stmt = conn.createStatement();
        stmt.executeUpdate( query );
        
        log.info( "Table_PyramidLevels::deleteTuplesByLayerId() -- ok" );
    }
    
    /**
     * delete tuples given by their level id
     * precondition:
     *	JDBC connection opened
     * @param conn		JDBC connection
     * @param levelId
     * @throws SQLException
     **/
    public void deleteTuplesByLevelId( CommanderConnection conn, int levelId )
            throws SQLException, ConnectionFailedException
    {
        log.info( "Table_PyramidLevels::deleteTuplesByLevelId()" );
        
        // --- plausi checks ----------------------------------
        // - connection object
        if (conn == null)
        {
            log.info( "Table_PyramidLevels::deleteTuplesByLevelId() -- Error: no database connection." );
            throw new ConnectionFailedException( "no database connection." );
        }
        
        // --- action -----------------------------------------
        String query = "delete from " + Globals.TABLE_PYRAMIDLEVELS
                + " where " + Globals.TABLE_PYRAMIDLEVELS_LEVELID + " = " + levelId;
        Statement stmt = conn.createStatement();
        stmt.executeUpdate( query );
        
        log.info( "Table_PyramidLevels::deleteTuplesByLevelId() -- ok" );
    }
    
    /**
     * fetch one tuple by its (unique) id (sorted by scale factor), keep in local result set
     * precondition:
     *	JDBC connection opened
     * @param conn		JDBC connection
     * @param id		service tuple id
     * @throws SQLException
     **/
    public void fetchTupleById( CommanderConnection conn, int lId ) throws SQLException
    {
        log.info( "Table_PyramidLevels::fetchTupleById( conn, " + lId + " )" );
        
        // execute query
        String query = "select "
                + Globals.TABLE_PYRAMIDLEVELS_LEVELID + ", "
                + Globals.TABLE_PYRAMIDLEVELS_LAYERID + ", "
                + Globals.TABLE_PYRAMIDLEVELS_COLLNAME + ", "
                + Globals.TABLE_PYRAMIDLEVELS_SCALEFACTOR
                + " from " + Globals.TABLE_PYRAMIDLEVELS
                + " where " + Globals.TABLE_PYRAMIDLEVELS_LEVELID + " = " + lId;
        log.debug( "want to execute: " + query );
        Statement stmt = conn.createStatement();
        resultSet = stmt.executeQuery( query );
        
        log.info( "Table_PyramidLevels::fetchTupleById()" );
    }
    
    
    /**
     * fetch all tuples from table, store result locally as a JDBC ResultList
     * precondition:
     *	JDBC connection opened
     * @param conn		JDBC connection
     * @throws SQLException
     **/
    /* NEEDED? ADAPT!
     * private void fetchAllTuples( CommanderConnection conn )
     * throws SQLException
     * {
     * log.info( "Table_PyramidLevels::fetchAllTuples()" );
     * 
     * // execute query
     * String query = "select "
     * + Globals.TABLE_STYLE_CONNECTIONID + ", "
     * + Globals.TABLE_STYLE_SERVER + ", "
     * + Globals.TABLE_STYLE_PORT + ", "
     * + Globals.TABLE_STYLE_USER + ", "
     * + Globals.TABLE_STYLE_PASSWD + ", "
     * + Globals.TABLE_STYLE_DATABASE + ", "
     * + Globals.TABLE_STYLE_SERVERTIMEOUT + ", "
     * + Globals.TABLE_STYLE_SERVERCONNECTIONSPRECLAIMED + ", "
     * + Globals.TABLE_STYLE_SERVERCONNECTIONSONDEMAND
     * + " from " + Globals.TABLE_STYLE;
     * log.debug( "want to execute: " + query );
     * Statement stmt = conn.createStatement();
     * stmt.setFetchSize(0);			// Turn cursor off
     * resultSet = stmt.executeQuery( query );
     * stmt.close();
     * 
     * log.info( "Table_PyramidLevels::fetchAllTuples()" );
     * return;
     * }
     */
    /**
     * fetch all styles pertaining to a particular layer, store result locally as a JDBC ResultList
     * precondition:
     *	JDBC connection opened
     * @param conn		JDBC connection
     * @throws SQLException
     **/
    public void fetchTuplesByLayerId( CommanderConnection conn, int lId ) throws SQLException
    {
        log.info( "Table_PyramidLevels::fetchTuplesByLayerId( conn, " + lId + " )" );
        
        // execute query
        String query = "select "
                + Globals.TABLE_PYRAMIDLEVELS_LEVELID + ", "
                + Globals.TABLE_PYRAMIDLEVELS_LAYERID + ", "
                + Globals.TABLE_PYRAMIDLEVELS_COLLNAME + ", "
                + Globals.TABLE_PYRAMIDLEVELS_SCALEFACTOR
                + " from " + Globals.TABLE_PYRAMIDLEVELS
                + " where " + Globals.TABLE_PYRAMIDLEVELS_LAYERID + " = " + lId
                + " order by " + Globals.TABLE_PYRAMIDLEVELS_SCALEFACTOR;
        log.debug( "want to execute: " + query );
        Statement stmt = conn.createStatement();
        stmt.setFetchSize(0);			// Turn cursor off
        resultSet = stmt.executeQuery( query );
        
        log.info( "Table_PyramidLevels::fetchTuplesByLayerId()"  );
    }
    
    
    /**
     * cursor over result set of previous fetchAllTuples()
     * if successful, the tuple variables are set with the current fetch values
     */
    public boolean next()
            throws SQLException
    {
        log.info( "Table_PyramidLevels::next()" );
        
        boolean moreAvailable = resultSet.next();
        
        if (moreAvailable)
        {
            levelId		= resultSet.getInt( Globals.TABLE_PYRAMIDLEVELS_LEVELID );
            layerId		= resultSet.getInt( Globals.TABLE_PYRAMIDLEVELS_LAYERID );
            collectionName	= resultSet.getString( Globals.TABLE_PYRAMIDLEVELS_COLLNAME );
            scaleFactor	= resultSet.getFloat( Globals.TABLE_PYRAMIDLEVELS_SCALEFACTOR );
            
            tupleValid = true;
        }
        else
        {
            
            tupleValid = false;
            close();
        }
        
        log.info( "Table_PyramidLevels::next() -> " + moreAvailable );
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
        log.info( "Table_PyramidLevels::close()" );
        
        resultSet.getStatement().close();
        resultSet.close();
        resultSet = null;
        
        tupleValid = false;
        
        log.info( "Table_PyramidLevels::close()" );
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
    
    public int getLevelId()
    {
        return ( tupleValid ?  levelId:  UNDEF_INT );
    }
    public int getLayerId()
    {
        return ( tupleValid ?  layerId:  UNDEF_INT );
    }
    public String getCollectionName()
    {
        return ( tupleValid ?  collectionName:  UNDEF_STRING );
    }
    public double getScaleFactor()
    {
        return ( tupleValid ?  scaleFactor:  UNDEF_FLOAT );
    }
    
    /**
     * Method toString
     * @return String
     */
    public String toString()
    {
        String stringRepresentation = "PyramidLevel( "
                + "tupleValid='" + tupleValid + "'; "
                + "levelId=" + levelId + ", "
                + "layerId=" + layerId + ", "
                + "collectionName=" + collectionName + ", "
                + "scaleFactor=" + scaleFactor + " )";
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
    // - layer set:
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
    public static final MapType MAPTYPE1 = MapType.RGB;
    
    // - value set 1:
    public static final int    LEVELID1 = 42;
    public static final String COLLNAME1 = "collectionName1";
    public static final double SCALEFACTOR1 = 2.;
    
    // - value set 2:
    public static final int    LEVELID2 = 48;
    public static final String COLLNAME2 = "collectionName2";
    public static final double SCALEFACTOR2 = 4.;
    
    // - value set 3:
    public static final int    LEVELID3 = 96;
    public static final String COLLNAME3 = "collectionName3";
    public static final double SCALEFACTOR3 = 5.;
    
    public static void main( String[] args )
    {
        // --- start action -----------------------------------
        
        System.out.println( "START class Table_PyramidLevel component test" );
        
        // --- cmd line parameter check ------------------------
        if (args.length != 6)
        {
            System.err.println( "Usage: Table_PyramidLevel msgFile url user password jdbc debugLevel" );
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
            
            // - create a reference instance
            Table_Layer myLayer1 = new Table_Layer();
            int lay1 = myLayer1.insertTuple( c, NAME1, TITLE1, SRS1, AUTHORITY1, LATLONXMIN1, LATLONXMAX1, LATLONYMIN1, LATLONYMAX1, BBOXXMIN1, BBOXXMAX1, BBOXYMIN1, BBOXYMAX1, ATTRIBUTIONURL1, ATTRIBUTIONTITLE1, LOGOWIDTH1, LOGOHEIGHT1, LOGOFORMAT1, LOGOURL1, FEATUREURL1, RESOLUTION1, MAPTYPE1 );
            System.out.println( "insert layer -> " + lay1 );
            
            // --- test -------------------------------------------
            
            // - create a reference instance
            Table_PyramidLevels myPLevel1 = new Table_PyramidLevels();
            int id1 = myPLevel1.insertTuple( c, lay1, COLLNAME1, SCALEFACTOR1 );
            System.out.println( "insert tuple #1 -> " + id1 );
            
            // - update reference instance
            myPLevel1.updateTuple( c, LEVELID2, lay1, COLLNAME2, SCALEFACTOR2 );
            
            // - fetch values via 2nd instance, check all values
            Table_PyramidLevels myPLevel2 = new Table_PyramidLevels();
            myPLevel2.fetchTupleById( c, id1 );
            if ( myPLevel2.getLevelId() != LEVELID2 )
                System.out.println( "Error: attribute updated != attributed fetched: " + Globals.TABLE_PYRAMIDLEVELS_LEVELID );
            if (! myPLevel2.getCollectionName().equals( COLLNAME2 ) )
                System.out.println( "Error: attribute updated != attributed fetched: " + Globals.TABLE_PYRAMIDLEVELS_COLLNAME );
            if ( myPLevel2.getScaleFactor() != SCALEFACTOR2 )
                System.out.println( "Error: attribute updated != attributed fetched: " + Globals.TABLE_PYRAMIDLEVELS_SCALEFACTOR );
            
            // - create second instance
            int id2 = myPLevel2.insertTuple( c, lay1, COLLNAME3, SCALEFACTOR3 );
            System.out.println( "insert tuple #2 -> " + id2 );
            
            // - fetch all levels per layer
            Table_PyramidLevels myPLevel3 = new Table_PyramidLevels();
            myPLevel3.fetchTuplesByLayerId( c, lay1 );
            while (myPLevel3.next())
            {
                System.out.println( "  element: " + myPLevel3.toString() );
            }
            
            // - delete instance 1 and check it's gone
            myPLevel1.deleteTuple( c, id1, lay1 );
            try
            {
                myPLevel2.fetchTupleById( c, id1 );
                System.out.println( "Error: deleted tuple still seems to exist." );
            }
            catch( Exception e )
            {
                System.out.println( "fetching tuple deleted caused exception, that's ok." );
            }
            
            // --- test cleanup -----------------------------------
            c.abort();
            c.close();
            
        }
        catch (Exception e)
        {
            System.out.println( "Error: caught exception: " + e.getMessage() );
        }
        
        System.out.println( "END class Table_PyramidLevel component test" );
    }
}
