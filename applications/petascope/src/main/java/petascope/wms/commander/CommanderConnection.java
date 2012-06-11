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
 * maintains database connection, using the Singleton patternn.
 * The connection is opened and closed explicitly.
 * The class discloses the SQL Statement objects as abstract units, ie,
 * not intended for direct database handling bypassing CommanderConnection.
 * Statement processing is available for both parametrized, prepared queries
 * and for ad-hoc queries (parametrized NOT YET).
 *
 * COMMENTS
 * - uses TableLocked for maintaining a temporary lock against concurrent use
 *
 *********************************************************** */

package petascope.wms.commander;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.SQLWarning;
import java.sql.Statement;
import java.util.Properties;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;


/**
 * This class represents one connection to the underlying DBMS. It offers a set of
 * prepared statements for querying the meta-tables and means for executing
 * arbitrary queries.
 **/
public class CommanderConnection
{
    private static final Logger log = LoggerFactory.getLogger(CommanderConnection.class);
    /// DBMS service URL
    private String jdbcUrl = "";
    /// user name for login to this service
    private String jdbcUser = "";
    /// password for login
    private String jdbcPasswd = "";
    /// driver; needs to adhere to common JDBC driver syntax!
    private String jdbcDriver = "";
    
    /**
     * A (singleton) instance of a connection
     **/
    private Connection jdbcInstance = null;
    
    /**
     * constructor
     * @param jdbcUrl	DBMS service URL
     * @param jdbcUser	user name for login to this service
     * @param jdbcPasswd	password for login
     * @param jdbcDriver	driver; needs to adhere to common JDBC driver syntax!
     * @return CommanderConnection
     * @throws ConnectionFailedException
     * @throws ConfigurationException
     **/
    public CommanderConnection( String jdbcUrl, String jdbcUser, String jdbcPasswd, String jdbcDriver )
            throws ConnectionFailedException, ConfigurationException, SQLException
    {
        log.info( "CommanderConnection()" );
        
        // --- check parameters for null to safeguard open() ----------
        this.jdbcUrl    = (jdbcUrl==null ? "" : jdbcUrl);
        this.jdbcUser   = (jdbcUser==null ? "" : jdbcUser);
        this.jdbcPasswd = (jdbcPasswd==null ? "" : jdbcPasswd);
        this.jdbcDriver = (jdbcDriver==null ? "" : jdbcDriver);
        
        log.error( "JDBC connection: using URL '" + jdbcUrl + "' with user '" + jdbcUser + "' and driver '" + jdbcDriver + "'" );
        
        // --- open JDBC connection --------------------------------------
        open();				// modifies connection object!
        // throws exception if database is locked
        
        log.info( "CommanderConnection()" );
    }
    
    public Connection getConnection() {
        return jdbcInstance;
    }
    
    /**
     * Establish database connection based on JDBC connection parameters passed;
     * precondition:
     *	url, usr, pwd, jdbc != null
     **/
    public void open()
            throws ConnectionFailedException, ConfigurationException, SQLException
    {
        log.info( "open()" );
        
        // if there is already a connection then try to close it first
        if (jdbcInstance != null)
        {
            try
            {
                close();
            }
            catch( ConnectionFailedException c1 )
            {
                log.warn( "Warning: JDBC error during force-close of connection for reopen: " + c1.getMessage() );
            }
            catch( ConfigurationException c2 )
            {
                log.warn( "Warning: JDBC error during force-close of connection for reopen: " + c2.getMessage() );
            }
        }
        
        // now open connection
        try
        {
            // determine JDBC driver class
            log.debug( "Getting Class.forName('" + jdbcDriver + "')" );
            Class.forName(jdbcDriver);
            
            // setup login credentials
            Properties pro = new Properties();
            pro.setProperty( Globals.PROPERTY_USER, jdbcUser );
            pro.setProperty( Globals.PROPERTY_PASSWORD, jdbcPasswd );
            
            // open connection
            log.warn( "Opening connection for url '" + jdbcUrl + "' using driver {" + jdbcDriver + "} and properties '"  + pro  + "'" );
            jdbcInstance = DriverManager.getConnection( jdbcUrl, pro );
            log.debug( "Connection sucessfully established." );
            
            // deactivate auto-commit mode
            jdbcInstance.setAutoCommit( false );
            log.debug( "Auto-commit mode disabled." );
            
            // set transaction isolation level
            jdbcInstance.setTransactionIsolation( Connection.TRANSACTION_SERIALIZABLE );
            log.debug( "Transaction isolation level set to SERIALIZABLE." );
            
            /* unused
             * // --- check whether database is locked
             * Table_Locked tl = new Table_Locked();
             * if ( tl.locked() )
             * {
             * String msg = "open: database locked by another client.";
             * log.error( msg );
             * throw new LockedException( msg );
             * }
             * else
             * {
             * // - lock database for us using our <IP,time> identifier
             * String myId = FIXME;
             * tl.lock( myId );
             * }
             */
        }
        catch (SQLException e1)
        {
            String msg = "Error: Unable to open connection to '" + jdbcUrl + "': " + e1.getMessage();
            log.info( "open() -- " + msg );
            throw new ConnectionFailedException(msg);
        }
        catch (java.lang.ClassNotFoundException e2)
        {
            String msg = "Error: Could not find JDBC driver: " + e2.getMessage();
            log.info( "open() -- " + msg );
            throw new ConnectionFailedException(msg);
        }
        
        log.info( "open() -- ok" );
    }
    
