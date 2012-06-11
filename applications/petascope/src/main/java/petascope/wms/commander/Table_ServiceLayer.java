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
 * provide access to the "ServiceLayer" table of rasgeo commander.
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
 * This class accomplishes tuple-at-a-time access to the ServiceLayer table,
 * encapsulating SQL statements.
 **/
public class Table_ServiceLayer
{
    private static Logger log = LoggerFactory.getLogger(Table_ServiceLayer.class);
            
    /// maximum value allowed for layerseq paramter in the servicelayer table
    /// to avoid overflow (16-bit int -> should be somewhat below 2**15)
    private static final int MAX_LAYERSEQ = 32000;
    
    /**
     * string & int result for invalid call of getter method
     * (we tentatively use null here to cause problems elsewhere when working with such an invalid result)
     */
    private static final int UNDEF_INT = -1;
    
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
    private int serviceId = UNDEF_INT;
    private int layerId = UNDEF_INT;
    private int layerSeq = UNDEF_INT;
    
    /**
     * constructor
     * @return CommanderConnection
     **/
    public Table_ServiceLayer()
    {
        log.info( "Table_ServiceLayer::Table_ServiceLayer()" );
        tupleValid = false;
        log.info( "Table_ServiceLayer::Table_ServiceLayer()" );
    }
    
    /**
     * insert one tuple; instance's local attribute set is not modified.
     * precondition:
     *	JDBC connection opened
     *	all strings non-null
     * @param conn		JDBC connection
     * @param id		service tuple id
     * @throws SQLException
     **/
    public void insertTuple( CommanderConnection conn, int serviceId, int layerId, int layerSeq ) throws SQLException, ConnectionFailedException, InvalidInputException
    {
        log.info( "Table_ServiceLayer::insertTuple()" );
        
        // --- plausi checks ----------------------------------
        // - connection object
        if (conn == null)
        {
            log.info( "Table_ServiceLayer::insertTuple() -- Error: no database connection." );
            throw new ConnectionFailedException( "no database connection." );
        }
        
        // --- action -----------------------------------------
        // make VERY sure that attribute names and values are listed in the same sequence!
        String query = "insert into " + Globals.TABLE_SERVICELAYER + " ( "
                + Globals.TABLE_SERVICELAYER_SERVICEID + ", "
                + Globals.TABLE_SERVICELAYER_LAYERID + ", "
                + Globals.TABLE_SERVICELAYER_LAYERSEQ + ") "
                + " values( "
                + serviceId + ", "
                + layerId + ", "
                + layerSeq + ")";
        log.debug( "want to execute: " + query );
        Statement stmt = conn.createStatement();
        stmt.executeUpdate( query );
        
        log.info( "Table_ServiceLayer::insertTuple()" );
        return;
    }
    
    /**
     * update (service,layer) tuple with seq#
     * null strings will be set to ""
     * precondition:
     *	JDBC connection opened
     *	tuple with id exists
     * @param conn		JDBC connection
     * @param id		service tuple id
     * @throws SQLException
     **/
    public void updateTuple( CommanderConnection conn, int serviceId, int layerId, int layerSeq ) throws SQLException, ConnectionFailedException
    {
        log.info( "Table_ServiceLayer::updateTuple()" );
        
        // --- plausi checks ----------------------------------
        // - connection object
        if (conn == null)
        {
            log.info( "Table_ServiceLayer::updateTuple() -- Error: no database connection." );
            throw new ConnectionFailedException( "no database connection." );
        }
        
        // --- action -----------------------------------------
        String query = "update " + Globals.TABLE_SERVICELAYER + " set "
                + Globals.TABLE_SERVICELAYER_LAYERSEQ + " = " + layerSeq
                + " where " + Globals.TABLE_SERVICELAYER_SERVICEID + " = " + serviceId
                + "   and " + Globals.TABLE_SERVICELAYER_LAYERID + " = " + layerId;
        log.debug( "want to execute: " + query );
        Statement stmt = conn.createStatement();
        stmt.executeUpdate( query );
        
        log.info( "Table_ServiceLayer::updateTuple()" );
    }
    
