/*
 * This file is part of Petascope.
 *
 * Petascope is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * Petascope is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with Petascope. If not, see <http://www.gnu.org/licenses/>.
 *
 * For more information please see <http://www.Petascope.org>
 * or contact Peter Baumann via <baumann@rasdaman.com>.
 *
 * Copyright 2009 Jacobs University Bremen, Peter Baumann.
 */

package wcs.server.core;

import java.io.FileInputStream;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.Properties;

public class MetadataDb {

    Connection con;
    Statement sta;
    ResultSet result;
    String driver, url, user, pass;

    private void log(String msg)
    {
        System.out.println(msg);
    }

    /**
     * Default Constructor. Initializes the database access information from a file on disk
     * 
     * @param dbParamsFile path to the properties file
     */
    public MetadataDb(String dbParamsFile) throws WCSException
    {
        
        Properties dbParams = new Properties();
        try
        {
            dbParams.load(new FileInputStream(dbParamsFile));

            driver = dbParams.getProperty("metadata_driver");
            url = dbParams.getProperty("metadata_url");
            user = dbParams.getProperty("metadata_user");
            pass = dbParams.getProperty("metadata_pass");

            Class.forName(driver);
			con = DriverManager.getConnection(url, user, pass);
		}
        catch(Exception e)
        {
            log("User = " + user);
            log("Pass = " + pass);
            log("URL = " + url);
            log("Driver = " + driver);
            throw new WCSException("NoApplicableCode",
                    "Could not connect to the metadata database!",
                    "Could not connect to the metadata database!");
        }
    }

    /**
     * Makes sure the connection to the database is alive. Reconnects if needed. 
     */
    private void ensureConnection()
    {
        synchronized (this) {
            try {
                if (con == null || !con.isClosed()) {
                    con.close();
                    con = DriverManager.getConnection(url, user, pass);
                }
            } catch (SQLException ex) {
                System.err.println("SQLException: " + ex.getMessage());
            }
        }
    }

    /**
     * Execute a query on the database that is currently open.
     * @param query SQL of the query
     * @return a ResultSet object, with the results of the query
     */
    public ResultSet executeQuery(String query)
    {
        try
        {
            ensureConnection();
			sta = con.createStatement();
	   		result = sta.executeQuery(query);
		} 
        catch(SQLException ex)
        {
			System.err.println("SQLException: " + ex.getMessage());
            result = null;
		}

        return result;
    }

}
