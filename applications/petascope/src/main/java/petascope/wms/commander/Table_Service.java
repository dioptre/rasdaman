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
 * provide access to the "Service" table of rasgeo commander.
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
 * This class accomplishes tuple-at-a-time access to the Service table,
 * encapsulating SQL statements.
 **/
public class Table_Service
{
    private static Logger log = LoggerFactory.getLogger(Table_Service.class);
            
    /**
     * string & int result for invalid call of getter method
     * (we tentatively use null here to cause problems elsewhere when working with such an invalid result)
     */
    private static final String UNDEF_STRING = null;
    private static final int    UNDEF_INT    = -1;
    
    /**
     * result set for fetchAllTuples() results
     */
    private ResultSet resultSet = null;
    
    /**
     * flag indicating whether current tuple (see next) contains a value set retrieved from the database
     */
    private boolean tupleValid = false;
    
    /**
     * value set for one tuple of this table; values are good only if tupleValid==true
     */
    private int    serviceId          = UNDEF_INT;
    private String updateSequence     = UNDEF_STRING;
    private int availability	  = UNDEF_INT;
    private String name               = UNDEF_STRING;
    private String title              = UNDEF_STRING;
    private String abstractText       = UNDEF_STRING;
    private String keywords           = UNDEF_STRING;
    private String fees               = UNDEF_STRING;
    private String accessConstraints  = UNDEF_STRING;
    private String hostName           = UNDEF_STRING;
    private int    port               = UNDEF_INT;
    private String path               = UNDEF_STRING;
    private String formats            = UNDEF_STRING;
    private String baseLayerName      = UNDEF_STRING;
    private String vendorCapabilities = UNDEF_STRING;
    private String contactPerson		= UNDEF_STRING;
    private String contactOrganization	= UNDEF_STRING;
    private String addressType		= UNDEF_STRING;
    private String address			= UNDEF_STRING;
    private String city			= UNDEF_STRING;
    private String stateOrProvince		= UNDEF_STRING;
    private String postCode			= UNDEF_STRING;
    private String country			= UNDEF_STRING;
    private String contactVoiceTelephone	= UNDEF_STRING;
    private String contactFacsimileTelephone		= UNDEF_STRING;
    private String contactElectronicMailAddress		= UNDEF_STRING;
    