    /**
     * delete tuple given by its (service,layer) id
     * precondition:
     *	JDBC connection opened
     * @param conn		JDBC connection
     * @param id		service tuple id
     * @throws SQLException
     **/
    public void deleteTuple( CommanderConnection conn, int serviceId, int layerId ) throws SQLException, ConnectionFailedException
    {
        log.info( "Table_ServiceLayer::deleteTuple()" );
        
        // --- plausi checks ----------------------------------
        // - connection object
        if (conn == null)
        {
            log.info( "Table_ServiceLayer::deleteTuple() -- Error: no database connection." );
            throw new ConnectionFailedException( "no database connection." );
        }
        
        // --- action -----------------------------------------
        String query = "delete from " + Globals.TABLE_SERVICELAYER
                + " where " + Globals.TABLE_SERVICELAYER_SERVICEID + " = " + serviceId
                + "   and " + Globals.TABLE_SERVICELAYER_LAYERID + " = " + layerId;
        log.debug( "want to execute: " + query );
        Statement stmt = conn.createStatement();
        stmt.executeUpdate( query );
        
        log.info( "Table_ServiceLayer::deleteTuple() -- ok" );
    }
    
    /**
     * delete tuple given by its service id
     * precondition:
     *	JDBC connection opened
     * @param conn		JDBC connection
     * @param serviceId
     * @throws SQLException
     **/
    public void deleteTuplesByServiceId( CommanderConnection conn, int serviceId ) throws SQLException, ConnectionFailedException
    {
        log.info( "Table_ServiceLayer::deleteTuplesByServiceId()" );
        
        // --- plausi checks ----------------------------------
        // - connection object
        if (conn == null)
        {
            log.info( "Table_ServiceLayer::deleteTuplesByServiceId() -- Error: no database connection." );
            throw new ConnectionFailedException( "no database connection." );
        }
        
        // --- action -----------------------------------------
        String query = "delete from " + Globals.TABLE_SERVICELAYER
                + " where " + Globals.TABLE_SERVICELAYER_SERVICEID + " = " + serviceId;
        log.debug( "want to execute: " + query );
        Statement stmt = conn.createStatement();
        stmt.executeUpdate( query );
        
        log.info( "Table_ServiceLayer::deleteTuplesByServiceId() -- ok" );
    }
    
    /**
     * delete tuple given by its layer id
     * precondition:
     *	JDBC connection opened
     * @param conn		JDBC connection
     * @param layerId
     * @throws SQLException
     **/
    public void deleteTuplesByLayerId( CommanderConnection conn, int layerId ) throws SQLException, ConnectionFailedException
    {
        log.info( "Table_ServiceLayer::deleteTuplesByLayerId()" );
        
        // --- plausi checks ----------------------------------
        // - connection object
        if (conn == null)
        {
            log.info( "Table_ServiceLayer::deleteTuplesByLayerId() -- Error: no database connection." );
            throw new ConnectionFailedException( "no database connection." );
        }
        
        // --- action -----------------------------------------
        String query = "delete from " + Globals.TABLE_SERVICELAYER
                + " where " + Globals.TABLE_SERVICELAYER_LAYERID + " = " + layerId;
        log.debug( "want to execute: " + query );
        Statement stmt = conn.createStatement();
        stmt.executeUpdate( query );
        
        log.info( "Table_ServiceLayer::deleteTuplesByLayerId() -- ok" );
    }
    
