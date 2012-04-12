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

package petascope.wcs2;

import org.junit.Assert;
import org.junit.Ignore;
import org.junit.Test;
import petascope.BaseTestCase;
import petascope.exceptions.RasdamanException;
import org.junit.After;
import org.junit.Before;
import java.util.Scanner;
import java.util.regex.Pattern;
import java.util.regex.Matcher;
import java.io.IOException;
import petascope.wcs2.extensions.*;
import petascope.core.DbMetadataSource;
import petascope.wcs2.handlers.Response;
import petascope.ConfigManager;
import petascope.exceptions.WCSException;
import petascope.exceptions.PetascopeException;
import org.xml.sax.SAXException;

/**
 * This a test class used to test if petascope can
 * properly parse multiple requests that require
 * access to a rasdaman database.
 *
 * @author Ernesto Rodriguez <ernesto4160@gmail.com>
 */

public class ParseMultipleRasdamanQueryTest extends BaseTestCase{

    //The number of concurrent requests that will be preformed.
    private final int NUM_REQUESTS = 5;

    /**
     * Test case for parsing multiple queries that require
     * rasdaman. Muliple queries will be simultaneously generated
     * this method should complete without exceptions. If an
     * exception results the test case fails. The exception
     * will result if a parser is invoked multiple times
     * from different threads.
     */
    @Test
    public void concurrentRasdamanRequests() throws Exception {

	 DbMetadataSource meta = new DbMetadataSource(ConfigManager.METADATA_DRIVER,
                    ConfigManager.METADATA_URL,
                    ConfigManager.METADATA_USER,
                    ConfigManager.METADATA_PASS, false);


	 PreformWCS2RasdamanQuery requests[] = new PreformWCS2RasdamanQuery[NUM_REQUESTS];
	 KVPProtocolExtension pext = new KVPProtocolExtension();

	 for(int i = 0; i < NUM_REQUESTS; i++){

	     requests[i] = new PreformWCS2RasdamanQuery(pext, meta);
	     (new Thread(requests[i])).start();
	 }	 

	 boolean isDone = false;

	 while(!isDone) {

	     isDone = true;

	     for(int i = 0; i < NUM_REQUESTS; i++){

		 if(!requests[i].isDone())
		     isDone = false;
	     }
	 }

	 for(int i = 0; i < NUM_REQUESTS; i++){

	     try {
		 
		 throw requests[i].result();

	     } catch(NullPointerException e) {

		 //The request completed successfully
	     }
	 }	
    }

}