    /**
     * Closes the connection.
     * @throws ConnectionFailedException
     **/
    public void close()
            throws ConnectionFailedException, ConfigurationException
    {
        log.info( "close()" );
        /* unused
         * // --- unlock database
         * Table_Locked tl = new Table_Locked();
         * tl.unlock();
         * tl = null;
         */
        // singleton pattern: ensure a single instance
        if (jdbcInstance != null)
        {
            try
            {
                jdbcInstance.close();
            }
            catch (SQLException e1)
            {
                String msg = "Error: Could not close connection: " + e1.getMessage();
                log.info( "close() -- " + msg );
                throw new ConnectionFailedException( msg );
            }
        }
        
        log.info( "close()" );
    }
    
    /**
     * Method createStatement
     * returns a Statement, which needs to be close()d by the caller later.
     * @param query
     * @return Statement
     * @throws SQLException
     *
     **/
    public Statement createStatement()
            throws SQLException
    {
        log.info( "createStatement()" );
        
        Statement result = jdbcInstance.createStatement();
        
        log.info( "createStatement()" );
        return( result );
    }
    /**
     * auxiliary internal method printWarningsIfAny()
     * writes eventual SQL warnings as trace output into system log; no further action.
     * @param stmt the statement to be analysed for warnings; pass null to ignore
     */
    private void printWarningsIfAny( Statement stmt )
    {
        if (stmt != null)
        {
            try
            {
                SQLWarning warning = stmt.getWarnings();
                if (warning != null)
                {
                    log.warn("Warning(s) issued by JDBC:");
                    while (warning != null)
                    {
                        log.warn( "\tMessage: " + warning.getMessage() );
                        log.warn( "\tSQLState: " + warning.getSQLState() );
                        log.warn( "\tVendor error code: " +  warning.getErrorCode() );
                        warning = warning.getNextWarning();
                    }
                }
                
                ResultSet rs = stmt.getResultSet();
                if (rs != null)
                {
                    SQLWarning warn = rs.getWarnings();
                    if (warn != null)
                    {
                        log.warn("Warning(s) issued by JDBC:");
                        while (warn != null)
                        {
                            log.warn("\tMessage: " + warn.getMessage() );
                            log.warn("\tSQLState: " + warn.getSQLState() );
                            log.warn("\tVendor error code: " + warn.getErrorCode() );
                            warn = warn.getNextWarning();
                        }
                    }
                }
            }
            catch (SQLException e1)
            {
                log.warn( "Warning: caught SQLException during analysis of SQL warning." );
            }
        }
    }
    
    /**
     * Commits a transaction (non-null connection only).
     * @throws SQLException
     **/
    public void commit() throws SQLException
    {
        log.info( "commit()" );
        
        if (jdbcInstance == null)
            log.warn( "Warning: ignoring attempt to commit TA on null connection object.");
        else
        {
            try
            {
                jdbcInstance.commit();
            }
            catch ( SQLException e )
            {
                log.error( "Error: SQLException during commit: " + e.getMessage() );
            }
        }
        
        log.info( "commit()" );
    }
    