    /**
     * fetch one tuple by its (service,layer) id, keep in local result set
     * precondition:
     *	JDBC connection opened
     * @param conn		JDBC connection
     * @param id		service tuple id
     * @throws SQLException, InvalidInputException
     **/
    public void fetchTupleById( CommanderConnection conn, int serviceId, int layerId ) throws SQLException
    {
        log.info( "Table_ServiceLayer::fetchTupleById()" );
        
        // execute query
        String query = "select "
                + Globals.TABLE_SERVICELAYER_SERVICEID + ", "
                + Globals.TABLE_SERVICELAYER_LAYERID + ", "
                + Globals.TABLE_SERVICELAYER_LAYERSEQ
                + " from " + Globals.TABLE_SERVICELAYER
                + " where " + Globals.TABLE_SERVICELAYER_SERVICEID + " = " + serviceId
                + "   and " + Globals.TABLE_SERVICELAYER_LAYERID + " = " + layerId;
        log.debug( "want to execute: " + query );
        Statement stmt = conn.createStatement();
        resultSet = stmt.executeQuery( query );
        
        log.info( "Table_ServiceLayer::fetchTupleById()" );
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
        log.info( "Table_ServiceLayer::fetchAllTuples()" );
        
        // execute query
        String query = "select "
                + Globals.TABLE_SERVICELAYER_SERVICEID + ", "
                + Globals.TABLE_SERVICELAYER_LAYERID + ", "
                + Globals.TABLE_SERVICELAYER_LAYERSEQ
                + " from " + Globals.TABLE_SERVICELAYER;
        log.debug( "want to execute: " + query );
        Statement stmt = conn.createStatement();
        stmt.setFetchSize(0);			// Turn cursor off
        resultSet = stmt.executeQuery( query );
        
        log.info( "Table_ServiceLayer::fetchAllTuples()" );
    }
    
    
    /**
     * fetch all layers pertaining to a particular service, store result locally as a JDBC ResultList
     * precondition:
     *	JDBC connection opened
     * @param conn		JDBC connection
     * @throws SQLException
     **/
    public void fetchTuplesByServiceId( CommanderConnection conn, int sId ) throws SQLException
    {
        log.info( "Table_ServiceLayer::fetchTuplesByServiceId()" );
        
        // execute query
        String query = "select "
                + Globals.TABLE_SERVICELAYER_SERVICEID + ", "
                + Globals.TABLE_SERVICELAYER_LAYERID + ", "
                + Globals.TABLE_SERVICELAYER_LAYERSEQ
                + " from " + Globals.TABLE_SERVICELAYER
                + " where " + Globals.TABLE_SERVICELAYER + "." + Globals.TABLE_SERVICELAYER_SERVICEID
                + "     = " + sId
                + " order by " + Globals.TABLE_SERVICELAYER + "." + Globals.TABLE_SERVICELAYER_LAYERSEQ;
        log.debug( "want to execute: " + query );
        Statement stmt = conn.createStatement();
        stmt.setFetchSize(0);			// Turn cursor off
        resultSet = stmt.executeQuery( query );
        
        log.info( "Table_ServiceLayer::fetchTuplesByServiceId()" );
    }
    