    /**
     * constructor
     * @return CommanderConnection
     **/
    public Table_Service()
    {
        log.info( "Table_Service::Table_Service()" );
        tupleValid = false;
        log.info( "Table_Service::Table_Service()" );
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
    public int insertTuple( CommanderConnection conn, String updateSequence, int availability, String name, String title, String abstractText, String keywords, String fees, String accessConstraints, String hostName, int port, String path, String formats, String baseLayerName, String vendorCapabilities, String contactPerson, String contactOrganization, String addressType, String address, String city, String stateOrProvince, String postCode, String country, String contactVoiceTelephone, String contactFacsimileTelephone, String contactElectronicMailAddress)
            throws SQLException, ConnectionFailedException, InvalidInputException
    {
        log.info( "Table_Service::insertTuple()" );
        
        // --- plausi checks ----------------------------------
        // - connection object
        if (conn == null)
        {
            log.info( "Table_Service::insertTuple() -- Error: no database connection." );
            throw new ConnectionFailedException( "no database connection." );
        }
        
        // - null string
        if (updateSequence==null || name==null || title==null || abstractText==null || keywords==null || fees==null || accessConstraints==null || hostName==null || path==null || formats==null || baseLayerName==null || vendorCapabilities==null )
        {
            log.info( "Table_Service::insertTuple() -- Error: null parameter passed" );
            throw new InvalidInputException( "null parameter passed" );
        }
        
        // --- action -----------------------------------------
        // do this until we find out how to get back a new value from INSERT
        String query = "select max(" + Globals.TABLE_SERVICE_SERVICEID + ")"
                + " from " + Globals.TABLE_SERVICE;
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
            log.info( "Table_Service::insertTuple() -- Error: cannot get new Services id" );
            throw new InvalidInputException( "Error: cannot get new Services id" );
        }
        // make VERY sure that attribute names and values are listed in the same sequence!
        query = "insert into " + Globals.TABLE_SERVICE + " ( "
                + Globals.TABLE_SERVICE_SERVICEID + ", "
                + Globals.TABLE_SERVICE_UPDATESEQUENCE + ", "
                + Globals.TABLE_SERVICE_AVAILABILITY + ", "
                + Globals.TABLE_SERVICE_NAME + ", "
                + Globals.TABLE_SERVICE_TITLE + ", "
                + Globals.TABLE_SERVICE_ABSTRACT + ", "
                + Globals.TABLE_SERVICE_KEYWORDS + ", "
                + Globals.TABLE_SERVICE_FEES + ", "
                + Globals.TABLE_SERVICE_ACCESSCONSTRAINTS + ", "
                + Globals.TABLE_SERVICE_HOSTNAME + ", "
                + Globals.TABLE_SERVICE_PORT + ", "
                + Globals.TABLE_SERVICE_PATH + ", "
                + Globals.TABLE_SERVICE_FORMATS + ", "
                + Globals.TABLE_SERVICE_BASELAYERNAME + ", "
                + Globals.TABLE_SERVICE_VENDORCAPABILITIES + ", "
                + Globals.TABLE_SERVICE_CONTACTPERSON + ", "
                + Globals.TABLE_SERVICE_CONTACTORGANIZATION + ", "
                + Globals.TABLE_SERVICE_ADDRESSTYPE + ", "
                + Globals.TABLE_SERVICE_ADDRESS + ", "
                + Globals.TABLE_SERVICE_CITY + ", "
                + Globals.TABLE_SERVICE_STATE + ", "
                + Globals.TABLE_SERVICE_POSTCODE + ", "
                + Globals.TABLE_SERVICE_COUNTRY + ", "
                + Globals.TABLE_SERVICE_TEL + ", "
                + Globals.TABLE_SERVICE_FAX + ", "
                + Globals.TABLE_SERVICE_EMAIL + " )"
                + " values( "
                + + newId + ", "
                + "'" + updateSequence + "', "
                + availability + ", "
                + "'" + name + "', "
                + "'" + title + "', "
                + "'" + abstractText + "', "
                + "'" + keywords + "', "
                + "'" + fees + "', "
                + "'" + accessConstraints + "', "
                + "'" + hostName + "', "
                + port + ", "
                + "'" + path + "', "
                + "'" + formats + "', "
                + "'" + baseLayerName + "', "
                + "'" + vendorCapabilities + "', "
                + "'" + contactPerson + "', "
                + "'" + contactOrganization + "', "
                + "'" + addressType + "', "
                + "'" + address + "', "
                + "'" + city + "', "
                + "'" + stateOrProvince + "', "
                + "'" + postCode + "', "
                + "'" + country + "', "
                + "'" + contactVoiceTelephone + "', "
                + "'" + contactFacsimileTelephone + "', "
                + "'" + contactElectronicMailAddress + " )";
        log.debug( "want to execute: " + query );
        stmt.executeUpdate( query );
        
        log.info( "Table_Service::insertTuple() -> "  + newId );
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
    public void updateTuple( CommanderConnection conn, int id, String updateSequence, int availability, String name, String title, String abstractText, String keywords, String fees, String accessConstraints, String hostName, int port, String path, String formats, String baseLayerName, String vendorCapabilities, String contactPerson, String contactOrganization, String addressType, String address, String city, String stateOrProvince, String postCode, String country, String contactVoiceTelephone, String contactFacsimileTelephone, String contactElectronicMailAddress )
            
            throws SQLException, ConnectionFailedException
    {
        log.info( "Table_Service::updateTuple( id=" + id + " )" );
        
        // --- plausi checks ----------------------------------
        // - connection object
        if (conn == null)
        {
            log.info( "Table_Service::updateTuple() -- Error: no database connection." );
            throw new ConnectionFailedException( "no database connection." );
        }
        // - null string -> ""
        if (updateSequence==null)
            updateSequence = "";
        if (name==null)
            name = "";
        if (title==null)
            title = "";
        if (abstractText==null)
            abstractText = "";
        if (keywords==null)
            keywords = "";
        if (fees==null)
            fees = "";
        if (accessConstraints==null)
            accessConstraints = "";
        if (hostName==null)
            hostName = "";
        if (path==null)
            path = "";
        if (formats==null)
            formats = "";
        if (baseLayerName==null)
            baseLayerName = "";
        if (vendorCapabilities==null)
            vendorCapabilities = "";
        if (contactPerson==null)
            contactPerson = "";
        if (contactOrganization==null)
            contactOrganization = "";
        if (addressType==null)
            addressType = "";
        if (address==null)
            address = "";
        if (city==null)
            city = "";
        if (stateOrProvince==null)
            stateOrProvince = "";
        if (postCode==null)
            postCode = "";
        if (country==null)
            country = "";
        if (contactVoiceTelephone==null)
            contactVoiceTelephone = "";
        if (contactFacsimileTelephone==null)
            contactFacsimileTelephone = "";
        if (contactElectronicMailAddress==null)
            contactElectronicMailAddress = "";
        
        // --- action -----------------------------------------
        String query = "update " + Globals.TABLE_SERVICE + " set "
                + Globals.TABLE_SERVICE_UPDATESEQUENCE + " = '" + updateSequence + "', "
                + Globals.TABLE_SERVICE_AVAILABILITY + " = " + availability + ", "
                + Globals.TABLE_SERVICE_NAME + " = '" + name + "', "
                + Globals.TABLE_SERVICE_TITLE + " = '" + title + "', "
                + Globals.TABLE_SERVICE_ABSTRACT + " = '" + abstractText + "', "
                + Globals.TABLE_SERVICE_KEYWORDS + " = '" + keywords + "', "
                + Globals.TABLE_SERVICE_FEES + " = '" + fees + "', "
                + Globals.TABLE_SERVICE_ACCESSCONSTRAINTS + " = '" + accessConstraints + "', "
                + Globals.TABLE_SERVICE_HOSTNAME + " = '" + hostName + "', "
                + Globals.TABLE_SERVICE_PORT + " = " + port + ", "
                + Globals.TABLE_SERVICE_PATH + " = '" + path + "', "
                + Globals.TABLE_SERVICE_FORMATS + " = '" + formats + "', "
                + Globals.TABLE_SERVICE_BASELAYERNAME + " = '" + baseLayerName + "', "
                + Globals.TABLE_SERVICE_VENDORCAPABILITIES + " = '" + vendorCapabilities + "', "
                + Globals.TABLE_SERVICE_CONTACTPERSON + " = '" + contactPerson + "', "
                + Globals.TABLE_SERVICE_CONTACTORGANIZATION + " = '" + contactOrganization + "', "
                + Globals.TABLE_SERVICE_ADDRESSTYPE + " = '" + addressType + "', "
                + Globals.TABLE_SERVICE_ADDRESS + " = '" + address + "', "
                + Globals.TABLE_SERVICE_CITY + " = '" + city + "', "
                + Globals.TABLE_SERVICE_STATE + " = '" + stateOrProvince + "', "
                + Globals.TABLE_SERVICE_POSTCODE + " = '" + postCode + "', "
                + Globals.TABLE_SERVICE_COUNTRY + " = '" + country + "', "
                + Globals.TABLE_SERVICE_TEL + " = '" + contactVoiceTelephone + "', "
                + Globals.TABLE_SERVICE_FAX + " = '" + contactFacsimileTelephone + "', "
                + Globals.TABLE_SERVICE_EMAIL + " = '" + contactElectronicMailAddress
                + " where " + Globals.TABLE_SERVICE_SERVICEID + " = " + id;
        Statement stmt = conn.createStatement();
        stmt.executeUpdate( query );
        
        log.info( "Table_Service::updateTuple()" );
    }
    
    /**
     * update service availability
     * FIXME: generalize updateTuple to only address non-null parameters, then this method is obsolete
     * precondition:
     *	JDBC connection opened
     *	tuple with id exists
     * @param conn		JDBC connection
     * @param id		service tuple id
     * @throws SQLException
     **/
    public void updateAvailability( CommanderConnection conn, int id, int availability )
            throws SQLException, ConnectionFailedException
    {
        log.info( "Table_Service::updateAvailability( " + id + ", " + availability + " )" );
        
        // --- plausi checks ----------------------------------
        // - connection object
        if (conn == null)
        {
            log.info( "Table_Service::updateAvailability() -- Error: no database connection." );
            throw new ConnectionFailedException( "no database connection." );
        }
        if (availability != Globals.YES_AVAILABLE && availability != Globals.NOT_AVAILABLE)
        {
            log.info( "Table_Service::updateAvailability() -- Error: illegal availability value." );
            throw new IllegalArgumentException( "illegal availability value." );
        }
        
        // --- action -----------------------------------------
        String query = "update " + Globals.TABLE_SERVICE + " set "
                + Globals.TABLE_SERVICE_UPDATESEQUENCE + " = '" + updateSequence + "', "
                + Globals.TABLE_SERVICE_AVAILABILITY + " = " + availability + ", "
                + Globals.TABLE_SERVICE_NAME + " = '" + name + "', "
                + Globals.TABLE_SERVICE_TITLE + " = '" + title + "', "
                + Globals.TABLE_SERVICE_ABSTRACT + " = '" + abstractText + "', "
                + Globals.TABLE_SERVICE_KEYWORDS + " = '" + keywords + "', "
                + Globals.TABLE_SERVICE_FEES + " = '" + fees + "', "
                + Globals.TABLE_SERVICE_ACCESSCONSTRAINTS + " = '" + accessConstraints + "', "
                + Globals.TABLE_SERVICE_HOSTNAME + " = '" + hostName + "', "
                + Globals.TABLE_SERVICE_PORT + " = " + port + ", "
                + Globals.TABLE_SERVICE_PATH + " = '" + path + "', "
                + Globals.TABLE_SERVICE_FORMATS + " = '" + formats + "', "
                + Globals.TABLE_SERVICE_BASELAYERNAME + " = '" + baseLayerName + "', "
                + Globals.TABLE_SERVICE_VENDORCAPABILITIES + " = '" + vendorCapabilities + "', "
                + Globals.TABLE_SERVICE_CONTACTPERSON + " = '" + contactPerson + "', "
                + Globals.TABLE_SERVICE_CONTACTORGANIZATION + " = '" + contactOrganization + "', "
                + Globals.TABLE_SERVICE_ADDRESSTYPE + " = '" + addressType + "', "
                + Globals.TABLE_SERVICE_ADDRESS + " = '" + address + "', "
                + Globals.TABLE_SERVICE_CITY + " = '" + city + "', "
                + Globals.TABLE_SERVICE_STATE + " = '" + stateOrProvince + "', "
                + Globals.TABLE_SERVICE_POSTCODE + " = '" + postCode + "', "
                + Globals.TABLE_SERVICE_COUNTRY + " = '" + country + "', "
                + Globals.TABLE_SERVICE_TEL + " = '" + contactVoiceTelephone + "', "
                + Globals.TABLE_SERVICE_FAX + " = '" + contactFacsimileTelephone + "', "
                + Globals.TABLE_SERVICE_EMAIL + " = '" + contactElectronicMailAddress
                + " where " + Globals.TABLE_SERVICE_SERVICEID + " = " + id;
        Statement stmt = conn.createStatement();
        stmt.executeUpdate( query );
        
        log.info( "Table_Service::updateAvailability()" );
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
        log.info( "Table_Service::deleteTuple()" );
        
        // --- plausi checks ----------------------------------
        // - connection object
        if (conn == null)
        {
            log.info( "Table_Service::updateTuple() -- Error: no database connection." );
            throw new ConnectionFailedException( "no database connection." );
        }
        
        // --- action -----------------------------------------
        String query = "delete from " + Globals.TABLE_SERVICE + " where " + Globals.TABLE_SERVICE_SERVICEID + " = " + id;
        Statement stmt = conn.createStatement();
        stmt.executeUpdate( query );
        
        log.info( "Table_Service::deleteTuple() -- ok" );
    }
    
    /**
     * fetch one tuple by its (unique) id, keep in local result set
     * precondition:
     *	JDBC connection opened
     * @param conn		JDBC connection
     * @param id		service tuple id
     * @throws SQLException
     **/
    public void fetchTupleById( CommanderConnection conn, int id ) throws SQLException
    {
        log.info( "Table_Service::fetchTupleById()" );
        
        // execute query
        String query = "select "
                + Globals.TABLE_SERVICE_SERVICEID + ", "
                + Globals.TABLE_SERVICE_UPDATESEQUENCE + ", "
                + Globals.TABLE_SERVICE_AVAILABILITY + ", "
                + Globals.TABLE_SERVICE_NAME + ", "
                + Globals.TABLE_SERVICE_TITLE + ", "
                + Globals.TABLE_SERVICE_ABSTRACT + ", "
                + Globals.TABLE_SERVICE_KEYWORDS + ", "
                + Globals.TABLE_SERVICE_FEES + ", "
                + Globals.TABLE_SERVICE_ACCESSCONSTRAINTS + ", "
                + Globals.TABLE_SERVICE_HOSTNAME + ", "
                + Globals.TABLE_SERVICE_PORT + ", "
                + Globals.TABLE_SERVICE_PATH + ", "
                + Globals.TABLE_SERVICE_FORMATS + ", "
                + Globals.TABLE_SERVICE_BASELAYERNAME + ", "
                + Globals.TABLE_SERVICE_VENDORCAPABILITIES + ", "
                + Globals.TABLE_SERVICE_CONTACTPERSON + ", "
                + Globals.TABLE_SERVICE_CONTACTORGANIZATION + ", "
                + Globals.TABLE_SERVICE_ADDRESSTYPE + ", "
                + Globals.TABLE_SERVICE_ADDRESS + ", "
                + Globals.TABLE_SERVICE_CITY + ", "
                + Globals.TABLE_SERVICE_STATE + ", "
                + Globals.TABLE_SERVICE_POSTCODE + ", "
                + Globals.TABLE_SERVICE_COUNTRY + ", "
                + Globals.TABLE_SERVICE_TEL + ", "
                + Globals.TABLE_SERVICE_FAX + ", "
                + Globals.TABLE_SERVICE_EMAIL
                + " from " + Globals.TABLE_SERVICE + " where " + Globals.TABLE_SERVICE_SERVICEID + " = " + id;
        Statement stmt = conn.createStatement();
        resultSet = stmt.executeQuery( query );
        
        log.info( "Table_Service::fetchTupleById()" );
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
        log.info( "Table_Service::fetchAllTuples()" );
        
        // execute query
        String query = "select "
                + Globals.TABLE_SERVICE_SERVICEID + ", "
                + Globals.TABLE_SERVICE_AVAILABILITY + ", "
                + Globals.TABLE_SERVICE_UPDATESEQUENCE + ", "
                + Globals.TABLE_SERVICE_NAME + ", "
                + Globals.TABLE_SERVICE_TITLE + ", "
                + Globals.TABLE_SERVICE_ABSTRACT + ", "
                + Globals.TABLE_SERVICE_KEYWORDS + ", "
                + Globals.TABLE_SERVICE_FEES + ", "
                + Globals.TABLE_SERVICE_ACCESSCONSTRAINTS + ", "
                + Globals.TABLE_SERVICE_HOSTNAME + ", "
                + Globals.TABLE_SERVICE_PORT + ", "
                + Globals.TABLE_SERVICE_PATH + ", "
                + Globals.TABLE_SERVICE_FORMATS + ", "
                + Globals.TABLE_SERVICE_BASELAYERNAME + ", "
                + Globals.TABLE_SERVICE_VENDORCAPABILITIES + ", "
                + Globals.TABLE_SERVICE_CONTACTPERSON + ", "
                + Globals.TABLE_SERVICE_CONTACTORGANIZATION + ", "
                + Globals.TABLE_SERVICE_ADDRESSTYPE + ", "
                + Globals.TABLE_SERVICE_ADDRESS + ", "
                + Globals.TABLE_SERVICE_CITY + ", "
                + Globals.TABLE_SERVICE_STATE + ", "
                + Globals.TABLE_SERVICE_POSTCODE + ", "
                + Globals.TABLE_SERVICE_COUNTRY + ", "
                + Globals.TABLE_SERVICE_TEL + ", "
                + Globals.TABLE_SERVICE_FAX + ", "
                + Globals.TABLE_SERVICE_EMAIL
                + " from " + Globals.TABLE_SERVICE + " order by " + Globals.TABLE_SERVICE_NAME;
        Statement stmt = conn.createStatement();
        stmt.setFetchSize(0);			// Turn cursor off
        resultSet = stmt.executeQuery( query );
        
        log.info( "Table_Service::fetchAllTuples()" );
    }
    
    /**
     * cursor over result set of previous fetchAllTuples()
     * if successful, the tuple variables are set with the current fetch values
     */
    public boolean next()
            throws SQLException
    {
        log.info( "Table_Service::next()" );
        boolean moreAvailable = resultSet.next();
        
        if (moreAvailable)
        {
            serviceId          = resultSet.getInt(    Globals.TABLE_SERVICE_SERVICEID );
            updateSequence     = resultSet.getString( Globals.TABLE_SERVICE_UPDATESEQUENCE );
            availability	   = resultSet.getInt( Globals.TABLE_SERVICE_AVAILABILITY );
            name               = resultSet.getString( Globals.TABLE_SERVICE_NAME );
            title              = resultSet.getString( Globals.TABLE_SERVICE_TITLE );
            abstractText       = resultSet.getString( Globals.TABLE_SERVICE_ABSTRACT );
            keywords           = resultSet.getString( Globals.TABLE_SERVICE_KEYWORDS );
            fees               = resultSet.getString( Globals.TABLE_SERVICE_FEES );
            accessConstraints  = resultSet.getString( Globals.TABLE_SERVICE_ACCESSCONSTRAINTS );
            hostName           = resultSet.getString( Globals.TABLE_SERVICE_HOSTNAME );
            port               = resultSet.getInt(    Globals.TABLE_SERVICE_PORT );
            path               = resultSet.getString( Globals.TABLE_SERVICE_PATH );
            formats            = resultSet.getString( Globals.TABLE_SERVICE_FORMATS );
            baseLayerName      = resultSet.getString( Globals.TABLE_SERVICE_BASELAYERNAME );
            vendorCapabilities = resultSet.getString( Globals.TABLE_SERVICE_VENDORCAPABILITIES );
            
            contactPerson			= resultSet.getString( Globals.TABLE_SERVICE_CONTACTPERSON );
            contactOrganization		= resultSet.getString( Globals.TABLE_SERVICE_CONTACTORGANIZATION );
            addressType			= resultSet.getString( Globals.TABLE_SERVICE_ADDRESSTYPE );
            address				= resultSet.getString( Globals.TABLE_SERVICE_ADDRESS );
            city				= resultSet.getString( Globals.TABLE_SERVICE_CITY );
            stateOrProvince			= resultSet.getString( Globals.TABLE_SERVICE_STATE );
            postCode			= resultSet.getString( Globals.TABLE_SERVICE_POSTCODE );
            country				= resultSet.getString( Globals.TABLE_SERVICE_COUNTRY );
            contactVoiceTelephone		= resultSet.getString( Globals.TABLE_SERVICE_TEL );
            contactFacsimileTelephone	= resultSet.getString( Globals.TABLE_SERVICE_FAX );
            contactElectronicMailAddress	= resultSet.getString( Globals.TABLE_SERVICE_EMAIL );

            tupleValid = true;
        }
        else
        {
            tupleValid = false;
            close();
        }
        
        log.info( "Table_Service::next() -> " + moreAvailable );
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
        log.info( "Table_Service::close()" );
        
        resultSet.getStatement().close();
        resultSet.close();
        resultSet = null;
        
        tupleValid = false;
        
        log.info( "Table_Service::close()" );
    }
    
    /**
     * determine number of services stored in database
     **/
    public int getCard( CommanderConnection conn )
            throws SQLException
    {
        int result = Globals.NO_KEY;
        
        log.info( "Table_Service::getCard()" );
        
        Statement stmt = conn.createStatement();
        stmt.setFetchSize(0);			// Turn cursor off
        resultSet = stmt.executeQuery( "SELECT count(" + Globals.TABLE_SERVICE_SERVICEID + ")"
                + " from " + Globals.TABLE_SERVICE );
        
        if (resultSet == null)				// empty table?
            result = 0;
        else
            result = resultSet.getInt( 1 );
        
        log.info( "Table_Service::newCard() -> " + result );
        return result;
    }
    
    /**
     * is current tuple valid, ie, has there been a fetch before?
     * precondition:
     *	successful call to fetchTuple()
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
    public int getId()
    {
        return ( tupleValid ? serviceId : UNDEF_INT );
    }
    
    public String getUpdateSequence()
    {
        return ( tupleValid ? updateSequence : UNDEF_STRING );
    }
    public int getAvailability()
    {
        return ( tupleValid ? availability : UNDEF_INT );
    }
    
    public String getName()
    {
        return ( tupleValid ? name : UNDEF_STRING );
    }
    
    public String getTitle()
    {
        return ( tupleValid ? title : UNDEF_STRING );
    }
    
    public String getAbstract()
    {
        return ( tupleValid ? abstractText : UNDEF_STRING );
    }
    
    public String getKeywords()
    {
        return ( tupleValid ? keywords : UNDEF_STRING );
    }
    
    public String getFees()
    {
        return ( tupleValid ? fees : UNDEF_STRING );
    }
    
    public String getAccessConstraints()
    {
        return ( tupleValid ? accessConstraints : UNDEF_STRING );
    }
    
    public String getHostName()
    {
        return ( tupleValid ? hostName : UNDEF_STRING );
    }
    
    public int  getPort()
    {
        return ( tupleValid ? port : UNDEF_INT );
    }
    
    public String getPath()
    {
        return ( tupleValid ? path : UNDEF_STRING );
    }
    
    public String getFormats()
    {
        return ( tupleValid ? formats : UNDEF_STRING );
    }
    
    public String getBaseLayerName()
    {
        return ( tupleValid ? baseLayerName : UNDEF_STRING );
    }
    
    public String getVendorCapabilities()
    {
        return ( tupleValid ? vendorCapabilities : UNDEF_STRING );
    }
    
    public String getContactPerson()
    {
        return ( tupleValid ? contactPerson : UNDEF_STRING );
    }
    
    public String getContactOrganization()
    {
        return ( tupleValid ? contactOrganization : UNDEF_STRING );
    }
    
    public String getAddressType()
    {
        return ( tupleValid ? addressType : UNDEF_STRING );
    }
    
    public String getAddress()
    {
        return ( tupleValid ? address : UNDEF_STRING );
    }
    
    public String getCity()
    {
        return ( tupleValid ? city : UNDEF_STRING );
    }
    
    public String getStateOrProvince()
    {
        return ( tupleValid ? stateOrProvince : UNDEF_STRING );
    }
    
    public String getPostCode()
    {
        return ( tupleValid ? postCode : UNDEF_STRING );
    }
    
    public String getCountry()
    {
        return ( tupleValid ? country : UNDEF_STRING );
    }
    
    public String getContactVoiceTelephone()
    {
        return ( tupleValid ? contactVoiceTelephone : UNDEF_STRING );
    }
    
    public String getContactFacsimileTelephone()
    {
        return ( tupleValid ? contactFacsimileTelephone : UNDEF_STRING );
    }
    
    public String getContactElectronicMailAddress()
    {
        return ( tupleValid ? contactElectronicMailAddress : UNDEF_STRING );
    }
    
    /**
     * Method toString
     * @return String
     */
    public String toString()
    {
        String stringRepresentation = "Service( "
                + "tupleValid='" + tupleValid + "'; "
                + "updateSequence='" + updateSequence + "', "
                + "availability='" + availability + "', "
                + "name='" + name + "', "
                + "title='" + title + "', "
                + "abstractText='" + abstractText + "', "
                + "keywords='" + keywords + "', "
                + "fees='" + fees + "', "
                + "accessConstraints='" + accessConstraints + "', "
                + "hostName='" + hostName + "', "
                + "port='" + port + "', "
                + "path='" + path + "', "
                + "formats='" + formats + "', "
                + "baseLayerName='" + baseLayerName + "', "
                + "vendorCapabilities='" + vendorCapabilities + "', "
                + "contactPerson='" + contactPerson + "', "
                + "contactOrganization='" + contactOrganization + "', "
                + "addressType='" + addressType + "', "
                + "address='" + address + "', "
                + "city='" + city + "', "
                + "stateOrProvince='" + stateOrProvince + "', "
                + "postCode='" + postCode + "', "
                + "country='" + country + "', "
                + "contactVoiceTelephone='" + contactVoiceTelephone + "', "
                + "contactFacsimileTelephone='" + contactFacsimileTelephone + "', "
                + "contactElectronicMailAddress='" + contactElectronicMailAddress + "' )";
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
    
    // - stub dataset for DatabaseConnection tuple:
    static final String SERVER1                      = "server1";
    static final int    DBCPORT1                     = 41;
    static final String USER1                        = "user1";
    static final String PASSWD1                      = "passwd1";
    static final String DATABASE1                    = "database1";
    static final int    SERVERTIMEOUT1               = 67;
    static final int    SERVERCONNECTIONSPRECLAIMED1 = 17;
    static final int    SERVERCONNECTIONSONDEMAND1   = 29;
    
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
    static final int PORT1                  = 42;
    static final String PATH1               = "path1";
    static final String FORMATS1            = "formats1";
    static final String BASELAYERNAME1      = "baseLayerName1";
    static final String VENDORCAPABILITIES1 = "vendorCapabilities1";
    static final String CONTACTPERSON1	= "contactPerson1";
    static final String CONTACTORGANIZATION1	= "contactOrganization1";
    static final String ADDRESSTYPE1	= "addressType1";
    static final String ADDRESS1	= "address1";
    static final String CITY1	= "city1";
    static final String STATE1	= "stateOrProvince1";
    static final String POSTCODE1	= "postCode1";
    static final String COUNTRY1	= "country1";
    static final String TEL1	= "contactVoiceTelephone1";
    static final String FAX1	= "contactFacsimileTelephone1";
    static final String EMAIL1	= "contactElectronicMailAddress1";
    
    // - value set 2:
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
    static final String ADDRESS2	= "address2";
    static final String CITY2	= "city2";
    static final String STATE2	= "stateOrProvince2";
    static final String POSTCODE2	= "postCode2";
    static final String COUNTRY2	= "country2";
    static final String TEL2	= "contactVoiceTelephone2";
    static final String FAX2	= "contactFacsimileTelephone2";
    static final String EMAIL2	= "contactElectronicMailAddress2";
    
    // - value set 3:
    static final String UPDATESEQUENCE3     = "updateSequence3";
    static final int AVAILABILITY3		= 0;
    static final String NAME3               = "name3";
    static final String TITLE3              = "title3";
    static final String ABSTRACT3           = "abstractText3";
    static final String KEYWORDS3           = "keywords3";
    static final String FEES3               = "fees3";
    static final String ACCESSCONSTRAINTS3  = "accessConstraints3";
    static final String HOSTNAME3           = "hostName3";
    static final int PORT3                  = 198;
    static final String PATH3               = "path3";
    static final String FORMATS3            = "formats3";
    static final String BASELAYERNAME3      = "baseLayerName3";
    static final String VENDORCAPABILITIES3 = "vendorCapabilities3";
    static final String CONTACTPERSON3	= "contactPerson3";
    static final String CONTACTORGANIZATION3	= "contactOrganization3";
    static final String ADDRESSTYPE3	= "addressType3";
    static final String ADDRESS3	= "address3";
    static final String CITY3	= "city3";
    static final String STATE3	= "stateOrProvince3";
    static final String POSTCODE3	= "postCode3";
    static final String COUNTRY3	= "country3";
    static final String TEL3	= "contactVoiceTelephone3";
    static final String FAX3	= "contactFacsimileTelephone3";
    static final String EMAIL3	= "contactElectronicMailAddress3";
    
    /**
     * component test for this class
     * requires:
     *	DatabaseConnection
     **/
    public static void main( String[] args )
    {
        // --- start action -----------------------------------
        
        System.out.println( "START class Table_Service component test" );
        
        // --- cmd line parameter check ------------------------
        if (args.length != 6)
        {
            System.err.println( "Usage: Table_Service messageFile jdbcUrl user password jdbcDriver debugLevel" );
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
            Table_Service myService1 = new Table_Service();
            int id1 = myService1.insertTuple( c, UPDATESEQUENCE1, AVAILABILITY1, NAME1, TITLE1, ABSTRACT1, KEYWORDS1, FEES1, ACCESSCONSTRAINTS1, HOSTNAME1, PORT1, PATH1, FORMATS1, BASELAYERNAME1, VENDORCAPABILITIES1, CONTACTPERSON1, CONTACTORGANIZATION1, ADDRESSTYPE1, ADDRESS1, CITY1, STATE1, POSTCODE1, COUNTRY1, TEL1, FAX1, EMAIL1 );
            System.out.println( "insert tuple 1 -> " + id1 );
            
            // - update reference instance
            myService1.updateTuple( c, id1, UPDATESEQUENCE2, AVAILABILITY2, NAME2, TITLE2, ABSTRACT2, KEYWORDS2, FEES2, ACCESSCONSTRAINTS2, HOSTNAME2, PORT2, PATH2, FORMATS2, BASELAYERNAME2, VENDORCAPABILITIES2, CONTACTPERSON2, CONTACTORGANIZATION2, ADDRESSTYPE2, ADDRESS2, CITY2, STATE2, POSTCODE2, COUNTRY2, TEL2, FAX2, EMAIL2 );
            
            // - fetch values via 2nd instance, check all values
            Table_Service myService2 = new Table_Service();
            myService2.fetchTupleById( c, id1 );
            if (! myService2.getUpdateSequence().equals( UPDATESEQUENCE2 ) )
                System.out.println( "Error: attribute updated != attributed fetched: " + Globals.TABLE_SERVICE_UPDATESEQUENCE );
            if (myService2.getAvailability()!= AVAILABILITY2  )
                System.out.println( "Error: attribute updated != attributed fetched: " + Globals.TABLE_SERVICE_AVAILABILITY );
            if (! myService2.getName().equals( NAME2 ) )
                System.out.println( "Error: attribute updated != attributed fetched: " + Globals.TABLE_SERVICE_NAME );
            if (! myService2.getTitle().equals( TITLE2 ) )
                System.out.println( "Error: attribute updated != attributed fetched: " + Globals.TABLE_SERVICE_TITLE );
            if (! myService2.getAbstract().equals( ABSTRACT2 ) )
                System.out.println( "Error: attribute updated != attributed fetched: " + Globals.TABLE_SERVICE_ABSTRACT );
            if (! myService2.getKeywords().equals( KEYWORDS2 ) )
                System.out.println( "Error: attribute updated != attributed fetched: " + Globals.TABLE_SERVICE_KEYWORDS );
            if (! myService2.getFees().equals( FEES2 ) )
                System.out.println( "Error: attribute updated != attributed fetched: " + Globals.TABLE_SERVICE_FEES );
            if (! myService2.getAccessConstraints().equals( ACCESSCONSTRAINTS2 ) )
                System.out.println( "Error: attribute updated != attributed fetched: " + Globals.TABLE_SERVICE_ACCESSCONSTRAINTS );
            if (! myService2.getHostName().equals( HOSTNAME2 ) )
                System.out.println( "Error: attribute updated != attributed fetched: " + Globals.TABLE_SERVICE_HOSTNAME );
            if (myService2.getPort() != PORT2 )
                System.out.println( "Error: attribute updated != attributed fetched: " + Globals.TABLE_SERVICE_PORT );
            if (! myService2.getPath().equals( PATH2 ) )
                System.out.println( "Error: attribute updated != attributed fetched: " + Globals.TABLE_SERVICE_PATH );
            if (! myService2.getFormats().equals( FORMATS2 ) )
                System.out.println( "Error: attribute updated != attributed fetched: " + Globals.TABLE_SERVICE_FORMATS );
            if (! myService2.getBaseLayerName().equals( BASELAYERNAME2 ) )
                System.out.println( "Error: attribute updated != attributed fetched: " + Globals.TABLE_SERVICE_BASELAYERNAME );
            if (! myService2.getVendorCapabilities().equals( VENDORCAPABILITIES2 ) )
                System.out.println( "Error: attribute updated != attributed fetched: " + Globals.TABLE_SERVICE_VENDORCAPABILITIES );
            if (! myService2.getContactPerson().equals( CONTACTPERSON2 ) )
                System.out.println( "Error: attribute updated != attributed fetched: " + Globals.TABLE_SERVICE_CONTACTPERSON );
            if (! myService2.getContactOrganization().equals( CONTACTORGANIZATION2 ) )
                System.out.println( "Error: attribute updated != attributed fetched: " + Globals.TABLE_SERVICE_CONTACTORGANIZATION );
            if (! myService2.getAddressType().equals( ADDRESSTYPE2 ) )
                System.out.println( "Error: attribute updated != attributed fetched: " + Globals.TABLE_SERVICE_ADDRESSTYPE );
            if (! myService2.getAddress().equals( ADDRESS2 ) )
                System.out.println( "Error: attribute updated != attributed fetched: " + Globals.TABLE_SERVICE_ADDRESS );
            if (! myService2.getCity().equals( CITY2 ) )
                System.out.println( "Error: attribute updated != attributed fetched: " + Globals.TABLE_SERVICE_CITY );
            if (! myService2.getStateOrProvince().equals( STATE2 ) )
                System.out.println( "Error: attribute updated != attributed fetched: " + Globals.TABLE_SERVICE_STATE );
            if (! myService2.getPostCode().equals( POSTCODE2 ) )
                System.out.println( "Error: attribute updated != attributed fetched: " + Globals.TABLE_SERVICE_POSTCODE );
            if (! myService2.getCountry().equals( COUNTRY2 ) )
                System.out.println( "Error: attribute updated != attributed fetched: " + Globals.TABLE_SERVICE_COUNTRY );
            if (! myService2.getContactVoiceTelephone().equals( TEL2 ) )
                System.out.println( "Error: attribute updated != attributed fetched: " + Globals.TABLE_SERVICE_TEL );
            if (! myService2.getContactFacsimileTelephone().equals( FAX2 ) )
                System.out.println( "Error: attribute updated != attributed fetched: " + Globals.TABLE_SERVICE_FAX );
            if (! myService2.getContactElectronicMailAddress().equals( EMAIL2 ) )
                System.out.println( "Error: attribute updated != attributed fetched: " + Globals.TABLE_SERVICE_EMAIL );

            // - create another instance
            Table_Service myService3 = new Table_Service();
            int id3 = myService3.insertTuple( c, UPDATESEQUENCE3, AVAILABILITY3, NAME3, TITLE3, ABSTRACT3, KEYWORDS3, FEES3, ACCESSCONSTRAINTS3, HOSTNAME3, PORT3, PATH3, FORMATS3, BASELAYERNAME3, VENDORCAPABILITIES3, CONTACTPERSON3, CONTACTORGANIZATION3, ADDRESSTYPE3, ADDRESS3, CITY3, STATE3, POSTCODE3, COUNTRY3, TEL3, FAX3, EMAIL3);
            System.out.println( "insert tuple #2 -> " + id3 );
            
            // - fetch list of entries
            System.out.println( "listing all tuples:" );
            myService2.fetchAllTuples( c );
            while (myService2.next())
            {
                System.out.println( "  service: " + myService2.toString() );
            }
            System.out.println( "listing done." );
            
            // - delete instance 1 and check it's gone
            myService1.deleteTuple( c, id1 );
            try
            {
                myService2.fetchTupleById( c, id1 );
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
        
        System.out.println( "END class Table_Service component test" );
    }
}
