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
package petascope.wcs2.parsers;

import java.util.HashMap;
import java.util.ArrayList;
import java.util.Map;
import java.util.logging.Level;
import java.util.logging.Logger;
import org.junit.After;
import org.junit.AfterClass;
import org.junit.Before;
import org.junit.BeforeClass;
import org.junit.Test;
import static org.junit.Assert.*;

/**
 * Test class for KeyValue Coverage Parser
 * @author Alex Dumitru <m.dumitru@jacobs-univesity.de>
 */
public class KVPGetCoverageParserTest {

    private ArrayList<String> inputs;
    private ArrayList<HashMap<String, String>> correctResults;

    public KVPGetCoverageParserTest() {
    }

    @BeforeClass
    public static void setUpClass() throws Exception {
    }

    @AfterClass
    public static void tearDownClass() throws Exception {
    }

    @Before
    public void setUp() {
        inputs = new ArrayList<String>();
        correctResults = new ArrayList<HashMap<String, String>>();
        inputs.add("service=WCS&Request=GetCoverage&version=2.0.0&CoverageId=mean_summer_airtemp&format=image/tiff&subset=x(10,15)");
        correctResults.add(new HashMap<String, String>());
        correctResults.get(0).put("subsetx(10,15)", "x(10,15)");
        inputs.add("service=WCS&Request=GetCoverage&version=2.0.0&CoverageId=mean_summer_airtemp&format=image/tiff&subsetX=x(10,15)");
        correctResults.add(new HashMap<String, String>());
        correctResults.get(1).put("subsetXx(10,15)", "x(10,15)");
        inputs.add("service=WCS&Request=GetCoverage&version=2.0.0&CoverageId=mean_summer_airtemp&format=image/tiff&subset=x(20,25)&subset=y(20,30)");
        correctResults.add(new HashMap<String, String>());
        correctResults.get(2).put("subsetx(20,25)", "x(20,25)");
        correctResults.get(2).put("subsety(20,30)", "y(20,30)");
        inputs.add("service=WCS&Request=GetCoverage&version=2.0.0&CoverageId=mean_summer_airtemp&format=image/tiff&subset=x(10,35)&subsetD=y(10,30)");
        correctResults.add(new HashMap<String, String>());
        correctResults.get(3).put("subsetx(10,35)", "x(10,35)");
        correctResults.get(3).put("subsetDy(10,30)", "y(10,30)");
        inputs.add("service=WCS&Request=GetCoverage&version=2.0.0&CoverageId=mean_summer_airtemp&format=image/tiff&suBSet=x(100,305)&sUBsetD=y(210,330)");
        correctResults.add(new HashMap<String, String>());
        correctResults.get(4).put("suBSetx(100,305)", "x(100,305)");
        correctResults.get(4).put("sUBsetDy(210,330)", "y(210,330)");
    }

    @After
    public void tearDown() {
    }

    /**
     * Test of parseSubsetParams method, of class KVPGetCoverageParser.
     * The method should be made public before running the test
     */
    @Test
    public void testParseSubsetParams() throws Exception {
        KVPGetCoverageParser inst = new KVPGetCoverageParser();
        for (int i = 0; i < inputs.size(); i++) {
            HashMap<String, String> res = inst.parseSubsetParams(inputs.get(i));            
            for (Map.Entry<String, String> kv : res.entrySet()) {
                assertTrue(correctResults.get(i).containsValue(kv.getValue()));
            }
            for (Map.Entry<String, String> kv : correctResults.get(i).entrySet()) {
                assertTrue(res.containsValue(kv.getValue()));
            }            
        }
    }
}
