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

import org.junit.Assert;
import org.junit.Ignore;
import org.junit.Test;
import petascope.BaseTestCase;
import petascope.exceptions.WCSException;
import org.junit.After;
import org.junit.Before;

/**
  Test class for the XMLDescribeCoverageParser. Tests whether the parser
  properly validates that the recieved xml is structured according with the wcs2 
  version 2.0.0 standard.
  @author Ernesto Rodriguez <ernesto4160@gmail.com>
*/

public class XMLDescribeCoverageSchemaTest extends BaseTestCase{

    private XMLDescribeCoverageParser parser;

    private final String invalidXML="<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
	+"<wcs:DescribeCoverage  xmlns:xsi='http://www.w3.org/2001/XMLSchema-instance'\n"
	+"xmlns:wcs='http://www.opengis.net/wcs/2.0'\n"
	+"xmlns:gml='http://www.opengis.net/gml/3.2'\n"
	+"xsi:schemaLocation='http://www.opengis.net/wcs/2.0 ../../wcsAll.xsd'\n"
	+"services=\"WCS\" version=\"2.0.0\">"
	+"<wcs:CoverageId>NIR</wcs:CoverageId></wcs:DescribeCoverage>";

    private final String validXML="<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
	+"<wcs:DescribeCoverage  xmlns:xsi='http://www.w3.org/2001/XMLSchema-instance'\n"
	+"xmlns:wcs='http://www.opengis.net/wcs/2.0'\n"
	+"xmlns:gml='http://www.opengis.net/gml/3.2'\n"
	+"xsi:schemaLocation='http://www.opengis.net/wcs/2.0 ../../wcsAll.xsd'\n"
	+"service=\"WCS\" version=\"2.0.0\">"
	+"<wcs:CoverageId>NIR</wcs:CoverageId></wcs:DescribeCoverage>";
    
    @Before
    public void setUp() {

	parser=new XMLDescribeCoverageParser();
    }

    @After
    public void tearDown(){

    }

    @Test
    public void testValidXML() throws WCSException{
	
	parser.parse(validXML);
    }

    
    @Test(expected=WCSException.class)
    public void testInvalidXML() throws WCSException{
	parser.parse(invalidXML);
    }
}