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
 * provide access to the "Style" table of rasgeo commander.
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
 * This class accomplishes tuple-at-a-time access to the Style table,
 * encapsulating SQL statements.
 **/
public class Table_Style
{
    private static Logger log = LoggerFactory.getLogger(Table_Style.class);
    /**
     * string & int result for invalid call of getter method
     * (we tentatively use null here to cause problems elsewhere when working with such an invalid result)
     */
    private static final String UNDEF_STRING = null;
    private static final int    UNDEF_INT    = -1;
    
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
    private int styleId = UNDEF_INT;
    private int layerId = UNDEF_INT;
    private String name = UNDEF_STRING;
    private String title = UNDEF_STRING;
    private String abstractText = UNDEF_STRING;
    private int legendWidth = UNDEF_INT;
    private int legendHeight = UNDEF_INT;
    private String legendURL = UNDEF_STRING;
    private String sheetURL = UNDEF_STRING;
    private String rasqlOp = UNDEF_STRING;
    
    /**
     * constructor
     * @return CommanderConnection
     **/
    public Table_Style()
    {
        log.info( "Table_Style::Table_Style()" );
        tupleValid = false;
        log.info( "Table_Style::Table_Style()" );
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
    public int insertTuple( CommanderConnection conn, int layerId, String name, String title, String abstractText, int legendWidth, int legendHeight, String legendURL, String sheetURL, String rasqlOp )
            
            throws SQLException, ConnectionFailedException, InvalidInputException
    {
        log.info( "Table_Style::insertTuple()" );
        
        // --- plausi checks ----------------------------------
        // - connection object
        if (conn == null)
        {
            log.info( "Table_Style::insertTuple() -- Error: no database connection." );
            throw new ConnectionFailedException( "no database connection." );
        }
        
        // - null string
        if (name==null || title==null || abstractText==null || legendURL==null || sheetURL==null || rasqlOp==null )
        {
            log.info( "Table_Style::insertTuple() -- Error: null parameter passed" );
            throw new InvalidInputException( "null parameter passed" );
        }
        
        // - illegal numeric values
        if (legendWidth < 0 || legendHeight < 0)
        {
            log.info( "Table_Style::insertTuple() -- Error: negative integer values passed" );
            throw new InvalidInputException( "negative integer values passed" );
        }
        
        // --- action -----------------------------------------
        // do this until we find out how to get back a new value from INSERT
        String query = "select max(" + Globals.TABLE_STYLES_STYLEID + ")"
                + " from " + Globals.TABLE_STYLES;
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
            log.info( "Table_Style::insertTuple() -- Error: cannot get new Styles id" );
            throw new InvalidInputException( "Error: cannot get new Styles id" );
        }
        // make VERY sure that attribute names and values are listed in the same sequence!
        query = "insert into " + Globals.TABLE_STYLES + " ( "
                + Globals.TABLE_STYLES_STYLEID + ", "
                + Globals.TABLE_STYLES_LAYERID + ", "
                + Globals.TABLE_STYLES_NAME + ", "
                + Globals.TABLE_STYLES_TITLE + ", "
                + Globals.TABLE_STYLES_ABSTRACT + ", "
                + Globals.TABLE_STYLES_LEGENDWIDTH + ", "
                + Globals.TABLE_STYLES_LEGENDHEIGHT + ", "
                + Globals.TABLE_STYLES_LEGENDURL + ", "
                + Globals.TABLE_STYLES_SHEETURL + ", "
                + Globals.TABLE_STYLES_RASQLOP + " )"
                + " values( "
                + newId + ", "
                + layerId + ", "
                + "'" + name + "', "
                + "'" + title + "', "
                + "'" + abstractText + "', "
                + legendWidth + ", "
                + legendHeight + ", "
                + "'" + legendURL + "', "
                + "'" + sheetURL + "', "
                + "'" + rasqlOp + "' )";
        log.debug( "want to execute: " + query );
        stmt = conn.createStatement();
        stmt.executeUpdate( query );
        
        log.info( "Table_Style::insertTuple() -> "  + newId );
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
    public void updateTuple( CommanderConnection conn, int styleId, int layerId, String name, String title, String abstractText, int legendWidth, int legendHeight, String legendURL, String sheetURL, String rasqlOp )
            throws SQLException, ConnectionFailedException
    {
        log.info( "Table_Style::updateTuple( id=" + styleId + " )" );
        
        // --- plausi checks ----------------------------------
        // - connection object
        if (conn == null)
        {
            log.info( "Table_Style::updateTuple() -- Error: no database connection." );
            throw new ConnectionFailedException( "no database connection." );
        }
        // - null string -> ""
        if (name==null)
            name = "";
        if (title==null)
            title = "";
        if (abstractText==null)
            abstractText = "";
        if (legendURL==null)
            legendURL = "";
        if (sheetURL==null)
            sheetURL = "";
        if (rasqlOp==null)
            rasqlOp = "";
        
        // - negative values
        if (legendWidth < 0
                || legendHeight < 0)
        {
            log.error( "Error: negative number input." );
            log.info( "Table_Style::updateTuple()" );
            return;
        }
        // --- action -----------------------------------------
        String query = "update " + Globals.TABLE_STYLES + " set "
                + Globals.TABLE_STYLES_STYLEID + " = " + styleId + ", "
                + Globals.TABLE_STYLES_LAYERID + " = " + layerId + ", "
                + Globals.TABLE_STYLES_NAME + " = '" + name + "', "
                + Globals.TABLE_STYLES_TITLE + " = '" + title + "', "
                + Globals.TABLE_STYLES_ABSTRACT + " = '" + abstractText + "', "
                + Globals.TABLE_STYLES_LEGENDWIDTH + " = " + legendWidth + ", "
                + Globals.TABLE_STYLES_LEGENDHEIGHT + " = " + legendHeight + ", "
                + Globals.TABLE_STYLES_LEGENDURL + " = '" + legendURL + "', "
                + Globals.TABLE_STYLES_SHEETURL + " = '" + sheetURL + "', "
                + Globals.TABLE_STYLES_RASQLOP + " = '" + rasqlOp + "'"
                + " where " + Globals.TABLE_STYLES_STYLEID + " = " + styleId
                + "   and " + Globals.TABLE_STYLES_LAYERID + " = " + layerId;
        log.debug( "want to execute: " + query );
        Statement stmt = conn.createStatement();
        stmt.executeUpdate( query );
        
        log.info( "Table_Style::updateTuple()" );
    }
    
    /**
     * delete tuple given by its (unique) id
     * precondition:
     *	JDBC connection opened
     * @param conn		JDBC connection
     * @param styleId
     * @param layerId
     * @throws SQLException
     **/
    public void deleteTuple( CommanderConnection conn, int styleId, int layerId )
            throws SQLException, ConnectionFailedException
    {
        log.info( "Table_Style::deleteTuple()" );
        
        // --- plausi checks ----------------------------------
        // - connection object
        if (conn == null)
        {
            log.info( "Table_Style::deleteTuple() -- Error: no database connection." );
            throw new ConnectionFailedException( "no database connection." );
        }
        
        // --- action -----------------------------------------
        String query = "delete from " + Globals.TABLE_STYLES
                + " where " + Globals.TABLE_STYLES_STYLEID + " = " + styleId
                + "   and " + Globals.TABLE_STYLES_LAYERID + " = " + layerId;
        log.debug( "want to execute: " + query );
        Statement stmt = conn.createStatement();
        stmt.executeUpdate( query );
        
        log.info( "Table_Style::deleteTuple() -- ok" );
    }
    
    /**
     * delete tuple set given by its layer id
     * precondition:
     *	JDBC connection opened
     * @param conn		JDBC connection
     * @param layerId	layer identifier
     * @throws SQLException
     **/
    public void deleteTuplesByLayerId( CommanderConnection conn, int layerId )
            throws SQLException, ConnectionFailedException
    {
        log.info( "Table_Style::deleteTuplesByLayerId()" );
        
        // --- plausi checks ----------------------------------
        // - connection object
        if (conn == null)
        {
            log.info( "Table_Style::deleteTuplesByLayerId() -- Error: no database connection." );
            throw new ConnectionFailedException( "no database connection." );
        }
        
        // --- action -----------------------------------------
        String query = "delete from " + Globals.TABLE_STYLES
                + " where " + Globals.TABLE_STYLES_LAYERID + " = " + layerId;
        log.debug( "want to execute: " + query );
        Statement stmt = conn.createStatement();
        stmt.executeUpdate( query );
        
        log.info( "Table_Style::deleteTuplesByLayerId() -- ok" );
    }
    
    /**
     * fetch one tuple by its (unique) id, keep in local result set
     * precondition:
     *	JDBC connection opened
     * @param conn		JDBC connection
     * @param id		service tuple id
     * @throws SQLException
     **/
    private void fetchTupleById( CommanderConnection conn, int id ) throws SQLException
    {
        log.info( "Table_Style::fetchTupleById()" );
        
        // execute query
        String query = "select "
                + Globals.TABLE_STYLES_STYLEID + ", "
                + Globals.TABLE_STYLES_LAYERID + ", "
                + Globals.TABLE_STYLES_NAME + ", "
                + Globals.TABLE_STYLES_TITLE + ", "
                + Globals.TABLE_STYLES_ABSTRACT + ", "
                + Globals.TABLE_STYLES_LEGENDWIDTH + ", "
                + Globals.TABLE_STYLES_LEGENDHEIGHT + ", "
                + Globals.TABLE_STYLES_LEGENDURL + ", "
                + Globals.TABLE_STYLES_SHEETURL + ", "
                + Globals.TABLE_STYLES_RASQLOP
                + " from " + Globals.TABLE_STYLES
                + " where " + Globals.TABLE_STYLES_STYLEID + " = " + id;
        log.debug( "want to execute: " + query );
        Statement stmt = conn.createStatement();
        resultSet = stmt.executeQuery( query );
        
        log.info( "Table_Style::fetchTupleById()" );
    }
    
    /**
     * fetch all tuples from table, store result locally as a JDBC ResultList
     * precondition:
     *	JDBC connection opened
     * @param conn		JDBC connection
     * @throws SQLException
     **/
    public void fetchAllTuples( CommanderConnection conn ) throws SQLException
    {
        log.info( "Table_Style::fetchAllTuples()" );
        
        // execute query
        String query = "select "
                + Globals.TABLE_STYLES_STYLEID + ", "
                + Globals.TABLE_STYLES_LAYERID + ", "
                + Globals.TABLE_STYLES_NAME + ", "
                + Globals.TABLE_STYLES_TITLE + ", "
                + Globals.TABLE_STYLES_ABSTRACT + ", "
                + Globals.TABLE_STYLES_LEGENDWIDTH + ", "
                + Globals.TABLE_STYLES_LEGENDHEIGHT + ", "
                + Globals.TABLE_STYLES_LEGENDURL + ", "
                + Globals.TABLE_STYLES_SHEETURL + ", "
                + Globals.TABLE_STYLES_RASQLOP
                + " from " + Globals.TABLE_STYLES;
        log.debug( "want to execute: " + query );
        Statement stmt = conn.createStatement();
        stmt.setFetchSize(0);			// Turn cursor off
        resultSet = stmt.executeQuery( query );
        
        log.info( "Table_Style::fetchAllTuples()" );
    }
    
    /**
     * fetch all styles pertaining to a particular layer, store result locally as a JDBC ResultList
     * precondition:
     *	JDBC connection opened
     * @param conn		JDBC connection
     * @throws SQLException
     **/
    public void fetchTuplesByLayerId( CommanderConnection conn, int lId )
            throws SQLException
    {
        log.info( "Table_Style::fetchTuplesByLayerId()" );
        
        // execute query
        String query = "select "
                + Globals.TABLE_STYLES_STYLEID + ", "
                + Globals.TABLE_STYLES_LAYERID + ", "
                + Globals.TABLE_STYLES_NAME + ", "
                + Globals.TABLE_STYLES_TITLE + ", "
                + Globals.TABLE_STYLES_ABSTRACT + ", "
                + Globals.TABLE_STYLES_LEGENDWIDTH + ", "
                + Globals.TABLE_STYLES_LEGENDHEIGHT + ", "
                + Globals.TABLE_STYLES_LEGENDURL + ", "
                + Globals.TABLE_STYLES_SHEETURL + ", "
                + Globals.TABLE_STYLES_RASQLOP
                + " from " + Globals.TABLE_STYLES
                + " where " + Globals.TABLE_STYLES_LAYERID + " = " + lId
                + " order by " + Globals.TABLE_STYLES_NAME;
        log.debug( "want to execute: " + query );
        Statement stmt = conn.createStatement();
        stmt.setFetchSize(0);			// Turn cursor off
        resultSet = stmt.executeQuery( query );
        
        log.info( "Table_Style::fetchTuplesByLayerId()" );
    }
    
    /**
     * cursor over result set of previous fetchAllTuples()
     * if successful, the tuple variables are set with the current fetch values
     */
    public boolean next()
            throws SQLException
    {
        log.debug("Enter next()");
        boolean moreAvailable = resultSet.next();
        
        if (moreAvailable)
        {
            styleId		= resultSet.getInt( Globals.TABLE_STYLES_STYLEID );
            layerId		= resultSet.getInt( Globals.TABLE_STYLES_LAYERID );
            name		= resultSet.getString( Globals.TABLE_STYLES_NAME );
            title		= resultSet.getString( Globals.TABLE_STYLES_TITLE );
            abstractText	= resultSet.getString( Globals.TABLE_STYLES_ABSTRACT );
            legendWidth	= resultSet.getInt( Globals.TABLE_STYLES_LEGENDWIDTH );
            legendHeight	= resultSet.getInt( Globals.TABLE_STYLES_LEGENDHEIGHT );
            legendURL	= resultSet.getString( Globals.TABLE_STYLES_LEGENDURL );
            sheetURL	= resultSet.getString( Globals.TABLE_STYLES_SHEETURL );
            rasqlOp		= resultSet.getString( Globals.TABLE_STYLES_RASQLOP );
            log.debug("Tuple valid!");
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
    
    public int getStyleId()
    {
        return( tupleValid ? styleId : UNDEF_INT );
    }
    public int getLayerId()
    {
        return( tupleValid ? layerId : UNDEF_INT );
    }
    public String getName()
    {
        return( tupleValid ? name : UNDEF_STRING );
    }
    public String getTitle()
    {
        return( tupleValid ? title : UNDEF_STRING );
    }
    public String getAbstract()
    {
        return( tupleValid ? abstractText : UNDEF_STRING );
    }
    public int getLegendWidth()
    {
        return( tupleValid ? legendWidth : UNDEF_INT );
    }
    public int getLegendHeight()
    {
        return( tupleValid ? legendHeight : UNDEF_INT );
    }
    public String getLegendURL()
    {
        return( tupleValid ? legendURL : UNDEF_STRING );
    }
    public String getSheetURL()
    {
        return( tupleValid ? sheetURL : UNDEF_STRING );
    }
    public String getRasqlOp()
    {
        return( tupleValid ? rasqlOp : UNDEF_STRING );
    }
    
    /**
     * Method toString
     * @return String
     */
    public String toString()
    {
        String stringRepresentation = "Style( "
                + "tupleValid='" + tupleValid + "'; "
                + "styleId=" + styleId + ", "
                + "layerId=" + layerId + ", "
                + "name=" + name + ", "
                + "title=" + title + ", "
                + "abstract=" + abstractText + ", "
                + "legendWidth=" + legendWidth + ", "
                + "legendHeight=" + legendHeight + ", "
                + "legendURL=" + legendURL + ", "
                + "sheetURL=" + sheetURL + ", "
                + "rasqlOp=" + rasqlOp + " )";
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
    // - layer value set 1:
    public static final String LNAME1 = "name1";
    public static final String LTITLE1 = "title1";
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
    
    // - value set 1:
    public static final String STYLEID1 = "styleId1";
    public static final String LAYERID1 = "layerId1";
    public static final String NAME1 = "name1";
    public static final String TITLE1 = "title1";
    public static final String ABSTRACT1 = "abstract1";
    public static final int    LEGENDWIDTH1 = 43;
    public static final int    LEGENDHEIGHT1 = 44;
    public static final String LEGENDURL1 = "legendURL1";
    public static final String SHEETURL1 = "sheetURL1";
    public static final String RASQLOP1 = "rasqlOp1";
    
    // - value set 2:
    public static final String STYLEID2 = "styleId2";
    public static final String LAYERID2 = "layerId2";
    public static final String NAME2 = "name2";
    public static final String TITLE2 = "title2";
    public static final String ABSTRACT2 = "abstract2";
    public static final int    LEGENDWIDTH2 = 86;
    public static final int    LEGENDHEIGHT2 = 88;
    public static final String LEGENDURL2 = "legendURL2";
    public static final String SHEETURL2 = "sheetURL2";
    public static final String RASQLOP2 = "rasqlOp2";
    
    
    public static void main( String[] args )
    {
        // --- start action -----------------------------------
        
        System.out.println( "START class Table_Styles component test" );
        
        // --- cmd line parameter check ------------------------
        if (args.length != 6)
        {
            System.err.println( "Usage: Table_Styles msgFile url user password jdbc debugLevel" );
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
            
            Table_Layer myLayer1 = new Table_Layer();
            int lay1 = myLayer1.insertTuple( c, LNAME1, LTITLE1, SRS1, AUTHORITY1, LATLONXMIN1, LATLONXMAX1, LATLONYMIN1, LATLONYMAX1, BBOXXMIN1, BBOXXMAX1, BBOXYMIN1, BBOXYMAX1, ATTRIBUTIONURL1, ATTRIBUTIONTITLE1, LOGOWIDTH1, LOGOHEIGHT1, LOGOFORMAT1, LOGOURL1, FEATUREURL1, RESOLUTION1, MAPTYPE1 );
            System.out.println( "insert Layer -> " + lay1 );
            
            // --- test -------------------------------------------
            
            // - create a reference instance
            Table_Style myStyle1 = new Table_Style();
            int id1 = myStyle1.insertTuple( c, lay1, NAME1, TITLE1, ABSTRACT1, LEGENDWIDTH1, LEGENDHEIGHT1, LEGENDURL1, SHEETURL1, RASQLOP1 );
            
            System.out.println( "insert tuple #1 -> " + id1 );
            
            // - update reference instance
            myStyle1.updateTuple( c, id1, lay1, NAME2, TITLE2, ABSTRACT2, LEGENDWIDTH2, LEGENDHEIGHT2, LEGENDURL2, SHEETURL2, RASQLOP2 );
            
            // - fetch values via 2nd instance, check all values
            Table_Style myStyle2 = new Table_Style();
            myStyle2.fetchTupleById( c, id1 );
            if ( myStyle2.getStyleId() != id1 )
                System.out.println( "Error: atribute updated != attributed fetched: " + Globals.TABLE_STYLES_STYLEID );
            if ( myStyle2.getLayerId() != lay1 )
                System.out.println( "Error: atribute updated != attributed fetched: " + Globals.TABLE_STYLES_LAYERID );
            if (! myStyle2.getName().equals( NAME2 ) )
                System.out.println( "Error: atribute updated != attributed fetched: " + Globals.TABLE_STYLES_NAME );
            if (! myStyle2.getTitle().equals( TITLE2 ) )
                System.out.println( "Error: atribute updated != attributed fetched: " + Globals.TABLE_STYLES_TITLE );
            if (! myStyle2.getAbstract().equals( ABSTRACT2 ) )
                System.out.println( "Error: atribute updated != attributed fetched: " + Globals.TABLE_STYLES_ABSTRACT );
            if ( myStyle2.getLegendWidth() != LEGENDWIDTH2 )
                System.out.println( "Error: atribute updated != attributed fetched: " + Globals.TABLE_STYLES_LEGENDWIDTH );
            if ( myStyle2.getLegendHeight() != LEGENDHEIGHT2 )
                System.out.println( "Error: atribute updated != attributed fetched: " + Globals.TABLE_STYLES_LEGENDHEIGHT );
            if (! myStyle2.getLegendURL().equals( LEGENDURL2 ) )
                System.out.println( "Error: atribute updated != attributed fetched: " + Globals.TABLE_STYLES_LEGENDURL );
            if (! myStyle2.getSheetURL().equals( SHEETURL2 ) )
                System.out.println( "Error: atribute updated != attributed fetched: " + Globals.TABLE_STYLES_SHEETURL );
            if (! myStyle2.getRasqlOp().equals( RASQLOP2 ) )
                System.out.println( "Error: atribute updated != attributed fetched: " + Globals.TABLE_STYLES_RASQLOP );
            
            // - delete instance 1 and check it's gone
            myStyle1.deleteTuple( c, id1, lay1 );
            try
            {
                myStyle2.fetchTupleById( c, id1 );
                System.out.println( "Error: deleted tuple still seems to exist." );
            }
            catch( Exception e )
            {
                System.out.println( "fetching tuple deleted caused exception, that's ok." );
            }
            
            // - create second instance
            int st2 = myStyle2.insertTuple( c, lay1, NAME2, TITLE2, ABSTRACT2, LEGENDWIDTH2, LEGENDHEIGHT2, LEGENDURL2, SHEETURL2, RASQLOP2 );
            System.out.println( "insert tuple #2 -> " + st2 );
            
            // --- test cleanup -----------------------------------
            c.abort();
            c.close();
            
        }
        catch (Exception e)
        {
            System.out.println( "Error: caught exception: " + e.getMessage() );
        }
        
        System.out.println( "END class Table_Style component test" );
    }
}