    /**
     * Aborts a transaction.
     **/
    public void abort()
    {
        log.info( "abort()" );
        
        if (jdbcInstance == null)
            log.warn( "Warning: ignoring attempt to abort non-existing TA.");
        else
        {
            try
            {
                jdbcInstance.rollback();
            }
            catch (SQLException e)
            {
                log.error( "Error: SQLException during rollback: " + e.getMessage() );
            }
        }
        
        log.info( "abort()" );
    }
    
    /**
     * Method toString
     * @return String
     */
    public String toString()
    {
        String stringRepresentation = "CommanderConnection( "
                + (jdbcInstance==null?"no instance allocated":"instance allocated")
                + " )";	// no other relevant instance variables as for now
        return( stringRepresentation );
    }
    
    
    /**
     * main() for standalone testing
     * @param String url database URL
     * @param String user database login name
     * @param String password database login password
     * @param String jdbc database JDBC connectivity
     */
    public static void main( String[] args )
    {
        System.out.println( "class CommanderConnection component test" );
        
        // --- cmd line parameter check ------------------------
        if (args.length != 6)
        {
            System.err.println( "Usage: CommanderConnection msgFile url user password jdbc debugLevel" );
            return;
        }
        String msgFile  = args[0];
        String url      = args[1];
        String usr      = args[2];
        String pwd      = args[3];
        String jdbc     = args[4];
        log.info( "Parameters used: msgFile=" + msgFile + ", url=" + url + ", user=" + usr + ", password=" + pwd + ", jdbc=" + jdbc );
        
        
        // --- START preparation -----------------------
        // none here.
        // --- END preparation -------------------------
        System.out.println( "opening connection" );
        // --- START test connection handling ----------
        // - plain open, good case
        
        
        try
        {
            
            CommanderConnection c = new CommanderConnection( url, usr, pwd, jdbc );
            System.out.println( "Trying to open a connection already open. Should follow exception" );
            c.close();
            c.open();
            c.open();
            
        }
        catch( SQLException e1 )
        {
            System.out.println( "Error: received SQLException during test execution: " + e1.getMessage() );
        }
        catch( ConnectionFailedException e2 )
        {
            System.out.println( "Error: received ConnectionFailedException during test execution: " + e2.getMessage() );
        }
        catch( ConfigurationException e3 )
        {
            System.out.println( "Error: received ConfigurationException during test execution: " + e3.getMessage() );
        }
        
        try
        {
            // - bad case: illegal parameters (null)
            System.out.println( "Trying to open a connection using illegal parameters (null). Should follow exception" );
            CommanderConnection cNull = new CommanderConnection( null, null, null, null );
            cNull.open();
            cNull.close();
            cNull = null;
        }
        catch( SQLException e1 )
        {
            System.out.println( "Error: received SQLException during test execution: " + e1.getMessage() );
        }
        catch( ConnectionFailedException e2 )
        {
            System.out.println( "Error: received ConnectionFailedException during test execution: " + e2.getMessage() );
        }
        catch( ConfigurationException e3 )
        {
            System.out.println( "Error: received ConfigurationException during test execution: " + e3.getMessage() );
        }
        
        try
        {
            
            
            // - bad case: illegal parameters (random values)
            System.out.println("Trying to open a connection using illegal parameters (random values).Should follow exception" );
            CommanderConnection cIllegal = new CommanderConnection( "param_1", "param_2", "param_3", "param_4" );
            
            cIllegal.open();
            cIllegal.close();
            cIllegal = null;
            
            
        }
        catch( SQLException e1 )
        {
            System.out.println( "Error: received SQLException during test execution: " + e1.getMessage() );
        }
        catch( ConnectionFailedException e2 )
        {
            System.out.println( "Error: received ConnectionFailedException during test execution: " + e2.getMessage() );
        }
        catch( ConfigurationException e3 )
        {
            System.out.println( "Error: received ConfigurationException during test execution: " + e3.getMessage() );
        }
        
        System.out.println( "class CommanderConnection component test finished" );
        
        // --- END test connection handling ------------
        
        // --- START cleanup ---------------------------
        // ...and check what happens at object freeing
        // --- END cleanup -----------------------------
    }
}