    /**
     * fetch (singleton) service pertaining to a particular layer, store result locally as a JDBC ResultList
     * precondition:
     *	JDBC connection opened
     * @param conn		JDBC connection
     * @throws SQLException
     **/
    public void fetchTuplesByLayerId( CommanderConnection conn, int lId ) throws SQLException
    {
        log.info( "Table_ServiceLayer::fetchTuplesByLayerId()" );
        
        // execute query
        String query = "select "
                + Globals.TABLE_SERVICELAYER_SERVICEID + ", "
                + Globals.TABLE_SERVICELAYER_LAYERID + ", "
                + Globals.TABLE_SERVICELAYER_LAYERSEQ
                + " from " + Globals.TABLE_SERVICELAYER
                + " where " + Globals.TABLE_SERVICELAYER_LAYERID + " = " + lId;
        log.debug( "want to execute: " + query );
        Statement stmt = conn.createStatement();
        stmt.setFetchSize(0);			// Turn cursor off
        resultSet = stmt.executeQuery( query );
        
        log.info( "Table_ServiceLayer::fetchTuplesByServiceId()" );
    }
    
    
    /**
     * cursor over result set of previous fetchAllTuples()
     * if successful, the tuple variables are set with the current fetch values
     */
    public boolean next() throws SQLException
    {
        boolean moreAvailable = resultSet.next();
        
        if (moreAvailable)
        {
            this.serviceId	= resultSet.getInt( Globals.TABLE_SERVICELAYER_SERVICEID );
            this.layerId	= resultSet.getInt( Globals.TABLE_SERVICELAYER_LAYERID );
            this.layerSeq	= resultSet.getInt( Globals.TABLE_SERVICELAYER_LAYERSEQ );
            
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
    public void close() throws SQLException
    {
        log.info( "Table_ServiceLayer::close()" );
        
        resultSet.getStatement().close();
        resultSet.close();
        resultSet = null;
        
        tupleValid = false;
        
        log.info( "Table_ServiceLayer::close()" );
    }
    
    
    /**
     * find a layer seq number for appending at the end (ie, larger than any existing)
     */
    public int newSeqNo( CommanderConnection conn, int serviceId ) throws Exception	// FIXME: can we be more specific please?
    {
        log.info( "Table_ServiceLayer::newSeqNo( dbc, serviceId=" + serviceId + " )" );
        
        Statement stmt = conn.createStatement();
        ResultSet rs;
        rs = stmt.executeQuery( "SELECT max(layerseq)+1"
                + " from " + Globals.TABLE_SERVICELAYER
                + " where " + Globals.TABLE_SERVICELAYER_SERVICEID + "=" + serviceId);
        stmt.setFetchSize(0);			// Turn cursor off
        
        int layerseq;
        if (rs == null)
            layerseq = 1;
        else
        {
            rs.next();
            layerseq = rs.getInt(1);
        }
        
        // if it exceeds the max allowed int value, raise an exception
        if(layerseq > MAX_LAYERSEQ)
        {
            Exception e = new Exception("Error: layer sequencing overflow. Please contact your administrator.");
            throw e;
        }
        
        log.info( "Table_ServiceLayer::newSeqNo() -> " + layerseq );
        return layerseq;
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
    public int getServiceId()
    {
        return ( tupleValid ? serviceId : UNDEF_INT );
    }
    public int getLayerId()
    {
        return ( tupleValid ? layerId : UNDEF_INT );
    }
    public int getLayerSeq()
    {
        return ( tupleValid ? layerSeq : UNDEF_INT );
    }
    
    /**
     * Method toString
     * @return String
     */
    public String toString()
    {
        String stringRepresentation = "ServiceLayer( "
                + "tupleValid='" + tupleValid + "'; "
                + "serviceId=" + serviceId + ", "
                + "layerId=" + layerId + ", "
                + "layerSeq=" + layerSeq + " )";
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
    
    // - databaseConnection value set:
    static final String SERVER1                      = "server1";
    static final int    DPORT1                       = 41;
    static final String USER1                        = "user1";
    static final String PASSWD1                      = "passwd1";
    static final String DATABASE1                    = "database1";
    static final int    SERVERTIMEOUT1               = 67;
    static final int    SERVERCONNECTIONSPRECLAIMED1 = 17;
    static final int    SERVERCONNECTIONSONDEMAND1   = 29;
    
    // - service value set 1:
    // - value set 1:
    static final String UPDATESEQUENCE1     = "updateSequence1";
    static final int AVAILABILITY1		= 0;
    static final String NAME1               = "name1";
    static final String TITLE1              = "title1";
    static final String ABSTRACT1           = "abstractText1";
    static final String KEYWORDS1           = "keywords1";
    static final String FEES1               = "fees1";
    static final String ACCESSCONSTRAINTS1  = "accessConstraints1";
    static final String HOSTNAME1           = "hostName1";
    static final int    PORT1               = 42;
    static final String PATH1               = "path1";
    static final String FORMATS1            = "formats1";
    static final String BASELAYERNAME1      = "baseLayerName1";
    static final String VENDORCAPABILITIES1 = "vendorCapabilities1";
    static final String CONTACTPERSON1	= "contactPerson1";
    static final String CONTACTORGANIZATION1	= "contactOrganization1";
    static final String ADDRESSTYPE1	= "addressType1";
    static final String ADDRESS1		= "address1";
    static final String CITY1		= "city1";
    static final String STATE1		= "stateOrProvince1";
    static final String POSTCODE1		= "postCode1";
    static final String COUNTRY1		= "country1";
    static final String TEL1		= "contactVoiceTelephone1";
    static final String FAX1		= "contactFacsimileTelephone1";
    static final String EMAIL1		= "contactElectronicMailAddress1";
    
    // - service value set 1:
    static final String UPDATESEQUENCE2     = "updateSequence2";
    static final int AVAILABILITY2		= 0;
    static final String NAME2               = "name2";
    static final String TITLE2              = "title2";
    static final String ABSTRACT2           = "abstractText2";
    static final String KEYWORDS2           = "keywords2";
    static final String FEES2               = "fees2";
    static final String ACCESSCONSTRAINTS2  = "accessConstraints2";
    static final String HOSTNAME2           = "hostName2";
    static final int PORT2                  = 98;
    static final String PATH2               = "path2";
    static final String FORMATS2            = "formats2";
    static final String BASELAYERNAME2      = "baseLayerName2";
    static final String VENDORCAPABILITIES2 = "vendorCapabilities2";
    static final String CONTACTPERSON2	= "contactPerson2";
    static final String CONTACTORGANIZATION2	= "contactOrganization2";
    static final String ADDRESSTYPE2	= "addressType2";
    static final String ADDRESS2		= "address2";
    static final String CITY2		= "city2";
    static final String STATE2		= "stateOrProvince2";
    static final String POSTCODE2		= "postCode2";
    static final String COUNTRY2		= "country2";
    static final String TEL2		= "contactVoiceTelephone2";
    static final String FAX2		= "contactFacsimileTelephone2";
    static final String EMAIL2		= "contactElectronicMailAddress2";
    
    // - layer value set 1:
    public static final String LNAME1 = "name1";
    public static final String LTITLE1 = "title1";
    public static final String SRS1 = "srs1";
    public static final String AUTHORITY1 = "authority1";
    public static final String LATLONXMIN1 = "11.";
    public static final String LATLONXMAX1 = "22.";
    public static final String LATLONYMIN1 = "33.";
    public static final String LATLONYMAX1 = "44.";
    public static final String BBOXXMIN1 = "1.";
    public static final String BBOXXMAX1 = "2.";
    public static final String BBOXYMIN1 = "3.";
    public static final String BBOXYMAX1 = "4.";
    public static final String ATTRIBUTIONURL1 = "attributionURL1";
    public static final String ATTRIBUTIONTITLE1 = "attributionTitle1";
    public static final int    LOGOWIDTH1 = 100;
    public static final int    LOGOHEIGHT1 = 100;
    public static final String LOGOFORMAT1 = "logoFormat1";
    public static final String LOGOURL1 = "logoURL1";
    public static final String FEATUREURL1 = "featureURL1";
    public static final double RESOLUTION1 = 1.111;
    public static final MapType MAPTYPE1 = MapType.GREY;
    
    // - layer value set 2:
    public static final String LNAME2 = "name2";
    public static final String LTITLE2 = "title2";
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
    
    // - LayerService seq# test value set:
    public static final int SEQ1 = 42;
    public static final int SEQ2 = 84;
    
    
    public static void main( String[] args )
    {
        // --- start action -----------------------------------
        
        System.out.println( "START class Table_ServiceLayer component test" );
        
        // --- cmd line parameter check ------------------------
        if (args.length != 6)
        {
            System.err.println( "Usage: Table_ServiceLayer msgFile url user password jdbc debugLevel" );
            return;
        }
        String msgFile  = args[0];
        String url      = args[1];
        String usr      = args[2];
        String pwd      = args[3];
        String jdbc     = args[4];
        System.out.println( "Parameters: msgFile=" + msgFile + ", url=" + url + ", user=" + usr + ", password=" + pwd + ", jdbc=" + jdbc );
        
        try
        {
            // --- test preparation -------------------------------
            
            CommanderConnection c = new CommanderConnection( url, usr, pwd, jdbc );
            
            // - set up service tuples
            Table_Service myService1 = new Table_Service();
            int srv1 = myService1.insertTuple( c, UPDATESEQUENCE1, AVAILABILITY1, NAME1, TITLE1, ABSTRACT1, KEYWORDS1, FEES1, ACCESSCONSTRAINTS1, HOSTNAME1, PORT1, PATH1, FORMATS1, BASELAYERNAME1, VENDORCAPABILITIES1, CONTACTPERSON1, CONTACTORGANIZATION1, ADDRESSTYPE1, ADDRESS1, CITY1, STATE1, POSTCODE1, COUNTRY1, TEL1, FAX1, EMAIL1 );
            System.out.println( "insert service #1 -> " + srv1 );
            
            Table_Service myService2 = new Table_Service();
            int srv2 = myService2.insertTuple( c, UPDATESEQUENCE2, AVAILABILITY2, NAME2, TITLE2, ABSTRACT2, KEYWORDS2, FEES2, ACCESSCONSTRAINTS2, HOSTNAME2, PORT2, PATH2, FORMATS2, BASELAYERNAME2, VENDORCAPABILITIES2, CONTACTPERSON2, CONTACTORGANIZATION2, ADDRESSTYPE2, ADDRESS2, CITY2, STATE2, POSTCODE2, COUNTRY2, TEL2, FAX2, EMAIL2 );
            System.out.println( "insert service #2 -> " + srv2 );
            
            // - set up layer tuples
            Table_Layer myLayer1 = new Table_Layer();
            int lay1 = myLayer1.insertTuple( c, LNAME1, LTITLE1, SRS1, AUTHORITY1, LATLONXMIN1, LATLONXMAX1, LATLONYMIN1, LATLONYMAX1, BBOXXMIN1, BBOXXMAX1, BBOXYMIN1, BBOXYMAX1, ATTRIBUTIONURL1, ATTRIBUTIONTITLE1, LOGOWIDTH1, LOGOHEIGHT1, LOGOFORMAT1, LOGOURL1, FEATUREURL1, RESOLUTION1, MAPTYPE1 );
            System.out.println( "insert layer #1 -> " + lay1 );
            
            Table_Layer myLayer2 = new Table_Layer();
            int lay2 = myLayer2.insertTuple( c, LNAME2, LTITLE2, SRS2, AUTHORITY2, LATLONXMIN2, LATLONXMAX2, LATLONYMIN2, LATLONYMAX2, BBOXXMIN2, BBOXXMAX2, BBOXYMIN2, BBOXYMAX2, ATTRIBUTIONURL2, ATTRIBUTIONTITLE2, LOGOWIDTH2, LOGOHEIGHT2, LOGOFORMAT2, LOGOURL2, FEATUREURL2, RESOLUTION2, MAPTYPE2 );
            System.out.println( "insert layer #2 -> " + lay2 );
            
            // --- test -------------------------------------------
            
            // - create a reference instance
            Table_ServiceLayer myServiceLayer1 = new Table_ServiceLayer();
            myServiceLayer1.insertTuple( c, srv1, lay1, SEQ1 );
            System.out.println( "insert Table_ServiceLayer tuple #1 with seq=" + SEQ1 );
            
            // - update reference instance
            myServiceLayer1.updateTuple( c, srv1, lay1, SEQ2 );
            
            // - fetch values via 2nd instance, check all values
            Table_ServiceLayer myServiceLayer2 = new Table_ServiceLayer();
            myServiceLayer2.fetchTupleById( c, srv1, lay1 );
            if (myServiceLayer2.getServiceId() != srv1 )
                System.out.println( "Error: attribute updated != attributed fetched: " + Globals.TABLE_SERVICELAYER_SERVICEID );
            if (myServiceLayer2.getLayerId() != lay1 )
                System.out.println( "Error: attribute updated != attributed fetched: " + Globals.TABLE_SERVICELAYER_LAYERID );
            if (myServiceLayer2.getLayerSeq() != SEQ2 )
                System.out.println( "Error: attribute updated != attributed fetched: " + Globals.TABLE_SERVICELAYER_LAYERID );
            
            // - delete instance 1 and check it's gone
            myServiceLayer1.deleteTuple( c, srv1, lay1 );
            try
            {
                myServiceLayer2.fetchTupleById( c, srv1, lay1 );
                System.out.println( "Error: deleted tuple still seems to exist." );
            }
            catch( Exception e )
            {
                System.out.println( "fetching tuple deleted caused exception, that's ok." );
            }
            
            // - create second instance
            myServiceLayer2.insertTuple( c, srv1, lay2, SEQ1 );
            System.out.println( "insert Table_ServiceLayer tuple #1 with seq=" + SEQ1 );
            
            // --- test cleanup -----------------------------------
            c.abort();
            c.close();
            
        }
        catch (Exception e)
        {
            System.out.println( "Error: caught exception: " + e.getMessage() );
        }
        
        System.out.println( "END class Table_ServiceLayer component test" );
    }
}